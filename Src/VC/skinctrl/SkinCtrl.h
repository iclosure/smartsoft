// SkinCtrl.h : SkinCtrl DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "Resource.h"		// ������

#ifdef DLLSKINCTRL_EXPORTS
#define DLLSKINCTRL_API __declspec(dllexport)
#ifdef _MSC_VER
#pragma warning( push )
#endif
#pragma warning (disable : 4275)	// warning C4275: �� dll �ӿ� class��CWinApp������ dll �ӿ� class��CSkinHookApp���Ļ�
#else
#define DLLSKINCTRL_API __declspec(dllimport)

#define _SKINCTRL_LIBNAME_1_	"skinctrl"

#ifdef _UNICODE
#define _SKINCTRL_LIBNAME_2_	_SKINCTRL_LIBNAME_1_##"u"
#else
#define _SKINCTRL_LIBNAME_2_	_SKINCTRL_LIBNAME_1_
#endif

#ifdef _DEBUG
#define _SKINCTRL_LIBNAME_3_	_SKINCTRL_LIBNAME_2_##"d"
#else
#define _SKINCTRL_LIBNAME_3_	_SKINCTRL_LIBNAME_2_
#endif

#define _SKINCTRL_LIBNAME_		_SKINCTRL_LIBNAME_3_##".lib"

#pragma comment(lib, _SKINCTRL_LIBNAME_)

#ifndef _SKINCTRL_HIDE_AUTOLINK_OUTPUT_
#pragma comment (lib, _SKINCTRL_LIBNAME_)
#pragma message("Automatically linking with " _SKINCTRL_LIBNAME_)
#endif	// _SKINCTRL_HIDE_AUTOLINK_OUTPUT_
#endif	// DLLSKINCTRL_EXPORTS

// CSkinCtrlApp
// �йش���ʵ�ֵ���Ϣ������� SkinCtrl.cpp
//

class CSkinCtrlApp : public CWinApp
{
public:
	CSkinCtrlApp();

// ��д
public:
	virtual BOOL InitInstance(); 
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

// �ؼ�ͷ�ļ�����

#include "BCGCBProInc.h"
#include "SkinDialog.h"
#include "SkinNumEdit.h"

#ifdef DLLSKINCTRL_EXPORTS
#ifdef _MSC_VER
#pragma warning( pop )
#endif
#endif	// DLLSKINCTRL_EXPORTS