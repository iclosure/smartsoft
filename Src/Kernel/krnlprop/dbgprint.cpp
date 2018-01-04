
#include "stdafx.h"
#include "krnlprop.h"
#include "dbgprint.h"
#include <stdio.h>
#include <tchar.h>

#if defined(_DEBUG)

// Global variables
static TCHAR _g_stringPrefix[HR_DEBUG_PRINT_PREFIX_MAX_SIZE] = { 0 };

BOOL __cdecl _HR_PRINT_SET_PREFIX(__in LPCTSTR Prefix)
{
	errno_t result = _tcscpy_s(_g_stringPrefix, HR_DEBUG_PRINT_PREFIX_MAX_SIZE, Prefix);

	return (result == 0) ? TRUE : FALSE;
}

BOOL __cdecl _HR_PRINT_GET_PREFIX(__out LPTSTR Prefix, __in size_t size)
{
	errno_t result = _tcsncpy_s(Prefix, size, _g_stringPrefix, HR_DEBUG_PRINT_PREFIX_MAX_SIZE);

	return (result == 0) ? TRUE : FALSE;
}

ULONG __cdecl _HrDbgPrint(__in LPCTSTR Format, ...)
{
	va_list ap;
	ULONG n;
	TCHAR szData[356] = { 0 };

	lstrcat(szData, _g_stringPrefix);

	va_start(ap, Format);

	n = _vsntprintf_s(&szData[lstrlen(_g_stringPrefix)], 256, 256, Format, ap);

	va_end(ap);

	OutputDebugString(szData);

	return n;
}

ULONG __cdecl HrDbgPrintFuncBegin(__in LPCTSTR MethodName)
{
	return _HrDbgPrint(_T("-> %s\n"), MethodName);
}

ULONG __cdecl HrDbgPrintFuncEnd(__in LPCTSTR MethodName)
{
	return _HrDbgPrint(_T("<- %s\n"), MethodName);
}

#endif // DBG
