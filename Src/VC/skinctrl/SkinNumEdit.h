#pragma once


// CSkinNumEdit

class DLLSKINCTRL_API CSkinNumEdit : public CEdit
{
	DECLARE_DYNAMIC(CSkinNumEdit)

public:
	CSkinNumEdit();
	virtual ~CSkinNumEdit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


