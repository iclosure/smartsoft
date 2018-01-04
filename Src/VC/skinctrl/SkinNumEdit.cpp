// SkinNumEdit.cpp : 实现文件
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
		// 编辑快捷键操作
		if (::GetFocus() == this->GetSafeHwnd()
			&& (::GetKeyState(VK_CONTROL) & 0xFF00) == 0xFF00)
		{
			// 禁止粘贴
			if (pMsg->wParam == 'V' || pMsg->wParam == 'v')
			{
				// 检测内容格式
				if (OpenClipboard())	// 打开剪切板
				{
					HANDLE hData = GetClipboardData( CF_TEXT );	// 以文本方式获取剪切板内容
					PCHAR buffer = (PCHAR)GlobalLock( hData );	// 锁定剪切板全局缓存
					CString clipText( buffer );					// 转换为Unicode编码
					GlobalUnlock( hData );						// 解锁剪切板全局缓存
					CloseClipboard();
					if (! clipText.IsEmpty())	// 非空
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
								return TRUE;		// 放弃本次输入
							}
						}

						if (nPos != CB_ERR && nPos != 0)	
						{
							return TRUE;		// 如果符号不在首位则放弃本次输入
						}

						if (text.Left(1) == '.')
						{
							return TRUE;		// 如果小数点在首位则放弃本次输入
						}

						// 如果输入后有两个以上小数点则放弃本次输入
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



// CSkinNumericEdit 消息处理程序

void CSkinNumEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(isprint(nChar))
	{
		if (__super::GetStyle() & ES_READONLY)	// 控件为只读
		{
			return;
		}

		// 非数字键、'.'、'-'不处理
		if (!( isdigit(nChar) || (nChar == '.') || (nChar == '-')))
		{
			return;		// 放弃本次输入
		}

		// 检测内容格式
		CString strBefore, strAfter;
		__super::GetWindowText(strBefore);
		strAfter = strBefore;
		if (! strBefore.IsEmpty())	// 非空
		{
			int nCurStart, nCurEnd;
			__super::GetSel(nCurStart, nCurEnd);
			strAfter.Delete(nCurStart, nCurEnd - nCurStart);
			strAfter.Insert(nCurStart, nChar);

			int nPos = strAfter.Find('-') ;
			if (nPos != CB_ERR && nPos != 0)	
			{
				return ;		// 如果符号不在首位则放弃本次输入
			}

			if (strAfter.Left(1) == '.')
			{
				return ;		// 如果小数点或者0在首位则放弃本次输入
			}

			// 如果输入后有两个小数点则放弃本次输入
			if (nChar == '.' && strBefore.Find('.') != CB_ERR)
			{
				return ;
			}
		}
	}
	__super::OnChar(nChar, nRepCnt, nFlags);
}
