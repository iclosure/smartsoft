//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of BCGControlBar Library Professional Edition
// Copyright (C) 1998-2012 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPVisualManagerVS2012.cpp: implementation of the CBCGPVisualManagerVS2012 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPVisualManagerVS2012.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPDrawManager.h"
#include "BCGPTabWnd.h"
#include "BCGPAutoHideButton.h"
#include "BCGPColorBar.h"
#include "BCGPCalculator.h"
#include "BCGPCalendarBar.h"
#include "bcgpstyle.h"
#include "BCGPAutoHideDockBar.h"
#include "BCGPSlider.h"
#include "BCGPStatusBar.h"
#include "BCGPToolbarEditBoxButton.h"
#include "BCGPToolBox.h"
#include "BCGPPropList.h"
#include "BCGPOutlookButton.h"
#include "CustomizeButton.h"
#include "BCGPCustomizeMenuButton.h"
#include "BCGPToolbarComboBoxButton.h"
#include "BCGPGroup.h"
#include "BCGPReBar.h"
#include "BCGPGridCtrl.h"
#include "BCGPMiniFrameWnd.h"
#include "BCGPCaptionBar.h"
#include "BCGPURLLinkButton.h"
#include "BCGPPopupWindow.h"
#include "BCGPBreadcrumb.h"

#ifndef BCGP_EXCLUDE_PLANNER
#include "BCGPPlannerViewDay.h"
#include "BCGPPlannerViewMonth.h"
#include "BCGPPlannerViewMulti.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CBCGPVisualManagerVS2012, CBCGPVisualManagerVS2010)
IMPLEMENT_DYNCREATE(CBCGPVisualManagerVS11, CBCGPVisualManagerVS2012)	// Backward compatibility

BOOL CBCGPVisualManagerVS2012::m_bAutoGrayscaleImages = TRUE;
COLORREF CBCGPVisualManagerVS2012::m_clrAccent = RGB(19, 130, 206);
CBCGPVisualManagerVS2012::Style CBCGPVisualManagerVS2012::m_Style = CBCGPVisualManagerVS2012::VS2012_Light;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPVisualManagerVS2012::CBCGPVisualManagerVS2012()
{
	OnUpdateSystemColors ();
	m_bConnectMenuToParent = TRUE;
}
//**************************************************************************************************************
CBCGPVisualManagerVS2012::~CBCGPVisualManagerVS2012()
{
}
//**************************************************************************************************************
void CBCGPVisualManagerVS2012::SetAccentColor(AccentColor color)
{
	SetAccentColorRGB(AccentColorToRGB(color));
}
//**************************************************************************************************************
void CBCGPVisualManagerVS2012::SetAccentColorRGB(COLORREF color)
{
	m_clrAccent = color;

	CBCGPVisualManagerVS2012* pThis = DYNAMIC_DOWNCAST(CBCGPVisualManagerVS2012, CBCGPVisualManager::GetInstance ());
	if (pThis != NULL)
	{
		pThis->OnUpdateSystemColors();
	}
}
//**************************************************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetAccentColorRGB()
{
    return m_clrAccent;
}
//**************************************************************************************************************
void CBCGPVisualManagerVS2012::SetStyle(Style style)
{
	m_Style = style;

	CBCGPVisualManagerVS2012* pThis = DYNAMIC_DOWNCAST(CBCGPVisualManagerVS2012, CBCGPVisualManager::GetInstance ());
	if (pThis != NULL)
	{
		pThis->OnUpdateSystemColors();
	}
}
//**************************************************************************************************************
COLORREF CBCGPVisualManagerVS2012::AccentColorToRGB(AccentColor color)
{
	switch (color)
	{
	case VS2012_Blue:
	default:
		return RGB(19, 130, 206);

	case VS2012_Brown:
		return RGB(160,80,0);

	case VS2012_Green:
		return RGB(51,153,51);

	case VS2012_Lime:
		return RGB(137, 164, 48);

	case VS2012_Magenta:
		return RGB(216,0,115);

	case VS2012_Orange:
		return RGB(240,150,9);

	case VS2012_Pink:
		return RGB(230,113,184);

	case VS2012_Purple:
		return RGB(162,0,255);

	case VS2012_Red:
		return RGB(229,20,0);

	case VS2012_Teal:
		return RGB(0,171,169);
	}
}
//**************************************************************************************************************
void CBCGPVisualManagerVS2012::OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
								CRect rectClient, CRect rectClip,
								BOOL bNCArea)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerXP::OnFillBarBackground (pDC, pBar, rectClient, rectClip, bNCArea);
		return;
	}

	ASSERT_VALID(pBar);
	ASSERT_VALID(pDC);

	if (pBar->IsOnGlass ())
	{
		pDC->FillSolidRect (rectClient, RGB (0, 0, 0));
		return;
	}

	if (DYNAMIC_DOWNCAST (CBCGPReBar, pBar) != NULL || DYNAMIC_DOWNCAST (CBCGPReBar, pBar->GetParent ()))
	{
		FillRebarPane (pDC, pBar, rectClient);
		return;
	}

    if (DYNAMIC_DOWNCAST(CBCGPColorBar, pBar) != NULL)
	{
		if (pBar->IsDialogControl ())
		{
			pDC->FillRect(rectClient, &globalData.brBtnFace);
			return;
		}
		else
		{
			if (DYNAMIC_DOWNCAST(CDialog, pBar->GetParent()) != NULL)
			{
				pDC->FillRect(rectClient, &GetDlgBackBrush(pBar->GetParent()));
				return;
			}
		}
	}

    if (DYNAMIC_DOWNCAST(CBCGPPopupMenuBar, pBar) != NULL)
	{
		pDC->FillRect (rectClip, &m_brMenuLight);
		return;
	}

    if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPCaptionBar)))
	{
		pDC->FillRect(rectClip, &m_brMenuLight);
		return;
	}

#if !defined(BCGP_EXCLUDE_TOOLBOX) && !defined(BCGP_EXCLUDE_TASK_PANE)
 	if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPToolBoxPage)) || pBar->IsKindOf (RUNTIME_CLASS (CBCGPToolBox)) || pBar->IsKindOf (RUNTIME_CLASS (CBCGPToolBoxEx)))
	{
		pDC->FillRect(rectClip, &m_brMenuLight);
		return;
	}
#endif

	if (pBar->IsDialogControl ())
	{
		pDC->FillRect(rectClient, &globalData.brBtnFace);
		return;
	}

	pDC->FillRect(rectClient, &globalData.brBarFace);
}
//*********************************************************************************************************
void CBCGPVisualManagerVS2012::OnHighlightMenuItem (CDC* pDC, CBCGPToolbarMenuButton* pButton,
											CRect rect, COLORREF& clrText)
{
	if ((pButton->m_nStyle & TBBS_DISABLED) && globalData.m_nBitsPerPixel > 8 && !globalData.IsHighContastMode ())
	{
		rect.DeflateRect(1, 0);
		pDC->Draw3dRect(rect, m_clrMenuSeparator, m_clrMenuSeparator);
		return;
	}

	CBCGPVisualManagerXP::OnHighlightMenuItem (pDC, pButton, rect, clrText);
}
//*********************************************************************************************************
void CBCGPVisualManagerVS2012::OnEraseTabsArea (CDC* pDC, CRect rect, const CBCGPBaseTabWnd* pTabWnd)
{
	CBCGPVisualManagerXP::OnEraseTabsArea (pDC, rect, pTabWnd);
}
//*********************************************************************************************************
void CBCGPVisualManagerVS2012::OnEraseTabsButton (CDC* pDC, CRect rect,
											  CBCGPButton* pButton,
											  CBCGPBaseTabWnd* pBaseTab)
{
	CBCGPVisualManagerXP::OnEraseTabsButton (pDC, rect, pButton, pBaseTab);
}
//*********************************************************************************************************
BOOL CBCGPVisualManagerVS2012::OnEraseTabsFrame (CDC* pDC, CRect rect, const CBCGPBaseTabWnd* /*pTabWnd*/)
{
	ASSERT_VALID (pDC);

	pDC->FillRect(rect, &globalData.brBarFace);
	return TRUE;
}
//*********************************************************************************************************
void CBCGPVisualManagerVS2012::OnDrawTabBorder(CDC* pDC, CBCGPTabWnd* pTabWnd, CRect rectBorder, COLORREF clrBorder,
										 CPen& penLine)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pTabWnd);

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () ||
		(pTabWnd->IsDialogControl () && !pTabWnd->IsPropertySheetTab()) || !pTabWnd->IsMDITab() ||
		pTabWnd->IsFlatTab () || pTabWnd->IsOneNoteStyle () || pTabWnd->IsVS2005Style ())
	{
		CBCGPVisualManagerVS2010::OnDrawTabBorder(pDC, pTabWnd, rectBorder, clrBorder, penLine);
		return;
	}

	COLORREF clrActiveTab = pTabWnd->GetTabBkColor (pTabWnd->GetActiveTab ());
	if (clrActiveTab == (COLORREF)-1)
	{
		clrActiveTab =  pTabWnd->IsMDIFocused() && pTabWnd->IsActiveInMDITabGroup() ? m_clrAccent : globalData.clrBarShadow;
	}

	CRect rectTop = rectBorder;
	rectTop.DeflateRect(pTabWnd->GetTabBorderSize() + 1, 0);
	rectTop.bottom = rectTop.top + pTabWnd->GetTabBorderSize();
	rectTop.top++;

	pDC->FillSolidRect(rectTop, pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM ? globalData.clrBarFace : clrActiveTab);

	CRect rectBottom = rectBorder;
	rectBottom.DeflateRect(pTabWnd->GetTabBorderSize() + 1, 0);
	rectBottom.top = rectBottom.bottom - pTabWnd->GetTabBorderSize();
	rectBottom.bottom--;

	pDC->FillSolidRect(rectBottom, pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM ? clrActiveTab : globalData.clrBarFace);
}
//*********************************************************************************************************
void CBCGPVisualManagerVS2012::OnDrawTab (CDC* pDC, CRect rectTab,
						int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pTabWnd);
	ASSERT_VALID (pDC);

	const BOOL bIsHighlight = (iTab == pTabWnd->GetHighlightedTab ());

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () ||
		(pTabWnd->IsDialogControl () && !pTabWnd->IsPropertySheetTab()) ||
		pTabWnd->IsFlatTab () || pTabWnd->IsOneNoteStyle () || pTabWnd->IsVS2005Style () || pTabWnd->IsLeftRightRounded() ||
		pTabWnd->GetTabBkColor(iTab) != (COLORREF)-1)
	{
		CBCGPVisualManagerVS2010::OnDrawTab (pDC, rectTab, iTab, bIsActive, pTabWnd);
		return;
	}

	if (iTab != 0 && !pTabWnd->IsMDITab())
	{
		pDC->MoveTo(rectTab.left, rectTab.top + 1);
		pDC->LineTo(rectTab.left, rectTab.bottom - 1);
		
		rectTab.left++;
	}

	if (bIsActive || bIsHighlight)
	{
		CRect rectFill = rectTab;

		COLORREF clrTab = pTabWnd->GetTabBkColor(iTab);
		if (clrTab == (COLORREF)-1)
		{
			clrTab = globalData.clrBarLight;

			if (bIsActive)
			{
				if (pTabWnd->IsMDITab())
				{
					if (pTabWnd->IsMDIFocused() && pTabWnd->IsActiveInMDITabGroup())
					{
						clrTab = m_clrAccent;
					}
					else
					{
						clrTab = globalData.clrBarShadow;
					}
				}
				else
				{
					clrTab = m_clrHighlight;
				}
			}
			else if (pTabWnd->IsMDITab())
			{
				clrTab = m_clrAccentLight;
			}
		}

		if (pTabWnd->IsMDITab())
		{
			if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
			{
				rectFill.top--;
			}
			else
			{
				rectFill.bottom++;
			}
		}

		pDC->FillSolidRect(rectFill, clrTab);
	}

	COLORREF clrTabText = pTabWnd->GetTabTextColor(iTab);
	if (clrTabText == (COLORREF)-1)
	{
		if (pTabWnd->IsMDITab())
		{
			if (bIsActive)
			{
				if (pTabWnd->IsMDIFocused() && pTabWnd->IsActiveInMDITabGroup())
				{
					clrTabText = RGB(255, 255, 255);
				}
				else
				{
					clrTabText = globalData.clrBarText;
				}
			}
			else if (bIsHighlight)
			{
				clrTabText = RGB(255, 255, 255);
			}
			else
			{
				clrTabText = globalData.clrBarText;
			}
		}
		else
		{
			clrTabText = (bIsActive || bIsHighlight) ? m_clrAccentText : globalData.clrBarText;
		}
	}

	OnDrawTabContent(pDC, rectTab, iTab, bIsActive, pTabWnd, clrTabText);
}
//**********************************************************************************
void CBCGPVisualManagerVS2012::OnDrawTabContent (CDC* pDC, CRect rectTab,
							int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd,
							COLORREF clrText)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () ||
		(pTabWnd->IsDialogControl () && !pTabWnd->IsPropertySheetTab()) ||
		pTabWnd->GetTabBkColor(iTab) != (COLORREF)-1 ||
		clrText != (COLORREF)-1)
	{
	}
	else
	{
		if (!bIsActive)
		{
			pDC->SetTextColor(globalData.clrBarText);
		}
	}

	CBCGPVisualManagerVS2010::OnDrawTabContent (pDC, rectTab, iTab, bIsActive, pTabWnd, clrText);
}
//**********************************************************************************
void CBCGPVisualManagerVS2012::OnDrawTabCloseButton (CDC* pDC, CRect rect, 
											   const CBCGPBaseTabWnd* pTabWnd,
											   BOOL bIsHighlighted,
											   BOOL bIsPressed,
											   BOOL bIsDisabled)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || 
		(pTabWnd->GetTabBkColor(m_nCurrDrawedTab) != (COLORREF)-1 && !bIsHighlighted && !bIsPressed) ||
		((pTabWnd->IsOneNoteStyle () || pTabWnd->IsLeftRightRounded()) && m_nCurrDrawedTab == pTabWnd->GetActiveTab()))
	{
		CBCGPVisualManagerVS2010::OnDrawTabCloseButton (pDC, rect, pTabWnd, bIsHighlighted, bIsPressed, bIsDisabled);
		return;
	}

	rect.DeflateRect(1, 0);

	CBCGPMenuImages::IMAGE_STATE state = (bIsPressed && bIsHighlighted) || m_Style == VS2012_Dark ? CBCGPMenuImages::ImageWhite : 
		CBCGPMenuImages::ImageBlack;

	if (pTabWnd->IsMDITab())
	{
		BOOL bActive = m_nCurrDrawedTab == pTabWnd->GetActiveTab() && pTabWnd->IsMDIFocused() && pTabWnd->IsActiveInMDITabGroup();

		if (bActive)
		{
			state = CBCGPMenuImages::ImageWhite;
		}

		if (bIsPressed && bIsHighlighted)
		{
			if (bActive)
			{
				pDC->FillSolidRect (rect, m_clrAccentText);
			}
			else
			{
				pDC->FillRect(rect, &m_brAccent);
			}

			state = CBCGPMenuImages::ImageWhite;
		}
		else if (bIsHighlighted || bIsPressed)
		{
			pDC->FillRect (rect, &m_brAccentLight);
			state = CBCGPMenuImages::ImageWhite;
		}
	}
	else
	{
		if (bIsHighlighted)
		{
			pDC->FillSolidRect(rect, bIsPressed ? m_clrAccent : globalData.clrBarShadow);
		}

		if (pTabWnd->IsFlatTab() && !bIsHighlighted && m_nCurrDrawedTab == pTabWnd->GetActiveTab())
		{
			state = CBCGPMenuImages::ImageBlack;
		}
	}

	CBCGPMenuImages::Draw (pDC, CBCGPMenuImages::IdClose, rect, state);

	if (bIsHighlighted && !pTabWnd->IsMDITab())
	{
		pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);
	}
}
//*********************************************************************************************************
void CBCGPVisualManagerVS2012::OnDrawAutoHideButtonBorder (CDC* pDC, CRect rect, CRect rectBorderSize, CBCGPAutoHideButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawAutoHideButtonBorder (pDC, rect, rectBorderSize, pButton);
		return;
	}

	DWORD dwAlign = (pButton->GetAlignment () & CBRS_ALIGN_ANY);

	switch (dwAlign)
	{
	case CBRS_ALIGN_LEFT:
	case CBRS_ALIGN_RIGHT:
		rect.bottom--;
		break;

	case CBRS_ALIGN_TOP:
	case CBRS_ALIGN_BOTTOM:
		rect.right--;
		break;
	}

	if (pButton->IsHighlighted ())
	{
		pDC->FillRect(rect, &m_brMenuLight);
	}
}
//*********************************************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetAutoHideButtonTextColor (CBCGPAutoHideButton* pButton)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetAutoHideButtonTextColor (pButton);
	}

	return pButton->IsHighlighted() ? m_clrAccentText : globalData.clrBarText;
}
//*********************************************************************************************************
COLORREF CBCGPVisualManagerVS2012::OnDrawControlBarCaption (CDC* pDC, CBCGPDockingControlBar* pBar, 
			BOOL bActive, CRect rectCaption, CRect rectButtons)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::OnDrawControlBarCaption (pDC, pBar, bActive, rectCaption, rectButtons);
	}

	ASSERT_VALID (pDC);

	rectCaption.bottom++;

	if (bActive)
	{
		pDC->FillRect(rectCaption, &m_brAccent);
		return RGB(255, 255, 255);
	}
	else
	{
		pDC->FillRect(rectCaption, &globalData.brBarFace);
		return globalData.clrBarText;
	}
}
//****************************************************************************************
void CBCGPVisualManagerVS2012::OnDrawControlBarCaptionText (CDC* pDC, CBCGPDockingControlBar* pBar, BOOL bActive, const CString& strTitle, CRect& rectCaption)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawControlBarCaptionText(pDC, pBar, bActive, strTitle, rectCaption);
		return;
	}

	ASSERT_VALID (pDC);

	int nTextWidth = 0;

	if (!strTitle.IsEmpty())
	{
		CRect rectText = rectCaption;

		pDC->DrawText (strTitle, rectText, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);

		CSize sizeText = pDC->GetTextExtent(strTitle);
		nTextWidth = sizeText.cx + sizeText.cy / 2;
	}

	CRect rectGripper = rectCaption;
	rectGripper.left += nTextWidth;

	COLORREF clrGripper = m_clrGripper;
	COLORREF clrGripperBk = m_clrGripperBk;

	if (bActive)
	{
		m_clrGripper = RGB(255, 255, 255);
		m_clrGripperBk = m_clrAccent;
	}

	OnDrawBarGripper (pDC, rectGripper, TRUE, pBar);

	m_clrGripper = clrGripper;
	m_clrGripperBk = clrGripperBk;
}
//*********************************************************************************************************
void CBCGPVisualManagerVS2012::OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz, CBCGPBaseControlBar* pBar)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawBarGripper(pDC, rectGripper, bHorz, pBar);
		return;
	}

	ASSERT_VALID (pDC);

	COLORREF clrTextOld = pDC->SetTextColor (m_clrGripper);
	COLORREF clrBkOld = pDC->SetBkColor (m_clrGripperBk);

	CRect rectFill = rectGripper;
	int nSize = 5;
	
	CBCGPToolBar* pToolBar = DYNAMIC_DOWNCAST (CBCGPToolBar, pBar);
	if (pToolBar != NULL)
	{
		if (bHorz)
		{
			int xCenter = rectFill.CenterPoint ().x;
			rectFill.left = xCenter - 1;
			rectFill.right = rectFill.left + nSize;
			rectFill.DeflateRect (0, 2);
		}
		else
		{
			int yCenter = rectFill.CenterPoint ().y;
			rectFill.top = yCenter - 1;
			rectFill.bottom = rectFill.top + nSize;
			rectFill.DeflateRect (2, 0);
		}
	}
	else
	{
		rectFill.top = rectFill.CenterPoint().y - nSize / 2;
		rectFill.bottom = rectFill.top + nSize;
	}

	pDC->FillRect (rectFill, bHorz ? &m_brGripperHorz : &m_brGripperVert);

	pDC->SetTextColor (clrTextOld);
	pDC->SetBkColor (clrBkOld);
}
//*********************************************************************************************************
BOOL CBCGPVisualManagerVS2012::IsAutoGrayscaleImages()
{
	return m_bAutoGrayscaleImages && globalData.m_nBitsPerPixel > 8 && !globalData.m_bIsBlackHighContrast;
}
//*********************************************************************************************************
void CBCGPVisualManagerVS2012::ModifyGlobalColors ()
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::ModifyGlobalColors();
		return;
	}

	if (m_Style == VS2012_Light)
	{
		globalData.clrBarFace = RGB (239, 239, 242);
		globalData.clrBarText = RGB (30, 30, 30);
		globalData.clrBarShadow = RGB (203, 205, 218);
		globalData.clrBarHilite = RGB (255, 255, 255);
		globalData.clrBarDkShadow = RGB(162, 164, 165);
		globalData.clrBarLight = RGB (252, 252, 252);
	}
	else
	{
		globalData.clrBarFace = RGB (58, 58, 58);
		globalData.clrBarText = RGB (241, 241, 241);
		globalData.clrBarShadow = RGB (120, 120, 120);
		globalData.clrBarHilite = RGB (104, 104, 104);
		globalData.clrBarDkShadow = RGB(190, 190, 190);
		globalData.clrBarLight = RGB (90, 90, 90);
	}

	globalData.brBarFace.DeleteObject ();
	globalData.brBarFace.CreateSolidBrush (globalData.clrBarFace);
}
//************************************************************************************
void CBCGPVisualManagerVS2012::OnUpdateSystemColors ()
{
	CBCGPVisualManagerVS2010::OnUpdateSystemColors ();

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return;
	}

	m_dblGrayScaleLumRatio = (m_Style == VS2012_Light) ? 0.8 : 1.1;

	CBCGPMenuImages::SetColor (CBCGPMenuImages::ImageWhite, RGB(255, 255, 255));

	m_brAccent.DeleteObject();
	m_brAccent.CreateSolidBrush(m_clrAccent);

	m_clrAccentLight = CBCGPDrawManager::PixelAlpha(m_clrAccent, m_Style == VS2012_Light ? 140 : 75);
	
	m_brAccentLight.DeleteObject();
	m_brAccentLight.CreateSolidBrush(m_clrAccentLight);

	m_clrAccentText = CBCGPDrawManager::PixelAlpha (m_clrAccent, m_Style == VS2012_Light ? 75 : 140);
	m_clrGripper = m_Style == VS2012_Light ? RGB (90, 91, 93) : RGB(208, 210, 211);

	m_clrTextDisabled = globalData.clrBarDkShadow;

	m_clrMenuSeparator = globalData.clrBarShadow;

	m_clrBarBkgnd = globalData.clrBarLight;
	m_brBarBkgnd.DeleteObject();
	m_brBarBkgnd.CreateSolidBrush (globalData.clrBarLight);

	m_clrGripperBk = globalData.clrBarFace;

	m_brTabs.DeleteObject();
	m_brTabs.CreateSolidBrush((m_Style == VS2012_Light) ? globalData.clrBarShadow : globalData.clrBarLight);

	m_clrHighlight = globalData.clrBarLight;
	m_brHighlight.DeleteObject();
	m_brHighlight.CreateSolidBrush(m_clrHighlight);

	m_clrHighlightDn = m_clrAccent;
	m_brHighlightDn.DeleteObject();
	m_brHighlightDn.CreateSolidBrush(m_clrHighlightDn);

	m_clrHighlightChecked = globalData.clrBarFace;
	m_brHighlightChecked.DeleteObject();
	m_brHighlightChecked.CreateSolidBrush(m_clrHighlightChecked);

	m_clrHighlightMenuItem = globalData.clrBarHilite;
	m_clrHighlightGradientLight = globalData.clrBarHilite;
	m_clrHighlightGradientDark = globalData.clrBarHilite;

	m_clrHighlightDnGradientDark = m_clrAccent;
	m_clrHighlightDnGradientLight =  m_clrAccent;

	m_clrMenuLight = m_Style == VS2012_Light ? RGB(231, 232, 236) : RGB(27, 27, 27);

	m_brMenuConnect.DeleteObject();
	m_brMenuConnect.CreateSolidBrush(m_clrMenuLight);

	m_clrPressedButtonBorder = m_clrMenuItemBorder = m_clrAccent;

	m_clrMenuItemBorder = m_clrHighlight;
	m_penMenuItemBorder.DeleteObject ();
	m_penMenuItemBorder.CreatePen (PS_SOLID, 1, m_clrMenuItemBorder);

	m_brMenuLight.DeleteObject ();
	m_brMenuLight.CreateSolidBrush (m_clrMenuLight);

	m_brTabBack.DeleteObject ();
	m_brTabBack.CreateSolidBrush (globalData.clrBarFace);

	m_brFace.DeleteObject ();
	m_brFace.CreateSolidBrush (m_clrToolBarGradientLight);

	m_clrToolbarDisabled = globalData.clrBarHilite;

	m_penBottomLine.DeleteObject ();
	m_penBottomLine.CreatePen (PS_SOLID, 1, m_clrToolBarBottomLine);

	m_brMenuButtonDroppedDown.DeleteObject ();
	m_brMenuButtonDroppedDown.CreateSolidBrush (m_clrBarBkgnd);

	m_brMenuItemCheckedHighlight.DeleteObject ();
	m_brMenuItemCheckedHighlight.CreateSolidBrush (globalData.clrBarLight);

	m_penSeparator.DeleteObject ();
	m_clrSeparator = globalData.clrBarDkShadow;
	m_penSeparator.CreatePen (PS_SOLID, 1, m_clrSeparator);

	m_clrMenuBorder = globalData.clrBarShadow;

	m_clrMenuGutter = globalData.clrBarLight;
	m_brMenuGutter.DeleteObject();
	m_brMenuGutter.CreateSolidBrush(m_clrMenuGutter);

	m_clrEditBoxBorder = m_clrSeparator;

	m_clrCombo = globalData.clrBarLight;
	m_clrComboDisabled = globalData.clrBarFace;

	m_clrCaptionBarGradientDark = m_clrHighlight;
	m_clrCaptionBarGradientLight = m_clrHighlight;

	m_brDlgBackground.DeleteObject();
	m_brDlgBackground.CreateSolidBrush(globalData.clrBarFace);

	m_clrEditCtrlSelectionBkActive = CBCGPDrawManager::PixelAlpha(m_clrAccent, 150);
	m_clrEditCtrlSelectionTextActive = RGB(0, 0, 0);
	
	m_clrEditCtrlSelectionBkInactive = globalData.clrBarFace;
	m_clrEditCtrlSelectionTextInactive = globalData.clrBarText;

	m_clrGridSelectionTextActive = RGB(0, 0, 0);
	m_clrGridSelectionTextInactive = RGB(0, 0, 0);

	m_brGridSelectionBkActive.DeleteObject();
	m_brGridSelectionBkActive.CreateSolidBrush(m_clrEditCtrlSelectionBkActive);
	m_brGridSelectionBkInactive.DeleteObject();
	m_brGridSelectionBkInactive.CreateSolidBrush(m_clrEditCtrlSelectionBkInactive);

	m_clrReportGroupText = m_clrInactiveCaptionGradient1;

	m_brAutohideButton.DeleteObject();
	m_brAutohideButton.CreateSolidBrush(globalData.clrBarFace);

	m_brDlgButtonsArea.DeleteObject ();
	m_brDlgButtonsArea.CreateSolidBrush (globalData.clrBarShadow);

	m_clrBarGradientDark = m_clrBarGradientLight = globalData.clrBarFace;
	m_clrToolBarGradientDark = m_clrToolBarGradientLight = globalData.clrBarFace;

	m_clrPlannerWork      = globalData.clrBarFace;
	m_clrPalennerLine     = globalData.clrBarDkShadow;
	m_clrPlannerTodayFill = m_clrAccent;

	m_clrPlannerTodayLine = m_clrAccentText;
	m_penPlannerTodayLine.DeleteObject ();
	m_penPlannerTodayLine.CreatePen(PS_SOLID, 1, m_clrPlannerTodayLine);

	m_brGripperHorz.DeleteObject();
	m_brGripperVert.DeleteObject();

	WORD bits[4] = {0xF0, 0xD0, 0xF0, 0x70};
		
	CBitmap bm;
	bm.CreateBitmap(4, 4, 1, 1, bits);
	
	m_brGripperHorz.CreatePatternBrush(&bm);
	m_brGripperVert.CreatePatternBrush(&bm);
}
//*********************************************************************************************************
COLORREF CBCGPVisualManagerVS2012::OnFillCommandsListBackground (CDC* pDC, CRect rect, BOOL bIsSelected)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::OnFillCommandsListBackground (pDC, rect, bIsSelected);
	}

	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	rect.left = 0;

	if (bIsSelected)
	{
		pDC->FillRect(rect, &m_brAccent);
		pDC->Draw3dRect(rect, RGB(255, 255, 255), RGB(255, 255, 255));
		return RGB(255, 255, 255);
	}
	else
	{
		pDC->FillRect (rect, &m_brMenuLight);
		return globalData.clrBarText;
	}
}
//*********************************************************************************************************
void CBCGPVisualManagerVS2012::GetTabFrameColors (const CBCGPBaseTabWnd* pTabWnd,
				   COLORREF& clrDark,
				   COLORREF& clrBlack,
				   COLORREF& clrHighlight,
				   COLORREF& clrFace,
				   COLORREF& clrDarkShadow,
				   COLORREF& clrLight,
				   CBrush*& pbrFace,
				   CBrush*& pbrBlack)
{
	ASSERT_VALID (pTabWnd);
	
	CBCGPVisualManagerXP::GetTabFrameColors (pTabWnd,
			   clrDark, clrBlack,
			   clrHighlight, clrFace,
			   clrDarkShadow, clrLight,
			   pbrFace, pbrBlack);

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || pTabWnd->IsDialogControl () ||
		pTabWnd->IsFlatTab ())
	{
		return;
	}

	clrFace = clrBlack;
	pbrFace  = &m_brTabs;
}
//*********************************************************************************************************
void CBCGPVisualManagerVS2012::OnDrawTabResizeBar (CDC* pDC, CBCGPBaseTabWnd* pWndTab, 
									BOOL bIsVert, CRect rect,
									CBrush* pbrFace, CPen* pPen)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || pWndTab->IsFlatTab ())
	{
		CBCGPVisualManagerVS2010::OnDrawTabResizeBar (pDC, pWndTab, bIsVert, rect, pbrFace, pPen);
		return;
	}

	ASSERT_VALID (pDC);
	pDC->FillRect(rect, &globalData.brBarFace);
}
//*********************************************************************************************************
BCGP_SMARTDOCK_THEME CBCGPVisualManagerVS2012::GetSmartDockingTheme ()
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || !globalData.IsWindowsLayerSupportAvailable ())
	{
		return CBCGPVisualManagerVS2010::GetSmartDockingTheme ();
	}

	return BCGP_SDT_VS2005;
}
//*********************************************************************************************************
void CBCGPVisualManagerVS2012::OnDrawSlider (CDC* pDC, CBCGPSlider* pSlider, CRect rect, BOOL bAutoHideMode)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawSlider (pDC, pSlider, rect, bAutoHideMode);
		return;
	}

	ASSERT_VALID (pDC);
	pDC->FillRect(rect, &globalData.brBarFace);
}
//*********************************************************************************************************
int CBCGPVisualManagerVS2012::GetTabButtonState (CBCGPTabWnd* pTab, CBCGTabButton* pButton)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetTabButtonState (pTab, pButton);
	}

	return (int) (m_Style == VS2012_Dark ? CBCGPMenuImages::ImageWhite : CBCGPMenuImages::ImageBlack);
}
//*****************************************************************************
int CBCGPVisualManagerVS2012::GetMenuDownArrowState (CBCGPToolbarMenuButton* pButton, BOOL bHightlight, BOOL bPressed, BOOL bDisabled)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetMenuDownArrowState (pButton, bHightlight, bPressed, bDisabled);
	}

	CBCGPWnd* pWnd = DYNAMIC_DOWNCAST(CBCGPWnd, pButton->GetParentWnd());

	if (pWnd != NULL && pWnd->IsOnGlass())
	{
		return CBCGPVisualManagerVS2010::GetMenuDownArrowState (pButton, bHightlight, bPressed, bDisabled);
	}

	if (pButton->IsDroppedDown())
	{
		return CBCGPMenuImages::ImageWhite;
	}

	return (int) ((m_Style == VS2012_Light) ? CBCGPMenuImages::ImageBlack : CBCGPMenuImages::ImageWhite);
}
//*****************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetStatusBarPaneTextColor (CBCGPStatusBar* pStatusBar, CBCGStatusBarPaneInfo* pPane)
{
	ASSERT (pPane != NULL);

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || pPane->clrText != (COLORREF)-1)
	{
		return CBCGPVisualManagerVS2010::GetStatusBarPaneTextColor (pStatusBar, pPane);
	}

	return (pPane->nStyle & SBPS_DISABLED) ? m_clrTextDisabled : globalData.clrBarText;
}
//*****************************************************************************
void CBCGPVisualManagerVS2012::OnDrawComboDropButton (CDC* pDC, CRect rect,
												BOOL bDisabled,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* pButton)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID (this);

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPVisualManagerVS2010::OnDrawComboDropButton (pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
		return;
	}

	COLORREF clrFill = globalData.clrBarFace;

	if (!bDisabled)
	{
		if (bIsDropped)
		{
			pDC->FillRect(rect, &m_brHighlightDn);
			clrFill = m_clrHighlightDn;
		}
		else if (bIsHighlighted)
		{
			pDC->FillRect (rect, &m_brAccentLight);
			clrFill = m_clrAccent;
		}
		else
		{
			pDC->FillRect (rect, &m_brBarBkgnd);
			clrFill = m_clrBarBkgnd;
		}
	}
	else
	{
		pDC->FillRect(rect, &globalData.brBarFace);
	}

	CBCGPMenuImages::DrawByColor(pDC, CBCGPMenuImages::IdArowDown, rect, clrFill);
}
//*************************************************************************************
void CBCGPVisualManagerVS2012::OnDrawComboBorder (CDC* pDC, CRect rect,
												BOOL bDisabled,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* pButton)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPVisualManagerVS2010::OnDrawComboBorder (pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
		return;
	}

	if (bDisabled)
	{
		pDC->Draw3dRect(rect, m_clrTextDisabled, m_clrTextDisabled);
	}
}
//*************************************************************************************
void CBCGPVisualManagerVS2012::OnDrawEditBorder (CDC* pDC, CRect rect,
												BOOL bDisabled,
												BOOL bIsHighlighted,
												CBCGPToolbarEditBoxButton* pButton)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || !CBCGPToolbarEditBoxButton::IsFlatMode () || CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPVisualManagerVS2010::OnDrawEditBorder (pDC, rect, bDisabled, bIsHighlighted, pButton);
		return;
	}
}
//*************************************************************************************
void CBCGPVisualManagerVS2012::OnFillButtonInterior (CDC* pDC,
	CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnFillButtonInterior(pDC, pButton, rect, state);
		return;
	}

	CBCGPControlBar* pWnd = DYNAMIC_DOWNCAST(CBCGPControlBar, pButton->GetParentWnd());
	if (pWnd != NULL && pWnd->IsDialogControl())
	{
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	CBCGPToolbarMenuButton* pMenuButton = DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);

	CWnd* pWndParentMenu = (pMenuButton != NULL) ? pMenuButton->GetParentWnd() : NULL;
	BOOL bIsPopupMenuButton = pWndParentMenu != NULL && pWndParentMenu->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar));

	if (pMenuButton != NULL && !bIsPopupMenuButton && pMenuButton->IsDroppedDown ())
	{
		if (pWndParentMenu != NULL && pWndParentMenu->IsKindOf (RUNTIME_CLASS (CBCGPMenuBar)))
		{
			CRect rectFill = rect;
			rectFill.DeflateRect(1, 1, 1, 0);

			if (CBCGPToolBarImages::m_bIsDrawOnGlass)
			{
				CBCGPDrawManager dm (*pDC);

				dm.DrawRect (rect, (COLORREF)-1, m_clrMenuGutter);
				dm.DrawRect (rectFill, m_clrMenuLight, (COLORREF)-1);
			}
			else
			{
				pDC->Draw3dRect(rect, m_clrMenuBorder, m_clrMenuBorder);
				pDC->FillRect (rectFill, &m_brMenuLight);
			}
		}
		else
		{
			rect.DeflateRect(1, 0);

			if (CBCGPToolBarImages::m_bIsDrawOnGlass)
			{
				CBCGPDrawManager dm (*pDC);
				dm.DrawRect (rect, m_clrAccent, (COLORREF)-1);
			}
			else
			{
				pDC->FillRect (rect, &m_brAccent);
			}
		}

		return;
	}

	CBCGPVisualManagerXP::OnFillButtonInterior (pDC, pButton, rect, state);
}
//***********************************************************************************
void CBCGPVisualManagerVS2012::OnFillHighlightedArea (CDC* pDC, CRect rect, CBrush* pBrush, CBCGPToolbarButton* pButton)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || pButton == NULL)
	{
		CBCGPVisualManagerVS2010::OnFillHighlightedArea (pDC, rect, pBrush, pButton);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pBrush);
	ASSERT_VALID (pButton);

	BOOL bIsPopupMenu = pButton->GetParentWnd () != NULL &&
		pButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar));

	CBCGPDrawManager dm (*pDC);

	if (pBrush == &m_brHighlight)
	{
		dm.DrawRect(rect, bIsPopupMenu ? m_clrHighlightMenuItem : m_clrHighlight, (COLORREF)-1);
		return;
	}
	else if (pBrush == &m_brHighlightDn)
	{
		dm.DrawRect(rect, (pButton->m_nStyle & TBBS_CHECKED) == 0 ? m_clrAccent : m_clrHighlight, (COLORREF)-1);
		return;
	}
}
//*********************************************************************************************************
void CBCGPVisualManagerVS2012::OnHighlightRarelyUsedMenuItems (CDC* pDC, CRect rectRarelyUsed)
{
	ASSERT_VALID (pDC);

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnHighlightRarelyUsedMenuItems (pDC, rectRarelyUsed);
		return;
	}

	rectRarelyUsed.left --;
	rectRarelyUsed.right = rectRarelyUsed.left + CBCGPToolBar::GetMenuImageSize ().cx + 
		2 * GetMenuImageMargin () + 2;

	pDC->FillRect (rectRarelyUsed, &m_brHighlight);
}
//*************************************************************************************
void CBCGPVisualManagerVS2012::OnDrawButtonBorder (CDC* pDC, CBCGPToolbarButton* pButton, CRect rect, BCGBUTTON_STATE state)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawButtonBorder (pDC, pButton, rect, state);
		return;
	}

	CBCGPControlBar* pWnd = DYNAMIC_DOWNCAST(CBCGPControlBar, pButton->GetParentWnd());
	if (pWnd != NULL && pWnd->IsDialogControl())
	{
		CBCGPVisualManager::OnDrawButtonBorder(pDC, pButton, rect, state);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->m_nStyle & TBBS_CHECKED)
	{
		COLORREF clrBorder = (!CBCGPToolBar::IsCustomizeMode () && (pButton->m_nStyle & TBBS_DISABLED)) ? globalData.clrBarShadow : m_clrAccent;

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);

			dm.DrawRect (rect, (COLORREF)-1, clrBorder);
		}
		else
		{
			pDC->Draw3dRect(rect, clrBorder, clrBorder);
		}
	}
}
//*************************************************************************************
int CBCGPVisualManagerVS2012::GetTabExtraHeight(const CBCGPTabWnd* pTabWnd)
{
	ASSERT_VALID(pTabWnd);

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () ||
		(pTabWnd->IsDialogControl () && !pTabWnd->IsPropertySheetTab()) || !pTabWnd->IsMDITab() ||
		pTabWnd->IsFlatTab () || pTabWnd->IsOneNoteStyle () || pTabWnd->IsVS2005Style ())
	{
		return CBCGPVisualManagerVS2010::GetTabExtraHeight(pTabWnd);
	}

	return 0;
}
//*************************************************************************************
BOOL CBCGPVisualManagerVS2012::OnEraseMDIClientArea (CDC* pDC, CRect rectClient)
{
	if (globalData.m_nBitsPerPixel > 8 && !globalData.IsHighContastMode ())
	{
		CMDIFrameWnd* pMDIFrameWnd = DYNAMIC_DOWNCAST (CMDIFrameWnd, AfxGetMainWnd ());
		if (pMDIFrameWnd != NULL)
		{
			pDC->FillRect(rectClient, &globalData.brBarFace);
			return TRUE;
		}
	}

	return CBCGPVisualManagerVS2010::OnEraseMDIClientArea (pDC, rectClient);
}
//*************************************************************************************
void CBCGPVisualManagerVS2012::OnFillCombo (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted,
										CBCGPToolbarComboBoxButton* pButton)
{
	ASSERT_VALID (pDC);

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPVisualManagerXP::OnFillCombo(pDC, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
		return;
	}

	COLORREF clr = bDisabled ? m_clrComboDisabled : (bIsHighlighted || bIsDropped) ? globalData.clrBarHilite : m_clrCombo;

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);

		dm.DrawRect (rect, clr, (COLORREF)-1);
	}
	else
	{
		pDC->FillSolidRect (rect, clr);
	}
}
//************************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetComboboxTextColor(CBCGPToolbarComboBoxButton* pButton, 
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetComboboxTextColor(pButton, bDisabled, bIsDropped, bIsHighlighted);
	}

	return bDisabled ? globalData.clrGrayedText : globalData.clrBarText;
}
//************************************************************************************
COLORREF CBCGPVisualManagerVS2012::OnFillMiniFrameCaption (CDC* pDC, 
								CRect rectCaption, 
								CBCGPMiniFrameWnd* pFrameWnd,
								BOOL bActive)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pFrameWnd);

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::OnFillMiniFrameCaption (pDC, rectCaption, pFrameWnd, bActive);
	}

	if (DYNAMIC_DOWNCAST (CBCGPBaseToolBar, pFrameWnd->GetControlBar ()) != NULL)
	{
		bActive = FALSE;
	}

	if (bActive)
	{
		pDC->FillRect(rectCaption, &m_brAccent);
		return RGB(255, 255, 255);
	}
	else
	{
		pDC->FillRect(rectCaption, &globalData.brBarFace);
		return globalData.clrBarText;
	}
}
//************************************************************************************
void CBCGPVisualManagerVS2012::OnDrawMiniFrameBorder (
										CDC* pDC, CBCGPMiniFrameWnd* pFrameWnd,
										CRect rectBorder, CRect rectBorderSize)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawMiniFrameBorder (pDC, pFrameWnd, rectBorder, rectBorderSize);
		return;
	}

	ASSERT_VALID (pDC);

	CRect rectClip = rectBorder;
	rectClip.DeflateRect(rectClip);

	pDC->ExcludeClipRect(rectClip);

	if (pFrameWnd->GetSafeHwnd() != NULL && pFrameWnd->IsActive())
	{
		pDC->FillRect(rectBorder, &m_brAccent);
	}
	else
	{
		CBrush br(globalData.clrBarDkShadow);
		pDC->FillRect(rectBorder, &br);
	}

	pDC->SelectClipRgn (NULL);
}
//**************************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetToolbarButtonTextColor (CBCGPToolbarButton* pButton, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	ASSERT_VALID (pButton);

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || pButton->IsKindOf (RUNTIME_CLASS (CBCGPOutlookButton)))
	{
		return CBCGPVisualManagerVS2010::GetToolbarButtonTextColor (pButton, state);
	}

	if (pButton->GetParentWnd() != NULL && pButton->GetParentWnd()->IsKindOf (RUNTIME_CLASS (CBCGPReBar)))
	{
		return CBCGPVisualManagerVS2010::GetToolbarButtonTextColor (pButton, state);
	}

	BOOL bDisabled = (CBCGPToolBar::IsCustomizeMode () && !pButton->IsEditable ()) ||
			(!CBCGPToolBar::IsCustomizeMode () && (pButton->m_nStyle & TBBS_DISABLED));

	CBCGPControlBar* pWnd = DYNAMIC_DOWNCAST(CBCGPControlBar, pButton->GetParentWnd());
	if (pWnd != NULL && pWnd->IsDialogControl())
	{
		return bDisabled ? globalData.clrGrayedText : globalData.clrBtnText;
	}

	BOOL bIsDroppedDownOnToolbar = pButton->IsDroppedDown();
	if (bIsDroppedDownOnToolbar)
	{
		if (pButton->GetParentWnd() != NULL && pButton->GetParentWnd()->IsKindOf (RUNTIME_CLASS (CBCGPMenuBar)))
		{
			bIsDroppedDownOnToolbar = FALSE;
		}
	}

	COLORREF clrLight = m_Style == VS2012_Light ? globalData.clrBarHilite : globalData.clrBarText;

	return bDisabled ? m_clrTextDisabled : (bIsDroppedDownOnToolbar || state == ButtonsIsHighlighted && (pButton->m_nStyle & TBBS_PRESSED)) ? clrLight : globalData.clrBarText;
}
//**************************************************************************************
BOOL CBCGPVisualManagerVS2012::OnDrawPushButton (CDC* pDC, CRect rect, CBCGPButton* pButton, COLORREF& clrText)
{
	ASSERT_VALID(pDC);

	BOOL bOnGlass = pButton->GetSafeHwnd() != NULL && pButton->m_bOnGlass;

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || bOnGlass ||
		m_brDlgBackground.GetSafeHandle () == NULL)
	{
		return CBCGPVisualManagerVS2010::OnDrawPushButton (pDC, rect, pButton, clrText);
	}

	ASSERT_VALID (pDC);

	clrText = globalData.clrBarText;

	globalData.DrawParentBackground (pButton, pDC);

	BOOL bDefault = pButton->GetSafeHwnd() != NULL && pButton->IsDefaultButton () && pButton->IsWindowEnabled ();

	CPen pen (PS_SOLID, 1, bDefault ? m_clrAccentText : globalData.clrBarDkShadow);
	CPen* pOldPen = pDC->SelectObject (&pen);

	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&globalData.brBarFace);

	if (pButton->GetSafeHwnd() != NULL && pButton->IsWindowEnabled ())
	{
		if (pButton->IsPressed ())
		{
			if (!pButton->GetCheck ())
			{
				pDC->SelectObject(&m_brHighlightDn);

				if (m_Style == VS2012_Light)
				{
					clrText = globalData.clrBarHilite;
				}
			}

			pDC->SelectObject(&m_penMenuItemBorder);
		}
		else if (pButton->IsHighlighted () || CWnd::GetFocus () == pButton || pButton->GetCheck ())
		{
			pDC->SelectObject(&m_brHighlight);
		}
	}

	pDC->RoundRect(rect, CPoint(5, 5));

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	return TRUE;
}
//**************************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetToolbarDisabledTextColor ()
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerXP::GetToolbarDisabledTextColor();
	}

	return m_clrTextDisabled;
}

#ifndef BCGP_EXCLUDE_PROP_LIST

COLORREF CBCGPVisualManagerVS2012::OnFillPropList(CDC* pDC, CBCGPPropList* pPropList, const CRect& rectClient, COLORREF& clrFill)
{
	ASSERT_VALID (pPropList);

	if (!pPropList->DrawControlBarColors() || globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerXP::OnFillPropList(pDC, pPropList, rectClient, clrFill);
	}

	pDC->FillSolidRect(rectClient, globalData.clrBarLight);
	clrFill = globalData.clrBarLight;

	return globalData.clrBarText;
}
//****************************************************************************************
void CBCGPVisualManagerVS2012::OnFillPropListToolbarArea(CDC* pDC, CBCGPPropList* pPropList, const CRect& rectToolBar)
{
	if ((pPropList != NULL && !pPropList->DrawControlBarColors()) || globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerXP::OnFillPropListToolbarArea(pDC, pPropList, rectToolBar);
		return;
	}

	pDC->FillRect(rectToolBar, &globalData.brBarFace);
}
//****************************************************************************************
COLORREF CBCGPVisualManagerVS2012::OnFillPropertyListSelectedItem(CDC* pDC, CBCGPProp* pProp, CBCGPPropList* pWndList, const CRect& rectFill, BOOL bFocused)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID(pProp);

	if (!pWndList->DrawControlBarColors() || globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerXP::OnFillPropertyListSelectedItem(pDC, pProp, pWndList, rectFill, bFocused);
	}

	if (bFocused)
	{
		pDC->FillRect (rectFill, &m_brAccent);
		return pProp->IsEnabled() ? RGB(255, 255, 255) : globalData.clrBarLight;
	}
	else
	{
		pDC->FillSolidRect(rectFill, globalData.clrBarShadow);
		return pProp->IsEnabled() ? globalData.clrBarText : globalData.clrBarDkShadow;
	}
}
//****************************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetPropListGroupTextColor(CBCGPPropList* pPropList)
{
	ASSERT_VALID (pPropList);

	if (!pPropList->DrawControlBarColors () || globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerXP::GetPropListGroupTextColor(pPropList);
	}

	return m_Style == VS2012_Light ? RGB(0, 0, 0) : globalData.clrBarText;
}
//***********************************************************************************
COLORREF CBCGPVisualManagerVS2012::OnFillPropListDescriptionArea(CDC* pDC, CBCGPPropList* pList, const CRect& rect)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pList);

	if (!pList->DrawControlBarColors () || globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerXP::OnFillPropListDescriptionArea(pDC, pList, rect);
	}

	pDC->FillSolidRect(rect, globalData.clrBarLight);
	return globalData.clrBarFace;
}
//***********************************************************************************
COLORREF CBCGPVisualManagerVS2012::OnFillPropListCommandsArea(CDC* pDC, CBCGPPropList* pList, const CRect& rect)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pList);

	if (!pList->DrawControlBarColors () || globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerXP::OnFillPropListCommandsArea(pDC, pList, rect);
	}

	pDC->FillSolidRect(rect, globalData.clrBarLight);
	return globalData.clrBarFace;
}
//***********************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetPropListCommandTextColor (CBCGPPropList* pPropList)
{	
	ASSERT_VALID (pPropList);

	if (!pPropList->DrawControlBarColors () || globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetPropListCommandTextColor (pPropList);
	}

	return m_clrAccentText;
}
//********************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetPropListDisabledTextColor(CBCGPPropList* pPropList)
{
	if (!pPropList->DrawControlBarColors () || globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetPropListDisabledTextColor(pPropList);
	}

	return globalData.clrBarDkShadow;
}

#endif


void CBCGPVisualManagerVS2012::OnDrawMenuBorder (CDC* pDC, CBCGPPopupMenu* pMenu, CRect rect)
{
	CBCGPVisualManagerXP::OnDrawMenuBorder (pDC, pMenu, rect);
}
//*****************************************************************************
void CBCGPVisualManagerVS2012::OnDrawMenuCheck (CDC* pDC, CBCGPToolbarMenuButton* pButton, CRect rect, BOOL bHighlight, BOOL bIsRadio)
{
	ASSERT_VALID (pButton);

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawMenuCheck (pDC, pButton, rect, bHighlight, bIsRadio);
		return;
	}

	int iImage = bIsRadio ? CBCGPMenuImages::IdRadio : CBCGPMenuImages::IdCheck;
	rect.DeflateRect(0, 1);
	CBCGPMenuImages::Draw (pDC, (CBCGPMenuImages::IMAGES_IDS) iImage, 
		rect,
		(pButton->m_nStyle & TBBS_DISABLED) ? CBCGPMenuImages::ImageGray : 
		(m_Style == VS2012_Light) ? CBCGPMenuImages::ImageBlack : CBCGPMenuImages::ImageWhite);
}
//****************************************************************************************
void CBCGPVisualManagerVS2012::OnFillMenuImageRect (CDC* pDC, CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnFillMenuImageRect(pDC, pButton, rect, state);
	}
}
//****************************************************************************************
void CBCGPVisualManagerVS2012::OnDrawMenuImageRectBorder (CDC* pDC, CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawMenuImageRectBorder(pDC, pButton, rect, state);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->m_nStyle & TBBS_CHECKED)
	{
		if (!CBCGPToolBar::IsCustomizeMode () && (pButton->m_nStyle & TBBS_DISABLED))
		{
			pDC->Draw3dRect(rect, globalData.clrBarShadow, globalData.clrBarShadow);
		}
		else if (pButton->GetImage() < 0 || DYNAMIC_DOWNCAST(CBCGPCustomizeMenuButton, pButton) != NULL)
		{
			COLORREF clrFrame = m_Style == VS2012_Light ? RGB(0, 0, 0) : globalData.clrBarText;

			rect.bottom++;

			pDC->Draw3dRect(rect, clrFrame, clrFrame);
			rect.DeflateRect(1, 1);
			pDC->Draw3dRect(rect, clrFrame, clrFrame);
		}
		else
		{
			pDC->Draw3dRect(rect, m_clrAccent, m_clrAccent);
		}
	}
}
//**************************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetMenuItemTextColor(CBCGPToolbarMenuButton* pButton, BOOL bHighlighted, BOOL bDisabled)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetMenuItemTextColor (pButton, bHighlighted, bDisabled);
	}

	return bDisabled ? m_clrTextDisabled : globalData.clrBarText;
}
//**************************************************************************************
void CBCGPVisualManagerVS2012::OnDrawCaptionButton (CDC* pDC, CBCGPCaptionButton* pButton, 
								BOOL bActive, BOOL bHorz, BOOL bMaximized, BOOL bDisabled,
								int nImageID /*= -1*/)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawCaptionButton (pDC, pButton, bActive, bHorz, bMaximized, bDisabled, nImageID);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

    CRect rc = pButton->GetRect ();

	const BOOL bHighlight = (pButton->m_bFocused || pButton->m_bDroppedDown) && !bDisabled;

	if (pButton->m_bPushed && bHighlight)
	{
		if (bActive)
		{
			pDC->FillSolidRect (rc, m_clrAccentText);
		}
		else
		{
			pDC->FillRect(rc, &m_brAccent);
		}
	}
	else if (bHighlight || pButton->m_bPushed)
	{
		if (bActive)
		{
			pDC->FillRect (rc, &m_brAccentLight);
		}
		else
		{
			pDC->FillSolidRect (rc, globalData.clrBarShadow);
		}
	}

	CBCGPMenuImages::IMAGES_IDS id = (CBCGPMenuImages::IMAGES_IDS)-1;
	
	if (nImageID != -1)
	{
		id = (CBCGPMenuImages::IMAGES_IDS)nImageID;
	}
	else
	{
		id = pButton->GetIconID (bHorz, bMaximized);
	}

	if (id != (CBCGPMenuImages::IMAGES_IDS)-1)
	{
		CSize sizeImage = CBCGPMenuImages::Size ();
		CPoint ptImage (rc.left + (rc.Width () - sizeImage.cx) / 2,
						rc.top + (rc.Height () - sizeImage.cy) / 2);

		OnDrawCaptionButtonIcon (pDC, pButton, id, bActive, bDisabled, ptImage);
	}
}
//**********************************************************************************
void CBCGPVisualManagerVS2012::OnDrawCaptionButtonIcon (CDC* pDC, 
													CBCGPCaptionButton* pButton,
													CBCGPMenuImages::IMAGES_IDS id,
													BOOL bActive, BOOL bDisabled,
													CPoint ptImage)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawCaptionButtonIcon (pDC, pButton, id, bActive, bDisabled, ptImage);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	CBCGPMenuImages::IMAGE_STATE imageState = (m_Style == VS2012_Dark ? CBCGPMenuImages::ImageLtGray : CBCGPMenuImages::ImageBlack);
	
	if (bDisabled)
	{
		imageState = CBCGPMenuImages::ImageGray;
	}
	else if (bActive || ((pButton->m_bFocused && pButton->m_bPushed) && m_Style == VS2012_Light))
	{
		imageState = CBCGPMenuImages::ImageWhite;
	}

	CBCGPMenuImages::Draw (pDC, id, ptImage, imageState);
}

#ifndef BCGP_EXCLUDE_TOOLBOX

BOOL CBCGPVisualManagerVS2012::OnEraseToolBoxButton (CDC* pDC, CRect rect,
											CBCGPToolBoxButton* pButton)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::OnEraseToolBoxButton(pDC, rect, pButton);
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->GetCheck ())
	{
		pDC->FillRect(rect, &m_brAccent);
	}
	else if (pButton->IsHighlighted ())
	{
		CBrush brFill(globalData.clrBarHilite);
		pDC->FillRect(rect, &brFill);
	}

	return TRUE;
}
//**********************************************************************************
BOOL CBCGPVisualManagerVS2012::OnDrawToolBoxButtonBorder (CDC* pDC, CRect& rect, 
												 CBCGPToolBoxButton* pButton, UINT uiState)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::OnDrawToolBoxButtonBorder(pDC, rect, pButton, uiState);
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->GetCheck ())
	{
		pDC->Draw3dRect(rect, globalData.clrBarHilite, globalData.clrBarHilite);
	}

	return TRUE;
}
//**********************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetToolBoxButtonTextColor (CBCGPToolBoxButton* pButton)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetToolBoxButtonTextColor(pButton);
	}

	if (m_Style == VS2012_Dark)
	{
		return globalData.clrBarText;
	}

	ASSERT_VALID (pButton);
	return pButton->m_bIsChecked ? globalData.clrBarHilite : globalData.clrBarText;
}

#endif	// BCGP_EXCLUDE_TOOLBOX

#ifndef BCGP_EXCLUDE_TASK_PANE

void CBCGPVisualManagerVS2012::OnDrawTasksGroupCaption(CDC* pDC, CBCGPTasksGroup* pGroup, 
								BOOL bIsHighlighted, BOOL bIsSelected, 
								BOOL bCanCollapse)
{
#ifndef BCGP_EXCLUDE_TOOLBOX
	BOOL bIsToolBox = pGroup->m_pPage->m_pTaskPane != NULL &&
		(pGroup->m_pPage->m_pTaskPane->IsKindOf (RUNTIME_CLASS (CBCGPToolBoxEx)));
#else
	BOOL bIsToolBox = FALSE;
#endif

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || !bIsToolBox)
	{
		CBCGPVisualManagerXP::OnDrawTasksGroupCaption(
										pDC, pGroup, 
										bIsHighlighted, bIsSelected, bCanCollapse);
		return;
	}

	CRect rectGroup = pGroup->m_rect;

	CRect rectFill = rectGroup;
	rectFill.DeflateRect (1, 1);

	pDC->FillRect(rectFill, &m_brDlgButtonsArea);

	if (bCanCollapse)
	{
		//--------------------
		// Draw expanding box:
		//--------------------
		int nBoxSize = 9;
		int nBoxOffset = 6;

		if (globalData.GetRibbonImageScale () != 1.)
		{
			nBoxSize = (int)(.5 + nBoxSize * globalData.GetRibbonImageScale ());
		}

		CRect rectButton = rectFill;
		
		rectButton.left += nBoxOffset;
		rectButton.right = rectButton.left + nBoxSize;
		rectButton.top = rectButton.CenterPoint ().y - nBoxSize / 2;
		rectButton.bottom = rectButton.top + nBoxSize;

		OnDrawExpandingBox (pDC, rectButton, !pGroup->m_bIsCollapsed, globalData.clrBarText);

		rectGroup.left = rectButton.right + nBoxOffset;
		bCanCollapse = FALSE;
	}

	// -----------------------
	// Draw group caption text
	// -----------------------
	CFont* pFontOld = pDC->SelectObject (&globalData.fontBold);
	COLORREF clrTextOld = pDC->SetTextColor (globalData.clrBarText);
	int nBkModeOld = pDC->SetBkMode(TRANSPARENT);
	
	int nTaskPaneHOffset = pGroup->m_pPage->m_pTaskPane->GetGroupCaptionHorzOffset();
	int nTaskPaneVOffset = pGroup->m_pPage->m_pTaskPane->GetGroupCaptionVertOffset();
	int nCaptionHOffset = (nTaskPaneHOffset != -1 ? nTaskPaneHOffset : m_nGroupCaptionHorzOffset);
	
	CRect rectText = rectGroup;
	rectText.left += nCaptionHOffset;
	rectText.top += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : m_nGroupCaptionVertOffset);
	rectText.right = max(rectText.left, 
						rectText.right - (bCanCollapse ? rectGroup.Height() : nCaptionHOffset));

	pDC->DrawText (pGroup->m_strName, rectText, DT_SINGLELINE | DT_VCENTER);

	pDC->SetBkMode(nBkModeOld);
	pDC->SelectObject (pFontOld);
	pDC->SetTextColor (clrTextOld);
}

#endif

void CBCGPVisualManagerVS2012::OnFillOutlookPageButton (CDC* pDC, const CRect& rect,
										BOOL bIsHighlighted, BOOL bIsPressed,
										COLORREF& clrText)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnFillOutlookPageButton (pDC, rect,
										bIsHighlighted, bIsPressed,
										clrText);
		return;
	}

	ASSERT_VALID (pDC);

	clrText = globalData.clrBarText;

	if (bIsPressed)
	{
		pDC->FillRect(rect, &m_brAccent);
		clrText = RGB(255, 255, 255);
	}
	else if (bIsHighlighted)
	{
		pDC->FillRect(rect, &m_brMenuLight);
	}
	else
	{
		pDC->FillRect(rect, &globalData.brBarFace);
	}
}
//****************************************************************************************
void CBCGPVisualManagerVS2012::OnDrawOutlookPageButtonBorder (CDC* pDC, 
							CRect& rectBtn, BOOL bIsHighlighted, BOOL bIsPressed)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawOutlookPageButtonBorder (pDC, 
							rectBtn, bIsHighlighted, bIsPressed);
		return;
	}

	ASSERT_VALID (pDC);

	COLORREF clrFrame = bIsHighlighted && bIsPressed ? globalData.clrBarHilite : globalData.clrBarShadow;
	pDC->Draw3dRect (rectBtn, clrFrame, clrFrame);
}
//****************************************************************************************
void CBCGPVisualManagerVS2012::OnFillOutlookBarCaption (CDC* pDC, CRect rectCaption, COLORREF& clrText)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnFillOutlookBarCaption (pDC, rectCaption, clrText);
		return;
	}

	pDC->FillRect(rectCaption, &m_brMenuLight);
	clrText = globalData.clrBarText;
}
//*******************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetCaptionBarTextColor (CBCGPCaptionBar* pBar)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetCaptionBarTextColor(pBar);
	}

	return globalData.clrBarText;
}
//****************************************************************************************
void CBCGPVisualManagerVS2012::OnDrawHeaderCtrlBorder (CBCGPHeaderCtrl* pCtrl, CDC* pDC, CRect& rect, BOOL bIsPressed, BOOL bIsHighlighted)
{
	BOOL bIsVMStyle = (pCtrl == NULL || pCtrl->m_bVisualManagerStyle);
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || !bIsVMStyle)
	{
		CBCGPVisualManagerVS2010::OnDrawHeaderCtrlBorder (pCtrl, pDC, rect, bIsPressed, bIsHighlighted);
		return;
	}

	COLORREF clrFill  = globalData.clrBarFace;
	COLORREF clrBorder = globalData.clrBarShadow;

	if (bIsPressed)
	{
		clrFill  = m_clrAccent;
		clrBorder = RGB(255, 255, 255);
	}
	else if (bIsHighlighted)
	{
		clrFill  = globalData.clrBarLight;
		clrBorder = RGB(255, 255, 255);
	}

	{
		CBCGPDrawManager dm (*pDC);
		dm.DrawRect (rect, clrFill, (COLORREF)-1);
	}

	CPen pen (PS_SOLID, 0, clrBorder);
	CPen* pOldPen = pDC->SelectObject (&pen);
	
	if (bIsPressed || bIsHighlighted)
	{
		pDC->MoveTo (rect.right - 1, rect.top);
		pDC->LineTo (rect.right - 1, rect.bottom - 1);
		pDC->LineTo (rect.left, rect.bottom - 1);
		pDC->LineTo (rect.left, rect.top - 1);
	}
	else
	{
		pDC->MoveTo (rect.right - 1, rect.top);
		pDC->LineTo (rect.right - 1, rect.bottom - 1);
		pDC->LineTo (rect.left - 1, rect.bottom - 1);
	}

	pDC->SelectObject (pOldPen);
}
//****************************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetHeaderCtrlTextColor (CBCGPHeaderCtrl* pCtrl, BOOL bIsPressed, BOOL bIsHighlighted)
{
	BOOL bIsVMStyle = (pCtrl == NULL || pCtrl->m_bVisualManagerStyle);
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || !bIsVMStyle)
	{
		return CBCGPVisualManagerVS2010::GetHeaderCtrlTextColor (pCtrl, bIsPressed, bIsHighlighted);
	}

	return globalData.clrBarText;
}
//****************************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetURLLinkColor (CBCGPURLLinkButton* pButton, BOOL bHover)
{
	ASSERT_VALID (pButton);

	if (!pButton->m_bVisualManagerStyle || pButton->m_bOnGlass || globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetURLLinkColor (pButton, bHover);
	}

	return bHover ? m_clrAccentText : m_clrAccent;
}

#ifndef BCGP_EXCLUDE_POPUP_WINDOW

void CBCGPVisualManagerVS2012::OnFillPopupWindowBackground (CDC* pDC, CRect rect)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerXP::OnFillPopupWindowBackground (pDC, rect);
		return;
	}

	pDC->FillSolidRect(rect, m_Style == VS2012_Light ? globalData.clrBarFace : m_clrGripper);
}
//**********************************************************************************
void CBCGPVisualManagerVS2012::OnDrawPopupWindowBorder (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	CBCGPVisualManagerXP::OnDrawPopupWindowBorder (pDC, rect);
}
//**********************************************************************************
COLORREF CBCGPVisualManagerVS2012::OnDrawPopupWindowCaption (CDC* pDC, CRect rectCaption, CBCGPPopupWindow* pPopupWnd)
{
	return CBCGPVisualManagerXP::OnDrawPopupWindowCaption (pDC, rectCaption, pPopupWnd);
}
//**********************************************************************************
void CBCGPVisualManagerVS2012::OnErasePopupWindowButton (CDC* pDC, CRect rc, CBCGPPopupWndButton* pButton)
{
	if (pButton->IsPressed ())
	{
		CBrush br (m_clrHighlightDn);
		pDC->FillRect (&rc, &br);
		return;
	}
	else if (pButton->IsHighlighted () || pButton->IsPushed ())
	{
		CBrush br (globalData.clrBarShadow);
		pDC->FillRect (&rc, &br);
		return;
	}

	CBCGPVisualManagerXP::OnErasePopupWindowButton (pDC, rc, pButton);
}
//**********************************************************************************
void CBCGPVisualManagerVS2012::OnDrawPopupWindowButtonBorder (CDC* pDC, CRect rc, CBCGPPopupWndButton* pButton)
{
	CBCGPVisualManagerXP::OnDrawPopupWindowButtonBorder (pDC, rc, pButton);
}

#endif	// BCGP_EXCLUDE_POPUP_WINDOW

COLORREF CBCGPVisualManagerVS2012::OnFillListBox(CDC* pDC, CBCGPListBox* pListBox, CRect rectClient)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::OnFillListBox(pDC, pListBox, rectClient);
	}

	ASSERT_VALID (pDC);

	pDC->FillRect(rectClient, &m_brMenuLight);
	return globalData.clrBarText;
}
//*******************************************************************************
COLORREF CBCGPVisualManagerVS2012::OnFillListBoxItem (CDC* pDC, CBCGPListBox* pListBox, int nItem, CRect rect, BOOL bIsHighlihted, BOOL bIsSelected)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::OnFillListBoxItem (pDC, pListBox, nItem, rect, bIsHighlihted, bIsSelected);
	}

	ASSERT_VALID (pDC);

	COLORREF clrText = (COLORREF)-1;

	if (bIsSelected)
	{
		pDC->FillRect (rect, &m_brAccent);
		clrText = RGB(255, 255, 255);
	}
	
	if (bIsHighlihted)
	{
		pDC->Draw3dRect(rect, RGB(255, 255, 255), RGB(255, 255, 255));
	}

	return clrText;
}
//*******************************************************************************
COLORREF CBCGPVisualManagerVS2012::OnFillComboBoxItem (CDC* pDC, CBCGPComboBox* pComboBox, int nItem, CRect rect, BOOL bIsHighlihted, BOOL bIsSelected)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::OnFillComboBoxItem (pDC, pComboBox, nItem, rect, bIsHighlihted, bIsSelected);
	}

	ASSERT_VALID (pDC);

	if (!bIsHighlihted && !bIsSelected)
	{
		pDC->FillRect(rect, &m_brMenuLight);
		return globalData.clrBarText;
	}

	COLORREF clrText = (COLORREF)-1;

	if (bIsSelected)
	{
		pDC->FillRect (rect, &m_brAccent);
		clrText = RGB(255, 255, 255);
	}
	
	if (bIsHighlihted)
	{
		pDC->Draw3dRect(rect, RGB(255, 255, 255), RGB(255, 255, 255));
	}

	return clrText;
}
//*******************************************************************************
HBRUSH CBCGPVisualManagerVS2012::GetListControlFillBrush(CBCGPListCtrl* pListCtrl)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetListControlFillBrush(pListCtrl);
	}

	return m_brMenuLight;
}
//*******************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetListControlTextColor(CBCGPListCtrl* pListCtrl)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetListControlTextColor(pListCtrl);
	}

	return m_Style == VS2012_Light ? RGB(0, 0, 0) : RGB(255, 255, 255);
}
//*****************************************************************************
void CBCGPVisualManagerVS2012::OnDrawShowAllMenuItems (CDC* pDC, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawShowAllMenuItems (pDC, rect, state);
		return;
	}

	CBCGPMenuImages::Draw (pDC, CBCGPMenuImages::IdArowShowAll, rect,
		m_Style == VS2012_Dark ? CBCGPMenuImages::ImageWhite : CBCGPMenuImages::ImageBlack);
}
//*****************************************************************************
int CBCGPVisualManagerVS2012::GetShowAllMenuItemsHeight (CDC* pDC, const CSize& sizeDefault)
{
	return CBCGPVisualManagerXP::GetShowAllMenuItemsHeight(pDC, sizeDefault);
}
//*****************************************************************************
void CBCGPVisualManagerVS2012::OnDrawSeparator (CDC* pDC, CBCGPBaseControlBar* pBar, CRect rect, BOOL bIsHoriz)
{
	CBCGPVisualManagerXP::OnDrawSeparator(pDC, pBar, rect, bIsHoriz);
}
//*************************************************************************************
void CBCGPVisualManagerVS2012::GetCalendarColors (const CBCGPCalendar* pCalendar, CBCGPCalendarColors& colors)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::GetCalendarColors(pCalendar, colors);
		return;
	}

	colors.clrCaption = globalData.clrBarFace;
	colors.clrCaptionText = globalData.clrBarText;
	colors.clrSelected = m_clrAccent;
	colors.clrSelectedText = RGB(255, 255, 255);
	colors.clrTodayBorder = m_clrAccentText;
}
//**************************************************************************************
void CBCGPVisualManagerVS2012::OnDrawCaptionBarInfoArea (CDC* pDC, CBCGPCaptionBar* pBar, CRect rect)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || pBar->m_clrBarBackground != (COLORREF)-1)
	{
		CBCGPVisualManagerVS2010::OnDrawCaptionBarInfoArea (pDC, pBar, rect);
		return;
	}

	pDC->FillRect(rect, &m_brMenuLight);
	pDC->Draw3dRect(rect, m_clrMenuBorder, m_clrMenuBorder);
}

#ifndef BCGP_EXCLUDE_GRID_CTRL

COLORREF CBCGPVisualManagerVS2012::OnFillGridGroupByBoxBackground (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	CBrush br (globalData.clrBtnShadow);
	pDC->FillRect (rect, &br);

	return globalData.clrBtnText;
}
//********************************************************************************
COLORREF CBCGPVisualManagerVS2012::OnFillGridGroupByBoxTitleBackground (CDC* pDC, CRect rect)
{
	return CBCGPVisualManagerXP::OnFillGridGroupByBoxTitleBackground(pDC, rect);
}
//********************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetGridGroupByBoxLineColor () const
{
	return globalData.clrBtnText;
}
//********************************************************************************
void CBCGPVisualManagerVS2012::OnDrawGridGroupByBoxItemBorder (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect)
{
	CBCGPVisualManagerXP::OnDrawGridGroupByBoxItemBorder (pCtrl, pDC, rect);
}
//********************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetGridLeftOffsetColor (CBCGPGridCtrl* pCtrl)
{
	ASSERT_VALID(pCtrl);

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || !pCtrl->IsControlBarColors ())
	{
		return CBCGPVisualManagerXP::GetGridLeftOffsetColor (pCtrl);
	}

	return m_Style == VS2012_Dark ? globalData.clrBarDkShadow : m_clrHighlight;
}
//********************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetGridItemSortedColor (CBCGPGridCtrl* pCtrl)
{
	return CBCGPVisualManagerXP::GetGridItemSortedColor(pCtrl);
}
//********************************************************************************
void CBCGPVisualManagerVS2012::OnFillGridGroupBackground (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rectFill)
{
	CBCGPVisualManagerXP::OnFillGridGroupBackground(pCtrl, pDC, rectFill);
}
//********************************************************************************
void CBCGPVisualManagerVS2012::OnDrawGridGroupUnderline (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rectFill)
{
	CBCGPVisualManagerXP::OnDrawGridGroupUnderline(pCtrl, pDC, rectFill);
}
//********************************************************************************
COLORREF CBCGPVisualManagerVS2012::OnFillGridRowBackground (CBCGPGridCtrl* pCtrl, 
												  CDC* pDC, CRect rectFill, BOOL bSelected)
{
	return CBCGPVisualManagerXP::OnFillGridRowBackground(pCtrl, pDC, rectFill, bSelected);
}
//********************************************************************************
COLORREF CBCGPVisualManagerVS2012::OnFillGridItem (CBCGPGridCtrl* pCtrl, 
											CDC* pDC, CRect rectFill,
											BOOL bSelected, BOOL bActiveItem, BOOL bSortedColumn)
{
	return CBCGPVisualManagerXP::OnFillGridItem(pCtrl, pDC, rectFill, bSelected, bActiveItem, bSortedColumn);
}
//********************************************************************************
void CBCGPVisualManagerVS2012::OnDrawGridHeaderMenuButton (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect, 
		BOOL bHighlighted, BOOL bPressed, BOOL bDisabled)
{
	CBCGPVisualManagerXP::OnFillGridItem(pCtrl, pDC, rect, bHighlighted, bPressed, bDisabled);
}
//********************************************************************************
void CBCGPVisualManagerVS2012::OnDrawGridSelectionBorder (CBCGPGridCtrl* /*pCtrl*/, CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->Draw3dRect (rect, globalData.clrBtnDkShadow, globalData.clrBtnDkShadow);
	rect.DeflateRect (1, 1);
	pDC->Draw3dRect (rect, globalData.clrBtnDkShadow, globalData.clrBtnDkShadow);
}
//********************************************************************************
BOOL CBCGPVisualManagerVS2012::OnSetGridColorTheme (CBCGPGridCtrl* pCtrl, BCGP_GRID_COLOR_DATA& theme)
{
	BOOL bRes = CBCGPVisualManagerVS2010::OnSetGridColorTheme (pCtrl, theme);

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return bRes;
	}

	theme.m_clrBackground = m_clrHighlight;
	theme.m_clrText = globalData.clrBarText;
	theme.m_clrHorzLine = theme.m_clrVertLine = globalData.clrBarShadow;
	
	theme.m_EvenColors.m_clrBackground = CBCGPDrawManager::SmartMixColors(globalData.clrBarFace, globalData.clrBarHilite);
	theme.m_EvenColors.m_clrGradient = (COLORREF)-1;
	theme.m_EvenColors.m_clrText = globalData.clrBarText;

	theme.m_OddColors.m_clrBackground = m_clrHighlight;
	theme.m_OddColors.m_clrGradient = (COLORREF)-1;
	theme.m_OddColors.m_clrText = globalData.clrBarText;

	theme.m_SelColors.m_clrBackground = m_clrAccent;
	theme.m_SelColors.m_clrGradient = (COLORREF)-1;
	theme.m_SelColors.m_clrText = RGB(255, 255, 255);

	theme.m_LeftOffsetColors.m_clrBackground = m_clrHighlight;
	theme.m_LeftOffsetColors.m_clrGradient = (COLORREF)-1;
	theme.m_LeftOffsetColors.m_clrBorder = globalData.clrBarShadow;
	theme.m_LeftOffsetColors.m_clrText = globalData.clrBarText;

	theme.m_HeaderColors.m_clrBackground = globalData.clrBarFace;
	theme.m_HeaderColors.m_clrBorder = globalData.clrBarShadow;
	theme.m_HeaderColors.m_clrGradient = (COLORREF)-1;
	theme.m_HeaderColors.m_clrText = globalData.clrBarText;

	theme.m_HeaderSelColors.m_clrBackground = m_clrAccent;
	theme.m_HeaderSelColors.m_clrBorder = globalData.clrBarShadow;
	theme.m_HeaderSelColors.m_clrGradient = (COLORREF)-1;
	theme.m_HeaderSelColors.m_clrText = RGB(255, 255, 255);

	theme.m_GroupColors.m_clrBorder = globalData.clrBarShadow;
	theme.m_GroupColors.m_clrBackground = globalData.clrBarFace;
	theme.m_GroupColors.m_clrGradient = (COLORREF)-1;
	theme.m_GroupColors.m_clrText = globalData.clrBarText;

	theme.m_GroupSelColors.m_clrBorder = globalData.clrBarShadow;
	theme.m_GroupSelColors.m_clrBackground = m_clrAccent;
	theme.m_GroupSelColors.m_clrGradient = (COLORREF)-1;
	theme.m_GroupSelColors.m_clrText = RGB(255, 255, 255);

	return TRUE;
}
//********************************************************************************
COLORREF CBCGPVisualManagerVS2012::OnFillReportCtrlRowBackground (CBCGPGridCtrl* pCtrl, 
												  CDC* pDC, CRect rectFill,
												  BOOL bSelected, BOOL bGroup)
{
	return CBCGPVisualManagerXP::OnFillReportCtrlRowBackground (pCtrl, pDC, rectFill, bSelected, bGroup);
}
//********************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetReportCtrlGroupBackgoundColor ()
{
	return CBCGPVisualManagerXP::GetReportCtrlGroupBackgoundColor ();
}
//********************************************************************************
void CBCGPVisualManagerVS2012::OnFillGridBackground (CDC* pDC, CRect rect)
{
	CBCGPVisualManagerXP::OnFillGridBackground(pDC, rect);
}
//********************************************************************************
void CBCGPVisualManagerVS2012::OnFillGridHeaderBackground (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect)
{
	CBCGPVisualManagerXP::OnFillGridHeaderBackground(pCtrl, pDC, rect);
}
//********************************************************************************
BOOL CBCGPVisualManagerVS2012::OnDrawGridHeaderItemBorder (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect, BOOL bPressed)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::OnDrawGridHeaderItemBorder(pCtrl, pDC, rect, bPressed);
	}

	CPen* pOldPen = pDC->SelectObject (&m_penSeparator);
	
	pDC->MoveTo (rect.right - 1, rect.top);
	pDC->LineTo (rect.right - 1, rect.bottom);
	
	pDC->MoveTo (rect.left, rect.top);
	pDC->LineTo (rect.right, rect.top);
	
	pDC->MoveTo (rect.left, rect.bottom - 1);
	pDC->LineTo (rect.right, rect.bottom - 1);
	
	pDC->SelectObject (pOldPen);

	return FALSE;
}
//********************************************************************************
void CBCGPVisualManagerVS2012::OnFillGridRowHeaderBackground (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect)
{
	CBCGPVisualManagerXP::OnFillGridRowHeaderBackground(pCtrl, pDC, rect);
}
//********************************************************************************
BOOL CBCGPVisualManagerVS2012::OnDrawGridRowHeaderItemBorder (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect, BOOL bPressed)
{
	return CBCGPVisualManagerXP::OnDrawGridRowHeaderItemBorder(pCtrl, pDC, rect, bPressed);
}
//********************************************************************************
void CBCGPVisualManagerVS2012::OnFillGridSelectAllAreaBackground (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect, BOOL bPressed)
{
	CBCGPVisualManagerXP::OnFillGridSelectAllAreaBackground(pCtrl, pDC, rect, bPressed);
}
//********************************************************************************
BOOL CBCGPVisualManagerVS2012::OnDrawGridSelectAllAreaBorder (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect, BOOL bPressed)
{
	return CBCGPVisualManagerXP::OnDrawGridSelectAllAreaBorder(pCtrl, pDC, rect, bPressed);
}

#endif // BCGP_EXCLUDE_GRID_CTRL

//********************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetEditBackSidebarColor(CBCGPEditCtrl* pEdit)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetEditBackSidebarColor(pEdit);
	}

	return globalData.clrBarFace;
}
//********************************************************************************
HBRUSH CBCGPVisualManagerVS2012::GetEditBackgroundBrush(CBCGPEditCtrl* pEdit)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetEditBackgroundBrush(pEdit);
	}

	return m_Style == VS2012_Light ? m_brWhite : globalData.brBarFace;
}
//********************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetEditTextColor(CBCGPEditCtrl* pEdit)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetEditTextColor(pEdit);
	}

	return globalData.clrBarText;
}
//********************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetEditLineNumbersBarBackColor(CBCGPEditCtrl* pEdit)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetEditLineNumbersBarBackColor(pEdit);
	}

	return globalData.clrBarFace;
}
//********************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetEditLineNumbersBarTextColor(CBCGPEditCtrl* pEdit)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetEditLineNumbersBarTextColor(pEdit);
	}

	return m_clrAccentText;
}
//*******************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetEditOutlineColor(CBCGPEditCtrl* pEdit)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetEditOutlineColor(pEdit);
	}

	return globalData.clrBarDkShadow;
}
//*******************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetTreeControlFillColor(CBCGPTreeCtrl* pTreeCtrl, BOOL bIsSelected, BOOL bIsFocused, BOOL bIsDisabled)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetTreeControlFillColor(pTreeCtrl, bIsSelected, bIsFocused, bIsDisabled);
	}

	if (bIsSelected)
	{
		return bIsFocused ? m_clrAccent : globalData.clrBarShadow;
	}

	return m_clrHighlight;
}
//*******************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetTreeControlTextColor(CBCGPTreeCtrl* pTreeCtrl, BOOL bIsSelected, BOOL bIsFocused, BOOL bIsDisabled)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetTreeControlTextColor(pTreeCtrl, bIsSelected, bIsFocused, bIsDisabled);
	}

	if (bIsSelected)
	{
		return bIsFocused ? RGB(255, 255, 255) : globalData.clrBarText;
	}

	return globalData.clrBarText;
}
//*********************************************************************************
CBrush& CBCGPVisualManagerVS2012::GetEditCtrlBackgroundBrush(CBCGPEdit* pEdit)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetEditCtrlBackgroundBrush(pEdit);
	}

	return m_brMenuLight;
}
//*********************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetEditCtrlTextColor(CBCGPEdit* pEdit)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetEditCtrlTextColor(pEdit);
	}

	return globalData.clrBarText;
}
//*************************************************************************************
HBRUSH CBCGPVisualManagerVS2012::GetToolbarEditColors(CBCGPToolbarButton* pButton, COLORREF& clrText, COLORREF& clrBk)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetToolbarEditColors(pButton, clrText, clrBk);
	}

	clrText = globalData.clrBarText;
	clrBk = m_clrHighlight;

	return (HBRUSH) m_brHighlight.GetSafeHandle ();
}
//*************************************************************************************
COLORREF CBCGPVisualManagerVS2012::BreadcrumbFillBackground (CDC& dc, CBCGPBreadcrumb* pControl, CRect rectFill)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () || !pControl->m_bVisualManagerStyle)
	{
		return CBCGPVisualManagerVS2010::BreadcrumbFillBackground(dc, pControl, rectFill);
	}

	dc.FillRect(&rectFill, &m_brMenuLight);
	return pControl->IsWindowEnabled() ? globalData.clrBarText : m_clrTextDisabled;
}

#ifndef BCGP_EXCLUDE_RIBBON

void CBCGPVisualManagerVS2012::OnDrawRibbonProgressBar (CDC* pDC, 
												  CBCGPRibbonProgressBar* pProgress, 
												  CRect rectProgress, CRect rectChunk,
												  BOOL bInfiniteMode)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawRibbonProgressBar (pDC, pProgress, 
												  rectProgress, rectChunk, bInfiniteMode);
		return;
	}

	ASSERT_VALID (pDC);

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);
		
		if (!rectChunk.IsRectEmpty ())
		{
			dm.DrawRect (rectChunk, m_clrAccent, (COLORREF)-1);
		}

		dm.DrawRect (rectProgress, (COLORREF)-1, globalData.clrBarDkShadow);
	}
	else
	{
		if (!rectChunk.IsRectEmpty ())
		{
			pDC->FillRect (rectChunk, &m_brAccent);
		}

		pDC->Draw3dRect (rectProgress, globalData.clrBarDkShadow, globalData.clrBarDkShadow);
	}
}

#endif

BOOL CBCGPVisualManagerVS2012::IsOwnerDrawScrollBar () const
{
	return globalData.m_nBitsPerPixel > 8 && !globalData.IsHighContastMode ();
}
//*************************************************************************************
void CBCGPVisualManagerVS2012::OnScrollBarDrawThumb (CDC* pDC, CBCGPScrollBar* /*pScrollBar*/, CRect rect, 
		BOOL bHorz, BOOL bHighlighted, BOOL bPressed, BOOL bDisabled)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return;
	}

	ASSERT_VALID(pDC);

	if (bHorz)
	{
		rect.InflateRect(1, 0);
	}
	else
	{
		rect.InflateRect(0, 1);
	}

	CBCGPDrawManager dm (*pDC);
	dm.DrawRect (rect, 
		bPressed ? m_clrAccent : bHighlighted ? globalData.clrBarHilite : globalData.clrBarFace, 
		bDisabled ? globalData.clrBarShadow : globalData.clrBarDkShadow);
}
//*************************************************************************************
void CBCGPVisualManagerVS2012::OnScrollBarDrawButton (CDC* pDC, CBCGPScrollBar* /*pScrollBar*/, CRect rect, 
		BOOL bHorz, BOOL bHighlighted, BOOL bPressed, BOOL bFirst, BOOL bDisabled)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return;
	}

	COLORREF clrFill = bPressed ? m_clrAccent : bHighlighted ? m_clrHighlight : globalData.clrBarFace;

	CBCGPDrawManager dm (*pDC);
	dm.DrawRect (rect, 
		clrFill, 
		bDisabled ? globalData.clrBarShadow : globalData.clrBarDkShadow);

	CBCGPMenuImages::IMAGES_IDS ids;
	if (bHorz)
	{
		ids = bFirst ? CBCGPMenuImages::IdArowLeftTab3d : CBCGPMenuImages::IdArowRightTab3d;
	}
	else
	{
		ids = bFirst ? CBCGPMenuImages::IdArowUpLarge : CBCGPMenuImages::IdArowDownLarge;
	}

	CBCGPMenuImages::IMAGE_STATE state = bDisabled ? CBCGPMenuImages::ImageGray : CBCGPMenuImages::GetStateByColor(clrFill);

	CBCGPMenuImages::Draw (pDC, ids, rect, state);
}
//*************************************************************************************
void CBCGPVisualManagerVS2012::OnScrollBarFillBackground (CDC* pDC, CBCGPScrollBar* /*pScrollBar*/, CRect rect, 
		BOOL /*bHorz*/, BOOL /*bHighlighted*/, BOOL bPressed, BOOL /*bFirst*/, BOOL /*bDisabled*/)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return;
	}

	CBCGPDrawManager dm (*pDC);
	dm.DrawRect (rect, bPressed ? m_clrAccent : m_clrHighlight, globalData.clrBarDkShadow);
}

#ifndef BCGP_EXCLUDE_PLANNER
//*******************************************************************************
void CBCGPVisualManagerVS2012::OnFillPlanner (CDC* pDC, CBCGPPlannerView* pView, 
		CRect rect, BOOL bWorkingArea)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pView);

	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnFillPlanner (pDC, pView, rect, bWorkingArea);
		return;
	}

	if (m_bPlannerBackItemSelected)
	{
		CBrush br (GetPlannerHourLineColor (pView, TRUE, FALSE));
		pDC->FillRect (rect, &br);
	}
	else
	{
		CBCGPVisualManagerVS2010::OnFillPlanner (pDC, pView, rect, bWorkingArea);
	}

	if (m_bPlannerBackItemToday && DYNAMIC_DOWNCAST (CBCGPPlannerViewDay, pView) == NULL)
	{
		pDC->Draw3dRect (rect, m_clrAccentText, m_clrAccentText);
	}
}
//*******************************************************************************
COLORREF CBCGPVisualManagerVS2012::OnFillPlannerCaption (CDC* pDC,
		CBCGPPlannerView* pView, CRect rect, BOOL bIsToday, BOOL bIsSelected,
		BOOL bNoBorder/* = FALSE*/, BOOL bHorz /*= TRUE*/)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::OnFillPlannerCaption (pDC,
			pView, rect, bIsToday, bIsSelected, bNoBorder, bHorz);
	}

	const BOOL bMonth = DYNAMIC_DOWNCAST(CBCGPPlannerViewMonth, pView) != NULL;

	if (bMonth && m_bPlannerCaptionBackItemHeader)
	{
		return globalData.clrBarText;
	}

	ASSERT_VALID (pDC);

	BOOL bDay = FALSE;

	if (!bMonth)
	{
		bDay = pView->IsKindOf (RUNTIME_CLASS (CBCGPPlannerViewDay));

		if (bDay)
		{
			if (!bIsToday)
			{
				if (pView->IsKindOf (RUNTIME_CLASS (CBCGPPlannerViewMulti)))
				{
					rect.top++;
					rect.left++;
				}
				else
				{
					rect.left++;
				}
			}
		}
	}
	else
	{
		if (!bIsToday)
		{
			rect.bottom--;
		}
	}

	COLORREF clrText   = globalData.clrBarText;
	COLORREF clrBorder = CLR_DEFAULT;

	if (bIsToday)
	{
		CBrush br (m_clrPlannerTodayFill);
		pDC->FillRect (rect, &br);

		clrText = RGB(255, 255, 255);
		clrBorder = m_clrPlannerTodayLine;
	}
	else
	{
		pDC->FillRect (rect, &globalData.brBarFace);
	}

	if (bIsSelected && !bDay)
	{
		clrText = m_clrAccentText;
	}

	if (clrBorder != CLR_DEFAULT && !bNoBorder)
	{
		pDC->Draw3dRect (rect, clrBorder, clrBorder);
	}

	return clrText;
}
//*******************************************************************************
void CBCGPVisualManagerVS2012::OnDrawPlannerCaptionText (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect, const CString& strText, 
		COLORREF clrText, int nAlign, BOOL bHighlight)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawPlannerCaptionText (pDC,
			pView, rect, strText, clrText, nAlign, bHighlight);
		return;
	}

	const int nTextMargin = 2;

	rect.DeflateRect (nTextMargin, 0);

	COLORREF clrOld = pDC->SetTextColor (clrText);

	pDC->DrawText (strText, rect, DT_SINGLELINE | DT_VCENTER | nAlign);

	pDC->SetTextColor (clrOld);
}
//*******************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetPlannerAppointmentTimeColor (CBCGPPlannerView* pView,
		BOOL bSelected, BOOL bSimple, DWORD dwDrawFlags)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () ||
		(bSelected && (dwDrawFlags & BCGP_PLANNER_DRAW_APP_OVERRIDE_SELECTION) == 0))
	{
		return CBCGPVisualManagerVS2010::GetPlannerAppointmentTimeColor (pView,
			bSelected, bSimple, dwDrawFlags);
	}

	return CLR_DEFAULT;
}
//*******************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetPlannerHourLineColor (CBCGPPlannerView* pView,
		BOOL bWorkingHours, BOOL bHour)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () ||
		GetPlannerViewBackgroundColor (pView) != m_clrPlannerWork)
	{
		return CBCGPVisualManagerVS2010::GetPlannerHourLineColor (pView, 
			bWorkingHours, bHour);
	}

	COLORREF colorFill = m_clrPalennerLine;

	if (!bHour)
	{
		if (CBCGPVisualManagerVS2012::GetStyle () == CBCGPVisualManagerVS2012::VS2012_Dark)
		{
			colorFill = globalData.clrBarShadow;
		}
		else
		{
			colorFill = bWorkingHours ? m_clrHighlight : globalData.clrBarShadow;
		}
	}

	return colorFill;
}
//*******************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetPlannerViewWorkingColor (CBCGPPlannerView* pView)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () ||
		GetPlannerViewBackgroundColor (pView) != m_clrPlannerWork)
	{
		return CBCGPVisualManagerVS2010::GetPlannerViewWorkingColor (pView);
	}

	return m_clrHighlightMenuItem;
}
//*******************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetPlannerViewNonWorkingColor (CBCGPPlannerView* pView)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode () ||
		GetPlannerViewBackgroundColor (pView) != m_clrPlannerWork)
	{
		return CBCGPVisualManagerVS2010::GetPlannerViewNonWorkingColor (pView);
	}

	return m_clrHighlight;
}
//*******************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetPlannerSelectionColor (CBCGPPlannerView* pView)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetPlannerSelectionColor (pView);
	}

	return m_clrAccent;
}
//*******************************************************************************
COLORREF CBCGPVisualManagerVS2012::GetPlannerSeparatorColor (CBCGPPlannerView* pView)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetPlannerSeparatorColor (pView);
	}

	return m_clrPalennerLine;
}
//*******************************************************************************
COLORREF CBCGPVisualManagerVS2012::OnFillPlannerTimeBar (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect, COLORREF& clrLine)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::OnFillPlannerTimeBar (pDC, pView, rect, clrLine);
	}

	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &globalData.brBarFace);
	
	clrLine = m_clrPalennerLine;

	return globalData.clrBarText;
}
//*******************************************************************************
void CBCGPVisualManagerVS2012::OnDrawPlannerTimeLine (CDC* pDC, CBCGPPlannerView* pView, CRect rect)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawPlannerTimeLine (pDC, pView, rect);
		return;
	}

	ASSERT_VALID (pDC);

	CBCGPDrawManager dm (*pDC);
	dm.FillGradient (rect, m_clrPlannerTodayFill, globalData.clrBarFace, TRUE);

	CPen* pOldPen = pDC->SelectObject (&m_penPlannerTodayLine);

	pDC->MoveTo (rect.left, rect.bottom);
	pDC->LineTo (rect.right, rect.bottom);

	pDC->SelectObject (pOldPen);
}
//*******************************************************************************
void CBCGPVisualManagerVS2012::OnFillPlannerWeekBar (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnFillPlannerWeekBar (pDC, pView, rect);
		return;
	}

	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brMenuLight);
}
//*******************************************************************************
void CBCGPVisualManagerVS2012::OnDrawPlannerHeader (CDC* pDC, 
	CBCGPPlannerView* pView, CRect rect)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawPlannerHeader (pDC, pView, rect);
		return;
	}

	ASSERT_VALID (pDC);

	COLORREF clr = GetPlannerSeparatorColor (pView);

	if (DYNAMIC_DOWNCAST(CBCGPPlannerViewMonth, pView) != NULL)
	{
		clr = m_clrHighlight;
	}

	CBrush br (clr);
	pDC->FillRect (rect, &br);

	if (pView->IsKindOf (RUNTIME_CLASS (CBCGPPlannerViewDay)))
	{
		if (rect.left == pView->GetAppointmentsRect().left)
		{
			CRect rect1 (rect);
			rect1.right = rect1.left + 1;

			if (pView->IsKindOf (RUNTIME_CLASS (CBCGPPlannerViewMulti)))
			{
				rect1.top++;
			}

			pDC->FillRect (rect1, &globalData.brBarFace);
		}
	}
}
//*******************************************************************************
void CBCGPVisualManagerVS2012::OnDrawPlannerHeaderPane (CDC* pDC, 
	CBCGPPlannerView* pView, CRect rect)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawPlannerHeaderPane (pDC, pView, rect);
		return;
	}

	if (DYNAMIC_DOWNCAST(CBCGPPlannerViewMonth, pView) != NULL)
	{
		pDC->Draw3dRect (rect.right - 1, rect.top - 2, 1, rect.Height () + 4, 
			m_clrPalennerLine, m_clrPalennerLine);
	}
}
//*******************************************************************************
void CBCGPVisualManagerVS2012::OnFillPlannerHeaderAllDay (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnFillPlannerHeaderAllDay (pDC, pView, rect);
		return;
	}

	ASSERT_VALID (pDC);
	
	pDC->FillRect (rect, &globalData.brBarFace);
}
//*******************************************************************************
void CBCGPVisualManagerVS2012::OnDrawPlannerHeaderAllDayItem (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect, BOOL bIsToday, BOOL bIsSelected)
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		CBCGPVisualManagerVS2010::OnDrawPlannerHeaderAllDayItem (pDC, pView, rect, 
			bIsToday, bIsSelected);
		return;
	}

	ASSERT_VALID (pDC);

	rect.left++;
	if (bIsSelected)
	{
		CBrush br (GetPlannerSelectionColor (pView));
		pDC->FillRect (rect, &br);
	}

	if (bIsToday)
	{
 		rect.top--;
		rect.left--;
		rect.bottom++;
		pDC->Draw3dRect (rect, m_clrPlannerTodayLine, m_clrPlannerTodayLine);
	}
}
//*******************************************************************************
void CBCGPVisualManagerVS2012::PreparePlannerBackItem (BOOL bIsToday, BOOL bIsSelected)
{
	m_bPlannerBackItemToday    = bIsToday;
	m_bPlannerBackItemSelected = bIsSelected;
}
//*******************************************************************************
DWORD CBCGPVisualManagerVS2012::GetPlannerDrawFlags () const
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetPlannerDrawFlags ();
	}

	return BCGP_PLANNER_DRAW_APP_OVERRIDE_SELECTION |
		   BCGP_PLANNER_DRAW_APP_NO_MULTIDAY_CLOCKS |
		   BCGP_PLANNER_DRAW_VIEW_WEEK_BAR |
		   BCGP_PLANNER_DRAW_VIEW_CAPTION_DAY_BOLD |
		   BCGP_PLANNER_DRAW_VIEW_CAPTION_DAY_COMPACT |
		   BCGP_PLANNER_DRAW_VIEW_DAYS_UPDOWN | 
		   BCGP_PLANNER_DRAW_VIEW_DAYS_UPDOWN_VCENTER;
}
//*******************************************************************************
int CBCGPVisualManagerVS2012::GetPlannerRowExtraHeight () const
{
	if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode ())
	{
		return CBCGPVisualManagerVS2010::GetPlannerRowExtraHeight ();
	}

	return 3;
}

#endif // BCGP_EXCLUDE_PLANNER
