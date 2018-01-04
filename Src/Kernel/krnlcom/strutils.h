
#ifndef _HR_STRUTILS_H_
#define _HR_STRUTILS_H_

HR_EXTERN_C_START

#include <ntdef.h>

#ifndef DECLARE_CONST_ANSI_STRING
#define DECLARE_CONST_ANSI_STRING(_var, _string) \
	const CHAR _var ## _buffer[] = _string; \
	__pragma(warning(push)) \
	__pragma(warning(disable:4221)) __pragma(warning(disable:4204)) \
	const ANSI_STRING _var = { sizeof(_string)-sizeof(CHAR), sizeof(_string), (PCH)_var ## _buffer } \
	__pragma(warning(pop))
#endif  // DECLARE_CONST_ANSI_STRING

#ifndef DECLARE_GLOBAL_CONST_ANSI_STRING
#define DECLARE_GLOBAL_CONST_ANSI_STRING(_var, _str) \
	extern const __declspec(selectany) ANSI_STRING _var = RTL_CONSTANT_STRING(_str)
#endif  // DECLARE_GLOBAL_CONST_ANSI_STRING

#ifndef DECLARE_ANSI_STRING_SIZE
#define DECLARE_ANSI_STRING_SIZE(_var, _size) \
	CHAR _var ## _buffer[_size]; \
	__pragma(warning(push)) \
	__pragma(warning(disable:4221)) __pragma(warning(disable:4204)) \
	ANSI_STRING _var = { 0, (_size) * sizeof(CHAR) , _var ## _buffer } \
	__pragma(warning(pop))
#endif  // DECLARE_ANSI_STRING_SIZE

NTSTATUS UnicodeStringDupString(
	OUT LPWSTR *strDest,
	IN LPWSTR strSrc,
	IN POOL_TYPE PoolType,
	IN BOOLEAN MultiStr);

VOID UnicodeStringAppendString(
	IN OUT PNTSTATUS pStatus,
	IN OUT PUNICODE_STRING pDest,
	IN PWCHAR pSrc,
	IN USHORT lenSrc);

VOID UnicodeStringAppendUnicodeString(
	IN OUT PNTSTATUS pStatus,
	IN OUT PUNICODE_STRING pDest,
	IN PUNICODE_STRING pSrc);

VOID UnicodeStringAppendString0(
	IN OUT PNTSTATUS pStatus,
	IN OUT PUNICODE_STRING pDest,
	IN PWCHAR pSrc);

VOID UnicodeStringAppendData(
	IN OUT PNTSTATUS pStatus,
	IN OUT PUNICODE_STRING pDest,
	IN ULONG data,
	IN ULONG base);

VOID UnicodeStringFree(
	IN OUT PUNICODE_STRING  pDest);

BOOLEAN UnicodeStringFreeBad(
	NTSTATUS status,
	IN OUT PUNICODE_STRING pDest);

VOID UnicodeStringAppendDeviceProperty(
	IN OUT PNTSTATUS pStatus,
	IN OUT PUNICODE_STRING pDest,
	IN PDEVICE_OBJECT pDevObj,
	IN DEVICE_REGISTRY_PROPERTY deviceProperty);

HR_EXTERN_C_END

#endif /* _HR_STRUTILS_H_ */
