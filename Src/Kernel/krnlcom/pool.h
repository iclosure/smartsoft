
#ifndef _HR_POOL_H_
#define _HR_POOL_H_

HR_EXTERN_C_START

#include <ntddk.h>

HR_EXTERN_C_END

// allocate and free definitions
#define HR_MEM_TAG		0xB0B0B0B0	// 

#define HR_ALLOCATE_POOL(PoolType, NumberOfBytes) \
	ExAllocatePoolWithTag(PoolType, NumberOfBytes, HR_MEM_TAG)

#define HR_ALLOCATE_POOL_WITH_QUOTA(PoolType, NumberOfBytes) \
	ExAllocatePoolWithQuotaTag(PoolType, NumberOfBytes, HR_MEM_TAG)

#define HR_FREE_POOL(P) \
	ExFreePool(P) /*ExFreePoolWithTag(P, HR_MEM_TAG)*/

#endif  // _HR_POOL_H_
