
#include "precomp.h"
#include "pool.h"
#include "hrcrtlib.h"

void* __CRTDECL operator new(size_t size)
{
	return HR_ALLOCATE_POOL(NonPagedPool, size);
}

void __CRTDECL operator delete(void* p)
{
	HR_FREE_POOL(p);
}
