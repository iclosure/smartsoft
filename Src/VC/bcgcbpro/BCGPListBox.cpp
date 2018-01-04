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
// BCGPListBox.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPListBox.h"
#include "BCGPDlgImpl.h"
#include "BCGPVisualManager.h"
#include "BCGPPropertySheet.h"
#include "BCGPStatic.h"
#include "TrackMouse.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CBCGPListBox, CListBox)

#ifndef _BCGSUITE_
#define PIN_AREA_WIDTH	(CBCGPVisualManager::GetInstance ()->GetPinSize(TRUE).cx + 10)
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPListBox

CBCGPListBox::CBCGPListBox()
{
	m_bVisualManagerStyle = FALSE;
	m_bOnGlass = FALSE;
	m_nHighlightedItem = -1;
	m_bTracked = FALSE;
	m_hImageList = NULL;
	m_sizeImage = CSize(0, 0);
	m_bBackstageMode = FALSE;
	m_bPropertySheetNavigator = FALSE;
	m_bPins = FALSE;
	m_bIsPinHighlighted = FALSE;
	m_nDescrRows = 0;
	m_nClickedItem = -1;
}

CBCGPListBox::~CBCGPListBox()
{
}


BEGIN_MESSAGE_MAP(CBCGPListBox, CListBox)
	//{{AFX_MSG_MAP(CBCGPListBox)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_CANCELMODE()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLVMMODE, OnBCGSetControlVMMode)
	ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLAERO, OnBCGSetControlAero)
	ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLBACKSTAGEMODE, OnBCGSetControlBackStageMode)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_CONTROL_REFLECT_EX(LBN_SELCHANGE, OnSelchange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPListBox message handlers

LRESULT CBCGPListBox::OnBCGSetControlVMMode (WPARAM wp, LPARAM)
{
	m_bVisualManagerStyle = (BOOL) wp;
	return 0;
}
//**************************************************************************
LRESULT CBCGPListBox::OnBCGSetControlAero (WPARAM wp, LPARAM)
{
	m_bOnGlass = (BOOL) wp;
	return 0;
}
//**************************************************************************
int CBCGPListBox::HitTest(CPoint pt, BOOL* pbPin)
{
	if (pbPin != NULL)
	{
		*pbPin = FALSE;
	}

	if ((GetStyle() & LBS_NOSEL) == LBS_NOSEL)
	{
		return -1;
	}

	for (int i = 0; i < GetCount (); i++)
	{
		CRect rectItem;
		GetItemRect (i, rectItem);

		if (rectItem.PtInRect (pt))
		{
			if (IsCaptionItem(i) || IsSeparatorItem(i))
			{
				return -1;
			}
			
#ifndef _BCGSUITE_
			if (pbPin != NULL && m_bPins)
			{
				*pbPin = (pt.x > rectItem.right - PIN_AREA_WIDTH);
			}
#endif
			return i;
		}
	}

	return -1;
}
//**************************************************************************
void CBCGPListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	CListBox::OnMouseMove (nFlags, point);

	if ((GetStyle() & LBS_NOSEL) == LBS_NOSEL)
	{
		return;
	}

	ASSERT (IsWindowEnabled ());

	BOOL bIsPinHighlighted = FALSE;
	int nHighlightedItem = HitTest(point, &bIsPinHighlighted);

	if (!m_bTracked)
	{
		m_bTracked = TRUE;
		
		TRACKMOUSEEVENT trackmouseevent;
		trackmouseevent.cbSize = sizeof(trackmouseevent);
		trackmouseevent.dwFlags = TME_LEAVE;
		trackmouseevent.hwndTrack = GetSafeHwnd();
		trackmouseevent.dwHoverTime = HOVER_DEFAULT;
		BCGPTrackMouse (&trackmouseevent);
	}

	if (nHighlightedItem != m_nHighlightedItem || m_bIsPinHighlighted != bIsPinHighlighted)
	{
		CRect rectItem;

		if (m_nHighlightedItem >= 0)
		{
			GetItemRect (m_nHighlightedItem, rectItem);
			InvalidateRect (rectItem);
		}

		m_nHighlightedItem = nHighlightedItem;
		m_bIsPinHighlighted = bIsPinHighlighted;

		if (m_nHighlightedItem >= 0)
		{
			GetItemRect (m_nHighlightedItem, rectItem);
			InvalidateRect (rectItem);
		}

		UpdateWindow ();
	}
}
//***********************************************************************************************	
LRESULT CBCGPListBox::OnMouseLeave(WPARAM,LPARAM)
{
	m_bTracked = FALSE;
	m_bIsPinHighlighted = FALSE;

	if (m_nHighlightedItem >= 0)
	{
		CRect rectItem;
		GetItemRect (m_nHighlightedItem, rectItem);

		m_nHighlightedItem = -1;
		RedrawWindow (rectItem);
	}

	return 0;
}
//***********************************************************************************************	
void CBCGPListBox::DrawItem(LPDRAWITEMSTRUCT /*lpDIS*/) 
{
}
//***********************************************************************************************	
void CBCGPListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	int nImageHeight = m_sizeImage.cy + 4;
	int nTextHeight = m_nDescrRows == 0 ?
		(globalData.GetTextHeight () * 9 / 5) : globalData.GetTextHeight () * (m_nDescrRows + 1) * 4 / 3;

	lpMeasureItemStruct->itemHeight = max(nImageHeight, nTextHeight);
}
//***********************************************************************************************	
void CBCGPListBox::OnDrawItemContent(CDC* pDC, CRect rect, int nIndex)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	const BOOL bIsCaption = IsCaptionItem(nIndex);

	CFont* pOldFont = NULL;

	if (!bIsCaption)
	{
		int nIcon = -1;
		if (m_hImageList != NULL && m_ItemIcons.Lookup(nIndex, nIcon))
		{
			CRect rectImage = rect;

			rectImage.top += (rect.Height () - m_sizeImage.cy) / 2;
			rectImage.bottom = rectImage.top + m_sizeImage.cy;

			rectImage.left += 4;
			rectImage.right = rectImage.left + m_sizeImage.cx;

			CImageList* pImageList = CImageList::FromHandle(m_hImageList);
			pImageList->Draw (pDC, nIcon, rectImage.TopLeft (), ILD_TRANSPARENT);

			rect.left += m_sizeImage.cx + max(10, m_sizeImage.cx / 3);
			rect.right -= 10;
		}
		else
		{
			rect.DeflateRect (10, 0);
		}
	}
#ifndef _BCGSUITE_
	else
	{
		pOldFont = pDC->SelectObject(&globalData.fontCaption);
	}
#endif

	CString strText;
	GetText (nIndex, strText);

	UINT uiDTFlags = DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX;

	if (bIsCaption)
	{
		int nTextHeight = pDC->DrawText (strText, rect, uiDTFlags);

		CBCGPStatic ctrl;
		ctrl.m_bBackstageMode = m_bBackstageMode;

		CRect rectSeparator = rect;
		rectSeparator.top = rect.CenterPoint().y + nTextHeight / 2;
		rectSeparator.bottom = rectSeparator.top + 1;
		rectSeparator.right -= 5;

#ifndef _BCGSUITE_
		if (!globalData.IsHighContastMode () && CBCGPVisualManager::GetInstance ()->IsOwnerDrawDlgSeparator(&ctrl))
		{
			CBCGPVisualManager::GetInstance ()->OnDrawDlgSeparator(pDC, &ctrl, rectSeparator, TRUE);
		}
		else
#endif
		{
			CPen pen (PS_SOLID, 1, globalData.clrBtnShadow);
			CPen* pOldPen = (CPen*)pDC->SelectObject (&pen);

			pDC->MoveTo(rectSeparator.left, rectSeparator.top);
			pDC->LineTo(rectSeparator.right, rectSeparator.top);

			pDC->SelectObject(pOldPen);
		}

		pDC->SelectObject(pOldFont);
	}
	else if (m_ItemDescriptions.IsEmpty() || m_nDescrRows == 0)
	{
		pDC->DrawText (strText, rect, uiDTFlags);
	}
	else
	{
		pOldFont = pDC->SelectObject(&globalData.fontBold);

		rect.DeflateRect(0, globalData.GetTextHeight() / 3);


		CRect rectTop = rect;
		rectTop.bottom = rectTop.top + rectTop.Height() / (m_nDescrRows + 1);

		pDC->DrawText (strText, rectTop, uiDTFlags);

		pDC->SelectObject(pOldFont);

		CString strDescr;
		if (m_ItemDescriptions.Lookup(nIndex, strDescr))
		{
			CRect rectBottom = rect;
			rectBottom.top = rectTop.bottom;

			uiDTFlags = DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX | DT_WORDBREAK;
			pDC->DrawText (strDescr, rectBottom, uiDTFlags);
		}
	}
}
//***************************************************************************************
BOOL CBCGPListBox::SetImageList (HIMAGELIST hImageList, int nVertMargin)
{
	ASSERT (hImageList != NULL);

	CImageList* pImageList = CImageList::FromHandle (hImageList);
	if (pImageList == NULL)
	{
		ASSERT (FALSE);
		return FALSE;
	}

	IMAGEINFO info;
	pImageList->GetImageInfo (0, &info);

	CRect rectImage = info.rcImage;
	m_sizeImage = rectImage.Size ();

	m_hImageList = hImageList;

	SetItemHeight(-1, max(GetItemHeight(-1), m_sizeImage.cy + 2 * nVertMargin));
	return TRUE;
}
//***********************************************************************************************************
BOOL CBCGPListBox::OnEraseBkgnd(CDC* /*pDC*/)
{
	if (GetHorizontalExtent() > 0)
	{
		return (BOOL)Default();
	}

	return TRUE;
}
//***********************************************************************************************************
void CBCGPListBox::OnPaint() 
{
	int cxScroll = GetScrollPos(SB_HORZ);
	BOOL bNoSel = ((GetStyle() & LBS_NOSEL) == LBS_NOSEL);

	CPaintDC dcPaint(this); // device context for painting

	CRect rectClient;
	GetClientRect(rectClient);

	CRgn rgn;
	rgn.CreateRectRgnIndirect (rectClient);
	dcPaint.SelectClipRgn (&rgn);

	{
		CBCGPMemDC memDC(dcPaint, this);
		CDC* pDC = &memDC.GetDC();

		COLORREF clrTextDefault = globalData.clrWindowText;

#ifndef _BCGSUITE_
		if (m_bVisualManagerStyle)
		{
			clrTextDefault = CBCGPVisualManager::GetInstance ()->OnFillListBox(pDC, this, rectClient);
		}
		else
#endif
		{
			pDC->FillRect(rectClient, &globalData.brWindow);
		}

#ifndef _BCGSUITE_
		if (m_bBackstageMode)
		{
			if (m_bPropertySheetNavigator)
			{
				CBCGPPropertySheet* pPropSheet = DYNAMIC_DOWNCAST(CBCGPPropertySheet, GetParent());
				if (pPropSheet != NULL)
				{
					ASSERT_VALID(pPropSheet);
					pPropSheet->OnDrawListBoxBackground(pDC, this);
				}

				CRect rectSeparator = rectClient;
				rectSeparator.left = rectSeparator.right - 3;
				rectSeparator.right -= 2;
				CBCGPStatic ctrl;
				ctrl.m_bBackstageMode = TRUE;

				CBCGPVisualManager::GetInstance ()->OnDrawDlgSeparator(pDC, &ctrl, rectSeparator, FALSE);
			}
		}
#endif
		int nStart = GetTopIndex ();
		int nCount = GetCount ();

		if (nStart != LB_ERR && nCount > 0)
		{
			pDC->SetBkMode (TRANSPARENT);

			CFont* pOldFont = pDC->SelectObject (&globalData.fontRegular);
			ASSERT_VALID (pOldFont);

			CArray<int,int> arSelection;

			int nSelCount = GetSelCount();
			if (nSelCount != LB_ERR && !bNoSel)
			{
				if (nSelCount > 0)
				{
					arSelection.SetSize (nSelCount);
					GetSelItems (nSelCount, arSelection.GetData());	
				}
			}
			else
			{
				int nSel = GetCurSel();
				if (nSel != LB_ERR)
				{
					nSelCount = 1;
					arSelection.Add (nSel);
				}
			}
	
			nSelCount = (int)arSelection.GetSize ();

			const BOOL bIsFocused = (CWnd::GetFocus() == this);
			
			BOOL bFocusRectDrawn = FALSE;

			for (int nIndex = nStart; nIndex < nCount; nIndex++)
			{
				CRect rect;
				GetItemRect(nIndex, rect);

				if (rect.bottom < rectClient.top || rectClient.bottom < rect.top)
				{
					break;
				}

				int cxExtent = GetHorizontalExtent();
				if (cxExtent > rect.Width())
				{
					rect.right = rect.left + cxExtent;
				}

				rect.OffsetRect(-cxScroll, 0);
				rect.DeflateRect(2, 0);

#ifndef _BCGSUITE_
				CRect rectPin(0, 0, 0, 0);
				if (m_bPins && !IsSeparatorItem(nIndex) && !IsCaptionItem(nIndex))
				{
					rectPin = rect;
					rect.right -= PIN_AREA_WIDTH;
					rectPin.left = rect.right;
				}
#endif
				BOOL bIsSelected = FALSE;
				for (int nSelIndex = 0; nSelIndex < nSelCount; nSelIndex++)
				{
					if (nIndex == arSelection[nSelIndex])
					{
						bIsSelected = TRUE;
						break;
					}
				}
			
				const BOOL bIsHighlihted = (nIndex == m_nHighlightedItem) || (bIsSelected && bIsFocused);

				COLORREF clrText = (COLORREF)-1;

				BOOL bIsCaptionItem = IsCaptionItem(nIndex);

				if (((bIsHighlihted && !m_bIsPinHighlighted) || bIsSelected) && !bIsCaptionItem && !bNoSel)
				{
					if (m_bVisualManagerStyle)
					{
						clrText = CBCGPVisualManager::GetInstance ()->OnFillListBoxItem (
							pDC, this, nIndex, rect, bIsHighlihted, bIsSelected);
					}
					else
					{
						pDC->FillRect (rect, &globalData.brHilite);
						
						if (bIsHighlihted)
						{
							pDC->DrawFocusRect (rect);
							bFocusRectDrawn = TRUE;
						}

						clrText = globalData.clrTextHilite;
					}
				}

				if (clrText == (COLORREF)-1)
				{
					pDC->SetTextColor (clrTextDefault);
				}
				else
				{
					pDC->SetTextColor (clrText);
				}

				OnDrawItemContent(pDC, rect, nIndex);

				if (bIsFocused && nSelCount <= 0 && !bFocusRectDrawn && !bIsCaptionItem && !bNoSel)
				{
					pDC->DrawFocusRect (rect);
					bFocusRectDrawn = TRUE;
				}

#ifndef _BCGSUITE_
				if (!rectPin.IsRectEmpty())
				{
					BOOL bIsPinHighlighted = FALSE;

					if (nIndex == m_nHighlightedItem && m_bIsPinHighlighted)
					{
						bIsPinHighlighted = TRUE;

						if (m_bVisualManagerStyle)
						{
							CBCGPVisualManager::GetInstance ()->OnFillListBoxItem (
								pDC, this, nIndex, rectPin, TRUE, FALSE);
						}
						else
						{
							pDC->FillRect (rect, &globalData.brHilite);
						}
					}

					BOOL bIsDark = TRUE;

					if (clrText != (COLORREF)-1)
					{
						if (GetRValue (clrText) > 192 &&
							GetGValue (clrText) > 192 &&
							GetBValue (clrText) > 192)
						{
							bIsDark = FALSE;
						}
					}

					BOOL bIsPinned = IsItemPinned(nIndex);

					CSize sizePin = CBCGPVisualManager::GetInstance ()->GetPinSize(bIsPinned);
					CRect rectPinImage(
						CPoint(
							rectPin.CenterPoint().x - sizePin.cx / 2,
							rectPin.CenterPoint().y - sizePin.cy / 2),
						sizePin);

					CBCGPVisualManager::GetInstance()->OnDrawPin(pDC, rectPinImage, bIsPinned,
						bIsDark, bIsPinHighlighted, FALSE, FALSE);
				}
#endif
			}

			pDC->SelectObject (pOldFont);
		}
	}

	dcPaint.SelectClipRgn (NULL);
}
//************************************************************************************************************
BOOL CBCGPListBox::OnSelchange() 
{
	RedrawWindow();
	return FALSE;
}
//************************************************************************************************************
void CBCGPListBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
	RedrawWindow();
}
//**************************************************************************
LRESULT CBCGPListBox::OnBCGSetControlBackStageMode (WPARAM, LPARAM)
{
	m_bBackstageMode = TRUE;
	return 0;
}
//**************************************************************************
void CBCGPListBox::AddCaption(LPCTSTR lpszCaption)
{
	int nIndex = AddString(lpszCaption == NULL ? _T("") : lpszCaption);

	m_lstCaptionIndexes.AddTail(nIndex);
}
//**************************************************************************
void CBCGPListBox::AddSeparator()
{
	AddCaption(NULL);
}
//**************************************************************************
void CBCGPListBox::CleanUp()
{
	ResetContent();

	m_sizeImage = CSize(0, 0);
	m_hImageList = NULL;

	m_lstCaptionIndexes.RemoveAll();
	m_ItemIcons.RemoveAll();
	m_ItemDescriptions.RemoveAll();
}
//**************************************************************************
BOOL CBCGPListBox::IsCaptionItem(int nIndex) const
{
	return m_lstCaptionIndexes.Find(nIndex) != 0;
}
//**************************************************************************
BOOL CBCGPListBox::IsSeparatorItem(int nIndex) const
{
	return IsCaptionItem(nIndex);
}
//**************************************************************************
void CBCGPListBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if ((GetStyle() & LBS_NOSEL) == LBS_NOSEL)
	{
		CListBox::OnLButtonDown(nFlags, point);
		return;
	}

	if (!m_lstCaptionIndexes.IsEmpty() && HitTest(point) == -1)
	{
		return;
	}
	
	CListBox::OnLButtonDown(nFlags, point);
	m_nClickedItem = HitTest(point);
}
//**************************************************************************
void CBCGPListBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if ((GetStyle() & LBS_NOSEL) == LBS_NOSEL)
	{
		CListBox::OnLButtonUp(nFlags, point);
		return;
	}

	HWND hwndThis = GetSafeHwnd();

	CListBox::OnLButtonUp(nFlags, point);

	if (!::IsWindow(hwndThis))
	{
		return;
	}

	if (IsSeparatorItem(m_nClickedItem))
	{
		return;
	}

	int nClickedItem = m_nClickedItem;
	m_nClickedItem = -1;

	BOOL bPin = FALSE;

	if (nClickedItem >= 0 && nClickedItem == HitTest(point, &bPin))
	{
		if (bPin)
		{
			OnClickPin(nClickedItem);
		}
		else
		{
			OnClickItem(nClickedItem);
		}
	}
}
//**************************************************************************
void CBCGPListBox::OnCancelMode() 
{
	CListBox::OnCancelMode();
	m_nClickedItem = -1;
}
//**************************************************************************
void CBCGPListBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	const int nPrevSel = GetCurSel();

	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);

	if ((GetStyle() & LBS_NOSEL) == LBS_NOSEL)
	{
		return;
	}

	if (m_lstCaptionIndexes.IsEmpty())
	{
		return;
	}

	const int nCurSel = GetCurSel();
	if (!IsCaptionItem(nCurSel) || nCurSel == nPrevSel)
	{
		return;
	}

	BOOL bFindNext = FALSE;

	switch (nChar)
	{
	case VK_HOME:
	case VK_DOWN:
	case VK_NEXT:
		bFindNext = TRUE;
		break;
	}

	int nNewSel = -1;

	if (bFindNext)
	{
		for(int i = nCurSel + 1; i < GetCount(); i++)
		{
			if (!IsCaptionItem(i))
			{
				nNewSel = i;
				break;
			}
		}
	}
	else
	{
		for(int i = nCurSel - 1; i >= 0; i--)
		{
			if (!IsCaptionItem(i))
			{
				nNewSel = i;
				break;
			}
		}
	}

	SetCurSel(nNewSel != -1 ? nNewSel : nPrevSel);
	RedrawWindow();
}
//**************************************************************************
BOOL CBCGPListBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (CListBox::OnMouseWheel(nFlags, zDelta, pt))
	{
		RedrawWindow();
		return TRUE;
	}

	return FALSE;
}
//**************************************************************************
void CBCGPListBox::EnableItemDescription(BOOL bEnable, int nRows)
{
	m_nDescrRows = bEnable ? nRows : 0;
}
//**************************************************************************
BOOL CBCGPListBox::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN && GetFocus()->GetSafeHwnd() == GetSafeHwnd())
	{
		if (OnReturnKey())
		{
			return TRUE;
		}
	}
	
	return CListBox::PreTranslateMessage(pMsg);
}

#ifndef _BCGSUITE_
void CBCGPListBox::EnablePins(BOOL bEnable)
{
	m_bPins = bEnable;

	if (GetSafeHwnd() != NULL)
	{
		RedrawWindow();
	}
}
//**********************************************************************************************
void CBCGPListBox::SetItemPinned(int nIndex, BOOL bSet, BOOL bRedraw)
{
	m_ItemPinStates.SetAt(nIndex, bSet);

	if (bRedraw && GetSafeHwnd() != NULL)
	{
		CRect rectItem;
		GetItemRect(nIndex, rectItem);

		RedrawWindow (rectItem);
	}
}
//**********************************************************************************************
BOOL CBCGPListBox::IsItemPinned(int nIndex)
{
	BOOL bState = FALSE;

	if (!m_ItemPinStates.Lookup(nIndex, bState))
	{
		return FALSE;
	}

	return bState;
}
//**********************************************************************************************
void CBCGPListBox::ResetPins()
{
	m_ItemPinStates.RemoveAll();
}
#endif

void CBCGPListBox::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CListBox::OnHScroll(nSBCode, nPos, pScrollBar);
	RedrawWindow();
}
