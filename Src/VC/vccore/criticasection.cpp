#include "stdafx.h"
#include "criticasection.h"
#include "criticalsection_p.h"

// 
// KCriticalSection
// 

KCriticalSection::KCriticalSection(void)
{
	d = new CriticalSectionPrivate();
}

KCriticalSection::~KCriticalSection(void)
{
	delete d;
}

void KCriticalSection::acquire()
{
#ifdef WIN32
	EnterCriticalSection(&d->m_critical);
#elif  QNX
	pthread_mutex_lock( &d->m_critical );
#elif  VXWORKS
	semTake (d->m_critical, WAIT_FOREVER);
#endif
	++d->m_lockCnt;
#if defined(_DEBUG)
	if (d->m_lockCnt > 1)
	{
		printf("The lock object re entry!\n");
	}
#endif // _DEBUG
}

void KCriticalSection::release()
{
	--d->m_lockCnt;
#ifdef WIN32
	LeaveCriticalSection(&d->m_critical);
#elif  QNX
	pthread_mutex_unlock( &d->m_critical );
#elif  VXWORKS
	semGive (d->m_critical);
#endif
}

// 
// KCriticalLocker
// 

KCriticalLocker::KCriticalLocker(KCriticalSection * cs) :
	d(cs)
{
	d->acquire();
}

KCriticalLocker::~KCriticalLocker(void)
{
	d->release();
}
