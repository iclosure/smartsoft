//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2012 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPVisualCollector.cpp: implementation of the CBCGPRibbonCollector class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPVisualCollector.h"
#include "BCGPVisualContainer.h"
#include "BCGPTextGaugeImpl.h"
#include "BCGPAnalogClock.h"
#include "BCGPImageGaugeImpl.h"
#include "BCGPDiagramConnector.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

class XBCGPTreeMapGroup: public CBCGPTreeMapGroup
{
	friend CBCGPVisualInfo::XTreeMapData* CollectTreeMapData(const CBCGPBaseTreeMapNode* pNode);
};

inline CBCGPVisualInfo::XTreeMapData* CollectTreeMapData(const CBCGPBaseTreeMapNode* pNode)
{
	if (pNode == NULL)
	{
		return NULL;
	}

	CBCGPVisualInfo::XTreeMapData* pInfo = NULL;

	const XBCGPTreeMapGroup* pGroup = (XBCGPTreeMapGroup*)DYNAMIC_DOWNCAST(CBCGPTreeMapGroup, pNode);
	if (pGroup != NULL)
	{
		CBCGPVisualInfo::XTreeMapGroup* pInfoGroup = new CBCGPVisualInfo::XTreeMapGroup;
		pInfo = pInfoGroup;

		pInfoGroup->m_strLabel = pGroup->GetLabel();
		pInfoGroup->m_szMargin = pGroup->GetMargin();
		pInfoGroup->m_brFill   = pGroup->m_brFill;
		pInfoGroup->m_brText   = pGroup->m_brText;
		pInfoGroup->m_fmtText  = pGroup->m_tf;

		const CArray<CBCGPBaseTreeMapNode*, CBCGPBaseTreeMapNode*>& arNodes = pGroup->GetSubNodes ();
		for (int i = 0; i < (int)arNodes.GetSize (); i++)
		{
			CBCGPVisualInfo::XTreeMapData* pData = CollectTreeMapData(arNodes[i]);
			if (pData != NULL)
			{
				pInfoGroup->m_arNodes.Add (pData);
			}
		}
	}
	else
	{
		CBCGPVisualInfo::XTreeMapNode* pInfoNode = new CBCGPVisualInfo::XTreeMapNode;
		pInfo = pInfoNode;

		pInfoNode->m_dblValue = pNode->GetValue();
		pInfoNode->m_strLabel = pNode->GetLabel();
	}

	return pInfo;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPVisualCollector::CBCGPVisualCollector(CBCGPVisualInfo& info, DWORD dwFlags)
	: m_Info   (info)
	, m_dwFlags(dwFlags)
{
}

CBCGPVisualCollector::~CBCGPVisualCollector()
{
}

void CBCGPVisualCollector::Collect(const CBCGPVisualContainer& container)
{
	CollectContainer (container, GetInfo ().GetContainer ());
}

void CBCGPVisualCollector::CollectContainer(const CBCGPVisualContainer& container, CBCGPVisualInfo::XContainer& info)
{
	info.m_bDrawDynamicObjectsOnTop = container.IsDrawDynamicObjectsOnTop ();
	info.m_Rect      = container.GetRect ();
	info.m_brFill    = container.GetFillBrush ();
	info.m_brOutline = container.GetOutlineBrush ();

	CPoint ptOffset (0, 0);

	if (!info.m_Rect.IsRectEmpty ())
	{
		ptOffset = info.m_Rect.TopLeft ();
	}

	for (int i = 0; i < container.GetCount (); i++)
	{
		const CBCGPBaseVisualObject* pElement = container[i];
		ASSERT_VALID (pElement);

		if (pElement != NULL)
		{
			CBCGPVisualInfo::XElement* pElementInfo = CollectElement (*pElement);
			if (pElementInfo != NULL)
			{
				pElementInfo->m_Rect.OffsetRect (-ptOffset);
				info.m_arElements.Add (pElementInfo);
			}
		}
	}
}

CBCGPVisualInfo::XElement* CBCGPVisualCollector::CollectElement(const CBCGPBaseVisualObject& element)
{
	CBCGPVisualInfo::XElement* info = NULL;

	if (element.IsKindOf (RUNTIME_CLASS (CBCGPTagCloud)))
	{
		CBCGPVisualInfo::XElementTagCloud* pNewInfo = new CBCGPVisualInfo::XElementTagCloud;
		info = pNewInfo;

		CollectBaseElement (element, *info);

		const CBCGPTagCloud* pElement = (const CBCGPTagCloud*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_SortOrder = pElement->GetSortOrder ();
		pNewInfo->m_bDescending = pElement->IsSortDescending ();
		pNewInfo->m_nMaxWeight = pElement->GetMaxWeight ();
		pNewInfo->m_dblFontSizeStep = pElement->GetFontSizeStep ();
		pNewInfo->m_szMargin.cx = pElement->GetHorzMargin ();
		pNewInfo->m_szMargin.cy = pElement->GetVertMargin ();
		pNewInfo->m_fmtBase = pElement->GetTextFormat ();
		pNewInfo->m_brFill = pElement->GetFillBrush ();
		pNewInfo->m_clrText = pElement->GetTextColor ();
		pNewInfo->m_clrTextHighlighted = pElement->GetHighlightedTextColor ();

		if ((m_dwFlags & e_CollectTagCloudData) == e_CollectTagCloudData)
		{
			const CList<CBCGPTagCloudElement*, CBCGPTagCloudElement*>& list = pElement->GetTagList ();

			POSITION pos = list.GetHeadPosition ();
			while (pos != NULL)
			{
				const CBCGPTagCloudElement* pData = list.GetNext (pos);
				if (pData == NULL)
				{
					continue;
				}

				CBCGPVisualInfo::XTagCloudData* pDataInfo = new CBCGPVisualInfo::XTagCloudData;
				pDataInfo->m_dblValue = pData->GetValue ();
				pDataInfo->m_strLabel = pData->GetLabel ();
				pDataInfo->m_brFill = pData->GetBackgroundBrush ();
				pDataInfo->m_clrText = pData->GetTextColor ();
				pDataInfo->m_clrTextHighlighted = pData->GetHighlightedColor ();
				pDataInfo->m_clrBorder = pData->GetBorderColor ();

				pNewInfo->m_arDataObjects.Add (pDataInfo);
			}
		}
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPTreeMap)))
	{
		CBCGPVisualInfo::XElementTreeMap* pNewInfo = new CBCGPVisualInfo::XElementTreeMap;
		info = pNewInfo;

		CollectBaseElement (element, *info);

		const CBCGPTreeMap* pElement = (const CBCGPTreeMap*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_LayoutType = pElement->GetLayoutType ();
		pNewInfo->m_Root.m_szMargin = pElement->GetGroupMargin();
		pNewInfo->m_Root.m_brFill = pElement->GetFillBrush();

		if ((m_dwFlags & e_CollectTreeMapData) == e_CollectTreeMapData)
		{
			const CArray<CBCGPBaseTreeMapNode*, CBCGPBaseTreeMapNode*>& arNodes = 
				pElement->GetRootGroup().GetSubNodes ();
			for (int i = 0; i < (int)arNodes.GetSize (); i++)
			{
				CBCGPVisualInfo::XTreeMapData* pInfoData = CollectTreeMapData(arNodes[i]);
				if (pInfoData != NULL)
				{
					pNewInfo->m_Root.m_arNodes.Add (pInfoData);
				}
			}
		}
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPGaugeImpl)))
	{
		info = CollectGaugeElement ((const CBCGPGaugeImpl&)element);
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPDiagramVisualObject)))
	{
		info = CollectDiagramElement ((const CBCGPDiagramVisualObject&)element);
	}

	return info;
}

CBCGPVisualInfo::XGaugeElement* CBCGPVisualCollector::CollectGaugeElement(const CBCGPGaugeImpl& element)
{
	CBCGPVisualInfo::XGaugeElement* info = NULL;

	if (element.IsKindOf (RUNTIME_CLASS (CBCGPCircularGaugeImpl)))
	{
		BOOL bKnob = element.IsKindOf (RUNTIME_CLASS (CBCGPKnob));
		BOOL bAnalogClock = element.IsKindOf (RUNTIME_CLASS (CBCGPAnalogClock));
		BOOL bAnalogClockChecked = FALSE;

		CBCGPVisualInfo::XElementCircular* pNewInfo = NULL;
		if (bKnob)
		{
			pNewInfo = new CBCGPVisualInfo::XElementKnob;
		}
		else if (bAnalogClock)
		{
			pNewInfo = new CBCGPVisualInfo::XElementAnalogClock;
			bAnalogClockChecked = !((CBCGPAnalogClock&)element).IsDateEnabled ();
		}
		else
		{
			pNewInfo = new CBCGPVisualInfo::XElementCircular;
		}
		info = pNewInfo;

		CollectBaseGaugeElement (element, *info);

		CBCGPCircularGaugeImpl* pElement = (CBCGPCircularGaugeImpl*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_Colors             = pElement->GetColors ();
		pNewInfo->m_fmtText            = pElement->GetTextFormat ();
		pNewInfo->m_dblCapSize         = pElement->GetCapSize ();

		if (!bKnob)
		{
			pNewInfo->m_bShapeByTicksArea  = pElement->IsShapeByTicksArea ();
		}

		int i = 0;
		for (i = 0; i < element.GetScalesCount (); i++)
		{
			CBCGPVisualInfo::XCircularScale* pScaleInfo = new CBCGPVisualInfo::XCircularScale;

			pScaleInfo->m_dblStart               = pElement->GetStart (i);
			pScaleInfo->m_dblFinish              = pElement->GetFinish (i);
			pScaleInfo->m_dblStep                = pElement->GetStep (i);
			pScaleInfo->m_strLabelFormat         = pElement->GetTextLabelFormat (i);
			pScaleInfo->m_dblMinorTickMarkSize   = pElement->GetTickMarkSize (FALSE, i);
			pScaleInfo->m_dblMajorTickMarkSize   = pElement->GetTickMarkSize (TRUE, i);
			pScaleInfo->m_dblMajorTickMarkStep   = pElement->GetMajorTickMarkStep (i);
			pScaleInfo->m_brFill                 = pElement->GetScaleFillBrush (i);
			pScaleInfo->m_brOutline              = pElement->GetScaleOutlineBrush (i);
			pScaleInfo->m_brText                 = pElement->GetScaleTextBrush (i);
			pScaleInfo->m_brTickMarkMinor        = pElement->GetScaleTickMarkBrush (FALSE, i);
			pScaleInfo->m_brTickMarkMajor        = pElement->GetScaleTickMarkBrush (TRUE, i);
			pScaleInfo->m_dblStartAngle          = pElement->GetTicksAreaStartAngle (i);
			pScaleInfo->m_dblFinishAngle         = pElement->GetTicksAreaFinishAngle (i);
			pScaleInfo->m_dblOffsetFromFrame     = pElement->GetScaleOffsetFromFrame (i);
			pScaleInfo->m_bRotateLabels          = pElement->IsLabelRotation (i);
			pScaleInfo->m_MinorTickMarkStyle     = pElement->GetTickMarkStyle (FALSE, i);
			pScaleInfo->m_MajorTickMarkStyle     = pElement->GetTickMarkStyle (TRUE, i);
			pScaleInfo->m_bIsClosed              = pElement->IsClosed (i);
			pScaleInfo->m_bDrawLastTickMark      = pElement->IsDrawLastTickMark (i);
			pScaleInfo->m_bAnimationThroughStart = pElement->IsAnimationThroughStart (i);

			pNewInfo->m_arScales.Add (pScaleInfo);
		}

		for (i = 0; i < pElement->GetDataCount (); i++)
		{
			const CBCGPGaugeDataObject* pData = (const CBCGPGaugeDataObject*)pElement->GetData (i);
			CBCGPVisualInfo::XGaugeData* pDataInfo = NULL;

			if (bKnob)
			{
				const CBCGPKnobPointer* pPointer = (const CBCGPKnobPointer*)pData;
				CBCGPVisualInfo::XKnobPointer* pPointerInfo = new CBCGPVisualInfo::XKnobPointer;
				pDataInfo = pPointerInfo;

				pPointerInfo->m_dblOffsetFromCenter = pPointer->GetOffsetFromCenter ();
				pPointerInfo->m_dblWidth  = pPointer->GetWidth ();
				pPointerInfo->m_Style     = pPointer->GetStyle ();
			}
			else
			{
				const CBCGPCircularGaugePointer* pPointer = (const CBCGPCircularGaugePointer*)pData;
				CBCGPVisualInfo::XCircularPointer* pPointerInfo = new CBCGPVisualInfo::XCircularPointer;
				pDataInfo = pPointerInfo;

				pPointerInfo->m_dblSize   = pPointer->GetSize ();
				pPointerInfo->m_dblWidth  = pPointer->GetWidth ();
				pPointerInfo->m_bExtraLen = pPointer->IsExtraLen ();
				pPointerInfo->m_Style     = pPointer->GetStyle ();
			}

			pDataInfo->m_nScale = pData->GetScale ();
			if (!bAnalogClock)
			{
				pDataInfo->m_dblValue = pData->GetValue ();
			}
			pDataInfo->m_brFill    = pData->GetFillBrush ();
			pDataInfo->m_brOutline = pData->GetOutlineBrush ();

			pNewInfo->m_arPointers.Add(pDataInfo);
		}

		for (i = 0; i < pElement->GetColoredRangesCount (); i++)
		{
			CBCGPGaugeColoredRangeObject* pRange = (CBCGPGaugeColoredRangeObject*)pElement->GetColoredRange (i);
			CBCGPVisualInfo::XCircularColoredRange* pRangeInfo = new CBCGPVisualInfo::XCircularColoredRange;

			pRangeInfo->m_nScale              = pRange->GetScale ();
			pRangeInfo->m_dblStartValue       = pRange->GetStartValue ();
			pRangeInfo->m_dblFinishValue      = pRange->GetFinishValue ();
			pRangeInfo->m_brFill              = pRange->GetFillBrush ();
			pRangeInfo->m_brOutline           = pRange->GetOutlineBrush ();
			pRangeInfo->m_brTickMarkFill      = pRange->GetTickMarkFillBrush ();
			pRangeInfo->m_brTickMarkOutline   = pRange->GetTickMarkOutlineBrush ();
			pRangeInfo->m_brTextLabel         = pRange->GetTextLabelBrush ();
			pRangeInfo->m_dblStartWidth       = pRange->GetStartWidth ();
			pRangeInfo->m_dblFinishWidth      = pRange->GetFinishWidth ();
			pRangeInfo->m_dblOffsetFromFrame  = pRange->GetOffsetFromFrame ();
			pNewInfo->m_arRanges.Add(pRangeInfo);
		}

		const CArray<CBCGPGaugeImpl*, CBCGPGaugeImpl*>& arSubGauges = pElement->GetSubGauges ();
		for (i = 0; i < (int)arSubGauges.GetSize (); i++)
		{
			CBCGPGaugeImpl* pSubGauge = arSubGauges[i];
			if (pSubGauge != NULL)
			{
				CBCGPVisualInfo::XGaugeElement* pSubGaugeInfo = CollectGaugeElement(*pSubGauge);
				if (pSubGaugeInfo != NULL)
				{
					pNewInfo->m_arSubGauges.Add (pSubGaugeInfo);
				}
				
				if (bAnalogClock && !bAnalogClockChecked)
				{
					if (((CBCGPAnalogClock*)pElement)->m_pDate == pSubGauge &&
						pSubGauge->IsKindOf (RUNTIME_CLASS(CBCGPNumericIndicatorImpl)))
					{
						((CBCGPVisualInfo::XElementAnalogClock*)pNewInfo)->m_nDateIndex = i;
						bAnalogClockChecked = TRUE;
					}
				}
			}
		}
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPLinearGaugeImpl)))
	{
		CBCGPVisualInfo::XElementLinear* pNewInfo = new CBCGPVisualInfo::XElementLinear;
		info = pNewInfo;

		CollectBaseGaugeElement (element, *info);

		CBCGPLinearGaugeImpl* pElement = (CBCGPLinearGaugeImpl*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_Colors      = pElement->GetColors ();
		pNewInfo->m_fmtText     = pElement->GetTextFormat ();
		pNewInfo->m_bIsVertical = pElement->IsVerticalOrientation ();

		int i = 0;
		for (i = 0; i < element.GetScalesCount (); i++)
		{
			CBCGPVisualInfo::XLinearScale* pScaleInfo = new CBCGPVisualInfo::XLinearScale;

			pScaleInfo->m_dblStart               = pElement->GetStart (i);
			pScaleInfo->m_dblFinish              = pElement->GetFinish (i);
			pScaleInfo->m_dblStep                = pElement->GetStep (i);
			pScaleInfo->m_strLabelFormat         = pElement->GetTextLabelFormat (i);
			pScaleInfo->m_dblMinorTickMarkSize   = pElement->GetTickMarkSize (FALSE, i);
			pScaleInfo->m_dblMajorTickMarkSize   = pElement->GetTickMarkSize (TRUE, i);
			pScaleInfo->m_dblMajorTickMarkStep   = pElement->GetMajorTickMarkStep (i);
			pScaleInfo->m_brFill                 = pElement->GetScaleFillBrush (i);
			pScaleInfo->m_brOutline              = pElement->GetScaleOutlineBrush (i);
			pScaleInfo->m_brText                 = pElement->GetScaleTextBrush (i);
			pScaleInfo->m_brTickMarkMinor        = pElement->GetScaleTickMarkBrush (FALSE, i);
			pScaleInfo->m_brTickMarkMajor        = pElement->GetScaleTickMarkBrush (TRUE, i);
			pScaleInfo->m_dblOffsetFromFrame     = pElement->GetScaleOffsetFromFrame (i);
			pScaleInfo->m_MinorTickMarkStyle     = pElement->GetTickMarkStyle (FALSE, i);
			pScaleInfo->m_MajorTickMarkStyle     = pElement->GetTickMarkStyle (TRUE, i);

			pNewInfo->m_arScales.Add (pScaleInfo);
		}

		for (i = 0; i < pElement->GetDataCount (); i++)
		{
			const CBCGPLinearGaugePointer* pPointer = (const CBCGPLinearGaugePointer*)pElement->GetData (i);
			CBCGPVisualInfo::XLinearPointer* pPointerInfo = new CBCGPVisualInfo::XLinearPointer;

			pPointerInfo->m_nScale    = pPointer->GetScale ();
			pPointerInfo->m_dblValue  = pPointer->GetValue ();
			pPointerInfo->m_brFill    = pPointer->GetFillBrush ();
			pPointerInfo->m_brOutline = pPointer->GetOutlineBrush ();
			pPointerInfo->m_dblSize   = pPointer->GetSize ();
			pPointerInfo->m_dblWidth  = pPointer->GetWidth ();
			pPointerInfo->m_Style     = pPointer->GetStyle ();
			pNewInfo->m_arPointers.Add(pPointerInfo);
		}

		for (i = 0; i < pElement->GetColoredRangesCount (); i++)
		{
			CBCGPGaugeColoredRangeObject* pRange = (CBCGPGaugeColoredRangeObject*)pElement->GetColoredRange (i);
			CBCGPVisualInfo::XLinearColoredRange* pRangeInfo = new CBCGPVisualInfo::XLinearColoredRange;

			pRangeInfo->m_nScale              = pRange->GetScale ();
			pRangeInfo->m_dblStartValue       = pRange->GetStartValue ();
			pRangeInfo->m_dblFinishValue      = pRange->GetFinishValue ();
			pRangeInfo->m_brFill              = pRange->GetFillBrush ();
			pRangeInfo->m_brOutline           = pRange->GetOutlineBrush ();
			pRangeInfo->m_brTickMarkFill      = pRange->GetTickMarkFillBrush ();
			pRangeInfo->m_brTickMarkOutline   = pRange->GetTickMarkOutlineBrush ();
			pRangeInfo->m_brTextLabel         = pRange->GetTextLabelBrush ();
			pRangeInfo->m_dblStartWidth       = pRange->GetStartWidth ();
			pRangeInfo->m_dblFinishWidth      = pRange->GetFinishWidth ();
			pRangeInfo->m_dblOffsetFromFrame  = pRange->GetOffsetFromFrame ();
			pNewInfo->m_arRanges.Add(pRangeInfo);
		}
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPNumericIndicatorImpl)))
	{
		CBCGPVisualInfo::XElementNumeric* pNewInfo = new CBCGPVisualInfo::XElementNumeric;
		info = pNewInfo;

		CollectBaseGaugeElement (element, *info);

		const CBCGPNumericIndicatorImpl* pElement = (const CBCGPNumericIndicatorImpl*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_Colors            = pElement->GetColors ();
		pNewInfo->m_fmtText           = pElement->GetTextFormat ();
		pNewInfo->m_Style             = pElement->GetStyle ();
		pNewInfo->m_nCells            = pElement->GetCells ();
		pNewInfo->m_nDecimals         = pElement->GetDecimals ();
		pNewInfo->m_nSeparatorWidth   = pElement->GetSeparatorWidth ();
		pNewInfo->m_bDrawSign         = pElement->IsDrawSign ();
		pNewInfo->m_bDrawDecimalPoint = pElement->IsDrawDecimalPoint ();
		pNewInfo->m_bDrawLeadingZeros = pElement->IsDrawLeadingZeros ();
		pNewInfo->m_dblValue          = pElement->GetValue ();
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPColorIndicatorImpl)))
	{
		CBCGPVisualInfo::XElementColor* pNewInfo = new CBCGPVisualInfo::XElementColor;
		info = pNewInfo;

		CollectBaseGaugeElement (element, *info);

		const CBCGPColorIndicatorImpl* pElement = (const CBCGPColorIndicatorImpl*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_Colors     = pElement->GetColors ();
		pNewInfo->m_Style      = pElement->GetStyle ();
		pNewInfo->m_bStretched = pElement->IsStretched ();
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPTextGaugeImpl)))
	{
		CBCGPVisualInfo::XElementText* pNewInfo = new CBCGPVisualInfo::XElementText;
		info = pNewInfo;

		CollectBaseGaugeElement (element, *info);

		const CBCGPTextGaugeImpl* pElement = (const CBCGPTextGaugeImpl*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_brText  = pElement->GetTextBrush ();
		pNewInfo->m_fmtText = pElement->GetTextFormat ();
		pNewInfo->m_strText = pElement->GetText ();
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPImageGaugeImpl)))
	{
		CBCGPVisualInfo::XElementImage* pNewInfo = new CBCGPVisualInfo::XElementImage;
		info = pNewInfo;

		CollectBaseGaugeElement (element, *info);

		const CBCGPImageGaugeImpl* pElement = (const CBCGPImageGaugeImpl*)&element;
		ASSERT_VALID (pElement);

		CollectImage (pElement->m_Image, pNewInfo->m_Image);
	}

	return info;
}

void CBCGPVisualCollector::CollectBaseElement (const CBCGPBaseVisualObject& element, CBCGPVisualInfo::XElement& info)
{
	info.m_ID.m_Name  = element.GetName ();
	info.m_ID.m_Value = element.GetID ();
	info.m_bIsVisible = element.IsVisible ();
	info.m_bIsAutoDestroy = element.IsAutoDestroy ();
	info.m_Rect       = element.GetRect ();
}


void CBCGPVisualCollector::CollectBaseGaugeElement (const CBCGPGaugeImpl& element, CBCGPVisualInfo::XGaugeElement& info)
{
	CollectBaseElement(element, info);

	info.m_nFrameSize = element.GetFrameSize ();
	info.m_bIsInteractiveMode = element.IsInteractiveMode ();
	info.m_strToolTip = element.GetToolTip ();
	info.m_strDescription = element.GetDescription ();

	info.m_Pos        = element.GetPos ();
	info.m_ptOffset   = element.GetOffset ();
}

CBCGPVisualInfo::XDiagramElement* CBCGPVisualCollector::CollectDiagramElement (const CBCGPDiagramVisualObject& element)
{
	CBCGPVisualInfo::XDiagramElement* info = NULL;

	if (element.IsKindOf (RUNTIME_CLASS (CBCGPDiagramConnector)))
	{
		CBCGPVisualInfo::XDiagramElementConnector* pNewInfo = NULL;
		
		if (element.IsKindOf (RUNTIME_CLASS (CBCGPDiagramShelfConnector)))
		{
			pNewInfo = new CBCGPVisualInfo::XElementDiagramConnectorShelf;
		}
		else if (element.IsKindOf (RUNTIME_CLASS (CBCGPDiagramElbowConnector)))
		{
			pNewInfo = new CBCGPVisualInfo::XElementDiagramConnectorElbow;
		}
		else
		{
			pNewInfo = new CBCGPVisualInfo::XElementDiagramConnector;
		}
		
		info = pNewInfo;

		CollectBaseDiagramElement (element, *info);

		const CBCGPDiagramConnector* pElement = (const CBCGPDiagramConnector*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_curveType = pElement->GetCurveType ();
		pNewInfo->m_arrowBegin.m_nShape = pElement->GetBeginArrow (&pNewInfo->m_arrowBegin.m_dLength, &pNewInfo->m_arrowBegin.m_dWidth);
		pNewInfo->m_arrowBegin.m_brFill = pElement->GetBeginArrowFillBrush ();
		pNewInfo->m_arrowBegin.m_brOutline = pElement->GetBeginArrowOutlineBrush ();
		pNewInfo->m_arrowEnd.m_nShape = pElement->GetEndArrow (&pNewInfo->m_arrowEnd.m_dLength, &pNewInfo->m_arrowEnd.m_dWidth);
		pNewInfo->m_arrowEnd.m_brFill = pElement->GetEndArrowFillBrush ();
		pNewInfo->m_arrowEnd.m_brOutline = pElement->GetEndArrowOutlineBrush ();

		for(int i = 0; i < pElement->GetPointCount (); i++)
		{
			CBCGPDiagramAnchorPoint& point = (const_cast<CBCGPDiagramConnector*>(pElement))->AnchorPoint (i);

			CBCGPVisualInfo::XDiagramAnchorPoint* pPointInfo = new CBCGPVisualInfo::XDiagramAnchorPoint;

			pPointInfo->m_idObject.m_nID        = point.m_idObject.m_nId;
			pPointInfo->m_idObject.m_bConnector = point.m_idObject.m_bConnector;
			pPointInfo->m_nConnectionPort       = point.m_nConnectionPort;
			pPointInfo->m_ptNullAnchor          = point.m_ptNullAnchor;

			pNewInfo->m_arPoints.Add (pPointInfo);
		}

		if (element.IsKindOf (RUNTIME_CLASS (CBCGPDiagramShelfConnector)))
		{
			((CBCGPVisualInfo::XElementDiagramConnectorShelf*)pNewInfo)->m_dShelfOffset = 
				((CBCGPDiagramShelfConnector*)pElement)->GetShelfSize ();
		}
		else if (element.IsKindOf (RUNTIME_CLASS (CBCGPDiagramElbowConnector)))
		{
			((CBCGPVisualInfo::XElementDiagramConnectorElbow*)pNewInfo)->m_Orientation = 
				((const CBCGPDiagramElbowConnector*)pElement)->GetOrientation ();
			((CBCGPVisualInfo::XElementDiagramConnectorElbow*)pNewInfo)->m_ptResizeHandle = 
				((const CBCGPDiagramElbowConnector*)pElement)->GetResizeHandlePoint ();
		}
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPDiagramTableShape)))
	{
		CBCGPVisualInfo::XElementDiagramTable* pNewInfo = new CBCGPVisualInfo::XElementDiagramTable;
		info = pNewInfo;

		CollectBaseDiagramElement (element, *info);

		const CBCGPDiagramTableShape* pElement = (const CBCGPDiagramTableShape*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_shape         = pElement->GetShape();
		pNewInfo->m_bCaption      = pElement->m_bCaption;
		pNewInfo->m_brCaptionFill = pElement->m_brCaptionFill;
		
		pNewInfo->m_CaptionData.m_strText = pElement->m_CaptionData.GetText ();
		pNewInfo->m_CaptionData.m_brText  = pElement->m_CaptionData.GetTextBrush ();
		pNewInfo->m_CaptionData.m_fmtText = pElement->m_CaptionData.GetTextFormat ();
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPDiagramShape)))
	{
		CBCGPVisualInfo::XElementDiagramShape* pNewInfo = new CBCGPVisualInfo::XElementDiagramShape;
		info = pNewInfo;

		CollectBaseDiagramElement (element, *info);

		const CBCGPDiagramShape* pElement = (const CBCGPDiagramShape*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_shape = pElement->GetShape();
	}
	else if (element.IsKindOf (RUNTIME_CLASS (CBCGPDiagramImageObject)))
	{
		CBCGPVisualInfo::XElementDiagramImage* pNewInfo = new CBCGPVisualInfo::XElementDiagramImage;
		info = pNewInfo;

		CollectBaseDiagramElement (element, *info);

		const CBCGPDiagramImageObject* pElement = (const CBCGPDiagramImageObject*)&element;
		ASSERT_VALID (pElement);

		pNewInfo->m_AlignHorz = pElement->GetHorizontalAlign ();
		pNewInfo->m_AlignVert = pElement->GetVerticalAlign ();
		pNewInfo->m_bLockAspectRatio = pElement->IsAspectRatioLocked ();

		CollectImage (pElement->m_Image, pNewInfo->m_Image);
	}
	else
	{
		CBCGPVisualInfo::XElementDiagramCustom* pNewInfo = new CBCGPVisualInfo::XElementDiagramCustom;
		info = pNewInfo;

		CollectBaseDiagramElement (element, *info);
	}

	if (info != NULL)
	{
		element.ToTag(info->m_strCustomName, info->m_strCustomProps);
	}

	return info;
}

void CBCGPVisualCollector::CollectBaseDiagramElement (const CBCGPDiagramVisualObject& element, CBCGPVisualInfo::XDiagramElement& info)
{
	CollectBaseElement(element, info);

	for (int i = 0; i < element.GetDataCount (); i++)
	{
		CBCGPVisualInfo::XDiagramTextData* pDataInfo = NULL;

		const CBCGPVisualDataObject* pDataObject = element.GetData (i);
		if (pDataObject == NULL)
		{
			continue;
		}

		if (pDataObject->IsKindOf (RUNTIME_CLASS(CBCGPDiagramTextDataObject)))
		{
			const CBCGPDiagramTextDataObject* pData = (const CBCGPDiagramTextDataObject*)pDataObject;
			CBCGPVisualInfo::XDiagramTextData* pDataText = new CBCGPVisualInfo::XDiagramTextData;

			pDataText->m_strText = pData->GetText ();
			pDataText->m_brText  = pData->GetTextBrush ();
			pDataText->m_fmtText = pData->GetTextFormat ();

			pDataInfo = pDataText;
		}

		if (pDataInfo != NULL)
		{
			info.m_arDataObjects.Add(pDataInfo);
		}
	}

	info.m_idItem.m_nID        = element.GetItemID ().m_nId;
	info.m_idItem.m_bConnector = element.GetItemID ().m_bConnector;
	info.m_brFill              = element.GetFillBrush ();
	info.m_brOutline           = element.GetOutlineBrush ();
	info.m_brShadow            = element.GetShadowBrush ();
}

void CBCGPVisualCollector::CollectImage (const CBCGPImage& image, CBCGPVisualInfo::XImage& info)
{
	if ((m_dwFlags & e_CollectTreeMapData) == 0)
	{
		return;
	}

	info.m_ID.m_Value           = image.GetResourceID ();
	info.m_strType              = image.GetResourceType ();
	info.m_strPath              = image.GetPath ();
	info.m_sizeIcon             = image.GetIconSize ();
	info.m_bIsAlphaIcon         = image.IsAlphaIcon ();
	info.m_bIsIgnoreAlphaBitmap = image.IsIgnoreAlphaBitmap ();
}
