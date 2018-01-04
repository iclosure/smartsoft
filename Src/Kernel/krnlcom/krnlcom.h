
#ifndef _HR_KRNLCOM_H_
#define _HR_KRNLCOM_H_

#include "krnlcomdefs.h"

HR_EXTERN_C_START

#include "hrstddef.h"
#include "dbgprint.h"
#include "halt.h"
#include "hrcore.h"
#include "pool.h"
#include "circfifo.h"
#include "strutils.h"
#include "reg9054.h"
#include "bitsopt.h"
#include "precisetime.h"

HR_EXTERN_C_END

/**
*
**/

#ifdef DBG
#define HR_BREAK_POINT() KdBreakPoint()  //_asm int 3
#else
#define HR_BREAK_POINT()
#endif

/**
 *  
 **/

 FORCEINLINE
 NTSTATUS HrRequestComplete(
	IN PIRP Irp,
	IN NTSTATUS Status)
{
	Irp->IoStatus.Status = Status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return Status;
}

FORCEINLINE
NTSTATUS HrRequestCompleteWithInformation(
	IN PIRP Irp,
	IN NTSTATUS Status,
	IN ULONG_PTR Information)
{
	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = Information;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return Status;
}

#endif	// _HR_KRNLCOM_H_
