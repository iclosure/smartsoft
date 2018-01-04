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
// BCGPMetroUITiles.h: interface for the CBCGPMetroUITiles class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPMETROUITILES_H__1274D27C_573D_4BC5_986D_A63658C8B32E__INCLUDED_)
#define AFX_BCGPMETROUITILES_H__1274D27C_573D_4BC5_986D_A63658C8B32E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXMT_H__
	#include <afxmt.h>
#endif

#include "BCGPVisualContainer.h"
#include "BCGPVisualCtrl.h"

extern BCGCBPRODLLEXPORT UINT BCGM_ON_CLICK_METRO_UI_TILE;

/////////////////////////////////////////////////////////////////////////////
// CBCGPMetroUITile

class BCGCBPRODLLEXPORT CBCGPMetroUITile : public CObject
{
	friend class CBCGPMetroUITiles;

public:
	enum BCGP_METRO_BADGE_GLYPH
	{
		BCGP_NONE = -1,
		BCGP_ACTIVITY,
		BCGP_ALERT,
		BCGP_AVAILABLE,
		BCGP_AWAY,
		BCGP_BUSY,
		BCGP_NEW_MESSAGE,
		BCGP_PAUSED,
		BCGP_PLAYING,
		BCGP_UNAVAILABLE,
		BCGP_ERROR
	};
	
	enum BCGP_METRO_IMAGE_EFFECT
	{
		BCGP_ANIMATION_NONE = 0,
		BCGP_ANIMATION_FADE,
	};

// Construction
public:
	CBCGPMetroUITile(const CString& strName,
		BOOL bIsWide = FALSE,
		const CBCGPColor& colorText = CBCGPColor(),
		const CBCGPBrush& brushBackground = CBCGPBrush(),
		const CBCGPColor& colorBorder = CBCGPColor());
	CBCGPMetroUITile();
	
protected:
	void CommonInit();
	
// Attributes
public:
	DWORD_PTR GetUserData() const { return m_dwUserData; }
	void SetUserData(DWORD_PTR dwUserData);

	int GetGroupID() const { return m_nGroup; }
	void SetGroupID(int nID, BOOL bRecalcLayout = TRUE);

	const CString& GetName() const { return m_strName; }
	void SetName(const CString& strName);

	int GetBadgeNumber() const { return m_nBadgeNumber; }
	void SetBadgeNumber(int nBadgeNumber);

	BCGP_METRO_BADGE_GLYPH GetBadgeGlyph() const { return m_BadgeGlyph; }
	void SetBadgeGlyph(BCGP_METRO_BADGE_GLYPH glyph);

	void ClearBadge();
	
	const CString& GetHeader() const { return m_strHeader; }
	void SetHeader(const CString& strHeader);

	const CString& GetText() const { return m_strText; }
	void SetText(const CString& strText);
	
	const CBCGPImage& GetImage() const { return m_Image; }
	void SetImage(const CBCGPImage& image, BCGP_METRO_IMAGE_EFFECT effect = BCGP_ANIMATION_NONE, int nAnimationTime = 1000 /* 1 sec */);
	
	const CBCGPColor& GetTextColor() const { return m_colorText; }
	void SetTextColor(const CBCGPColor& color) { m_colorText = color; }
	
	const CBCGPBrush& GetBackgroundBrush() const { return m_brushBackground; }
	void SetBackgroundBrush(const CBCGPBrush& brush) { m_brushBackground = brush; m_brushBackgroundDark.Empty(); }
	
	const CBCGPColor& GetBorderColor() const { return m_colorBorder; }
	void SetBorderColor(const CBCGPColor& color) { m_colorBorder = color; }
	
	double GetBorderWidth() const { return m_dblBorderWidth; }
	void SetBorderWidth(double dblWidth) { m_dblBorderWidth = dblWidth; }

	const CBCGPRect& GetRect() const { return m_rect; }
	
	const CString& GetToolTipText() const { return m_strToolTipText; }
	const CString& GetToolTipDescription() const { return m_strToolTipDescription; }
	
	void SetToolTipText(const CString& strToolTipText);
	void SetToolTipDescription(const CString& strToolTipDescription);

// Operation:
public:
	void Redraw();
	
// Overrides
public:
	virtual void DoDraw(CBCGPMetroUITiles* pOwner, CBCGPGraphicsManager* pGM, BOOL bIsPressed, BOOL bIsHighlighted);
	
	virtual void OnDrawName(CBCGPGraphicsManager* pGM, const CBCGPRect& rectText, const CBCGPTextFormat& tf, const CBCGPColor& clrText);
	virtual CBCGPSize GetNameSize(CBCGPGraphicsManager* pGM, const CBCGPTextFormat& tf, double dblWidth);
	
	virtual void OnDrawBadge(CBCGPGraphicsManager* pGM, const CBCGPRect& rectBadge, const CBCGPTextFormat& tf, const CBCGPColor& clrText);
	virtual CBCGPSize GetBadgeSize(CBCGPGraphicsManager* pGM, const CBCGPTextFormat& tf);

	virtual void OnDrawHeader(CBCGPGraphicsManager* pGM, const CBCGPRect& rectHeader, const CBCGPTextFormat& tf, const CBCGPColor& clrText);
	virtual CBCGPSize GetHeaderSize(CBCGPGraphicsManager* pGM, const CBCGPTextFormat& tf, double dblWidth);

	virtual void OnDrawText(CBCGPGraphicsManager* pGM, const CBCGPRect& rectText, const CBCGPTextFormat& tf, const CBCGPColor& clrText);
	virtual CBCGPSize GetTextSize(CBCGPGraphicsManager* pGM, const CBCGPTextFormat& tf, double dblWidth);
	
	virtual void OnDrawImage(CBCGPGraphicsManager* pGM, const CBCGPRect& rectImage);
	virtual CBCGPSize GetImageSize(CBCGPGraphicsManager* pGM);

protected:
	static VOID CALLBACK AnimTimerProc (HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	virtual BOOL OnAnimation();
	void StopAnimation();

protected:
	CBCGPMetroUITiles*		m_pOwner;
	DWORD_PTR				m_dwUserData;
	CString					m_strName;
	CBCGPSize				m_sizePadding;
	int						m_nBadgeNumber;
	BCGP_METRO_BADGE_GLYPH	m_BadgeGlyph;
	CString					m_strHeader;
	CString					m_strText;
	CBCGPImage				m_Image;
	double					m_dblImageOpacity;
	double					m_dblImageOpacityDelta;
	CString					m_strToolTipText;
	CString					m_strToolTipDescription;
	int						m_nGroup;
	int						m_nImportance;
	BOOL					m_bIsWide;
	CBCGPColor				m_colorText;
	CBCGPBrush				m_brushBackground;
	CBCGPBrush				m_brushBackgroundDark;
	CBCGPColor				m_colorBorder;
	double					m_dblBorderWidth;
	CBCGPRect				m_rect;
	UINT					m_nAnimationID;

	static CMap<UINT,UINT,CBCGPMetroUITile*,CBCGPMetroUITile*> m_mapAnimations;
	static CCriticalSection g_cs;			// For multi-theard applications
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPMetroUITiles

class BCGCBPRODLLEXPORT CBCGPMetroUITiles : public CBCGPBaseVisualObject  
{
	friend class CBCGPMetroUITile;
	
	DECLARE_DYNAMIC(CBCGPMetroUITiles)

// Construction
public:
	CBCGPMetroUITiles();
	virtual ~CBCGPMetroUITiles();

// Attributes
public:
	BOOL IsRoundedShapes() const { return m_bRoundedShapes; }
	void SetRoundedShapes(BOOL bSet = TRUE);

	BOOL IsHorizontalLayout() const { return m_bIsHorizontalLayout; }
	void SetHorizontalLayout(BOOL bIsHorizontalLayout);

	void SetHorzMargin(double nMargin);
	void SetVertMargin(double nMargin);

	double GetHorzMargin() const { return m_nHorzMargin; }
	double GetVertMargin() const { return m_nVertMargin; }

	const CBCGPSize& GetSquareSize() const { return m_sizeSquare; }
	void SetSquareSize(const CBCGPSize& size);

	const CBCGPBrush& GetFillBrush() const { return m_brFill; }
	void SetFillBrush(const CBCGPBrush& brFill);

	const CBCGPColor& GetTextColor() const { return m_colorTileText; }

	const CBCGPBrush& GetTileBrush() const { return m_brTileFill; }
	const CBCGPBrush& GetTileBrushDark() const { return m_brTileFillDark; }

// Operations
public:
	void Add(CBCGPMetroUITile* pTile, int nGroup = 0);
	void RemoveAll();

	const CList<CBCGPMetroUITile*, CBCGPMetroUITile*>& GetTilesList() const { return m_lstTiles; }

	CBCGPMetroUITile* HitTestTile(const CBCGPPoint& point) const;

// Overrides
public:
	virtual void OnDraw(CBCGPGraphicsManager* pGM, const CBCGPRect& rectClip, DWORD dwFlags = BCGP_DRAW_STATIC | BCGP_DRAW_DYNAMIC);
	virtual void SetRect(const CBCGPRect& rect, BOOL bRedraw = FALSE)
	{
		SetDirty();
		CBCGPBaseVisualObject::SetRect(rect, bRedraw);
	}

	virtual void OnClickAndHoldEvent(UINT nID, const CBCGPPoint& point);
	virtual void OnScroll(CBCGPVisualScrollBar* pScrollBar, double dblDelta);

protected:
	virtual void OnClickTile(CBCGPMetroUITile* pClickedTile);

	virtual BOOL OnMouseDown(int nButton, const CBCGPPoint& pt);
	virtual void OnMouseUp(int nButton, const CBCGPPoint& pt);
	virtual void OnMouseMove(const CBCGPPoint& pt);
	virtual void OnMouseLeave();
	virtual BOOL OnMouseWheel(const CBCGPPoint& pt, short zDelta);
	virtual BOOL OnSetMouseCursor(const CBCGPPoint& pt);
	virtual void OnCancelMode();
	virtual BOOL OnGetToolTip(const CBCGPPoint& pt, CString& strToolTip, CString& strDescr);

	virtual void OnScaleRatioChanged(const CBCGPSize& sizeScaleRatioOld);

	void RecalcLayout(CBCGPGraphicsManager* pGM);
	void RedrawTile(CBCGPMetroUITile* pTile);
	void AddSorted(CBCGPMetroUITile* pTile);

	void ReposScrollBar();

// Attributes
protected:
	BOOL											m_bIsHorizontalLayout;
	BOOL											m_bRoundedShapes;

	CList<CBCGPMetroUITile*, CBCGPMetroUITile*>		m_lstTiles;
	CList<CBCGPMetroUITile*, CBCGPMetroUITile*>		m_lstTilesSorted;
	CBCGPMetroUITile*								m_pHighlighted;
	CBCGPMetroUITile*								m_pPressed;

	double											m_nHorzMargin;
	double											m_nHorzMarginOriginal;

	double											m_nVertMargin;
	double											m_nVertMarginOriginal;

	double											m_dblCornerRadius;
	double											m_dblCornerRadiusOriginal;

	CBCGPSize										m_sizeSquare;
	CBCGPSize										m_sizeSquareOriginal;

	CBCGPBrush										m_brFill;
	CBCGPBrush										m_brTileFill;
	CBCGPBrush										m_brTileFillDark;
	CBCGPColor										m_colorTileText;

	CBCGPTextFormat									m_textFormatHeader;
	CBCGPTextFormat									m_textFormatName;
	CBCGPTextFormat									m_textFormatBadge;
	CBCGPTextFormat									m_textFormatText;

	double											m_dblTotalSize;

	double											m_nScrollBarHeight;
	CBCGPVisualScrollBar							m_ScrollBar;

	CBCGPImage										m_BadgeGlyphs;
};

/////////////////////////////////////////////////////////////////////////////
// CBCGPMetroUITilesCtrl window

class BCGCBPRODLLEXPORT CBCGPMetroUITilesCtrl : public CBCGPVisualCtrl
{
	// Construction
public:
	CBCGPMetroUITilesCtrl();
	
	// Attributes
public:
	virtual CBCGPMetroUITiles* GetMetroUITiles() 
	{ 
		if (m_pMetroUI == NULL)
		{
			m_pMetroUI = new CBCGPMetroUITiles();
		}
		
		return m_pMetroUI;
	}
	
	// Operations
public:
	
	// Overrides
	virtual CBCGPBaseVisualObject* GetVisualObject()
	{
		return GetMetroUITiles();
	}
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPMetroUITilesCtrl)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CBCGPMetroUITilesCtrl();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPMetroUITilesCtrl)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
	CBCGPMetroUITiles* m_pMetroUI;
};

#endif // !defined(AFX_BCGPMETROUITILES_H__1274D27C_573D_4BC5_986D_A63658C8B32E__INCLUDED_)
