#include "stdafx.h"
#include "mutex_p.h"

// 
// MutexPrivate
// 

MutexPrivate::MutexPrivate(void)
#if defined(_DEBUG)
	 : _name(_T("KMutex") + KUuid::guidBySting()),
	_count(0)
#endif  // _DEBUG
	{
#if defined(_DEBUG)
	_mutex = ::CreateMutex(NULL, FALSE, _name.c_str());
#else
		_mutex = ::CreateMutex(NULL, FALSE, KUuid::guidBySting().c_str());
#endif  // !_DEBUG
}

MutexPrivate::~MutexPrivate(void)
{
	::CloseHandle(_mutex);
}
