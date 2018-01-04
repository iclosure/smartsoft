
#ifndef _HR_KRNLCOM_TYPES_H_
#define _HR_KRNLCOM_TYPES_H_

#include "krnlcomdefs.h"

HR_EXTERN_C_START

#include <ntddk.h>

HR_EXTERN_C_END

//////////////////////////////////////////////////////////////////////////

//! driver context extension
typedef struct _DRIVER_CONTEXT_EXTENSION
{
	PDRIVER_OBJECT		TheDriver;
	UNICODE_STRING		RegistryPath;	// TODO

} DRIVER_CONTEXT_EXTENSION, *PDRIVER_CONTEXT_EXTENSION;

#endif	// _HR_KRNLCOM_TYPES_H_