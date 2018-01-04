#include "stdafx.h"
#include "stringconvert.h"

StrConv::StrConv()
{
}

StrConv::~StrConv()
{
}

std::wstring StrConv::Ansi2Unicode(const std::string& szAnsi)
{
	//calc block size to be returned
	int len = MultiByteToWideChar(CP_ACP, NULL, szAnsi.c_str(), szAnsi.size(), NULL, 0);
	//malloc and fill the returned block
	wchar_t* szUnicode = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, NULL, szAnsi.c_str(), szAnsi.size(), szUnicode, len);
	szUnicode[len] = 0;
	std::wstring rs = szUnicode;
	delete[] szUnicode;

	return rs;
}

std::string StrConv::Unicode2Ansi(const std::wstring& szUnicode)
{
	//calc block size to be returned
	int len = WideCharToMultiByte(CP_ACP, NULL, szUnicode.c_str(), szUnicode.size(), NULL, 0, NULL, NULL);
	//malloc and fill the returned block
	char* szAnsi = new char[len + 1];
	WideCharToMultiByte(CP_ACP, NULL, szUnicode.c_str(), szUnicode.size(), szAnsi, len, NULL, NULL);
	szAnsi[len] = 0;
	std::string rs = szAnsi;
	delete[] szAnsi;

	return rs;
}

std::wstring StrConv::Utf82Unicode(const std::string& szUtf8)
{
	//calc block size to be returned
	int len = MultiByteToWideChar(CP_UTF8, NULL, szUtf8.c_str(), szUtf8.size(), NULL, 0);
	//malloc and fill the returned block
	wchar_t* szUnicode = new wchar_t[len + 1];
	MultiByteToWideChar(CP_UTF8, NULL, szUtf8.c_str(), szUtf8.size(), szUnicode, len);
	szUnicode[len] = 0;
	std::wstring rs = szUnicode;
	delete[] szUnicode;

	return rs;
}

std::string StrConv::Unicode2Utf8(const std::wstring& szUnicode)
{
	//calc block size to be returned
	int len = WideCharToMultiByte(CP_UTF8, NULL, szUnicode.c_str(), szUnicode.size(), NULL, 0, NULL, NULL);
	//malloc and fill the returned block
	char* szUtf8 = new char[len + 1];
	WideCharToMultiByte(CP_UTF8, NULL, szUnicode.c_str(), szUnicode.size(), szUtf8, len, NULL, NULL);
	szUtf8[len] = 0;
	std::string rs = szUtf8;
	delete[] szUtf8;

	return rs;
}

std::string StrConv::Ansi2Utf8(const std::string& szAnsi)
{
	return Unicode2Utf8(Ansi2Unicode(szAnsi));
}

std::string StrConv::Utf82Ansi(const std::string& szUtf8)
{
	return Unicode2Ansi(Utf82Unicode(szUtf8));
}