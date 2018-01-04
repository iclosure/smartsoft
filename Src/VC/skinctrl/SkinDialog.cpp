// SkinDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SkinCtrl.h"
#include "SkinDialog.h"

#define WM_USER0		(WM_USER  + 001)	// ��Ϊһ���Ի����ڲ���Ϣ��	// (WM_USER + 001) ~ (WM_USER + 200)
#define WM_USER1		(WM_USER0 + 201)	// ��Ϊ������Ϣ��				// (WM_USER + 201) ~ (WM_USER + 400)
#define WM_USER2		(WM_USER1 + 201)	// ��Ϊָ��Ӧ����Ϣ��			// (WM_USER + 401) ~ (WM_USER + 600)
// �Զ�����̿ո����Ϣ
#define WM_KEY_SPACE			(WM_USER1 + 0)	// SendMessage()��ʽ

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

	// ���ñ�����ɫ
	//SetBackgroundColor(RGB(0, 156, 204));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
			return TRUE;	// ���� ENTER��
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


// CSkinDialog ��Ϣ�������

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
// �û�����
// 

// ʹ��/��ֹ��ʱ��
void CSkinDialog::SetTimerActive(BOOL bActive/* = TRUE*/, UINT nMillisecond/*  = 0*/)
{
	if (nMillisecond > 0)
	{
		m_nMillisecond = nMillisecond;
	}

	ASSERT(m_nMillisecond > 0);		// �ж��Ƿ��ѳ�ʼ����ʱ������

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
	ASSERT(lParam != NULL);		// ȷ��lParam��Ч��ΪWM_ANS_PARAM�������ݣ�
	return GetParent()->SendMessage(WM_ANS_QUIT);
}
