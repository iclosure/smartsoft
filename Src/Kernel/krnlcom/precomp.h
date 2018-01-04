
#ifndef _HR_PRECOMP_H_
#define _HR_PRECOMP_H_

#include "krnlcomdefs.h"

#pragma warning(push, 3)

HR_EXTERN_C_START

#include <ntddk.h>
/*#if KERNEL_FRAMEWORK_WDF
#include <wdf.h>
#else*/
#include <wdm.h>
//#endif

HR_EXTERN_C_END

#pragma warning(pop)

#include <ntstrsafe.h>

#include "krnlcom.h"
#include "version.h"

#endif /* _HR_PRECOMP_H_ */
