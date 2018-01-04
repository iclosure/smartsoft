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
// BCGPMetroUICtrl.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "BCGPMetroUITiles.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"

#ifndef _BCGSUITE_
#include "BCGPPopupMenu.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT BCGM_ON_CLICK_METRO_UI_TILE = ::RegisterWindowMessage (_T("BCGM_ON_CLICK_METRO_UI_TILE"));

CMap<UINT,UINT,CBCGPMetroUITile*,CBCGPMetroUITile*> CBCGPMetroUITile::m_mapAnimations;
CCriticalSection CBCGPMetroUITile::g_cs;

#define BADGE_GLYPH_SIZE	15
#define ANIMATION_STEP_TIME	50

/////////////////////////////////////////////////////////////////////////////
// CBCGPMetroUITile

CBCGPMetroUITile::CBCGPMetroUITile()
{
	CommonInit();
}
//*****************************************************************************************
CBCGPMetroUITile::CBCGPMetroUITile(const CString& strName,
										   BOOL bIsWide,
										   const CBCGPColor& colorText,
										   const CBCGPBrush& brushBackground,
										   const CBCGPColor& colorBorder)
{
	CommonInit();

	m_strName = strName;
	m_bIsWide = bIsWide;
	m_colorText = colorText;
	m_colorBorder = colorBorder,
	m_brushBackground = brushBackground;
}
//*****************************************************************************************
void CBCGPMetroUITile::CommonInit()
{
	m_nAnimationID = 0;
	m_pOwner = NULL;
	m_sizePadding = CBCGPSize(15.0, 5.0);
	m_dwUserData = 0;
	m_nBadgeNumber = -1;
	m_rect.SetRectEmpty();
	m_nImportance = 0;
	m_nGroup = 0;
	m_bIsWide = FALSE;
	m_dblBorderWidth = 2.0;
	m_dblImageOpacity = 1.0;
	m_dblImageOpacityDelta = 0.0;
}
//*****************************************************************************************
void CBCGPMetroUITile::DoDraw(CBCGPMetroUITiles* pMetroUI, CBCGPGraphicsManager* pGM, BOOL bIsPressed, BOOL bIsHighlighted)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pGM);

	if (m_rect.IsRectEmpty())
	{
		return;
	}

	CBCGPRect rect = m_rect;
	CBCGPRect rectShape = m_rect;

	CBCGPRoundedRect roundedRect(rect, pMetroUI->m_dblCornerRadius, pMetroUI->m_dblCornerRadius);

	if (!m_brushBackground.IsEmpty() && m_brushBackgroundDark.IsEmpty())
	{
		m_brushBackgroundDark = m_brushBackground;
		m_brushBackgroundDark.MakeDarker();
	}

	if (bIsPressed || bIsHighlighted)
	{
		const CBCGPBrush& br = !m_brushBackgroundDark.IsEmpty() ? m_brushBackgroundDark : pMetroUI->GetTileBrushDark();

		if (pMetroUI->IsRoundedShapes())
		{
			pGM->FillRoundedRectangle(roundedRect, br);
		}
		else
		{
			pGM->FillRectangle(rectShape, br);
		}
	}
	else
	{
		const CBCGPBrush& br = !m_brushBackground.IsEmpty() ? m_brushBackground : pMetroUI->GetTileBrush();

		if (pMetroUI->IsRoundedShapes())
		{
			pGM->FillRoundedRectangle(roundedRect, br);
		}
		else
		{
			pGM->FillRectangle(rectShape, br);
		}
	}

	rect.DeflateRect(m_sizePadding.cx, m_sizePadding.cy);

	CBCGPColor clrText = m_colorText;

	if (clrText.IsNull())
	{
		clrText = pMetroUI->GetTextColor();
	}

	CBCGPSize sizeName = GetNameSize(pGM, pMetroUI->m_textFormatName, rect.Width());
	CBCGPSize sizeBadge = GetBadgeSize(pGM, pMetroUI->m_textFormatBadge);
	CBCGPSize sizeHeader = GetHeaderSize(pGM, pMetroUI->m_textFormatHeader, rect.Width());
	CBCGPSize sizeText = GetTextSize(pGM, pMetroUI->m_textFormatText, rect.Width());
	
	CBCGPRect rectName = rect;
	rectName.top = rectName.bottom - sizeName.cy;

	CBCGPRect rectBadge = rect;
	rectBadge.top = rectBadge.bottom - sizeBadge.cy;
	rectBadge.left = rectBadge.right - sizeBadge.cx;

	rect.bottom = min(rectBadge.top, rectName.top);

	CBCGPRect rectHeader = rect;
	rectHeader.bottom = rectHeader.top + sizeHeader.cy;

	CBCGPRect rectText = rect;

	if (sizeHeader.cy == 0)
	{
		rectText.top = rect.top + 2 * m_sizePadding.cy;
	}
	else
	{
		rectText.top = rectHeader.bottom;
	}

	// Draw image:
	
	BOOL bIsFullSizeImage = FALSE;

	CBCGPSize sizeImage = GetImageSize(pGM);
	if (!sizeImage.IsEmpty())
	{
		CBCGPRect rectImage;

		if (sizeImage.cx < rect.Width() && sizeImage.cy < rect.Height())
		{
			rectImage = rect;
			rectImage.bottom = rectName.top;

			if (!m_strHeader.IsEmpty() || !m_strText.IsEmpty())
			{
				rectImage.right = rectImage.left + sizeImage.cx;

				rectHeader.left = rectImage.right + m_sizePadding.cx;
				rectText.left = rectImage.right + m_sizePadding.cx;
			}
		}
		else
		{
			rectImage = m_rect;
			bIsFullSizeImage = TRUE;
		}

		BOOL bClipRoundedRect = FALSE;
		
		if (bIsFullSizeImage && pMetroUI->IsRoundedShapes())
		{
			pGM->SetClipRoundedRect(roundedRect);
			bClipRoundedRect = TRUE;
		}

		OnDrawImage(pGM, rectImage);

		if (bClipRoundedRect)
		{
			pGM->ReleaseClipArea();
		}
	}

	if ((sizeImage.IsEmpty() || bIsFullSizeImage) && sizeText.IsEmpty())
	{
		rectHeader = rect;
	}

	// Draw texts:
	OnDrawName(pGM, rectName, pMetroUI->m_textFormatName, clrText);
	OnDrawHeader(pGM, rectHeader, pMetroUI->m_textFormatHeader, clrText);
	OnDrawText(pGM, rectText, pMetroUI->m_textFormatText, clrText);

	// Draw badge:
	if (bIsFullSizeImage && !sizeBadge.IsEmpty())
	{
		CBCGPRect rectFillBadge = rectBadge;
		rectFillBadge.InflateRect(4, 3);

		const CBCGPBrush& br = !m_brushBackground.IsEmpty() ? m_brushBackground : pMetroUI->GetTileBrush();
		pGM->FillRectangle(rectFillBadge, br);
	}

	OnDrawBadge(pGM, rectBadge, pMetroUI->m_textFormatBadge, clrText);

	if (!m_colorBorder.IsNull() && !bIsFullSizeImage)
	{
		if (pMetroUI->IsRoundedShapes())
		{
			roundedRect.rect.DeflateRect(m_dblBorderWidth / 2, m_dblBorderWidth / 2);
			pGM->DrawRoundedRectangle(roundedRect, CBCGPBrush(m_colorBorder), m_dblBorderWidth);
		}
		else
		{
			rectShape.DeflateRect(m_dblBorderWidth / 2, m_dblBorderWidth / 2);
			pGM->DrawRectangle(rectShape, CBCGPBrush(m_colorBorder), m_dblBorderWidth);
		}
	}
}
//*****************************************************************************************
CBCGPSize CBCGPMetroUITile::GetNameSize(CBCGPGraphicsManager* pGM, const CBCGPTextFormat& tf, double dblWidth)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pGM);

	if (m_strName.IsEmpty())
	{
		return CBCGPSize(0., 0.);
	}

	return pGM->GetTextSize(m_strName, tf, dblWidth);
}
//*****************************************************************************************
void CBCGPMetroUITile::OnDrawName(CBCGPGraphicsManager* pGM, const CBCGPRect& rectText, const CBCGPTextFormat& tf, const CBCGPColor& clrText)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pGM);

	pGM->DrawText(m_strName, rectText, tf, CBCGPBrush(clrText));
}
//*****************************************************************************************
CBCGPSize CBCGPMetroUITile::GetHeaderSize(CBCGPGraphicsManager* pGM, const CBCGPTextFormat& tf, double dblWidth)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pGM);

	if (m_strHeader.IsEmpty())
	{
		return CBCGPSize(0., 0.);
	}
	
	return pGM->GetTextSize(m_strHeader, tf, dblWidth);
}
//*****************************************************************************************
void CBCGPMetroUITile::OnDrawHeader(CBCGPGraphicsManager* pGM, const CBCGPRect& rectText, const CBCGPTextFormat& tf, const CBCGPColor& clrText)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pGM);

	pGM->DrawText(m_strHeader, rectText, tf, CBCGPBrush(clrText));
}
//*****************************************************************************************
CBCGPSize CBCGPMetroUITile::GetTextSize(CBCGPGraphicsManager* pGM, const CBCGPTextFormat& tf, double dblWidth)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pGM);

	if (m_strText.IsEmpty())
	{
		return CBCGPSize(0., 0.);
	}
	
	return pGM->GetTextSize(m_strText, tf, dblWidth);
}
//*****************************************************************************************
void CBCGPMetroUITile::OnDrawText(CBCGPGraphicsManager* pGM, const CBCGPRect& rectText, const CBCGPTextFormat& tf, const CBCGPColor& clrText)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pGM);

	pGM->DrawText(m_strText, rectText, tf, CBCGPBrush(clrText));
}
//*****************************************************************************************
CBCGPSize CBCGPMetroUITile::GetBadgeSize(CBCGPGraphicsManager* pGM, const CBCGPTextFormat& tf)
{
	if (m_nBadgeNumber >= 0)
	{
		CString strNumber;
		strNumber.Format(_T("%d"), m_nBadgeNumber);
		
		return pGM->GetTextSize(strNumber, tf);
	}
	
	return (m_BadgeGlyph == BCGP_NONE) ? CBCGPSize(0, 0) : CBCGPSize(BADGE_GLYPH_SIZE, BADGE_GLYPH_SIZE);
}
//*****************************************************************************************
void CBCGPMetroUITile::OnDrawBadge(CBCGPGraphicsManager* pGM, const CBCGPRect& rectBadge, const CBCGPTextFormat& tf, const CBCGPColor& clrText)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pGM);
	ASSERT_VALID(m_pOwner);

	if (m_nBadgeNumber >= 0)
	{
		CString strNumber;
		strNumber.Format(_T("%d"), m_nBadgeNumber);

		pGM->DrawText(strNumber, rectBadge, tf, CBCGPBrush(clrText));
		return;
	}

	if (m_BadgeGlyph == BCGP_NONE)
	{
		return;
	}

	CBCGPSize sizeIcon(BADGE_GLYPH_SIZE, BADGE_GLYPH_SIZE);
	int nImageIndex = (int)m_BadgeGlyph;

	pGM->DrawImage(m_pOwner->m_BadgeGlyphs, rectBadge.TopLeft(), sizeIcon, 1.0, CBCGPImage::BCGP_IMAGE_INTERPOLATION_MODE_LINEAR,
		CBCGPRect(CBCGPPoint(sizeIcon.cx * nImageIndex, 0), sizeIcon));
}
//*****************************************************************************************
CBCGPSize CBCGPMetroUITile::GetImageSize(CBCGPGraphicsManager* pGM)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pGM);

	return pGM->GetImageSize(m_Image);
}
//*****************************************************************************************
void CBCGPMetroUITile::OnDrawImage(CBCGPGraphicsManager* pGM, const CBCGPRect& rectImage)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pGM);

	CBCGPSize sizeImage = GetImageSize(pGM);
	if (!sizeImage.IsEmpty())
	{
		double xOffset = max(0, 0.5 * (rectImage.Width() - sizeImage.cx));
		double yOffset = max(0, 0.5 * (rectImage.Height() - sizeImage.cy));
		
		pGM->DrawImage(m_Image, rectImage.TopLeft() + CBCGPPoint(xOffset, yOffset), CBCGPSize(), m_dblImageOpacity);
	}
}
//*****************************************************************************************
void CBCGPMetroUITile::SetUserData(DWORD_PTR dwUserData) 
{ 
	ASSERT_VALID(this);
	m_dwUserData = dwUserData;
}
//*****************************************************************************************
void CBCGPMetroUITile::SetName(const CString& strName) 
{ 
	ASSERT_VALID(this);
	m_strName = strName;
}
//*****************************************************************************************
void CBCGPMetroUITile::SetGroupID(int nID, BOOL bRecalcLayout)
{
	ASSERT_VALID(this);

	if (m_nGroup == nID)
	{
		return;
	}

	m_nGroup = nID;

	if (bRecalcLayout && m_pOwner != NULL)
	{
		ASSERT_VALID(m_pOwner);
		m_pOwner->SetDirty(TRUE, TRUE);
	}
}
//*****************************************************************************************
void CBCGPMetroUITile::SetHeader(const CString& strHeader) 
{ 
	ASSERT_VALID(this);
	m_strHeader = strHeader;
}
//*****************************************************************************************
void CBCGPMetroUITile::SetText(const CString& strText) 
{ 
	ASSERT_VALID(this);
	m_strText = strText;
}
//*****************************************************************************************
void CBCGPMetroUITile::SetBadgeNumber(int nBadgeNumber)
{
	ASSERT_VALID(this);

	m_nBadgeNumber = nBadgeNumber;

	if (m_nBadgeNumber >= 0)
	{
		m_BadgeGlyph = BCGP_NONE;
	}
}
//*****************************************************************************************
void CBCGPMetroUITile::SetBadgeGlyph(BCGP_METRO_BADGE_GLYPH glyph)
{
	ASSERT_VALID(this);

	m_BadgeGlyph = glyph;

	if (m_BadgeGlyph != BCGP_NONE)
	{
		m_nBadgeNumber = -1;
	}
}
//*****************************************************************************************
void CBCGPMetroUITile::ClearBadge()
{
	ASSERT_VALID(this);

	m_nBadgeNumber = -1;
	m_BadgeGlyph = BCGP_NONE;
}
//*****************************************************************************************
void CBCGPMetroUITile::SetImage(const CBCGPImage& image, BCGP_METRO_IMAGE_EFFECT effect, int nAnimationTime) 
{ 
	ASSERT_VALID(this);

	if (m_nAnimationID != 0)
	{
		StopAnimation();
		Redraw();
	}

	nAnimationTime = max(nAnimationTime, ANIMATION_STEP_TIME);

	m_Image = image;

	if (effect == BCGP_ANIMATION_NONE || nAnimationTime <= 0 || m_pOwner == NULL)
	{
		return;
	}

	m_dblImageOpacity = 0.0;
	m_dblImageOpacityDelta = 1.0 / ((double) nAnimationTime / ANIMATION_STEP_TIME);

	m_nAnimationID = ((UINT) ::SetTimer (NULL, 0, ANIMATION_STEP_TIME, AnimTimerProc));
	
	g_cs.Lock ();
	m_mapAnimations.SetAt(m_nAnimationID, this);
	g_cs.Unlock ();
}
//*******************************************************************************
BOOL CBCGPMetroUITile::OnAnimation()
{
	ASSERT_VALID(this);

	m_dblImageOpacity += m_dblImageOpacityDelta;

	if (m_dblImageOpacity >= 1.0)
	{
		m_dblImageOpacity = 1.0;
		return TRUE;
	}

	return FALSE;
}
//*******************************************************************************
void CBCGPMetroUITile::StopAnimation()
{
	ASSERT_VALID(this);

	::KillTimer (NULL, m_nAnimationID);
	
	g_cs.Lock ();
	m_mapAnimations.RemoveKey(m_nAnimationID);
	g_cs.Unlock ();

	m_nAnimationID = 0;
	m_dblImageOpacity = 1.0;
}
//*******************************************************************************
VOID CALLBACK CBCGPMetroUITile::AnimTimerProc (HWND /*hwnd*/, UINT /*uMsg*/,
													   UINT_PTR idEvent, DWORD /*dwTime*/)
{
	CBCGPMetroUITile* pTile = NULL;

	g_cs.Lock ();
	BOOL bFound = m_mapAnimations.Lookup ((UINT) idEvent, pTile);
	g_cs.Unlock ();

	if (!bFound)
	{
		return;
	}

	ASSERT_VALID(pTile);

	if (pTile->OnAnimation())
	{
		pTile->StopAnimation();
	}

	pTile->Redraw();
}
//*****************************************************************************************
void CBCGPMetroUITile::SetToolTipText(const CString& strToolTipText)
{
	ASSERT_VALID(this);
	m_strToolTipText = strToolTipText;
}
//*****************************************************************************************
void CBCGPMetroUITile::SetToolTipDescription(const CString& strToolTipDescription)
{
	ASSERT_VALID(this);
	m_strToolTipDescription = strToolTipDescription;
}
//*****************************************************************************************
void CBCGPMetroUITile::Redraw()
{
	ASSERT_VALID(this);

	if (m_pOwner != NULL)
	{
		m_pOwner->Redraw();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPMetroUITiles

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CBCGPMetroUITiles, CBCGPBaseVisualObject)

CBCGPMetroUITiles::CBCGPMetroUITiles()
{
	CBCGPLocalResource locaRes;
	m_BadgeGlyphs.Load(IDR_BCGBARRES_GLYPH_BADGES);

	m_bIsHorizontalLayout = TRUE;
	m_bRoundedShapes = FALSE;

	m_nHorzMargin = 10.;
	m_nVertMargin = 10.;

	m_dblCornerRadius = m_dblCornerRadiusOriginal = 20.0;

	m_nScrollBarHeight = 20.;	// TODO
	m_dblTotalSize = 0.;

	m_nHorzMarginOriginal = m_nHorzMargin;
	m_nVertMarginOriginal = m_nVertMargin;

	m_brFill = CBCGPBrush(CBCGPColor::SteelBlue);

	m_brTileFill = CBCGPBrush(CBCGPColor::Green);
	
	m_brTileFillDark = m_brTileFill;
	m_brTileFillDark.MakeDarker();

	m_colorTileText = CBCGPColor::White;

	m_pHighlighted = NULL;
	m_pPressed = NULL;

	LOGFONT lf;
	globalData.fontRegular.GetLogFont(&lf);
	
	m_sizeSquareOriginal = m_sizeSquare = CBCGPSize(fabs(12. * lf.lfHeight) + 10, fabs(10. * lf.lfHeight) + 10);

	m_textFormatText = CBCGPTextFormat(lf);
	
	m_textFormatText.SetTextAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT_LEADING);
	m_textFormatText.SetTextVerticalAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT_LEADING);
	m_textFormatText.SetWordWrap();
	m_textFormatText.SetClipText();

	m_textFormatName = CBCGPTextFormat(lf);
	
	m_textFormatName.SetTextAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT_LEADING);
	m_textFormatName.SetTextVerticalAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT_LEADING);
	m_textFormatName.SetClipText();

	int lfHeightSaved = lf.lfHeight;
	int lfWeightSaved = lf.lfWeight;

	lf.lfHeight = 4 * lf.lfHeight / 3;
	lf.lfWeight = FW_BOLD;

	m_textFormatBadge = CBCGPTextFormat(lf);

	lf.lfWeight = lfWeightSaved;

	m_textFormatBadge.SetTextAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT_CENTER);
	m_textFormatBadge.SetTextVerticalAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT_CENTER);
	m_textFormatBadge.SetClipText();

	lf.lfHeight = 2 * lfHeightSaved;

	m_textFormatHeader = CBCGPTextFormat(lf);
	
	m_textFormatHeader.SetTextAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT_LEADING);
	m_textFormatHeader.SetTextVerticalAlignment(CBCGPTextFormat::BCGP_TEXT_ALIGNMENT_LEADING);
	m_textFormatHeader.SetClipText();
}
//*****************************************************************************************
CBCGPMetroUITiles::~CBCGPMetroUITiles()
{
	RemoveAll();
}
//*****************************************************************************************
void CBCGPMetroUITiles::SetHorizontalLayout(BOOL bIsHorizontalLayout)
{
	if (m_bIsHorizontalLayout == bIsHorizontalLayout)
	{
		return;
	}

	m_bIsHorizontalLayout = bIsHorizontalLayout;
	SetDirty();
}
//*****************************************************************************************
void CBCGPMetroUITiles::SetRoundedShapes(BOOL bSet)
{
	m_bRoundedShapes = bSet;
}
//*****************************************************************************************
void CBCGPMetroUITiles::SetHorzMargin(double nMargin)
{
	m_nHorzMargin = nMargin;
	m_nHorzMarginOriginal = m_nHorzMargin;

	SetDirty();
}
//*****************************************************************************************
void CBCGPMetroUITiles::SetVertMargin(double nMargin)
{
	m_nVertMargin = nMargin;
	m_nVertMarginOriginal = m_nVertMargin;

	SetDirty();
}
//*****************************************************************************************
void CBCGPMetroUITiles::SetSquareSize(const CBCGPSize& size)
{
	m_sizeSquareOriginal = m_sizeSquare = size;

	SetDirty();
}
//*****************************************************************************************
void CBCGPMetroUITiles::Add(CBCGPMetroUITile* pTile, int nGroup)
{
	pTile->m_pOwner = this;
	pTile->m_nGroup = nGroup;

	m_lstTiles.AddTail(pTile);

	AddSorted(pTile);
	SetDirty();
}
//*****************************************************************************************
void CBCGPMetroUITiles::RemoveAll()
{
	while (!m_lstTiles.IsEmpty())
	{
		delete m_lstTiles.RemoveHead();
	}

	m_lstTilesSorted.RemoveAll();

	m_pHighlighted = m_pPressed = NULL;

	SetDirty();
}
//*****************************************************************************************
void CBCGPMetroUITiles::RecalcLayout(CBCGPGraphicsManager* /*pGM*/)
{
	m_ScrollBar.SetParentVisualObject(this);

	m_dblTotalSize = 0.;

	m_ScrollBar.Reset();

	if (m_lstTiles.IsEmpty())
	{
		return;
	}

	POSITION pos = NULL;

	CBCGPRect rectClient = m_rect;

	if (m_bIsHorizontalLayout)
	{
		rectClient.bottom -= m_nScrollBarHeight;
	}
	else
	{
		rectClient.right -= m_nScrollBarHeight;
	}

	rectClient.DeflateRect(2 * m_nHorzMargin, 2 * m_nVertMargin);

	double x = rectClient.left;
	double y = rectClient.top;

	int nCurrGroup = 0;
	double xColumn = x;
	double yRow = y;

	for (pos = m_lstTilesSorted.GetHeadPosition(); pos != NULL;)
	{
		CBCGPMetroUITile* pTile = m_lstTilesSorted.GetNext(pos);
		ASSERT_VALID(pTile);

		CBCGPSize size = m_sizeSquare;

		if (pTile->m_nGroup != nCurrGroup)
		{
			nCurrGroup = pTile->m_nGroup;

			if (m_bIsHorizontalLayout)
			{
				if (m_dblTotalSize == 0.)
				{
					x = xColumn + 2 * m_sizeSquare.cx + 4 * m_nHorzMargin;
				}
				else
				{
					x = m_dblTotalSize + 4 * m_nHorzMargin;
				}

				y = rectClient.top;
				xColumn = x;
			}
			else
			{
				if (m_dblTotalSize == 0.)
				{
					y = yRow + 2 * m_sizeSquare.cy + 4 * m_nVertMargin;
				}
				else
				{
					y = m_dblTotalSize + 4 * m_nVertMargin;
				}

				x = rectClient.left;
				yRow = y;
			}
		}

		if (pTile->m_bIsWide)
		{
			size.cx *= 2;

			if (m_bIsHorizontalLayout)
			{
				if (x > xColumn)
				{
					x = xColumn;
					y += size.cy;
				}
			}
		}

		if (m_bIsHorizontalLayout)
		{
			if (y + size.cy > rectClient.bottom)
			{
				x += 2 * m_sizeSquare.cx;
				y = rectClient.top;
				xColumn = x;
			}
		}
		else
		{
			if (x + size.cx > rectClient.right)
			{
				y += m_sizeSquare.cy;
				x = rectClient.left;
				yRow = y;
			}
		}

		pTile->m_rect = CBCGPRect(CBCGPPoint(x, y), size);

		if (m_bIsHorizontalLayout)
		{
			m_dblTotalSize = max(m_dblTotalSize, pTile->m_rect.right);
		}
		else
		{
			m_dblTotalSize = max(m_dblTotalSize, pTile->m_rect.bottom);
		}

		pTile->m_rect.right -= m_nHorzMargin;
		pTile->m_rect.bottom -= m_nVertMargin;

		if (m_bIsHorizontalLayout)
		{
			if (pTile->m_bIsWide || x > xColumn)
			{
				x = xColumn;
				y += size.cy;
			}
			else
			{
				x += size.cx;
			}
		}
		else
		{
			x += size.cx;
		}
	}

	if (m_dblTotalSize != 0.)
	{
		m_ScrollBar.SetTotal(m_dblTotalSize);
		m_ScrollBar.SetStep(100);

		ReposScrollBar();
	}
}
//*****************************************************************************************
void CBCGPMetroUITiles::ReposScrollBar()
{
	CBCGPRect rectScroll;

	double dblViewSize = m_bIsHorizontalLayout ? m_rect.Width() : m_rect.Height();

	if (m_dblTotalSize > dblViewSize)
	{
		rectScroll = m_rect;

		if (m_bIsHorizontalLayout)
		{
			rectScroll.top = rectScroll.bottom - m_nScrollBarHeight;
		}
		else
		{
			rectScroll.left = rectScroll.right - m_nScrollBarHeight;
		}
	}

	m_ScrollBar.SetHorizontal(m_bIsHorizontalLayout);
	m_ScrollBar.SetRect(rectScroll);
}
//*****************************************************************************************
void CBCGPMetroUITiles::OnScroll(CBCGPVisualScrollBar* /*pScrollBar*/, double dblDelta)
{
	if (dblDelta == 0.)
	{
		return;
	}

	for (POSITION pos = m_lstTilesSorted.GetHeadPosition(); pos != NULL;)
	{
		CBCGPMetroUITile* pTile = m_lstTilesSorted.GetNext(pos);
		ASSERT_VALID(pTile);

		if (m_bIsHorizontalLayout)
		{
			pTile->m_rect.OffsetRect(-dblDelta, 0);
		}
		else
		{
			pTile->m_rect.OffsetRect(0, -dblDelta);
		}
	}

	Redraw();
}
//*****************************************************************************************
void CBCGPMetroUITiles::OnDraw(CBCGPGraphicsManager* pGM, const CBCGPRect& rectClip, DWORD dwFlags)
{
	if (IsDirty())
	{
		RecalcLayout(pGM);
		SetDirty(FALSE);
	}

	if ((dwFlags & BCGP_DRAW_DYNAMIC) == 0)
	{
		return;
	}

	pGM->FillRectangle(m_rect, m_brFill);

	for (POSITION pos = m_lstTilesSorted.GetHeadPosition(); pos != NULL;)
	{
		CBCGPMetroUITile* pTile = m_lstTilesSorted.GetNext(pos);
		ASSERT_VALID(pTile);

		CBCGPRect rectInter;
		if (rectInter.IntersectRect(pTile->m_rect, rectClip))
		{
			pGM->SetClipRect(pTile->m_rect);

			pTile->DoDraw(this, pGM, pTile == m_pPressed, pTile == m_pHighlighted);

			pGM->ReleaseClipArea();
		}
	}

	m_ScrollBar.DoDraw(pGM);
}
//*****************************************************************************************
void CBCGPMetroUITiles::OnClickAndHoldEvent(UINT nID, const CBCGPPoint& point)
{
	CBCGPBaseVisualObject::OnClickAndHoldEvent(nID, point);

	m_ScrollBar.OnScrollStep(m_ScrollBar.GetNextButton().PtInRect(point));
}
//*****************************************************************************************
BOOL CBCGPMetroUITiles::OnMouseDown(int nButton, const CBCGPPoint& pt)
{
	if (CBCGPPopupMenu::GetActiveMenu() == NULL && nButton == 0)
	{
		if (m_ScrollBar.OnMouseDown(pt))
		{
			m_pPressed = NULL;
			return TRUE;
		}

		m_pPressed = HitTestTile(pt);
	}

	return CBCGPBaseVisualObject::OnMouseDown(nButton, pt);
}
//*****************************************************************************************
void CBCGPMetroUITiles::OnMouseUp(int nButton, const CBCGPPoint& pt)
{
	CBCGPMetroUITile* pClickedTile = NULL;

	if (nButton == 0)
	{
		if (m_pPressed == m_pHighlighted && m_pHighlighted != NULL)
		{
			pClickedTile = m_pHighlighted;
		}

		m_pPressed = m_pHighlighted = NULL;
		m_ScrollBar.OnCancelMode();
	}

	if (pClickedTile != NULL)
	{
		RedrawTile(pClickedTile);

		OnClickTile(pClickedTile);
	}

	CBCGPBaseVisualObject::OnMouseUp(nButton, pt);
}
//*****************************************************************************************
void CBCGPMetroUITiles::OnMouseMove(const CBCGPPoint& pt)
{
	CBCGPBaseVisualObject::OnMouseMove(pt);

	if (CBCGPPopupMenu::GetActiveMenu() != NULL)
	{
		return;
	}

	if (m_ScrollBar.OnDragThumb(pt))
	{
		Redraw();
		return;
	}

	CBCGPMetroUITile* pOldHighlighted = m_pHighlighted;
	m_pHighlighted = HitTestTile(pt);

	if (m_pHighlighted != pOldHighlighted)
	{
		RedrawTile(pOldHighlighted);
		RedrawTile(m_pHighlighted);
	}
}
//*****************************************************************************************
void CBCGPMetroUITiles::OnMouseLeave()
{
	CBCGPBaseVisualObject::OnMouseLeave();

	CBCGPMetroUITile* pOldHighlighted = m_pHighlighted;
	m_pHighlighted = NULL;

	RedrawTile(pOldHighlighted);
}
//*****************************************************************************************
BOOL CBCGPMetroUITiles::OnSetMouseCursor(const CBCGPPoint& pt)
{
	if (CBCGPPopupMenu::GetActiveMenu() == NULL && HitTestTile(pt) != NULL)
	{
		::SetCursor (globalData.GetHandCursor ());
		return TRUE;
	}

	return CBCGPBaseVisualObject::OnSetMouseCursor(pt);
}
//*****************************************************************************************
void CBCGPMetroUITiles::OnCancelMode()
{
	CBCGPBaseVisualObject::OnCancelMode();

	CBCGPMetroUITile* pOldHighlighted = m_pHighlighted;
	m_pHighlighted = NULL;
	m_ScrollBar.OnCancelMode();

	RedrawTile(pOldHighlighted);
}
//*****************************************************************************************
BOOL CBCGPMetroUITiles::OnGetToolTip(const CBCGPPoint& pt, CString& strToolTip, CString& strDescr)
{
	strToolTip.Empty();

	if (CBCGPPopupMenu::GetActiveMenu() == NULL)
	{
		CBCGPMetroUITile* pTile = HitTestTile(pt);
		if (pTile != NULL)
		{
			ASSERT_VALID(pTile);

			if (!pTile->GetToolTipText().IsEmpty())
			{
				strToolTip = pTile->GetToolTipText();
				strDescr = pTile->GetToolTipDescription();
				return TRUE;
			}
			else if (!pTile->GetToolTipDescription().IsEmpty())
			{
				strToolTip = pTile->GetName();
				strDescr = pTile->GetToolTipDescription();
				return TRUE;
			}
		}
	}

	return CBCGPBaseVisualObject::OnGetToolTip(pt, strToolTip, strDescr);
}
//*****************************************************************************************
CBCGPMetroUITile* CBCGPMetroUITiles::HitTestTile(const CBCGPPoint& point) const
{
	for (POSITION pos = m_lstTilesSorted.GetHeadPosition(); pos != NULL;)
	{
		CBCGPMetroUITile* pTile = m_lstTilesSorted.GetNext(pos);
		if (pTile->m_rect.PtInRect(point))
		{
			return pTile;
		}
	}

	return NULL;
}
//*****************************************************************************************
void CBCGPMetroUITiles::RedrawTile(CBCGPMetroUITile* pTile)
{
	if (pTile != NULL)
	{
		GetOwner()->RedrawWindow((CRect)pTile->m_rect);
	}
}
//*****************************************************************************************
void CBCGPMetroUITiles::SetFillBrush(const CBCGPBrush& brFill)
{
	m_brFill = brFill;
	Redraw();
}
//*****************************************************************************************
void CBCGPMetroUITiles::AddSorted(CBCGPMetroUITile* pTileNew)
{
	for (POSITION pos = m_lstTilesSorted.GetHeadPosition(); pos != NULL;)
	{
		POSITION posSaved = pos;

		CBCGPMetroUITile* pTile = m_lstTilesSorted.GetNext(pos);
		ASSERT_VALID(pTile);

		if (pTile->m_nGroup > pTileNew->m_nGroup || 
			pTile->m_nImportance > pTileNew->m_nImportance)
		{
			m_lstTilesSorted.InsertBefore(posSaved, pTileNew);
			return;
		}
	}

	m_lstTilesSorted.AddTail(pTileNew);
}
//*****************************************************************************************
void CBCGPMetroUITiles::OnClickTile(CBCGPMetroUITile* pClickedTile)
{
	if (m_pWndOwner->GetSafeHwnd() == NULL)
	{
		return;
	}

	CWnd* pOwner = m_pWndOwner->GetOwner();
	if (pOwner->GetSafeHwnd() == NULL)
	{
		return;
	}

	m_pWndOwner->GetOwner()->PostMessage(BCGM_ON_CLICK_METRO_UI_TILE, (WPARAM)GetID(), (LPARAM)pClickedTile);
}
//*****************************************************************************************
void CBCGPMetroUITiles::OnScaleRatioChanged(const CBCGPSize& /*sizeScaleRatioOld*/)
{
	m_textFormatName.Scale(m_sizeScaleRatio.cy);
	m_textFormatText.Scale(m_sizeScaleRatio.cy);
	m_textFormatHeader.Scale(m_sizeScaleRatio.cy);
	m_textFormatBadge.Scale(m_sizeScaleRatio.cy);

	m_nHorzMargin = m_nHorzMarginOriginal * m_sizeScaleRatio.cx;
	m_nVertMargin = m_nVertMarginOriginal * m_sizeScaleRatio.cy;

	m_sizeSquare.cx = m_sizeSquareOriginal.cx * m_sizeScaleRatio.cx;
	m_sizeSquare.cy = m_sizeSquareOriginal.cy * m_sizeScaleRatio.cy;

	m_dblCornerRadius = m_dblCornerRadiusOriginal * m_sizeScaleRatio.cx;

	SetDirty();
}
//*******************************************************************************
BOOL CBCGPMetroUITiles::OnMouseWheel(const CBCGPPoint& pt, short zDelta)
{
	if (!m_ScrollBar.GetRect().IsRectEmpty())
	{
		const int nSteps = abs(zDelta) / WHEEL_DELTA;
		for (int i = 0; i < nSteps; i++)
		{
			m_ScrollBar.OnScrollStep(zDelta < 0);
		}

		return TRUE;
	}

	return CBCGPBaseVisualObject::OnMouseWheel(pt, zDelta);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPMetroUITilesCtrl

CBCGPMetroUITilesCtrl::CBCGPMetroUITilesCtrl()
{
	m_pMetroUI = NULL;
}

CBCGPMetroUITilesCtrl::~CBCGPMetroUITilesCtrl()
{
	if (m_pMetroUI != NULL)
	{
		delete m_pMetroUI;
	}
}

BEGIN_MESSAGE_MAP(CBCGPMetroUITilesCtrl, CBCGPVisualCtrl)
	//{{AFX_MSG_MAP(CBCGPMetroUITilesCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGPMetroUITilesCtrl message handlers
