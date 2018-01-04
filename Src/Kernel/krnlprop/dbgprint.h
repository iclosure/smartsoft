
#ifndef _HR_DEBUG_PRINT_H_
#define _HR_DEBUG_PRINT_H_

#include "tchar.h"

#define HR_DEBUG_PRINT_PREFIX_MAX_SIZE 80	// max size of prefix string

#if defined(_DEBUG)

/*PrefixFitlerString: Prefix filter string*/
BOOL __CRTDECL _HR_PRINT_SET_PREFIX(__in LPCTSTR Prefix);
BOOL __CRTDECL _HR_PRINT_GET_PREFIX(__out LPTSTR Prefix, __in size_t size);
ULONG __CRTDECL _HrDbgPrint(__in LPCTSTR Format, ...);
ULONG __CRTDECL HrDbgPrintFuncBegin(__in LPCTSTR MethodName);
ULONG __CRTDECL HrDbgPrintFuncEnd(__in LPCTSTR MethodName);

#define HR_PRINT_SET_PREFIX(_x_) _HR_PRINT_SET_PREFIX(_T(_x_))
#define HR_PRINT_GET_PREFIX(_x1_, _x2_) _HR_PRINT_GET_PREFIX(_x1_, _x2_)
#define HrUdPrint(_x_) _HrDbgPrint _x_
#define HR_PRINT_METHOD_BEGIN() HrDbgPrintFuncBegin(_T(__FUNCTION__))
#define HR_PRINT_METHOD_END() HrDbgPrintFuncEnd(_T(__FUNCTION__))

#else

#define HR_PRINT_SET_PREFIX(_x_)
#define HR_PRINT_GET_PREFIX(_x1_, _x2_)
#define HrUdPrint(_x_)
#define HR_PRINT_METHOD_BEGIN()
#define HR_PRINT_METHOD_END()

#endif  // !_DEBUG

#endif  // _HR_DEBUG_PRINT_H_
