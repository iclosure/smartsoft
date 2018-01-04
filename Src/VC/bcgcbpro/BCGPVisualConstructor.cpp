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
// BCGPVisualConstructor.cpp: implementation of the CBCGPRibbonConstrucor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPVisualConstructor.h"
#include "BCGPVisualContainer.h"
#include "BCGPTextGaugeImpl.h"
#include "BCGPAnalogClock.h"
#include "BCGPImageGaugeImpl.h"
#include "BCGPDiagramVisualObject.h"
#include "BCGPDiagramVisualContainer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

class XBCGPTreeMapGroup: public CBCGPTreeMapGroup
{
	friend CBCGPBaseTreeMapNode* CreateTreeMapData(const CBCGPVisualInfo::XTreeMapData& info);
};

inline CBCGPBaseTreeMapNode* CreateTreeMapData(const CBCGPVisualInfo::XTreeMapData& info)
{
	CBCGPBaseTreeMapNode* pData = NULL;

	if (info.GetName ().Compare (CBCGPVisualInfo::s_szTreeMapGroup) == 0)
	{
		const CBCGPVisualInfo::XTreeMapGroup& infoGroup = 
			(const CBCGPVisualInfo::XTreeMapGroup&)info;

		XBCGPTreeMapGroup* pGroup = (XBCGPTreeMapGroup*)new CBCGPTreeMapGroup(infoGroup.m_brFill, infoGroup.m_strLabel);
		pData = pGroup;

		pGroup->SetMargin (infoGroup.m_szMargin);
		pGroup->m_brText = infoGroup.m_brText;
		pGroup->m_tf     = infoGroup.m_fmtText;

		for (int i = 0; i < (int)infoGroup.m_arNodes.GetSize (); i++)
		{
			CBCGPBaseTreeMapNode* pNode = CreateTreeMapData (*infoGroup.m_arNodes[i]);
			if (pNode != NULL)
			{
				pGroup->AddSubNode (pNode);
			}
		}
	}
	else
	{
		const CBCGPVisualInfo::XTreeMapNode& infoNode = 
			(const CBCGPVisualInfo::XTreeMapNode&)info;

		CBCGPTreeMapNode* pNode = new CBCGPTreeMapNode(infoNode.m_dblValue, infoNode.m_strLabel);
		pData = pNode;
	}

	return pData;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPVisualConstructor::CBCGPVisualConstructor(const CBCGPVisualInfo& info)
	: m_Info(info)
{
}

CBCGPVisualConstructor::~CBCGPVisualConstructor()
{
}

void CBCGPVisualConstructor::Construct (CBCGPVisualContainer& container) const
{
	const CBCGPVisualInfo::XContainer& infoContainer = GetInfo ().GetContainer ();

	if (container.IsKindOf (RUNTIME_CLASS(CBCGPDiagramVisualContainer)))
	{
		CArray<CBCGPBaseVisualObject*, CBCGPBaseVisualObject*> arConnectors;
		CArray<CBCGPBaseVisualObject*, CBCGPBaseVisualObject*> arItems;

		int i = 0;
		for (i = 0; i < (int)infoContainer.m_arElements.GetSize (); i++)
		{
			CBCGPBaseVisualObject* pElement = CreateElement ((const CBCGPVisualInfo::XElement&)*infoContainer.m_arElements[i], container);
			if (pElement != NULL)
			{
				ASSERT_VALID(pElement);

				if (pElement->IsKindOf (RUNTIME_CLASS(CBCGPDiagramConnector)))
				{
					arConnectors.Add (pElement);
				}
				else
				{
					arItems.Add (pElement);
				}
			}
		}

		CBCGPDiagramVisualContainer& diagram = (CBCGPDiagramVisualContainer&)container;

		for (i = 0; i < (int)arItems.GetSize (); i++)
		{
			diagram.AddItem(arItems[i], arItems[i]->IsAutoDestroy ());
		}

		for (i = 0; i < (int)arConnectors.GetSize (); i++)
		{
			CBCGPDiagramConnector* pConnector = (CBCGPDiagramConnector*)arConnectors[i];

			diagram.AddConnector(pConnector, arConnectors[i]->IsAutoDestroy ());

			if (pConnector->IsKindOf (RUNTIME_CLASS (CBCGPDiagramShelfConnector)))
			{
				((CBCGPDiagramShelfConnector*)pConnector)->RecalcPoints ();
			}
			else if (pConnector->IsKindOf (RUNTIME_CLASS (CBCGPDiagramElbowConnector)))
			{
				((CBCGPDiagramElbowConnector*)pConnector)->RecalcPoints ();
			}
		}
	}
	else
	{
		for (int i = 0; i < (int)infoContainer.m_arElements.GetSize (); i++)
		{
			CBCGPBaseVisualObject* pElement = CreateElement ((const CBCGPVisualInfo::XElement&)*infoContainer.m_arElements[i], container);
			if (pElement != NULL)
			{
				ASSERT_VALID (pElement);

				container.Add(pElement);
			}
		}
	}

	container.SetDrawDynamicObjectsOnTop (infoContainer.m_bDrawDynamicObjectsOnTop);

	if (!infoContainer.m_Rect.IsRectEmpty ())
	{
		container.SetRect (infoContainer.m_Rect, TRUE, FALSE);
	}

	container.SetFillBrush (infoContainer.m_brFill);
	container.SetOutlineBrush (infoContainer.m_brOutline);
}

CBCGPBaseVisualObject* CBCGPVisualConstructor::CreateElement (const CBCGPVisualInfo::XElement& info, const CBCGPVisualContainer& container) const
{
	CBCGPBaseVisualObject* pElement = NULL;

	if (info.GetElementName ().Compare (CBCGPVisualInfo::s_szTagCloud) == 0)
	{
		const CBCGPVisualInfo::XElementTagCloud& infoElement = 
			(const CBCGPVisualInfo::XElementTagCloud&)info;

		CBCGPTagCloud* pNewElement = new CBCGPTagCloud;

		pElement = pNewElement;

		ConstructBaseElement (*pElement, info);

		pNewElement->SetSortOrder (infoElement.m_SortOrder);
		pNewElement->SetSortDescending (infoElement.m_bDescending);
		pNewElement->SetMaxWeight (infoElement.m_nMaxWeight);
		pNewElement->SetFontSizeStep (infoElement.m_dblFontSizeStep);
		pNewElement->SetHorzMargin (infoElement.m_szMargin.cx);
		pNewElement->SetVertMargin (infoElement.m_szMargin.cy);
		pNewElement->SetTextFormat (infoElement.m_fmtBase);
		pNewElement->SetFillBrush (infoElement.m_brFill);
		pNewElement->SetTextColor (infoElement.m_clrText);
		pNewElement->SetHighlightedTextColor (infoElement.m_clrTextHighlighted);
		
		for (int i = 0; i < (int)infoElement.m_arDataObjects.GetSize (); i++)
		{
			const CBCGPVisualInfo::XTagCloudData* pDataInfo = infoElement.m_arDataObjects[i];

			CBCGPTagCloudElement* pTag = new CBCGPTagCloudElement(pDataInfo->m_strLabel, 
				pDataInfo->m_dblValue, pDataInfo->m_clrText, pDataInfo->m_brFill, pDataInfo->m_clrBorder);
			pTag->SetHighlightedColor (pDataInfo->m_clrTextHighlighted);

			pNewElement->Add (pTag);
		}
	}
	else if (info.GetElementName ().Compare (CBCGPVisualInfo::s_szTreeMap) == 0)
	{
		const CBCGPVisualInfo::XElementTreeMap& infoElement = 
			(const CBCGPVisualInfo::XElementTreeMap&)info;

		CBCGPTreeMap* pNewElement = new CBCGPTreeMap;

		pElement = pNewElement;

		ConstructBaseElement (*pElement, info);

		pNewElement->SetLayoutType (infoElement.m_LayoutType);
		pNewElement->SetGroupMargin (infoElement.m_Root.m_szMargin);
		pNewElement->SetFillBrush (infoElement.m_Root.m_brFill);

		for (int i = 0; i < (int)infoElement.m_Root.m_arNodes.GetSize (); i++)
		{
			CBCGPBaseTreeMapNode* pNode = CreateTreeMapData (*infoElement.m_Root.m_arNodes[i]);

			CBCGPTreeMapGroup* pGroup = DYNAMIC_DOWNCAST(CBCGPTreeMapGroup, pNode);
			if (pGroup == NULL)
			{
				if (pNode != NULL)
				{
					delete pNode;
					continue;
				}
			}

 			pNewElement->AddGroup(pGroup);
		}
	}
	else if (info.GetElementName ().Compare (CBCGPVisualInfo::s_szCircularGauge) == 0 ||
		info.GetElementName ().Compare (CBCGPVisualInfo::s_szKnob) == 0 ||
		info.GetElementName ().Compare (CBCGPVisualInfo::s_szAnalogClock) == 0 ||
		info.GetElementName ().Compare (CBCGPVisualInfo::s_szLinearGauge) == 0 ||
		info.GetElementName ().Compare (CBCGPVisualInfo::s_szNumericInd) == 0 ||
		info.GetElementName ().Compare (CBCGPVisualInfo::s_szColorInd) == 0 ||
		info.GetElementName ().Compare (CBCGPVisualInfo::s_szTextInd) == 0 ||
		info.GetElementName ().Compare (CBCGPVisualInfo::s_szImage) == 0)
	{
		pElement = CreateGaugeElement((const CBCGPVisualInfo::XGaugeElement&)info, container);
	}
	else if (info.GetElementName ().Compare (CBCGPVisualInfo::s_szDiagramConnector) == 0 ||
		info.GetElementName ().Compare (CBCGPVisualInfo::s_szDiagramConnectorShelf) == 0 ||
		info.GetElementName ().Compare (CBCGPVisualInfo::s_szDiagramConnectorElbow) == 0 ||
		info.GetElementName ().Compare (CBCGPVisualInfo::s_szDiagramShape) == 0 ||
		info.GetElementName ().Compare (CBCGPVisualInfo::s_szDiagramTable) == 0 ||
		info.GetElementName ().Compare (CBCGPVisualInfo::s_szDiagramImage) == 0 ||
		info.GetElementName ().Compare (CBCGPVisualInfo::s_szDiagramCustom) == 0)
	{
		pElement = CreateDiagramElement((const CBCGPVisualInfo::XDiagramElement&)info, container);
	}

	return pElement;
}

void CBCGPVisualConstructor::ConstructBaseElement (CBCGPBaseVisualObject& element, const CBCGPVisualInfo::XElement& info) const
{
	element.SetName (info.m_ID.m_Name);
	element.SetID (info.m_ID.m_Value);
	element.SetRect (info.m_Rect);
	element.SetVisible (info.m_bIsVisible);
	element.SetAutoDestroy(info.m_bIsAutoDestroy);
}

CBCGPGaugeImpl* CBCGPVisualConstructor::CreateGaugeElement (const CBCGPVisualInfo::XGaugeElement& info, const CBCGPVisualContainer& container) const
{
	CBCGPGaugeImpl* pElement = NULL;

	if (info.GetElementName ().Compare (CBCGPVisualInfo::s_szCircularGauge) == 0 ||
		info.GetElementName ().Compare (CBCGPVisualInfo::s_szKnob) == 0 ||
		info.GetElementName ().Compare (CBCGPVisualInfo::s_szAnalogClock) == 0)
	{
		BOOL bKnob = info.GetElementName ().Compare (CBCGPVisualInfo::s_szKnob) == 0;
		BOOL bAnalogClock = info.GetElementName ().Compare (CBCGPVisualInfo::s_szAnalogClock) == 0;
		BOOL bAnalogClockChecked = FALSE;

		const CBCGPVisualInfo::XElementCircular& infoElement = 
			(const CBCGPVisualInfo::XElementCircular&)info;

		CBCGPCircularGaugeImpl* pNewElement = NULL;
		if (bKnob)
		{
			pNewElement = new CBCGPKnob;
		}
		else if (bAnalogClock)
		{
			pNewElement = new CBCGPAnalogClock;
			bAnalogClockChecked = ((CBCGPVisualInfo::XElementAnalogClock&)info).m_nDateIndex == -1;
		}
		else
		{
			pNewElement = new CBCGPCircularGaugeImpl;
		}

		pElement = pNewElement;

		ConstructGaugeElement (*pElement, info);

		pNewElement->SetColors (infoElement.m_Colors);

		if (!infoElement.m_fmtText.IsEmpty())
		{
			pNewElement->SetTextFormat (infoElement.m_fmtText);
		}

		pNewElement->SetCapSize (infoElement.m_dblCapSize);

		if (!bKnob && !bAnalogClock)
		{
			pNewElement->EnableShapeByTicksArea (infoElement.m_bShapeByTicksArea);
		}

		int i = 0;
		for (i = 0; i < (int)infoElement.m_arScales.GetSize (); i++)
		{
			const CBCGPVisualInfo::XCircularScale* pScaleInfo = 
				(const CBCGPVisualInfo::XCircularScale*)infoElement.m_arScales[i];

			int index = 0;
			if (i != 0)
			{
				index = pNewElement->AddScale ();
			}

			if (pScaleInfo->m_bIsClosed)
			{
				pNewElement->SetClosedRange (pScaleInfo->m_dblStart, pScaleInfo->m_dblFinish, pScaleInfo->m_dblStartAngle, 
					pScaleInfo->m_bDrawLastTickMark, pScaleInfo->m_bAnimationThroughStart, 
					index);
			}
			else
			{
				pNewElement->SetRange (pScaleInfo->m_dblStart, pScaleInfo->m_dblFinish, index);
				pNewElement->SetTicksAreaAngles (pScaleInfo->m_dblStartAngle, pScaleInfo->m_dblFinishAngle, index);
			}
			pNewElement->SetStep (pScaleInfo->m_dblStep, index);
			pNewElement->SetTextLabelFormat (pScaleInfo->m_strLabelFormat, index);
			pNewElement->SetTickMarkStyle (pScaleInfo->m_MinorTickMarkStyle, FALSE, pScaleInfo->m_dblMinorTickMarkSize, index);
			pNewElement->SetTickMarkStyle (pScaleInfo->m_MajorTickMarkStyle, TRUE, pScaleInfo->m_dblMajorTickMarkSize, index);
			pNewElement->SetMajorTickMarkStep (pScaleInfo->m_dblMajorTickMarkStep, index);
			pNewElement->SetScaleFillBrush (pScaleInfo->m_brFill, index);
			pNewElement->SetScaleOutlineBrush (pScaleInfo->m_brOutline, index);
			pNewElement->SetScaleTextBrush (pScaleInfo->m_brText, index);
			pNewElement->SetScaleTickMarkBrush (pScaleInfo->m_brTickMarkMinor, FALSE, index);
			pNewElement->SetScaleTickMarkBrush (pScaleInfo->m_brTickMarkMajor, TRUE, index);
			pNewElement->SetScaleOffsetFromFrame (pScaleInfo->m_dblOffsetFromFrame, index);
			pNewElement->EnableLabelsRotation (pScaleInfo->m_bRotateLabels, index);
		}

		if (bKnob)
		{
			if ((int)infoElement.m_arPointers.GetSize () > 0)
			{
				const CBCGPVisualInfo::XKnobPointer* pPointerInfo = 
					(const CBCGPVisualInfo::XKnobPointer*)infoElement.m_arPointers[0];

				CBCGPKnobPointer pointer(pPointerInfo->m_brFill, 
						pPointerInfo->m_brOutline, pPointerInfo->m_Style, pPointerInfo->m_dblOffsetFromCenter, 
						pPointerInfo->m_dblWidth);

				((CBCGPKnob*)pNewElement)->SetPointer (pointer, FALSE);
				pNewElement->SetValue (pPointerInfo->m_dblValue, 0, 0, FALSE);
			}
		}
		else
		{
			int nDefaultPointers = 1;
			if (bAnalogClock)
			{
				nDefaultPointers = (int)infoElement.m_arPointers.GetSize ();
				if (nDefaultPointers == 3)
				{
					((CBCGPAnalogClock*)pNewElement)->EnableSecondHand ();
				}
			}

			for (i = 0; i < (int)infoElement.m_arPointers.GetSize (); i++)
			{
				const CBCGPVisualInfo::XCircularPointer* pPointerInfo = 
					(const CBCGPVisualInfo::XCircularPointer*)infoElement.m_arPointers[i];

				CBCGPCircularGaugePointer pointer(pPointerInfo->m_brFill, 
						pPointerInfo->m_brOutline, pPointerInfo->m_Style, pPointerInfo->m_dblSize, 
						pPointerInfo->m_dblWidth, pPointerInfo->m_bExtraLen);

				if (i >= nDefaultPointers)
				{
					pNewElement->AddPointer (pointer, pPointerInfo->m_nScale, FALSE);
				}
				else
				{
					pNewElement->ModifyPointer (i, pointer, FALSE);
				}

				if (!bAnalogClock)
				{
					pNewElement->SetValue (pPointerInfo->m_dblValue, i, 0, FALSE);
				}
			}
		}

		for (i = 0; i < (int)infoElement.m_arRanges.GetSize (); i++)
		{
			const CBCGPVisualInfo::XCircularColoredRange* pRangeInfo = 
				(const CBCGPVisualInfo::XCircularColoredRange*)infoElement.m_arRanges[i];

			pNewElement->AddColoredRange (pRangeInfo->m_dblStartValue, pRangeInfo->m_dblFinishValue, pRangeInfo->m_brFill,
				pRangeInfo->m_brOutline, pRangeInfo->m_nScale, pRangeInfo->m_dblStartWidth, pRangeInfo->m_dblFinishWidth, 
				pRangeInfo->m_dblOffsetFromFrame, pRangeInfo->m_brTextLabel, pRangeInfo->m_brTickMarkOutline, pRangeInfo->m_brTickMarkFill, FALSE);
		}

		for (i = 0; i < (int)infoElement.m_arSubGauges.GetSize (); i++)
		{
			CBCGPVisualInfo::XGaugeElement* pSubGaugeInfo = infoElement.m_arSubGauges[i];
			CBCGPGaugeImpl* pSubGauge = CreateGaugeElement (*pSubGaugeInfo, container);
			if (pSubGauge != NULL)
			{
				pNewElement->AddSubGauge (pSubGauge, pSubGaugeInfo->m_Pos, pSubGaugeInfo->m_Rect.Size (), pSubGaugeInfo->m_ptOffset);
				
				if (bAnalogClock && !bAnalogClockChecked)
				{
					if (((CBCGPVisualInfo::XElementAnalogClock&)info).m_nDateIndex == i &&
						pSubGauge->IsKindOf (RUNTIME_CLASS(CBCGPNumericIndicatorImpl)))
					{
						((CBCGPAnalogClock*)pNewElement)->m_pDate = (CBCGPNumericIndicatorImpl*)pSubGauge;
						bAnalogClockChecked = TRUE;
					}
				}
			}
		}
	}
	else if (info.GetElementName ().Compare (CBCGPVisualInfo::s_szLinearGauge) == 0)
	{
		const CBCGPVisualInfo::XElementLinear& infoElement = 
			(const CBCGPVisualInfo::XElementLinear&)info;

		CBCGPLinearGaugeImpl* pNewElement = new CBCGPLinearGaugeImpl;

		pElement = pNewElement;

		ConstructGaugeElement (*pElement, info);

		pNewElement->SetColors (infoElement.m_Colors);
		if (!infoElement.m_fmtText.IsEmpty())
		{
			pNewElement->SetTextFormat (infoElement.m_fmtText);
		}
		pNewElement->SetVerticalOrientation (infoElement.m_bIsVertical);

		int i = 0;
		for (i = 0; i < (int)infoElement.m_arScales.GetSize (); i++)
		{
			const CBCGPVisualInfo::XLinearScale* pScaleInfo = 
				(const CBCGPVisualInfo::XLinearScale*)infoElement.m_arScales[i];

			int index = 0;
			if (i != 0)
			{
				index = pNewElement->AddScale ();
			}

			pNewElement->SetRange (pScaleInfo->m_dblStart, pScaleInfo->m_dblFinish, index);
			pNewElement->SetStep (pScaleInfo->m_dblStep, index);
			pNewElement->SetTextLabelFormat (pScaleInfo->m_strLabelFormat, index);
			pNewElement->SetTickMarkStyle (pScaleInfo->m_MinorTickMarkStyle, FALSE, pScaleInfo->m_dblMinorTickMarkSize, index);
			pNewElement->SetTickMarkStyle (pScaleInfo->m_MajorTickMarkStyle, TRUE, pScaleInfo->m_dblMajorTickMarkSize, index);
			pNewElement->SetMajorTickMarkStep (pScaleInfo->m_dblMajorTickMarkStep, index);
			pNewElement->SetScaleFillBrush (pScaleInfo->m_brFill, index);
			pNewElement->SetScaleOutlineBrush (pScaleInfo->m_brOutline, index);
			pNewElement->SetScaleTextBrush (pScaleInfo->m_brText, index);
			pNewElement->SetScaleTickMarkBrush (pScaleInfo->m_brTickMarkMinor, FALSE, index);
			pNewElement->SetScaleTickMarkBrush (pScaleInfo->m_brTickMarkMajor, TRUE, index);
			pNewElement->SetScaleOffsetFromFrame (pScaleInfo->m_dblOffsetFromFrame, index);
		}

		for (i = 0; i < (int)infoElement.m_arPointers.GetSize (); i++)
		{
			const CBCGPVisualInfo::XLinearPointer* pPointerInfo = 
				(const CBCGPVisualInfo::XLinearPointer*)infoElement.m_arPointers[i];

			CBCGPLinearGaugePointer pointer(pPointerInfo->m_brFill, 
					pPointerInfo->m_brOutline, pPointerInfo->m_Style, pPointerInfo->m_dblSize, 
					pPointerInfo->m_dblWidth);

			int index = pNewElement->AddPointer (pointer, pPointerInfo->m_nScale, FALSE);
			pNewElement->SetValue (pPointerInfo->m_dblValue, index, 0, FALSE);
		}

		for (i = 0; i < (int)infoElement.m_arRanges.GetSize (); i++)
		{
			const CBCGPVisualInfo::XLinearColoredRange* pRangeInfo = 
				(const CBCGPVisualInfo::XLinearColoredRange*)infoElement.m_arRanges[i];

			pNewElement->AddColoredRange (pRangeInfo->m_dblStartValue, pRangeInfo->m_dblFinishValue, pRangeInfo->m_brFill,
				pRangeInfo->m_brOutline, pRangeInfo->m_nScale, pRangeInfo->m_dblStartWidth, pRangeInfo->m_dblFinishWidth, 
				pRangeInfo->m_dblOffsetFromFrame, pRangeInfo->m_brTextLabel, pRangeInfo->m_brTickMarkOutline, pRangeInfo->m_brTickMarkFill, FALSE);
		}
	}
	else if (info.GetElementName ().Compare (CBCGPVisualInfo::s_szNumericInd) == 0)
	{
		const CBCGPVisualInfo::XElementNumeric& infoElement = 
			(const CBCGPVisualInfo::XElementNumeric&)info;

		CBCGPNumericIndicatorImpl* pNewElement = new CBCGPNumericIndicatorImpl;

		pElement = pNewElement;

		ConstructGaugeElement (*pElement, info);

		pNewElement->SetStyle (infoElement.m_Style);
		pNewElement->SetColors (infoElement.m_Colors);
		if (!infoElement.m_fmtText.IsEmpty())
		{
			pNewElement->SetTextFormat (infoElement.m_fmtText);
		}
		pNewElement->SetCells (infoElement.m_nCells);
		pNewElement->SetDecimals (infoElement.m_nDecimals);
		pNewElement->SetSeparatorWidth (infoElement.m_nSeparatorWidth);
		pNewElement->SetDrawSign (infoElement.m_bDrawSign);
		pNewElement->SetDrawDecimalPoint (infoElement.m_bDrawDecimalPoint);
		pNewElement->SetDrawLeadingZeros (infoElement.m_bDrawLeadingZeros);
		pNewElement->SetValue (infoElement.m_dblValue);
	}
	else if (info.GetElementName ().Compare (CBCGPVisualInfo::s_szColorInd) == 0)
	{
		const CBCGPVisualInfo::XElementColor& infoElement = 
			(const CBCGPVisualInfo::XElementColor&)info;

		CBCGPColorIndicatorImpl* pNewElement = new CBCGPColorIndicatorImpl;

		pElement = pNewElement;

		ConstructGaugeElement (*pElement, info);

		pNewElement->SetColors (infoElement.m_Colors);
		pNewElement->SetStyle (infoElement.m_Style);
		pNewElement->SetStretched (infoElement.m_bStretched);
	}
	else if (info.GetElementName ().Compare (CBCGPVisualInfo::s_szTextInd) == 0)
	{
		const CBCGPVisualInfo::XElementText& infoElement = 
			(const CBCGPVisualInfo::XElementText&)info;

		CBCGPTextGaugeImpl* pNewElement = new CBCGPTextGaugeImpl;

		pElement = pNewElement;

		ConstructGaugeElement (*pElement, info);

		pNewElement->SetTextBrush (infoElement.m_brText);
		if (!infoElement.m_fmtText.IsEmpty())
		{
			pNewElement->SetTextFormat (infoElement.m_fmtText);
		}
		pNewElement->SetText (infoElement.m_strText, infoElement.m_brText);
	}
	else if (info.GetElementName ().Compare (CBCGPVisualInfo::s_szImage) == 0)
	{
		const CBCGPVisualInfo::XElementImage& infoElement = 
			(const CBCGPVisualInfo::XElementImage&)info;

		CBCGPImageGaugeImpl* pNewElement = new CBCGPImageGaugeImpl;

		pElement = pNewElement;

		ConstructGaugeElement (*pElement, info);

		ConstructImage(pNewElement->m_Image, infoElement.m_Image);
	}

	return pElement;
}

void CBCGPVisualConstructor::ConstructGaugeElement (CBCGPGaugeImpl& element, const CBCGPVisualInfo::XGaugeElement& info) const
{
	ConstructBaseElement(element, info);

	element.SetInteractiveMode (info.m_bIsInteractiveMode);
	element.SetToolTip (info.m_strToolTip, info.m_strDescription);
	element.SetFrameSize (info.m_nFrameSize);
}

CBCGPDiagramVisualObject* CBCGPVisualConstructor::CreateDiagramObject(const CBCGPVisualInfo::XDiagramElement& info, const CBCGPVisualContainer& container) const
{
	CBCGPDiagramVisualObject* pElement = NULL;

	if (info.GetElementName ().Compare (CBCGPVisualInfo::s_szDiagramCustom) == 0 ||
		!info.m_strCustomName.IsEmpty ())
	{
		const CBCGPDiagramVisualContainer* pContainer = DYNAMIC_DOWNCAST(CBCGPDiagramVisualContainer, &container);
		if (pContainer != NULL)
		{
			pElement = pContainer->CreateCustomObject(info.m_strCustomName);
		}
	}
	else
	{
		if (info.GetElementName ().Compare (CBCGPVisualInfo::s_szDiagramConnector) == 0)
		{
			pElement = new CBCGPDiagramConnector;
		}
		else if (info.GetElementName ().Compare (CBCGPVisualInfo::s_szDiagramConnectorShelf) == 0)
		{
			pElement = new CBCGPDiagramShelfConnector;
		}
		else if (info.GetElementName ().Compare (CBCGPVisualInfo::s_szDiagramConnectorElbow) == 0)
		{
			pElement = new CBCGPDiagramElbowConnector;
		}
		else if (info.GetElementName ().Compare (CBCGPVisualInfo::s_szDiagramShape) == 0)
		{
			pElement = new CBCGPDiagramShape;
		}
		else if (info.GetElementName ().Compare (CBCGPVisualInfo::s_szDiagramTable) == 0)
		{
			pElement = new CBCGPDiagramTableShape;
		}
		else if (info.GetElementName ().Compare (CBCGPVisualInfo::s_szDiagramImage) == 0)
		{
			pElement = new CBCGPDiagramImageObject;
		}
	}

	return pElement;
}

CBCGPDiagramVisualObject* CBCGPVisualConstructor::CreateDiagramElement(const CBCGPVisualInfo::XDiagramElement& info, const CBCGPVisualContainer& container) const
{
	CBCGPDiagramVisualObject* pElement = CreateDiagramObject(info, container);

	if (pElement == NULL)
	{
		ASSERT(FALSE);
		return NULL;
	}

	if (pElement->IsKindOf (RUNTIME_CLASS (CBCGPDiagramConnector)))
	{
		const CBCGPVisualInfo::XElementDiagramConnector& infoElement = 
			(const CBCGPVisualInfo::XElementDiagramConnector&)info;

		CBCGPDiagramConnector* pNewElement = (CBCGPDiagramConnector*)pElement;

		ConstructDiagramElement (*pElement, info);

		pNewElement->SetCurveType (infoElement.m_curveType);
		pNewElement->SetBeginArrow (infoElement.m_arrowBegin.m_nShape, infoElement.m_arrowBegin.m_dLength, infoElement.m_arrowBegin.m_dWidth);
		pNewElement->SetBeginArrowFillBrush (infoElement.m_arrowBegin.m_brFill);
		pNewElement->SetBeginArrowOutlineBrush (infoElement.m_arrowBegin.m_brOutline);
		pNewElement->SetEndArrow (infoElement.m_arrowEnd.m_nShape, infoElement.m_arrowEnd.m_dLength, infoElement.m_arrowEnd.m_dWidth);
		pNewElement->SetEndArrowFillBrush (infoElement.m_arrowEnd.m_brFill);
		pNewElement->SetEndArrowOutlineBrush (infoElement.m_arrowEnd.m_brOutline);

		for(int i = 0; i < (int)infoElement.m_arPoints.GetSize (); i++)
		{
			const CBCGPVisualInfo::XDiagramAnchorPoint* pPointInfo = infoElement.m_arPoints[i];

			CBCGPDiagramAnchorPoint point;
			point.m_idObject.m_nId        = pPointInfo->m_idObject.m_nID;
			point.m_idObject.m_bConnector = pPointInfo->m_idObject.m_bConnector;
			point.m_nConnectionPort       = pPointInfo->m_nConnectionPort;
			point.m_ptNullAnchor          = pPointInfo->m_ptNullAnchor;

			pNewElement->m_arPoints.Add (point);
		}

		if (pElement->IsKindOf (RUNTIME_CLASS (CBCGPDiagramShelfConnector)))
		{
			CBCGPDiagramShelfConnector* pConnector = (CBCGPDiagramShelfConnector*)pElement;

			pConnector->SetShelfSize(((const CBCGPVisualInfo::XElementDiagramConnectorShelf&)infoElement).m_dShelfOffset);
		}
		else if (pElement->IsKindOf (RUNTIME_CLASS (CBCGPDiagramElbowConnector)))
		{
			CBCGPDiagramElbowConnector* pConnector = (CBCGPDiagramElbowConnector*)pElement;

			pConnector->SetOrientation(((const CBCGPVisualInfo::XElementDiagramConnectorElbow&)infoElement).m_Orientation);
			pConnector->SetResizeHandlePoint(((const CBCGPVisualInfo::XElementDiagramConnectorElbow&)infoElement).m_ptResizeHandle);
		}
	}
	else if (pElement->IsKindOf (RUNTIME_CLASS (CBCGPDiagramTableShape)))
	{
		const CBCGPVisualInfo::XElementDiagramTable& infoElement = 
			(const CBCGPVisualInfo::XElementDiagramTable&)info;

		CBCGPDiagramTableShape* pNewElement = (CBCGPDiagramTableShape*)pElement;

		ConstructDiagramElement (*pElement, info);

		pNewElement->m_shape         = infoElement.m_shape;
		pNewElement->m_bCaption      = infoElement.m_bCaption;
		pNewElement->m_brCaptionFill = infoElement.m_brCaptionFill;
		
		pNewElement->m_CaptionData.SetText (infoElement.m_CaptionData.m_strText, infoElement.m_CaptionData.m_brText);
		pNewElement->m_CaptionData.SetTextFormat (infoElement.m_CaptionData.m_fmtText);
	}
	else if (pElement->IsKindOf (RUNTIME_CLASS (CBCGPDiagramShape)))
	{
		const CBCGPVisualInfo::XElementDiagramShape& infoElement = 
			(const CBCGPVisualInfo::XElementDiagramShape&)info;

		CBCGPDiagramShape* pNewElement = (CBCGPDiagramShape*)pElement;

		ConstructDiagramElement (*pElement, info);

		pNewElement->m_shape = infoElement.m_shape;
	}
	else if (pElement->IsKindOf (RUNTIME_CLASS (CBCGPDiagramImageObject)))
	{
		const CBCGPVisualInfo::XElementDiagramImage& infoElement = 
			(const CBCGPVisualInfo::XElementDiagramImage&)info;

		CBCGPDiagramImageObject* pNewElement = (CBCGPDiagramImageObject*)pElement;

		ConstructDiagramElement (*pElement, info);

		pNewElement->SetImageAlign (infoElement.m_AlignHorz, infoElement.m_AlignVert, infoElement.m_bLockAspectRatio);

		ConstructImage(pNewElement->m_Image, infoElement.m_Image);
	}
	else
	{
		ConstructDiagramElement (*pElement, info);
	}

	if (pElement != NULL && !info.m_strCustomProps.IsEmpty ())
	{
		pElement->FromTag (info.m_strCustomProps);
	}

	return pElement;
}

void CBCGPVisualConstructor::ConstructDiagramElement (CBCGPDiagramVisualObject& element, const CBCGPVisualInfo::XDiagramElement& info) const
{
	ConstructBaseElement(element, info);

	element.SetItemID (CBCGPDiagramItemID(info.m_idItem.m_nID, info.m_idItem.m_bConnector));
	element.m_brFill    = info.m_brFill;
	element.m_brOutline = info.m_brOutline;
	element.m_brShadow  = info.m_brShadow;

	for (int i = 0; i < (int)info.m_arDataObjects.GetSize (); i++)
	{
		CBCGPVisualDataObject* pData = NULL;

		const CBCGPVisualInfo::XData* pDataObject = info.m_arDataObjects[i];
		if (pDataObject->GetName ().Compare (CBCGPVisualInfo::s_szDataDiagramText) == 0)
		{
			const CBCGPVisualInfo::XDiagramTextData* pDataInfo = 
				(const CBCGPVisualInfo::XDiagramTextData*)pDataObject;

			CBCGPDiagramTextDataObject* pDataText = new CBCGPDiagramTextDataObject(pDataInfo->m_strText, pDataInfo->m_brText);
			pDataText->SetTextFormat (pDataInfo->m_fmtText);

			pData = pDataText;
		}

		if (pData != NULL)
		{
			element.AddData (pData);
		}
	}
}

void CBCGPVisualConstructor::ConstructImage (CBCGPImage& image, const CBCGPVisualInfo::XImage& info) const
{
	if (info.m_ID.m_Value != 0)
	{
		image.Load (info.m_ID.m_Value, info.m_strType.IsEmpty () ? NULL : (LPCTSTR)info.m_strType);
	}
	else if (!info.m_strPath.IsEmpty ())
	{
		image.Load (info.m_strPath);
	}
}
