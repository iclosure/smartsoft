/************************************************
* transfer in character ansi, utf8, unicode
* @author rene/2011-5-13
*
* @notice if you use it into something mfc application, you will make sure to include it under these mcf's headers.
* or else, the application can't be compiled with a error message as below:
*    "fatal error C1189: #error :  WINDOWS.H already included.  MFC apps must not #include <windows.h>"
* for example: at least
*    #include <afxwin.h>;
*    #include <afxcmn.h>;
* and so on, and then
*    #include "stringconvert.h"
*
* if you use precompiled file, normally
*    #include "stdafx.h"
* and you can't also use it freely including it any where that you wanna
*
* @demo
std::string szAnsi = "abc123ƒ„Œ“À˚";

std::wstring szUnicode = StrConv::Ansi2Unicode(szAnsi);
szAnsi = StrConv::Unicode2Ansi(szUnicode);

std::string szUft8 = StrConv::Unicode2Utf8(szUnicode);
szUnicode = StrConv::Utf82Unicode(szUft8);

szAnsi = StrConv::Utf82Ansi(szUft8);
szUft8 = StrConv::Ansi2Utf8(szAnsi);
************************************************/

#ifndef _HR_STRING_CONVERT_H_
#define _HR_STRING_CONVERT_H_

#include <string>
#include <wtypes.h>
#include "vccore_global.h"

class VCCORE_EXPORT StrConv
{
public:
	StrConv();
	~StrConv();

	static std::wstring Ansi2Unicode(const std::string& szAnsi);
	static std::string Unicode2Ansi(const std::wstring& szUnicode);
	static std::wstring Utf82Unicode(const std::string& szUtf8);
	static std::string Unicode2Utf8(const std::wstring& szUnicode);
	static std::string Ansi2Utf8(const std::string& szAnsi);
	static std::string Utf82Ansi(const std::string& szUtf8);
};

#ifdef _UNICODE
#define TString2Utf8(/*tstring*/src) (StrConv::Unicode2Utf8(src))
#define CString2Utf8(/*const CString& */src) (StrConv::Unicode2Utf8(src.GetBuffer()).c_str())
#else
#define TString2Utf8(/*tstring*/src) (src)
#define CString2Utf8(/*const CString& */src) (src.GetBuffer())
#endif	// _UNICODE

#endif  // _HR_STRING_CONVERT_H_
