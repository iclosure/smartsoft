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
// BCGPProgressCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPVisualManager.h"
#ifndef _BCGSUITE_
#include "BCGPRibbonProgressBar.h"
#include "BCGPToolBarImages.h"
#endif
#include "bcgglobals.h"
#include "BCGPProgressCtrl.h"
#include "BCGPDlgImpl.h"

#ifndef _BCGSUITE_
	#define visualManagerMFC	CBCGPVisualManager::GetInstance ()
#else
	#define visualManagerMFC	CMFCVisualManager::GetInstance ()
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int idTimerMarquee = 1001;

/////////////////////////////////////////////////////////////////////////////
// CBCGPProgressCtrl

IMPLEMENT_DYNAMIC(CBCGPProgressCtrl, CProgressCtrl)

CBCGPProgressCtrl::CBCGPProgressCtrl()
{
	m_bVisualManagerStyle = FALSE;
	m_bOnGlass = FALSE;
	m_nMarqueeStep = 0;
}

CBCGPProgressCtrl::~CBCGPProgressCtrl()
{
}

BEGIN_MESSAGE_MAP(CBCGPProgressCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CBCGPProgressCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLVMMODE, OnBCGSetControlVMMode)
	ON_REGISTERED_MESSAGE(BCGM_ONSETCONTROLAERO, OnBCGSetControlAero)
	ON_MESSAGE(PBM_SETMARQUEE, OnSetMarquee)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPProgressCtrl message handlers

BOOL CBCGPProgressCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
//**************************************************************************
void CBCGPProgressCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBCGPMemDC memDC (dc, this);
	CDC* pDC = &memDC.GetDC ();

	if (!CBCGPVisualManager::GetInstance ()->OnFillParentBarBackground (this, pDC))
	{
		globalData.DrawParentBackground (this, pDC);
	}

	CRect rect;
	GetClientRect (rect);

	CRect rectProgress = rect;
	rectProgress.DeflateRect (1, 1);

	BOOL bInfiniteMode = (GetStyle() & PBS_MARQUEE) != 0;

	int nMin = 0;
	int nMax = 100;
	int nPos = m_nMarqueeStep;

	if (!bInfiniteMode)
	{
		GetRange (nMin, nMax);
		nPos = GetPos();
	}

	CRect rectChunk (0, 0, 0, 0);

	const int nMarginHorz = 1;
	const int nMarginVert = 1;

	if (nMax > nMin && rectProgress.Width () > (nMarginHorz * 2) &&
		rectProgress.Height () > (nMarginVert * 2))
	{
		rectChunk = rectProgress;
		rectChunk.DeflateRect (nMarginHorz, nMarginVert);

		int ptPos = nPos * rectChunk.Width () / (nMax - nMin);

		if (bInfiniteMode)
		{
			const int nChunkWidth = min(rectProgress.Height() * 3, rectProgress.Width() / 6);
			
			rectChunk.InflateRect (nChunkWidth, 0);
			ptPos = nPos * rectChunk.Width () / (nMax - nMin);

			rectChunk.left += ptPos;
			ptPos = nChunkWidth;
		}

		rectChunk.right = min (rectChunk.left + ptPos, rectProgress.right - nMarginHorz);
		rectChunk.left = max (rectChunk.left, rectProgress.left + nMarginHorz);
	}

	CBCGPDrawOnGlass dog (m_bOnGlass);

#ifndef BCGP_EXCLUDE_RIBBON

	CBCGPRibbonProgressBar dummy;
	dummy.SetRange(nMin, nMax);
	dummy.SetPos(nPos, FALSE);

	visualManagerMFC->OnDrawRibbonProgressBar (
		pDC, &dummy, rectProgress, rectChunk, bInfiniteMode);
#else
	visualManagerMFC->OnDrawStatusBarProgress (pDC, NULL,
										rectProgress, nMax - nMin, 
										GetPos (),
										globalData.clrHilite, 
										(COLORREF)-1, 
										(COLORREF)-1,
										FALSE);
#endif
}
//**************************************************************************
void CBCGPProgressCtrl::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS FAR* /*lpncsp*/) 
{
}
//**************************************************************************
void CBCGPProgressCtrl::OnNcPaint() 
{
}
//**************************************************************************
LRESULT CBCGPProgressCtrl::OnBCGSetControlVMMode (WPARAM wp, LPARAM)
{
	m_bVisualManagerStyle = (BOOL) wp;
	return 0;
}
//**************************************************************************
LRESULT CBCGPProgressCtrl::OnBCGSetControlAero (WPARAM wp, LPARAM)
{
	m_bOnGlass = (BOOL) wp;
	return 0;
}
//**************************************************************************
LRESULT CBCGPProgressCtrl::OnSetMarquee(WPARAM wp, LPARAM lp)
{
	BOOL bStart = (BOOL)wp;
	int nElapse = (lp == 0) ? 30 : (int)lp;

	if (bStart)
	{
		SetTimer(idTimerMarquee, nElapse, NULL);
	}
	else
	{
		KillTimer(idTimerMarquee);
	}

	return Default();
}
//**************************************************************************
void CBCGPProgressCtrl::OnTimer(UINT_PTR nIDEvent) 
{
	CProgressCtrl::OnTimer(nIDEvent);

	if (nIDEvent == idTimerMarquee)
	{
		m_nMarqueeStep++;
		if (m_nMarqueeStep > 100)
		{
			m_nMarqueeStep = 0;
		}

		RedrawWindow();
	}
}
