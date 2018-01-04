
#ifndef _HR_DEBUG_PRINT_H_
#define _HR_DEBUG_PRINT_H_

HR_EXTERN_C_START

#include <ntddk.h>
#include <ntstrsafe.h>

#define HR_DEBUG_PRINT_PREFIX_MAX_SIZE 80	// max size of prefix string

#if DBG

/*PrefixFitlerString: Prefix filter string*/
NTSTATUS __CRTDECL HR_PRINT_SET_PREFIX(__in NTSTRSAFE_PCSTR Prefix);
NTSTATUS __CRTDECL HR_PRINT_GET_PREFIX(__out NTSTRSAFE_PSTR Prefix, __in size_t size);
ULONG __CRTDECL HrDbgPrint(__in __drv_formatString(printf) NTSTRSAFE_PCSTR Format, ...);
ULONG __CRTDECL HrDbgPrintFuncBegin(__in NTSTRSAFE_PCSTR MethodName);
ULONG __CRTDECL HrDbgPrintFuncEnd(__in NTSTRSAFE_PCSTR MethodName);

#define HrKdPrint(_x_) HrDbgPrint _x_

#define HR_PRINT_METHOD_BEGIN() HrDbgPrintFuncBegin(__FUNCTION__)
#define HR_PRINT_METHOD_END() HrDbgPrintFuncEnd(__FUNCTION__)

#else

#define HR_PRINT_SET_PREFIX(_x_)
#define HR_PRINT_GET_PREFIX(_x1_, _x2_)
#define HrKdPrint(_x_)
#define HR_PRINT_METHOD_BEGIN()
#define HR_PRINT_METHOD_END()

#endif  // !DBG

HR_EXTERN_C_END

#endif  // _HR_DEBUG_PRINT_H_
