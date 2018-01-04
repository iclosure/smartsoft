// SkinCtrl.h : SkinCtrl DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "Resource.h"		// 主符号

#ifdef DLLSKINCTRL_EXPORTS
#define DLLSKINCTRL_API __declspec(dllexport)
#ifdef _MSC_VER
#pragma warning( push )
#endif
#pragma warning (disable : 4275)	// warning C4275: 非 dll 接口 class“CWinApp”用作 dll 接口 class“CSkinHookApp”的基
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
// 有关此类实现的信息，请参阅 SkinCtrl.cpp
//

class CSkinCtrlApp : public CWinApp
{
public:
	CSkinCtrlApp();

// 重写
public:
	virtual BOOL InitInstance(); 
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

// 控件头文件包含

#include "BCGCBProInc.h"
#include "SkinDialog.h"
#include "SkinNumEdit.h"

#ifdef DLLSKINCTRL_EXPORTS
#ifdef _MSC_VER
#pragma warning( pop )
#endif
#endif	// DLLSKINCTRL_EXPORTS