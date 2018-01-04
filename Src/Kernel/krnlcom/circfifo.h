
#ifndef _HR_CIRCULAR_FIFO_H_
#define _HR_CIRCULAR_FIFO_H_

HR_EXTERN_C_START

#include <ntdef.h>
/*#if KERNEL_FRAMEWORK_WDF
#include <wdf.h>
#else*/
#include <wdm.h>
//#endif  // !KERNEL_FRAMEWORK_WDF

#include <ntddser.h>  // for serial

// CIRCFIFO_INFO::AtomType ->
enum CIRCFIFO_ATOM_SIZE
{
	CIRCFIFO_ATOM_SIZE_1 = 1,
	CIRCFIFO_ATOM_SIZE_2 = 2,
	CIRCFIFO_ATOM_SIZE_4 = 4,
	CIRCFIFO_ATOM_SIZE_8 = 8,
	CIRCFIFO_ATOM_SIZE_ANY //...
};

typedef struct _CIRCFIFO_OBJECT CIRCFIFO_OBJECT, *PCIRCFIFO_OBJECT;
typedef VOID(*PCIRCFIFO_READ_ROUTINE)(PCIRCFIFO_OBJECT Object, PVOID Buffer);
typedef VOID(*PCIRCFIFO_WRITE_ROUTINE)(PCIRCFIFO_OBJECT Object, PVOID Buffer);

typedef VOID(*PCIRCFIFO_READ_BUFFER_ROUTINE)(PCIRCFIFO_OBJECT Object, PVOID Buffer, LONG Offset, LONG Count);
typedef VOID(*PCIRCFIFO_WRITE_BUFFER_ROUTINE)(PCIRCFIFO_OBJECT Object, PVOID Buffer, LONG Offset, LONG Count);

// 
typedef struct _CIRCFIFO_OBJECT
{
	LONG		MaxCount;	// 
	LONG		Count;		// 
	LONG		IndexRead;	// index of read
	LONG		IndexWrite;	// index of write
	LONG		Full80;		// the receive buffer is 80 percent full.

/*#if KERNEL_FRAMEWORK_WDF
	WDFSPINLOCK	Lock;		// 
#else*/
	KIRQL		OldIrql;	// 
	KSPIN_LOCK	Lock;		// 
//#endif  // !KERNEL_FRAMEWORK_WDF

	LONG		AtomSize;	// 
	union {
		PUINT8  _1;
		PUINT16 _2;
		PUINT32 _4;
		PUINT64 _8;
	} Buffer;

	PCIRCFIFO_READ_ROUTINE	ReadRoutine;
	PCIRCFIFO_WRITE_ROUTINE	WriteRoutine;

	PCIRCFIFO_READ_BUFFER_ROUTINE	ReadBufferRoutine;
	PCIRCFIFO_WRITE_BUFFER_ROUTINE	WriteBufferRoutine;

} CIRCFIFO_OBJECT, *PCIRCFIFO_OBJECT;

// 
/*#if KERNEL_FRAMEWORK_WDF
#define SPINLOCK_CREATE(_circfifo_) \
	status = WdfSpinLockCreate(WDF_NO_OBJECT_ATTRIBUTES, &(_circfifo_)->Lock); \
	if (!NT_SUCCESS(status)) \
{ \
	HrKdPrint(("WdfSpinLockCreate() failed as %08X\n", status)); \
	return status; \
}

#define SPINLOCK_LOCK(_circfifo_) \
	WdfSpinLockAcquire((_circfifo_)->Lock);

#define SPINLOCK_UNLOCK(_circfifo_) \
	WdfSpinLockRelease((_circfifo_)->Lock);
#else*/
#define SPINLOCK_CREATE(_circfifo_) \
	KeInitializeSpinLock(&(_circfifo_)->Lock);

#define SPINLOCK_LOCK(_circfifo_) \
	KeAcquireSpinLock(&(_circfifo_)->Lock, &(_circfifo_)->OldIrql);

#define SPINLOCK_UNLOCK(_circfifo_) \
	KeReleaseSpinLock(&(_circfifo_)->Lock, (_circfifo_)->OldIrql);
//#endif

NTSTATUS CircFifoCreate(PCIRCFIFO_OBJECT Object, LONG MaxCount, LONG AtomSize);
VOID CircFifoClear(PCIRCFIFO_OBJECT Object);
VOID CircFifoFree(PCIRCFIFO_OBJECT Object);

FORCEINLINE
LONG CircFifoMaxCount(PCIRCFIFO_OBJECT Object)
{
	return Object->MaxCount;
}

FORCEINLINE
LONG CircFifoAtomSize(PCIRCFIFO_OBJECT Object)
{
	return Object->AtomSize;
}

FORCEINLINE
LONG CircFifoCount(PCIRCFIFO_OBJECT Object)
{
	LONG result;

	SPINLOCK_LOCK(Object);

	result = Object->Count;

	SPINLOCK_UNLOCK(Object);

	return result;
}

FORCEINLINE
BOOLEAN CircFifoIsEmpty(PCIRCFIFO_OBJECT Object)
{
	BOOLEAN result;

	SPINLOCK_LOCK(Object);

	result = (Object->Count == 0) ? TRUE : FALSE;

	SPINLOCK_UNLOCK(Object);

	return result;
}

FORCEINLINE
BOOLEAN CircFifoIsFull(PCIRCFIFO_OBJECT Object)
{
	BOOLEAN result;

	SPINLOCK_LOCK(Object);

	result = (Object->Count == Object->MaxCount) ? TRUE : FALSE;

	SPINLOCK_UNLOCK(Object);

	return result;
}

FORCEINLINE
BOOLEAN CircFifoIsFull80(PCIRCFIFO_OBJECT Object)
{
	BOOLEAN result;

	SPINLOCK_LOCK(Object);

	result = (Object->Count >= Object->Full80) ? TRUE : FALSE;

	SPINLOCK_UNLOCK(Object);

	return result;
}

FORCEINLINE
LONG CircFifoRoom(PCIRCFIFO_OBJECT Object)
{
	LONG result;

	SPINLOCK_LOCK(Object);

	result = Object->MaxCount - Object->Count;

	SPINLOCK_UNLOCK(Object);

	return result;
}

FORCEINLINE
BOOLEAN CircFifoRead(PCIRCFIFO_OBJECT Object, PVOID Buffer)
{
	BOOLEAN result = TRUE;

	SPINLOCK_LOCK(Object);

	if (Object->Count == 0)
	{
		result = FALSE;
	}
	else
	{
		Object->ReadRoutine(Object, Buffer);
		Object->IndexRead = (Object->IndexRead + 1) % Object->MaxCount;
		Object->Count--;
	}

	SPINLOCK_UNLOCK(Object);

	return result;
}

FORCEINLINE
BOOLEAN CircFifoWrite(PCIRCFIFO_OBJECT Object, PVOID Buffer)
{
	BOOLEAN result = TRUE;

	SPINLOCK_LOCK(Object);

	if (Object->Count == Object->MaxCount)
	{
		Object->IndexRead = (Object->IndexRead + 1) % Object->MaxCount;
		Object->Count--;

		result = FALSE;
	}

	Object->WriteRoutine(Object, Buffer);
	Object->IndexWrite = (Object->IndexWrite + 1) % Object->MaxCount;
	Object->Count++;

	SPINLOCK_UNLOCK(Object);

	return result;
}

FORCEINLINE
LONG CircFifoReadBuffer(PCIRCFIFO_OBJECT Object, PVOID Buffer, LONG Count)
{
	SPINLOCK_LOCK(Object);
	
	if (Object->Count == 0)
	{
		Count = 0;
	}
	else
	{
		LONG overflow;
		
		if (Count > Object->Count)
		{
			Count = Object->Count;
		}

		overflow = Object->IndexRead + Count - (Object->MaxCount - 1);

		if (overflow <= 0)
		{
			Object->ReadBufferRoutine(Object, Buffer, 0, Count);
			Object->IndexRead = (Object->IndexRead + Count) % Object->MaxCount;
		}
		else
		{
			Object->ReadBufferRoutine(Object, Buffer, 0, Count - overflow);
			Object->IndexRead = 0;

			Object->ReadBufferRoutine(Object, Buffer, Count - overflow, overflow);
			Object->IndexRead = overflow - 1;
		}

		Object->Count -= Count;
	}
	
	SPINLOCK_UNLOCK(Object);

	return Count;
}

FORCEINLINE
VOID CircFifoWriteBuffer(PCIRCFIFO_OBJECT Object, PVOID Buffer, LONG Count)
{
	LONG overflow;

	SPINLOCK_LOCK(Object);

	overflow = Object->Count + Count - Object->MaxCount;

	if (overflow > 0)
	{
		Object->IndexRead = (Object->IndexRead + overflow) % Object->MaxCount;
		Object->Count -= overflow;
	}

	overflow = Object->IndexWrite + Count - (Object->MaxCount - 1);

	if (overflow <= 0)
	{
		Object->WriteBufferRoutine(Object, Buffer, 0, Count);
		Object->IndexWrite = (Object->IndexWrite + Count) % Object->MaxCount;
	}
	else
	{
		Object->WriteBufferRoutine(Object, Buffer, 0, Count - overflow);
		Object->IndexWrite = 0;

		Object->WriteBufferRoutine(Object, Buffer, Count - overflow, overflow);
		Object->IndexWrite = overflow - 1;
	}

	Object->Count += Count;

	SPINLOCK_UNLOCK(Object);
}

// Extend

FORCEINLINE
VOID CircFifoGetSerialEvent(PCIRCFIFO_OBJECT Object, PULONG SerialEvent)
{
	SPINLOCK_LOCK(Object);

	*SerialEvent |= (Object->Count > 0) ? SERIAL_EV_RXCHAR : 0;
	*SerialEvent |= (Object->Count >= Object->Full80) ? SERIAL_EV_RX80FULL : 0;

	SPINLOCK_UNLOCK(Object);
}

HR_EXTERN_C_END

#endif	// _HR_CIRCULAR_FIFO_H_
