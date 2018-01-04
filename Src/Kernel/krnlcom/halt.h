
#ifndef _HR_HALT_H_
#define _HR_HALT_H_

HR_EXTERN_C_START

#include <ntddk.h>
#include "pool.h"

#if defined(_PREFAST_)
	#define HR_HALT_ASSERT3(f, p1, p2, p3) __assume(exp)
#elif DBG
	#define HR_HALT_ASSERT3(f, p1, p2, p3) \
		if (f) { \
			__try { \
				KdPrint(("%s (%d):\n", __FILE__, __LINE__)); \
				KdPrint(("  HR_HALT_ASSERT3(\"%s\",\n", #f)); \
				KdPrint(("               0x%08X, 0x%08X, 0x%08X);\n", (ULONG)(p1), (ULONG)(p2), (ULONG)(p3))); \
				DbgBreakPoint(); \
			} __except (EXCEPTION_EXECUTE_HANDLER) { \
				KeBugCheckEx(HR_MEM_TAG, (__HALT_FILE__ << 16) + __LINE__, (ULONG_PTR)(p1), (ULONG_PTR)(p2), (ULONG_PTR)(p3)); \
			} \
		}
#else
	#define HR_HALT_ASSERT3(f, p1, p2, p3)
#endif

#define HR_HALT_ASSERT2(f, p1, p2) HR_HALT_ASSERT3(f, p1, p2, 0)
#define HR_HALT_ASSERT1(f, p1)     HR_HALT_ASSERT3(f, p1, 0, 0)
#define HR_HALT_ASSERT0(f)         HR_HALT_ASSERT3(f, 0, 0, 0)

HR_EXTERN_C_END

#endif // _HR_HALT_H_
