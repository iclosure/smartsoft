
#include "precomp.h"

/* __HALT_FILE__ used by HR_HALT_ASSERT to put it on BSOD */
#define __HALT_FILE__	4

/////////////////////////////////////////////////////////////////
// CIRCFIFO_ATOM_SIZE_1

FORCEINLINE
VOID _CircFifoRead_1(PCIRCFIFO_OBJECT Object, PUINT8 Buffer)
{
	*Buffer = Object->Buffer._1[Object->IndexRead];
}

FORCEINLINE
VOID _CircFifoWrite_1(PCIRCFIFO_OBJECT Object, PUINT8 Buffer)
{
	Object->Buffer._1[Object->IndexWrite] = *Buffer;
}

FORCEINLINE
VOID _CircFifoRead_1_Buffer(PCIRCFIFO_OBJECT Object, PVOID Buffer, LONG Offset, LONG Count)
{
	RtlCopyMemory(&((PUINT8)Buffer)[Offset], &Object->Buffer._1[Object->IndexRead], Count);
}

FORCEINLINE
VOID _CircFifoWrite_1_Buffer(PCIRCFIFO_OBJECT Object, PVOID Buffer, LONG Offset, LONG Count)
{
	RtlCopyMemory(&Object->Buffer._1[Object->IndexWrite], &((PUINT8)Buffer)[Offset], Count);
}

/////////////////////////////////////////////////////////////////
// CIRCFIFO_ATOM_SIZE_2

FORCEINLINE
VOID _CircFifoRead_2(PCIRCFIFO_OBJECT Object, PUINT16 Buffer)
{
	*Buffer = Object->Buffer._2[Object->IndexRead];
}

FORCEINLINE
VOID _CircFifoWrite_2(PCIRCFIFO_OBJECT Object, PUINT16 Buffer)
{
	Object->Buffer._2[Object->IndexWrite] = *Buffer;
}

FORCEINLINE
VOID _CircFifoRead_2_Buffer(PCIRCFIFO_OBJECT Object, PVOID Buffer, LONG Offset, LONG Count)
{
	RtlCopyMemory(&((PUINT16)Buffer)[Offset], &Object->Buffer._2[Object->IndexRead], Object->AtomSize * Count);
}

FORCEINLINE
VOID _CircFifoWrite_2_Buffer(PCIRCFIFO_OBJECT Object, PVOID Buffer, LONG Offset, LONG Count)
{
	RtlCopyMemory(&Object->Buffer._2[Object->IndexWrite], &((PUINT16)Buffer)[Offset], Object->AtomSize * Count);
}

/////////////////////////////////////////////////////////////////
// CIRCFIFO_ATOM_SIZE_4

FORCEINLINE
VOID _CircFifoRead_4(PCIRCFIFO_OBJECT Object, PUINT32 Buffer)
{
	*Buffer = Object->Buffer._4[Object->IndexRead];
}

FORCEINLINE
VOID _CircFifoWrite_4(PCIRCFIFO_OBJECT Object, PUINT32 Buffer)
{
	Object->Buffer._4[Object->IndexWrite] = *Buffer;
}

FORCEINLINE
VOID _CircFifoRead_4_Buffer(PCIRCFIFO_OBJECT Object, PVOID Buffer, LONG Offset, LONG Count)
{
	RtlCopyMemory(&((PUINT32)Buffer)[Offset], &Object->Buffer._4[Object->IndexRead], Object->AtomSize * Count);
}

FORCEINLINE
VOID _CircFifoWrite_4_Buffer(PCIRCFIFO_OBJECT Object, PVOID Buffer, LONG Offset, LONG Count)
{
	RtlCopyMemory(&Object->Buffer._4[Object->IndexWrite], &((PUINT32)Buffer)[Offset], Object->AtomSize * Count);
}

/////////////////////////////////////////////////////////////////
// CIRCFIFO_ATOM_SIZE_8

FORCEINLINE
VOID _CircFifoRead_8(PCIRCFIFO_OBJECT Object, PUINT64 Buffer)
{
	*Buffer = Object->Buffer._8[Object->IndexRead];
}

FORCEINLINE
VOID _CircFifoWrite_8(PCIRCFIFO_OBJECT Object, PUINT64 Buffer)
{
	Object->Buffer._8[Object->IndexWrite] = *Buffer;
}

FORCEINLINE
VOID _CircFifoRead_8_Buffer(PCIRCFIFO_OBJECT Object, PVOID Buffer, LONG Offset, LONG Count)
{
	RtlCopyMemory(&((PUINT64)Buffer)[Offset], &Object->Buffer._8[Object->IndexRead], Object->AtomSize * Count);
}

FORCEINLINE
VOID _CircFifoWrite_8_Buffer(PCIRCFIFO_OBJECT Object, PVOID Buffer, LONG Offset, LONG Count)
{
	RtlCopyMemory(&Object->Buffer._8[Object->IndexWrite], &((PUINT64)Buffer)[Offset], Object->AtomSize * Count);
}

/////////////////////////////////////////////////////////////////
// CIRCFIFO_ATOM_SIZE_ANY

FORCEINLINE
VOID _CircFifoRead_Any(PCIRCFIFO_OBJECT Object, PVOID Buffer)
{
	RtlCopyMemory(Buffer, &Object->Buffer._1[Object->IndexRead * Object->AtomSize], Object->AtomSize);
}

FORCEINLINE
VOID _CircFifoWrite_Any(PCIRCFIFO_OBJECT Object, PVOID Buffer)
{
	RtlCopyMemory(&Object->Buffer._1[Object->IndexWrite * Object->AtomSize], Buffer, Object->AtomSize);
}

FORCEINLINE
VOID _CircFifoRead_Any_Buffer(PCIRCFIFO_OBJECT Object, PVOID Buffer, LONG Offset, LONG Count)
{
	RtlCopyMemory(&((PUINT8)Buffer)[Object->AtomSize * Offset], &Object->Buffer._1[Object->AtomSize * Object->IndexRead], Object->AtomSize * Count);
}

FORCEINLINE
VOID _CircFifoWrite_Any_Buffer(PCIRCFIFO_OBJECT Object, PVOID Buffer, LONG Offset, LONG Count)
{
	RtlCopyMemory(&Object->Buffer._1[Object->AtomSize * Object->IndexWrite], &((PUINT8)Buffer)[Object->AtomSize * Offset], Object->AtomSize * Count);
}

/////////////////////////////////////////////////////////////////

NTSTATUS CircFifoCreate(PCIRCFIFO_OBJECT Object, LONG MaxCount, LONG AtomSize)
{
	NTSTATUS status = STATUS_SUCCESS;
	PVOID *buffer;

	HR_PRINT_METHOD_BEGIN();

	if (NULL == Object || 0 == MaxCount || 0 == AtomSize)
	{
		HrKdPrint(("STATUS_INVALID_PARAMETER\n"));
		return STATUS_INVALID_PARAMETER;
	}

	RtlZeroMemory(Object, sizeof(CIRCFIFO_OBJECT));

	HrKdPrint(("CIRCFIFO_ATOM_SIZE: %ld\n", AtomSize));

	switch (AtomSize)
	{
	case CIRCFIFO_ATOM_SIZE_1:
		buffer = &Object->Buffer._1;
		Object->ReadRoutine = _CircFifoRead_1;
		Object->WriteRoutine = _CircFifoWrite_1;
		Object->ReadBufferRoutine = _CircFifoRead_1_Buffer;
		Object->WriteBufferRoutine = _CircFifoWrite_1_Buffer;
		break;
	case CIRCFIFO_ATOM_SIZE_2:
		buffer = &Object->Buffer._2;
		Object->ReadRoutine = _CircFifoRead_2;
		Object->WriteRoutine = _CircFifoWrite_2;
		Object->ReadBufferRoutine = _CircFifoRead_2_Buffer;
		Object->WriteBufferRoutine = _CircFifoWrite_2_Buffer;
		break;
	case CIRCFIFO_ATOM_SIZE_4:
		buffer = &Object->Buffer._4;
		Object->ReadRoutine = _CircFifoRead_4;
		Object->WriteRoutine = _CircFifoWrite_4;
		Object->ReadBufferRoutine = _CircFifoRead_4_Buffer;
		Object->WriteBufferRoutine = _CircFifoWrite_4_Buffer;
		break;
	case CIRCFIFO_ATOM_SIZE_8:
		buffer = &Object->Buffer._8;
		Object->ReadRoutine = _CircFifoRead_8;
		Object->WriteRoutine = _CircFifoWrite_8;
		Object->ReadBufferRoutine = _CircFifoRead_8_Buffer;
		Object->WriteBufferRoutine = _CircFifoWrite_8_Buffer;
		break;
	case CIRCFIFO_ATOM_SIZE_ANY:
	default:
		buffer = &Object->Buffer._1;
		Object->ReadRoutine = _CircFifoRead_Any;
		Object->WriteRoutine = _CircFifoWrite_Any;
		Object->ReadBufferRoutine = _CircFifoRead_Any_Buffer;
		Object->WriteBufferRoutine = _CircFifoWrite_Any_Buffer;
		break;
	}

	SPINLOCK_CREATE(Object);

	*buffer = HR_ALLOCATE_POOL(NonPagedPool, MaxCount * AtomSize);
	if (!*buffer)
	{
		HrKdPrint(("HR_ALLOCATE_POOL Object->Buffer? failed as %08X\n", STATUS_INSUFFICIENT_RESOURCES));
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	Object->MaxCount = MaxCount;
	Object->AtomSize = AtomSize;
	Object->Full80 = MaxCount * 4 / 5;

	HR_PRINT_METHOD_END();

	return status;
}

VOID CircFifoClear(PCIRCFIFO_OBJECT Object)
{
	SPINLOCK_LOCK(Object);

	Object->Count = 0;
	Object->IndexRead = 0;
	Object->IndexWrite = 0;

	SPINLOCK_UNLOCK(Object);
}

VOID CircFifoFree(PCIRCFIFO_OBJECT Object)
{
	PVOID *buffer;

	HR_PRINT_METHOD_BEGIN();

	SPINLOCK_LOCK(Object);

	switch (Object->AtomSize)
	{
	case CIRCFIFO_ATOM_SIZE_1:
		buffer = &Object->Buffer._1;
		break;
	case CIRCFIFO_ATOM_SIZE_2:
		buffer = &Object->Buffer._2;
		break;
	case CIRCFIFO_ATOM_SIZE_4:
		buffer = &Object->Buffer._4;
		break;
	case CIRCFIFO_ATOM_SIZE_8:
		buffer = &Object->Buffer._8;
		break;
	default:
		buffer = &Object->Buffer._1;
		break;
	}

	if (*buffer != NULL)
	{
		HR_FREE_POOL(*buffer);
		*buffer = NULL;
	}

	Object->MaxCount = 0;
	Object->AtomSize = 0;
	Object->Count = 0;
	Object->IndexRead = 0;
	Object->IndexWrite = 0;

	SPINLOCK_UNLOCK(Object);

	HR_PRINT_METHOD_END();
}
