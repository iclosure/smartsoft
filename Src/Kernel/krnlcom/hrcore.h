#ifndef _HR_CORE_H_
#define _HR_CORE_H_

#if (NTDDI_VERSION >= NTDDI_WIN2K)

#include <ntddk.h>

//
// Time conversion related macros
//
//                                                     to    to     to
//                                                     us    ms     sec
#define HR_TIMEOUT_TO_SEC              ((LONGLONG) 1 * 10 * 1000 * 1000)
#define HR_TIMEOUT_TO_MS               ((LONGLONG) 1 * 10 * 1000)
#define HR_TIMEOUT_TO_US               ((LONGLONG) 1 * 10)

LONGLONG FORCEINLINE
HR_REL_TIMEOUT_IN_SEC(
	__in ULONGLONG Time)
{
	return Time * -1 * HR_TIMEOUT_TO_SEC;
}

LONGLONG FORCEINLINE
HR_ABS_TIMEOUT_IN_SEC(
	__in ULONGLONG Time)
{
	return Time * 1 * HR_TIMEOUT_TO_SEC;
}

LONGLONG FORCEINLINE
HR_REL_TIMEOUT_IN_MS(
	__in ULONGLONG Time)
{
	return Time * -1 * HR_TIMEOUT_TO_MS;
}

LONGLONG FORCEINLINE
HR_ABS_TIMEOUT_IN_MS(
	__in ULONGLONG Time)
{
	return Time * 1 * HR_TIMEOUT_TO_MS;
}

LONGLONG FORCEINLINE
HR_REL_TIMEOUT_IN_US(
	__in ULONGLONG Time)
{
	return Time * -1 * HR_TIMEOUT_TO_US;
}

LONGLONG FORCEINLINE
HR_ABS_TIMEOUT_IN_US(
	__in ULONGLONG Time)
{
	return Time * 1 * HR_TIMEOUT_TO_US;
}

#endif

#endif  // _HR_CORE_H_
