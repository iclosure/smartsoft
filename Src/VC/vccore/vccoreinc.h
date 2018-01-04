
#ifndef HR_VCCORE_INCLUDE_H
#define HR_VCCORE_INCLUDE_H

#define _VCCORE_LIBNAME_1_	"vccore"

#ifdef _UNICODE
#define _VCCORE_LIBNAME_2_	_VCCORE_LIBNAME_1_##"u"
#else
#define _VCCORE_LIBNAME_2_	_VCCORE_LIBNAME_1_
#endif

#ifdef _DEBUG
#define _VCCORE_LIBNAME_3_	_VCCORE_LIBNAME_2_##"d"
#else
#define _VCCORE_LIBNAME_3_	_VCCORE_LIBNAME_2_
#endif

#if _MSC_VER < 1300
#define _VCCORE_LIBNAME_4_	_VCCORE_LIBNAME_3_##""
#elif _MSC_VER == 1300
#define _VCCORE_LIBNAME_4_	_VCCORE_LIBNAME_3_##"70"
#elif _MSC_VER < 1400
#define _VCCORE_LIBNAME_4_	_VCCORE_LIBNAME_3_##"71"
#elif _MSC_VER < 1500
#define _VCCORE_LIBNAME_4_	_VCCORE_LIBNAME_3_##"80"
#elif _MSC_VER == 1500
#define _VCCORE_LIBNAME_4_	_VCCORE_LIBNAME_3_##"90"
#elif _MSC_VER == 1600
#define _VCCORE_LIBNAME_4_	_VCCORE_LIBNAME_3_##"100"
#elif _MSC_VER == 1700
#define _VCCORE_LIBNAME_4_	_VCCORE_LIBNAME_3_##"110"
#elif _MSC_VER == 1800
#define _VCCORE_LIBNAME_4_	_VCCORE_LIBNAME_3_##"120"
#else
#error "Unsupported VC++ version"
#endif

#define _VCCORE_LIBNAME_		_VCCORE_LIBNAME_4_##".lib"

#pragma comment(lib, _VCCORE_LIBNAME_)

#ifndef _VCCORE_HIDE_AUTOLINK_OUTPUT_
#pragma comment (lib, _VCCORE_LIBNAME_)
#pragma message("Automatically linking with " _VCCORE_LIBNAME_)
#endif	// _VCCORE_HIDE_AUTOLINK_OUTPUT_

#include "circfifo.h"
#include "criticasection.h"
#include "mmtimer.h"
#include "mutex.h"
#include "precitimer.h"
#include "resourcemgr.h"
#include "kstring.h"
#include "stringconvert.h"
#include "thread.h"
#include "waitcondition.h"

#endif  // HR_VCCORE_INCLUDE_H
