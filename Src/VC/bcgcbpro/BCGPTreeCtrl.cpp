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
// BCGPTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPTreeCtrl.h"
#include "BCGPDlgImpl.h"
#include "BCGPVisualManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPTreeCtrl

IMPLEMENT_DYNAMIC(CBCGPTreeCtrl, CTreeCtrl)

CBCGPTreeCtrl::CBCGPTreeCtrl()
{
	m_bVisualManagerStyle = FALSE;
}

CBCGPTreeCtrl::~CBCGPTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CBCGPTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CBCGPTreeCtrl)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLVMMODE, OnBCGSetControlVMMode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPTreeCtrl message handlers

void CBCGPTreeCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVCUSTOMDRAW pNMTVCD = (LPNMTVCUSTOMDRAW) pNMHDR;
	if (pNMTVCD == NULL)
	{
		*pResult = -1;
		return;
	}

	*pResult = CDRF_DODEFAULT;

	if (!m_bVisualManagerStyle)
	{
		return;
	}

	if (pNMTVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYPOSTPAINT | CDRF_NOTIFYITEMDRAW;
	}
	else if (pNMTVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		if (!IsWindowEnabled ())
		{
			COLORREF clrBk = CBCGPVisualManager::GetInstance ()->GetTreeControlFillColor(this, FALSE, FALSE, TRUE);
			COLORREF clrText = CBCGPVisualManager::GetInstance ()->GetTreeControlTextColor(this, FALSE, FALSE, TRUE);

			if (clrBk != (COLORREF)-1)
			{
				pNMTVCD->clrTextBk = clrBk;
			}

			if (clrText != (COLORREF)-1)
			{
				pNMTVCD->clrText = clrText;
			}

			return;
		}

		UINT nState = GetItemState ((HTREEITEM) pNMTVCD->nmcd.dwItemSpec, TVIF_STATE);
		if ((nState & (TVIS_SELECTED | TVIS_DROPHILITED)) == 0)
		{
			COLORREF clrBk = CBCGPVisualManager::GetInstance ()->GetTreeControlFillColor(this);
			COLORREF clrText = CBCGPVisualManager::GetInstance ()->GetTreeControlTextColor(this);

			if (clrBk != (COLORREF)-1)
			{
				pNMTVCD->clrTextBk = clrBk;
			}

			if (clrText != (COLORREF)-1)
			{
				pNMTVCD->clrText = clrText;
			}
			return;
		}

		if (GetFocus () != this && (nState & TVIS_DROPHILITED) == 0)
		{
			BOOL bIsSelected = (GetStyle () & TVS_SHOWSELALWAYS) == TVS_SHOWSELALWAYS;

			COLORREF clrBk = CBCGPVisualManager::GetInstance ()->GetTreeControlFillColor(this, bIsSelected);
			COLORREF clrText = CBCGPVisualManager::GetInstance ()->GetTreeControlTextColor(this, bIsSelected);

			if (clrBk != (COLORREF)-1)
			{
				pNMTVCD->clrTextBk = clrBk;
			}

			if (clrText != (COLORREF)-1)
			{
				pNMTVCD->clrText = clrText;
			}
		}
		else
		{
			COLORREF clrBk = CBCGPVisualManager::GetInstance ()->GetTreeControlFillColor(this, TRUE, TRUE);
			COLORREF clrText = CBCGPVisualManager::GetInstance ()->GetTreeControlTextColor(this, TRUE, TRUE);

			if (clrBk != (COLORREF)-1)
			{
				pNMTVCD->clrTextBk = clrBk;
			}

			if (clrText != (COLORREF)-1)
			{
				pNMTVCD->clrText = clrText;
			}
		}
	}
}
//**************************************************************************
LRESULT CBCGPTreeCtrl::OnBCGSetControlVMMode (WPARAM wp, LPARAM)
{
	m_bVisualManagerStyle = (BOOL) wp;
	return 0;
}
//****************************************************************************************
BOOL CBCGPTreeCtrl::OnEraseBkgnd(CDC* pDC) 
{
	if (m_bVisualManagerStyle)
	{
		COLORREF clr = CBCGPVisualManager::GetInstance ()->GetTreeControlFillColor(this);
		if (clr != (COLORREF)-1)
		{
			CRect rectClient;
			GetClientRect (&rectClient);

			CBrush br(clr);
			pDC->FillRect(rectClient, &br);

			return TRUE;
		}
	}

	return CTreeCtrl::OnEraseBkgnd(pDC);
}
