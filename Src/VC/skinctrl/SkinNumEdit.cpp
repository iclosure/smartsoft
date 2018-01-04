// SkinNumEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SkinCtrl.h"
#include "SkinNumEdit.h"


// CSkinNumEdit

IMPLEMENT_DYNAMIC(CSkinNumEdit, CEdit)

CSkinNumEdit::CSkinNumEdit()
{

}

BOOL CSkinNumEdit::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		// �༭��ݼ�����
		if (::GetFocus() == this->GetSafeHwnd()
			&& (::GetKeyState(VK_CONTROL) & 0xFF00) == 0xFF00)
		{
			// ��ֹճ��
			if (pMsg->wParam == 'V' || pMsg->wParam == 'v')
			{
				// ������ݸ�ʽ
				if (OpenClipboard())	// �򿪼��а�
				{
					HANDLE hData = GetClipboardData( CF_TEXT );	// ���ı���ʽ��ȡ���а�����
					PCHAR buffer = (PCHAR)GlobalLock( hData );	// �������а�ȫ�ֻ���
					CString clipText( buffer );					// ת��ΪUnicode����
					GlobalUnlock( hData );						// �������а�ȫ�ֻ���
					CloseClipboard();
					if (! clipText.IsEmpty())	// �ǿ�
					{
						int nCurStart, nCurEnd;
						__super::GetSel(nCurStart, nCurEnd);
						CString text;
						__super::GetWindowText( text);
						text.Insert(nCurStart, clipText);

						int nPos = text.Find('-') ;
						text.MakeLower();
						for (int i=0; i<text.GetLength(); i++)
						{
							TCHAR nChar = text[i];
							if (!((nChar >= _T('0') && nChar <= _T('9')) || (nChar == _T('.')) || (nChar == _T('-'))))
							{
								return TRUE;		// ������������
							}
						}

						if (nPos != CB_ERR && nPos != 0)	
						{
							return TRUE;		// ������Ų�����λ�������������
						}

						if (text.Left(1) == '.')
						{
							return TRUE;		// ���С��������λ�������������
						}

						// ������������������С�����������������
						nPos = text.Find('.');
						int nRevPos = text.ReverseFind('.');
						if (nPos != CB_ERR && nRevPos != CB_ERR && nPos != nRevPos)
						{
							return TRUE;
						}
					}
				}
			}
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

CSkinNumEdit::~CSkinNumEdit()
{
}

BEGIN_MESSAGE_MAP(CSkinNumEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CSkinNumericEdit ��Ϣ�������

void CSkinNumEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(isprint(nChar))
	{
		if (__super::GetStyle() & ES_READONLY)	// �ؼ�Ϊֻ��
		{
			return;
		}

		// �����ּ���'.'��'-'������
		if (!( isdigit(nChar) || (nChar == '.') || (nChar == '-')))
		{
			return;		// ������������
		}

		// ������ݸ�ʽ
		CString strBefore, strAfter;
		__super::GetWindowText(strBefore);
		strAfter = strBefore;
		if (! strBefore.IsEmpty())	// �ǿ�
		{
			int nCurStart, nCurEnd;
			__super::GetSel(nCurStart, nCurEnd);
			strAfter.Delete(nCurStart, nCurEnd - nCurStart);
			strAfter.Insert(nCurStart, nChar);

			int nPos = strAfter.Find('-') ;
			if (nPos != CB_ERR && nPos != 0)	
			{
				return ;		// ������Ų�����λ�������������
			}

			if (strAfter.Left(1) == '.')
			{
				return ;		// ���С�������0����λ�������������
			}

			// ��������������С�����������������
			if (nChar == '.' && strBefore.Find('.') != CB_ERR)
			{
				return ;
			}
		}
	}
	__super::OnChar(nChar, nRepCnt, nFlags);
}
