
#include "precomp.h"

/* __HALT_FILE__ used by HR_HALT_ASSERT to put it on BSOD */
#define __HALT_FILE__	2

#if DBG

// Global variables
static CHAR _g_stringPrefix[HR_DEBUG_PRINT_PREFIX_MAX_SIZE] = { 0 };

NTSTATUS __cdecl HR_PRINT_SET_PREFIX(__in NTSTRSAFE_PCSTR Prefix)
{
	return RtlStringCchCopyA(_g_stringPrefix, HR_DEBUG_PRINT_PREFIX_MAX_SIZE, Prefix);
}

NTSTATUS __cdecl HR_PRINT_GET_PREFIX(__out NTSTRSAFE_PSTR Prefix, __in size_t size)
{
	return RtlStringCchCopyNA(Prefix, size, _g_stringPrefix, HR_DEBUG_PRINT_PREFIX_MAX_SIZE);
}

ULONG __cdecl HrDbgPrint(__in __drv_formatString(printf) NTSTRSAFE_PCSTR Format, ...)
{
	va_list ap;
	ULONG n;

	va_start(ap, Format);

	n = vDbgPrintExWithPrefix(_g_stringPrefix, DPFLTR_DEFAULT_ID, DPFLTR_INFO_LEVEL, Format, ap);

	va_end(ap);

	return n;
}

ULONG __cdecl HrDbgPrintFuncBegin(__in NTSTRSAFE_PCSTR MethodName)
{
	return HrDbgPrint("-> %s\n", MethodName);
}

ULONG __cdecl HrDbgPrintFuncEnd(__in NTSTRSAFE_PCSTR MethodName)
{
	return HrDbgPrint("<- %s\n", MethodName);
}

#endif // DBG
