
#ifndef _HR_KERNEL_PROP_H_
#define _HR_KERNEL_PROP_H_

#include <Windows.h>

#include "krnlprop.h"
#include "dbgprint.h"

#define _HR_KRNLPROP_LIBNAME_1_	"krnlprop"

#ifdef _UNICODE
#define _HR_KRNLPROP_LIBNAME_2_		_HR_KRNLPROP_LIBNAME_1_##"u"
#else
#define _HR_KRNLPROP_LIBNAME_2_		_HR_KRNLPROP_LIBNAME_1_
#endif

#ifdef _DEBUG
#define _HR_KRNLPROP_LIBNAME_3_		_HR_KRNLPROP_LIBNAME_2_##"d"
#else
#define _HR_KRNLPROP_LIBNAME_3_		_HR_KRNLPROP_LIBNAME_2_
#endif

#define _HR_KRNLPROP_LIBNAME_		_HR_KRNLPROP_LIBNAME_3_##".lib"

#pragma comment(lib, _HR_KRNLPROP_LIBNAME_)

#ifndef _KRNLPROP_HIDE_AUTOLINK_OUTPUT_
#pragma message("Automatically linking with " _HR_KRNLPROP_LIBNAME_)
#endif	// _KRNLPROP_HIDE_AUTOLINK_OUTPUT_

#endif  // _HR_KERNEL_PROP_H_
