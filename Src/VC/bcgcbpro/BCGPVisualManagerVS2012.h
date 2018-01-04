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
// BCGPVisualManagerVS2012.h: interface for the CBCGPVisualManagerVS2012 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPVISUALMANAGERVS2012_H__A49EA5C5_FB20_4DDD_BC38_9AF16C691505__INCLUDED_)
#define AFX_BCGPVISUALMANAGERVS2012_H__A49EA5C5_FB20_4DDD_BC38_9AF16C691505__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGPVisualManagerVS2010.h"

class BCGCBPRODLLEXPORT CBCGPVisualManagerVS2012 : public CBCGPVisualManagerVS2010  
{
	DECLARE_DYNCREATE(CBCGPVisualManagerVS2012)

public:
	enum AccentColor
	{
		VS2012_Blue,
		VS2012_Brown,
		VS2012_Green,
		VS2012_Lime,
		VS2012_Magenta,
		VS2012_Orange,
		VS2012_Pink,
		VS2012_Purple,
		VS2012_Red,
		VS2012_Teal,
	};

	enum Style
	{
		VS2012_Light,
		VS2012_Dark,
	};

	static void SetStyle(Style style);
	static Style GetStyle() { return m_Style; }

	CBCGPVisualManagerVS2012();
	virtual ~CBCGPVisualManagerVS2012();

	static void SetAccentColor(AccentColor color);
	static void SetAccentColorRGB(COLORREF color);
    static COLORREF GetAccentColorRGB();

	static COLORREF AccentColorToRGB(AccentColor color);

	virtual void OnUpdateSystemColors ();
	virtual void ModifyGlobalColors ();

	virtual BOOL IsAutoGrayscaleImages();

	virtual BOOL IsEmbossDisabledBtnText ()
	{
		return FALSE;
	}

	virtual void OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL bNCArea = FALSE);

	virtual void OnHighlightRarelyUsedMenuItems (CDC* pDC, CRect rectRarelyUsed);
	virtual void OnHighlightMenuItem (CDC*pDC, CBCGPToolbarMenuButton* pButton,CRect rect, COLORREF& clrText);
	virtual void OnDrawShowAllMenuItems (CDC* pDC, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);
	virtual int GetShowAllMenuItemsHeight (CDC* pDC, const CSize& sizeDefault);
	virtual void OnDrawSeparator (CDC* pDC, CBCGPBaseControlBar* pBar, CRect rect, BOOL bIsHoriz);

	virtual int GetMenuDownArrowState (CBCGPToolbarMenuButton* pButton, BOOL bHightlight, BOOL bPressed, BOOL bDisabled);
	virtual COLORREF GetMenuItemTextColor (CBCGPToolbarMenuButton* pButton, BOOL bHighlighted, BOOL bDisabled);

	virtual void OnDrawMenuBorder (CDC* pDC, CBCGPPopupMenu* pMenu, CRect rect);

	virtual COLORREF OnDrawControlBarCaption (CDC* pDC, CBCGPDockingControlBar* pBar, 
		BOOL bActive, CRect rectCaption, CRect rectButtons);

	virtual void OnDrawCaptionButton (CDC* pDC, CBCGPCaptionButton* pButton, BOOL bActive, BOOL bHorz,
										BOOL bMaximized, BOOL bDisabled, int nImageID = -1);

	virtual void OnDrawCaptionButtonIcon (CDC* pDC, 
											CBCGPCaptionButton* pButton,
											CBCGPMenuImages::IMAGES_IDS id,
											BOOL bActive, BOOL bDisabled,
											CPoint ptImage);

	virtual void OnDrawControlBarCaptionText (CDC* pDC, CBCGPDockingControlBar* pBar, BOOL bActive, const CString& strTitle, CRect& rectCaption);
	virtual BOOL IsMinFrameCaptionBold(CBCGPMiniFrameWnd* /* pFrame */) { return FALSE; }

	virtual void OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz, CBCGPBaseControlBar* pBar);

	virtual CRect GetMenuImageFrameOffset () const
	{
		return CRect (4, 2, 0, 3);
	}

	virtual void OnDrawMenuCheck (CDC* pDC, CBCGPToolbarMenuButton* pButton, CRect rect, BOOL bHighlight, BOOL bIsRadio);

	virtual void OnFillMenuImageRect (CDC* pDC, CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);
	virtual void OnDrawMenuImageRectBorder (CDC* pDC, CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);

	virtual void OnDrawSlider (CDC* pDC, CBCGPSlider* pSlider, CRect rect, BOOL bAutoHideMode);

	virtual COLORREF GetToolbarDisabledTextColor ();

	virtual COLORREF OnFillCommandsListBackground (CDC* pDC, CRect rect, BOOL bIsSelected = FALSE);

	// Tabs support:
	virtual void OnDrawTab (CDC* pDC, CRect rectTab, int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd);
	virtual void OnDrawTabContent (CDC* pDC, CRect rectTab,
							int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd,
							COLORREF clrText);

	virtual void OnEraseTabsArea (CDC* pDC, CRect rect, 
								const CBCGPBaseTabWnd* pTabWnd);

	virtual void OnEraseTabsButton (CDC* pDC, CRect rect, CBCGPButton* pButton,
									CBCGPBaseTabWnd* pWndTab);

	virtual int GetTabButtonState (CBCGPTabWnd* pTab, CBCGTabButton* pButton);
	virtual BOOL OnEraseTabsFrame (CDC* pDC, CRect rect, const CBCGPBaseTabWnd* pTabWnd);
	virtual void OnDrawTabBorder(CDC* pDC, CBCGPTabWnd* pTabWnd, CRect rectBorder, COLORREF clrBorder, CPen& penLine);

	virtual BOOL IsMDITabsTopEdge ()			{	return FALSE;	}
	virtual BOOL AlwaysHighlight3DTabs () const	{	return TRUE;	}
	virtual BOOL IsDockingTabUpperCase()		{	return FALSE;	}
	virtual BOOL IsFocusedTabSeparateLook()		{	return TRUE;	}

	virtual void GetTabFrameColors (const CBCGPBaseTabWnd* pTabWnd,
				   COLORREF& clrDark,
				   COLORREF& clrBlack,
				   COLORREF& clrHighlight,
				   COLORREF& clrFace,
				   COLORREF& clrDarkShadow,
				   COLORREF& clrLight,
				   CBrush*& pbrFace,
				   CBrush*& pbrBlack);

	virtual void OnDrawTabResizeBar (CDC* pDC, CBCGPBaseTabWnd* pWndTab, 
									BOOL bIsVert, CRect rect,
									CBrush* pbrFace, CPen* pPen);

	virtual int GetTabExtraHeight(const CBCGPTabWnd* pTab);

	virtual void OnDrawTabCloseButton (CDC* pDC, CRect rect, 
											   const CBCGPBaseTabWnd* pTabWnd, 
											   BOOL bIsHighlighted,
											   BOOL bIsPressed,
											   BOOL bIsDisabled);
	// Auto-hide buttons:
	virtual void OnDrawAutoHideButtonBorder (CDC* pDC, CRect rectBounds, CRect rectBorderSize, CBCGPAutoHideButton* pButton);
	virtual COLORREF GetAutoHideButtonTextColor (CBCGPAutoHideButton* pButton);

	virtual BCGP_SMARTDOCK_THEME GetSmartDockingTheme ();

	virtual COLORREF GetStatusBarPaneTextColor (CBCGPStatusBar* pStatusBar, CBCGStatusBarPaneInfo* pPane);

	virtual void OnDrawComboDropButton (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted,
										CBCGPToolbarComboBoxButton* pButton);
	virtual void OnDrawComboBorder (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted,
										CBCGPToolbarComboBoxButton* pButton);
	virtual void OnDrawEditBorder (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsHighlighted,
										CBCGPToolbarEditBoxButton* pButton);
	
	virtual HBRUSH GetToolbarEditColors(CBCGPToolbarButton* pButton, COLORREF& clrText, COLORREF& clrBk);

	virtual void OnFillCombo		(CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted,
										CBCGPToolbarComboBoxButton* pButton);

	virtual COLORREF GetComboboxTextColor(CBCGPToolbarComboBoxButton* pButton, 
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted);

	virtual void OnFillButtonInterior (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);

	virtual void OnFillHighlightedArea (CDC* pDC, CRect rect, CBrush* pBrush, CBCGPToolbarButton* pButton);

	virtual void OnDrawButtonBorder (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);

	virtual int GetDockingTabsBordersSize ()	{	return 0;	}

	virtual BOOL OnEraseMDIClientArea (CDC* pDC, CRect rectClient);

	virtual BOOL AreCustomToolbarCtrlColors()	{	return TRUE;	}

	virtual CSize GetButtonExtraBorder () const	{	return CSize(0, 0);	}

	virtual int GetDockingBarCaptionExtraHeight () const	{	return 3;	}

	virtual COLORREF OnFillMiniFrameCaption (CDC* pDC, CRect rectCaption, 
											CBCGPMiniFrameWnd* pFrameWnd,
											BOOL bActive);
	virtual void OnDrawMiniFrameBorder (CDC* pDC, CBCGPMiniFrameWnd* pFrameWnd,
										CRect rectBorder, CRect rectBorderSize);
	virtual COLORREF GetToolbarButtonTextColor (CBCGPToolbarButton* pButton, CBCGPVisualManager::BCGBUTTON_STATE state);

	// Dialog:
	virtual CBrush& GetDlgBackBrush (CWnd* pDlg)
	{
		return m_brDlgBackground.GetSafeHandle () == NULL ? CBCGPVisualManagerVS2008::GetDlgBackBrush (pDlg) : m_brDlgBackground;
	}

	// Push button:
	virtual BOOL OnDrawPushButton (CDC* pDC, CRect rect, CBCGPButton* pButton, COLORREF& clrText);
	virtual BOOL IsDrawFocusRectOnPushButton(CBCGPButton* /*pButton*/) 	{ return FALSE; }

#ifndef BCGP_EXCLUDE_PROP_LIST
	virtual COLORREF OnFillPropList(CDC* pDC, CBCGPPropList* pList, const CRect& rectClient, COLORREF& clrFill);
	virtual void OnFillPropListToolbarArea(CDC* pDC, CBCGPPropList* pList, const CRect& rectToolBar);
	virtual COLORREF OnFillPropertyListSelectedItem(CDC* pDC, CBCGPProp* pProp, CBCGPPropList* pWndList, const CRect& rectFill, BOOL bFocused);
	virtual COLORREF GetPropListGroupTextColor(CBCGPPropList* pPropList);
	virtual COLORREF OnFillPropListDescriptionArea(CDC* pDC, CBCGPPropList* pList, const CRect& rect);
	virtual COLORREF OnFillPropListCommandsArea(CDC* pDC, CBCGPPropList* pList, const CRect& rect);
	virtual COLORREF GetPropListCommandTextColor (CBCGPPropList* pPropList);
	virtual COLORREF GetPropListDisabledTextColor(CBCGPPropList* pPropList);
#endif

	// Tasks pane:
#ifndef BCGP_EXCLUDE_TASK_PANE
	virtual void OnDrawTasksGroupCaption(CDC* pDC, CBCGPTasksGroup* pGroup, 
										BOOL bIsHighlighted = FALSE, BOOL bIsSelected = FALSE, 
										BOOL bCanCollapse = FALSE);
#endif

	// Toolbox:
#ifndef BCGP_EXCLUDE_TOOLBOX
	virtual BOOL OnEraseToolBoxButton (CDC* pDC, CRect rect, CBCGPToolBoxButton* pButton);
	virtual BOOL OnDrawToolBoxButtonBorder (CDC* pDC, CRect& rect, CBCGPToolBoxButton* pButton, UINT uiState);
	virtual COLORREF GetToolBoxButtonTextColor (CBCGPToolBoxButton* pButton);
#endif

	// Outlook bar:
	virtual void OnFillOutlookPageButton (	CDC* pDC, const CRect& rect,
										BOOL bIsHighlighted, BOOL bIsPressed,
										COLORREF& clrText);
	virtual void OnDrawOutlookPageButtonBorder (CDC* pDC, CRect& rectBtn, 
										BOOL bIsHighlighted, BOOL bIsPressed);

	virtual void OnFillOutlookBarCaption (CDC* pDC, CRect rectCaption, COLORREF& clrText);

	virtual COLORREF GetCaptionBarTextColor (CBCGPCaptionBar* pBar);

	virtual COLORREF GetHeaderCtrlTextColor (CBCGPHeaderCtrl* pCtrl, BOOL bIsPressed, BOOL bIsHighlighted);
	virtual void OnDrawHeaderCtrlBorder (CBCGPHeaderCtrl* pCtrl, CDC* pDC, CRect& rect, BOOL bIsPressed, BOOL bIsHighlighted);

	virtual COLORREF GetURLLinkColor (CBCGPURLLinkButton* pButton, BOOL bHover);

	// Popup window:
#ifndef BCGP_EXCLUDE_POPUP_WINDOW
	virtual void OnFillPopupWindowBackground (CDC* pDC, CRect rect);
	virtual void OnDrawPopupWindowBorder (CDC* pDC, CRect rect);
	virtual COLORREF OnDrawPopupWindowCaption (CDC* pDC, CRect rectCaption, CBCGPPopupWindow* pPopupWnd);
	virtual void OnErasePopupWindowButton (CDC* pDC, CRect rectClient, CBCGPPopupWndButton* pButton);
	virtual void OnDrawPopupWindowButtonBorder (CDC* pDC, CRect rectClient, CBCGPPopupWndButton* pButton);
#endif

	virtual COLORREF OnFillListBox(CDC* pDC, CBCGPListBox* pListBox, CRect rectClient);
	virtual COLORREF OnFillListBoxItem (CDC* pDC, CBCGPListBox* pListBox, int nItem, CRect rect, BOOL bIsHighlihted, BOOL bIsSelected);

	virtual COLORREF OnFillComboBoxItem(CDC* pDC, CBCGPComboBox* pComboBox, int nIndex, CRect rect, BOOL bIsHighlihted, BOOL bIsSelected);

	// List control:
	virtual HBRUSH GetListControlFillBrush(CBCGPListCtrl* pListCtrl);
	virtual COLORREF GetListControlTextColor(CBCGPListCtrl* pListCtrl);

	virtual void GetCalendarColors (const CBCGPCalendar* pCalendar, CBCGPCalendarColors& colors);
	virtual void OnDrawCaptionBarInfoArea (CDC* pDC, CBCGPCaptionBar* pBar, CRect rect);

public:
	static BOOL	m_bAutoGrayscaleImages;

	// Grid control:
#ifndef BCGP_EXCLUDE_GRID_CTRL
	virtual void OnFillGridBackground (CDC* pDC, CRect rect);
	virtual void OnFillGridHeaderBackground (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect);
	virtual BOOL OnDrawGridHeaderItemBorder (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect, BOOL bPressed);
	virtual void OnFillGridRowHeaderBackground (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect);
	virtual BOOL OnDrawGridRowHeaderItemBorder (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect, BOOL bPressed);
	virtual void OnFillGridSelectAllAreaBackground (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect, BOOL bPressed);
	virtual BOOL OnDrawGridSelectAllAreaBorder (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect, BOOL bPressed);
	virtual COLORREF OnFillGridGroupByBoxBackground (CDC* pDC, CRect rect);
	virtual COLORREF OnFillGridGroupByBoxTitleBackground (CDC* pDC, CRect rect);
	virtual COLORREF GetGridGroupByBoxLineColor () const;
	virtual void OnDrawGridGroupByBoxItemBorder (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect);
	virtual COLORREF GetGridLeftOffsetColor (CBCGPGridCtrl* pCtrl);
	virtual COLORREF GetGridItemSortedColor (CBCGPGridCtrl* pCtrl);
	virtual void OnFillGridGroupBackground (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rectFill);
	virtual void OnDrawGridGroupUnderline (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rectFill);
	virtual COLORREF OnFillGridRowBackground (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rectFill, BOOL bSelected);
	virtual COLORREF OnFillGridItem (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rectFill,
		BOOL bSelected, BOOL bActiveItem, BOOL bSortedColumn);
	virtual void OnDrawGridHeaderMenuButton (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect, 
		BOOL bHighlighted, BOOL bPressed, BOOL bDisabled);
	virtual void OnDrawGridSelectionBorder (CBCGPGridCtrl* pCtrl, CDC* pDC, CRect rect);

	virtual BOOL OnSetGridColorTheme (CBCGPGridCtrl* pCtrl, BCGP_GRID_COLOR_DATA& theme);

	// Report control:
	virtual COLORREF GetReportCtrlGroupBackgoundColor ();
	virtual COLORREF OnFillReportCtrlRowBackground (CBCGPGridCtrl* pCtrl, CDC* pDC,
		CRect rectFill, BOOL bSelected, BOOL bGroup);
#endif // BCGP_EXCLUDE_GRID_CTRL

	// Editor colors:
	virtual COLORREF GetEditBackSidebarColor(CBCGPEditCtrl* pEdit);
	virtual HBRUSH GetEditBackgroundBrush(CBCGPEditCtrl* pEdit);
	virtual COLORREF GetEditTextColor(CBCGPEditCtrl* pEdit);
	virtual COLORREF GetEditLineNumbersBarBackColor(CBCGPEditCtrl* pEdit);
	virtual COLORREF GetEditLineNumbersBarTextColor(CBCGPEditCtrl* pEdit);

	virtual COLORREF GetEditOutlineColor(CBCGPEditCtrl* pEdit);

	// Tree control:
	virtual COLORREF GetTreeControlFillColor(CBCGPTreeCtrl* pTreeCtrl, BOOL bIsSelected = FALSE, BOOL bIsFocused = FALSE, BOOL bIsDisabled = FALSE);
	virtual COLORREF GetTreeControlTextColor(CBCGPTreeCtrl* pTreeCtrl, BOOL bIsSelected = FALSE, BOOL bIsFocused = FALSE, BOOL bIsDisabled = FALSE);

	virtual CBrush& GetEditCtrlBackgroundBrush(CBCGPEdit* pEdit);
	virtual COLORREF GetEditCtrlTextColor(CBCGPEdit* pEdit);

	// Breadcrumb control:
	virtual COLORREF BreadcrumbFillBackground (CDC& dc, CBCGPBreadcrumb* pControl, CRect rectFill);

#ifndef BCGP_EXCLUDE_RIBBON
	virtual void OnDrawRibbonProgressBar (
			CDC* pDC, CBCGPRibbonProgressBar* pProgress, 
			CRect rectProgress, CRect rectChunk, BOOL bInfiniteMode);
#endif

	// Scrollbar
	virtual BOOL IsOwnerDrawScrollBar () const;

	virtual void OnScrollBarDrawThumb (CDC* pDC, CBCGPScrollBar* pScrollBar, CRect rect, 
		BOOL bHorz, BOOL bHighlighted, BOOL bPressed, BOOL bDisabled);
					
	virtual void OnScrollBarDrawButton (CDC* pDC, CBCGPScrollBar* pScrollBar, CRect rect, 
		BOOL bHorz, BOOL bHighlighted, BOOL bPressed, BOOL bFirst, BOOL bDisabled);

	virtual void OnScrollBarFillBackground (CDC* pDC, CBCGPScrollBar* pScrollBar, CRect rect, 
		BOOL bHorz, BOOL bHighlighted, BOOL bPressed, BOOL bFirst, BOOL bDisabled);

#ifndef BCGP_EXCLUDE_PLANNER
	virtual void OnFillPlanner (CDC* pDC, CBCGPPlannerView* pView, 
		CRect rect, BOOL bWorkingArea);

	virtual COLORREF OnFillPlannerCaption (CDC* pDC, CBCGPPlannerView* pView,
		CRect rect, BOOL bIsToday, BOOL bIsSelected, BOOL bNoBorder = FALSE,
		BOOL bHorz = TRUE);

	virtual void OnDrawPlannerCaptionText (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect, const CString& strText, 
		COLORREF clrText, int nAlign, BOOL bHighlight);

	virtual COLORREF GetPlannerAppointmentTimeColor (CBCGPPlannerView* pView,
		BOOL bSelected, BOOL bSimple, DWORD dwDrawFlags);

	virtual COLORREF GetPlannerHourLineColor (CBCGPPlannerView* pView,
		BOOL bWorkingHours, BOOL bHour);

	virtual COLORREF GetPlannerViewWorkingColor (CBCGPPlannerView* pView);
	virtual COLORREF GetPlannerViewNonWorkingColor (CBCGPPlannerView* pView);

	virtual COLORREF OnFillPlannerTimeBar (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect, COLORREF& clrLine);

	virtual void OnDrawPlannerTimeLine (CDC* pDC, CBCGPPlannerView* pView, CRect rect);

	virtual void OnFillPlannerWeekBar (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect);

	virtual void OnDrawPlannerHeader (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect);

	virtual void OnDrawPlannerHeaderPane (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect);

	virtual void OnFillPlannerHeaderAllDay (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect);
	virtual void OnDrawPlannerHeaderAllDayItem (CDC* pDC, 
		CBCGPPlannerView* pView, CRect rect, BOOL bIsToday, BOOL bIsSelected);

	virtual DWORD GetPlannerDrawFlags () const;
	
	virtual int GetPlannerRowExtraHeight () const;

	virtual COLORREF GetPlannerSelectionColor (CBCGPPlannerView* pView);
	virtual COLORREF GetPlannerSeparatorColor (CBCGPPlannerView* pView);

	virtual void PreparePlannerBackItem (BOOL bIsToday, BOOL bIsSelected);

#endif // BCGP_EXCLUDE_PLANNER

protected:
	static COLORREF	m_clrAccent;
	static Style	m_Style;
	CBrush			m_brAccent;
	COLORREF		m_clrAccentLight;
	CBrush			m_brAccentLight;
	COLORREF		m_clrAccentText;
	COLORREF		m_clrTextDisabled;
	COLORREF		m_clrGripperBk;
};

// Backward compatibility definitions:

class BCGCBPRODLLEXPORT CBCGPVisualManagerVS11 : public CBCGPVisualManagerVS2012
{
	DECLARE_DYNCREATE(CBCGPVisualManagerVS11)
};

#define VS11_Blue		VS2012_Blue
#define VS11_Brown		VS2012_Brown
#define VS11_Green		VS2012_Green
#define VS11_Lime		VS2012_Lime
#define VS11_Magenta	VS2012_Magenta
#define VS11_Orange		VS2012_Orange
#define VS11_Pink		VS2012_Pink
#define VS11_Purple		VS2012_Purple
#define VS11_Red		VS2012_Red
#define VS11_Teal		VS2012_Teal
#define VS11_Light		VS2012_Light
#define VS11_Dark		VS2012_Dark

#endif // !defined(AFX_BCGPVISUALMANAGERVS11_H__A49EA5C5_FB20_4DDD_BC38_9AF16C691505__INCLUDED_)
