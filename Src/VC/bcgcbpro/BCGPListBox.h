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
// BCGPListBox.h : header file
//

#if !defined(AFX_BCGPLISTBOX_H__7E778AEF_FFBB_486D_A7B3_D25D7868F5D9__INCLUDED_)
#define AFX_BCGPLISTBOX_H__7E778AEF_FFBB_486D_A7B3_D25D7868F5D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bcgcbpro.h"

/////////////////////////////////////////////////////////////////////////////
// CBCGPListBox window

class BCGCBPRODLLEXPORT CBCGPListBox : public CListBox
{
	friend class CBCGPPropertySheet;

	DECLARE_DYNAMIC(CBCGPListBox)

// Construction
public:
	CBCGPListBox();

// Attributes
public:
	BOOL	m_bOnGlass;
	BOOL	m_bVisualManagerStyle;

	BOOL IsBackstageMode() const
	{
		return m_bBackstageMode;
	}

	BOOL IsPropertySheetNavigator() const
	{
		return m_bPropertySheetNavigator;
	}

#ifndef _BCGSUITE_
	void EnablePins(BOOL bEnable = TRUE);
	BOOL HasPins() const
	{
		return m_bPins;
	}
#endif

	void EnableItemDescription(BOOL bEnable = TRUE, int nRows = 2);
	BOOL HasItemDescriptions() const
	{
		return m_nDescrRows > 0;
	}

// Operations
public:
	BOOL SetImageList (HIMAGELIST hImageList, int nVertMargin = 2);
	void SetItemImage(int nIndex, int nImageIndex)
	{
		m_ItemIcons.SetAt(nIndex, nImageIndex);
	}

	void SetItemDescription(int nIndex, const CString& strDescription)
	{
		m_ItemDescriptions.SetAt(nIndex, strDescription);
	}

#ifndef _BCGSUITE_
	void SetItemPinned(int nIndex, BOOL bSet = TRUE, BOOL bRedraw = FALSE);
	BOOL IsItemPinned(int nIndex);
	void ResetPins();
#endif

	void AddCaption(LPCTSTR lpszCaption);
	void AddSeparator();

	BOOL IsCaptionItem(int nIndex) const;
	BOOL IsSeparatorItem(int nIndex) const;

	void CleanUp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	virtual void OnClickPin(int /*nClickedItem*/)	{}
	virtual void OnClickItem(int /*nClickedItem*/)	{}
	virtual BOOL OnReturnKey() { return FALSE; }

	virtual void OnDrawItemContent(CDC* pDC, CRect rect, int nIndex);

// Implementation
public:
	virtual ~CBCGPListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPListBox)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	afx_msg BOOL OnSelchange();
	afx_msg LRESULT OnBCGSetControlVMMode (WPARAM, LPARAM);
	afx_msg LRESULT OnBCGSetControlAero (WPARAM, LPARAM);
	afx_msg LRESULT OnMouseLeave(WPARAM,LPARAM);
	afx_msg LRESULT OnBCGSetControlBackStageMode (WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

	int HitTest(CPoint pt, BOOL* pbPin = NULL);

	int			m_nHighlightedItem;
	int			m_nClickedItem;
	BOOL		m_bTracked;
	HIMAGELIST	m_hImageList;		// External images
	CSize		m_sizeImage;		// Image size
	BOOL		m_bBackstageMode;
	BOOL		m_bPropertySheetNavigator;
	BOOL		m_bIsPinHighlighted;
	BOOL		m_bPins;
	int			m_nDescrRows;

#ifndef _BCGSUITE_
	CMap<int, int, BOOL, BOOL>	m_ItemPinStates;
#endif

	CList<int, int>							m_lstCaptionIndexes;
	CMap<int, int, int, int>				m_ItemIcons;
	CMap<int, int, CString, const CString&>	m_ItemDescriptions;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPLISTBOX_H__7E778AEF_FFBB_486D_A7B3_D25D7868F5D9__INCLUDED_)
