// SkinDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinCtrl.h"
#include "SkinDialog.h"

#define WM_USER0		(WM_USER  + 001)	// 作为一个对话框内部消息用	// (WM_USER + 001) ~ (WM_USER + 200)
#define WM_USER1		(WM_USER0 + 201)	// 作为公共消息用				// (WM_USER + 201) ~ (WM_USER + 400)
#define WM_USER2		(WM_USER1 + 201)	// 作为指令应答消息用			// (WM_USER + 401) ~ (WM_USER + 600)
// 自定义键盘空格键消息
#define WM_KEY_SPACE			(WM_USER1 + 0)	// SendMessage()方式

#define WM_ANS_QUIT		(WM_USER2 + 2)		// 

// CSkinDialog

IMPLEMENT_DYNAMIC(CSkinDialog, CBCGPDialog)

CSkinDialog::CSkinDialog()
: m_bShowTitle( FALSE )
, m_nMillisecond( 50 )
, m_InternalTimerID( NULL )
, m_bAutoStatus( FALSE )
, m_rateY( 100 )
{
	EnableVisualManagerStyle(TRUE, TRUE);
	EnableLayout();
}

CSkinDialog::CSkinDialog(UINT nIDTemplate, CWnd* pParentWnd)
: CBCGPDialog(nIDTemplate, pParentWnd)
, m_bShowTitle( FALSE )
, m_nMillisecond( 50 )
, m_InternalTimerID( NULL )
, m_bAutoStatus( FALSE )
, m_rateY( 100 )
{
	EnableVisualManagerStyle(TRUE, TRUE);
	EnableLayout();
}

CSkinDialog::CSkinDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
: CBCGPDialog(lpszTemplateName, pParentWnd)
, m_bShowTitle( FALSE )
, m_nMillisecond( 50 )
, m_InternalTimerID( NULL )
, m_bAutoStatus( FALSE )
, m_rateY( 100 )
{
	EnableVisualManagerStyle(TRUE, TRUE);
	EnableLayout();
}

CSkinDialog::~CSkinDialog()
{
}

void CSkinDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

BOOL CSkinDialog::OnInitDialog()
{
	__super::OnInitDialog();

	// 设置背景颜色
	//SetBackgroundColor(RGB(0, 156, 204));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CSkinDialog::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:		// ESC
			AfxGetMainWnd()->PostMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
			return TRUE;	// 
		case VK_RETURN:		// ENTER
			return TRUE;	// 屏蔽 ENTER键
		case VK_SPACE:
			GetParent()->PostMessage(WM_KEY_SPACE);
			return TRUE;
		default:
			AfxGetMainWnd()->PostMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
			break;
		}
		break;
	case WM_SYSKEYDOWN:
		switch(pMsg->wParam)
		{
		case VK_MENU:
			AfxGetMainWnd()->PostMessage(WM_SYSKEYDOWN, pMsg->wParam, pMsg->lParam);
			return TRUE;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return __super::PreTranslateMessage(pMsg);
}

// 
BOOL CSkinDialog::FinalInitialze()
{
	return TRUE;
}

BOOL CSkinDialog::DestroyWindow()
{
	if (::IsWindow(m_hWnd))
	{
		if (m_InternalTimerID != NULL)
		{
			KillTimer(m_InternalTimerID);
			m_InternalTimerID = 0;
		}
	}

	return __super::DestroyWindow();
}

BEGIN_MESSAGE_MAP(CSkinDialog, CBCGPDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_MESSAGE(WM_ANS_QUIT, &CSkinDialog::OnAnsQuit)
	ON_WM_SHOWWINDOW()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CSkinDialog 消息处理程序

void CSkinDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bShowTitle && point.y < SKIN_DLG_TITLE_HEIGHT)
	{
		SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	}
	else
	{
		__super::OnLButtonDown(nFlags, point);
	}
}

void CSkinDialog::OnTimer(UINT_PTR nIDEvent)
{
	if (m_InternalTimerID == nIDEvent)
	{
		TimerProc();
	}
	else
	{
		__super::OnTimer(nIDEvent);
	}
}

// 
// 用户函数
// 

// 使能/禁止定时器
void CSkinDialog::SetTimerActive(BOOL bActive/* = TRUE*/, UINT nMillisecond/*  = 0*/)
{
	if (nMillisecond > 0)
	{
		m_nMillisecond = nMillisecond;
	}

	ASSERT(m_nMillisecond > 0);		// 判断是否已初始化定时器周期

	if (bActive)
	{
		if (NULL == m_InternalTimerID)
		{
			m_InternalTimerID = SetTimer(1, m_nMillisecond, NULL);
		}
	}
	else
	{
		if (m_InternalTimerID != NULL)
		{
			KillTimer(m_InternalTimerID);
			m_InternalTimerID = 0;
		}
	}
}

// WM_ANS_QUIT
// wParam:	-1 / index
// lParam: sender;
LRESULT CSkinDialog::OnAnsQuit(WPARAM wParam, LPARAM lParam)
{
	ASSERT(lParam != NULL);		// 确保lParam有效（为WM_ANS_PARAM类型数据）
	return GetParent()->SendMessage(WM_ANS_QUIT);
}
