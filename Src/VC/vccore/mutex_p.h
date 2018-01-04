#pragma once

#include "vccore_global.h"
#include <Windows.h>
#include "kobject.h"

// 
// MutexPrivate
// 

class MutexPrivate : public KObject
{
	friend class KMutex;
public:
	explicit MutexPrivate(void);
	virtual ~MutexPrivate(void);

	//bool wait(int timeout);
	//void wakeUp();

private:
	HR_DISABLE_COPY(MutexPrivate)
		HANDLE _mutex;
#if defined(_DEBUG)
	tstring _name;
	LONG _count;
#endif  // _DEBUG
};
