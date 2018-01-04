#pragma once

// ********************************************
// 名称：
// 功能：软件加密处理库
// 描述：
// 作者：Hi ROBOT
// 日期：2012-12-05 10:04
// ********************************************

#ifdef DLLRMTLIB_EXPORTS
#define DLLRMTLIB_API	__declspec(dllexport)
#else
#define DLLRMTLIB_API	__declspec(dllimport)

#define _RMTLIB_LIBNAME_1_	"rmtlib"

#ifdef _UNICODE
#define _RMTLIB_LIBNAME_2_	_RMTLIB_LIBNAME_1_##"u"
#else
#define _RMTLIB_LIBNAME_2_	_RMTLIB_LIBNAME_1_
#endif

#ifdef _DEBUG
#define _RMTLIB_LIBNAME_3_	_RMTLIB_LIBNAME_2_##"d"
#else
#define _RMTLIB_LIBNAME_3_	_RMTLIB_LIBNAME_2_
#endif

#define _RMTLIB_LIBNAME_		_RMTLIB_LIBNAME_3_##".lib"

#pragma comment(lib, _RMTLIB_LIBNAME_)

#ifndef _RMTLIB_HIDE_AUTOLINK_OUTPUT_
#pragma comment (lib, _RMTLIB_LIBNAME_)
#pragma message("Automatically linking with " _RMTLIB_LIBNAME_)
#endif	// _RMTLIB_HIDE_AUTOLINK_OUTPUT_
#endif	// DLLRMTLIB_EXPORTS

class DLLRMTLIB_API CRmtLib
{
public:
	CRmtLib(void);
	~CRmtLib(void);

public:
	BOOL License(const BYTE* strSN, UINT length);
};
