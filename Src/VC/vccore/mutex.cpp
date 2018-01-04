#include "stdafx.h"
#include "mutex.h"
#include "mutex_p.h"

//
// KMutex
// 

KMutex::KMutex(void)
{
	d = new MutexPrivate();
}

KMutex::~KMutex(void)
{
#if defined(_DEBUG)
	/*_thrtrace((_T("count: %d [%s]\n"), d->_count, d->_name.c_str()));
	if (d->_count != 0)
	{
		_thrtrace((_T("count is not equal to zero\n")));
	}*/
#endif  // _DEBUG
	delete d;
}

void KMutex::lock()
{
#if defined(_DEBUG)
	/*InterlockedIncrement(&d->_count);
	_thrtrace((_T("lock() count: %d [%s]\n"), d->_count, d->_name.c_str()));*/
#endif  // _DEBUG
	WaitForSingleObject(d->_mutex, INFINITE);
}

void KMutex::unlock()
{
#if defined(_DEBUG)
	/*InterlockedDecrement(&d->_count);
	_thrtrace((_T("unlock() count: %d [%s]\n"), d->_count, d->_name.c_str()));*/
#endif  // _DEBUG
	::ReleaseMutex(d->_mutex);
}
