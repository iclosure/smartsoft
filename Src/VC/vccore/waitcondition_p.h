#pragma once

#include "mutex.h"
#include <ObjBase.h>

// 
// WaitEventPrivate
// 

class WaitEventPrivate
{
public:
	explicit WaitEventPrivate(tstring name);
	virtual ~WaitEventPrivate(void);

	bool setEvent();
	bool resetEvent();
	
private:
	HR_DISABLE_COPY(WaitEventPrivate)

	tstring q_name;
	int q_priority;
	HANDLE q_event;
	KMutex q_mutex;
	friend class KWaitEvent;
	friend class KWaitEventQueue;
};
