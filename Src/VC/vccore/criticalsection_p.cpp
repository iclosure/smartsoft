#include "stdafx.h"
#include "criticalsection_p.h"

// 
// CriticalSectionPrivate
// 

CriticalSectionPrivate::CriticalSectionPrivate() :
	m_lockCnt(0)
{
	//初始化
#ifdef WIN32
	InitializeCriticalSection(&m_critical);   
#elif  QNX
	//m_critical = PTHREAD_MUTEX_INITIALIZER;
	//m_critical.count = _NTO_SYNC_NONRECURSIVE;
	//m_critical.owner = _NTO_SYNC_INITIALIZER;
	pthread_mutex_init(&m_critical, NULL);
#elif  VXWORKS
	m_critical = semBCreate (SEM_Q_PRIORITY, SEM_FULL);
#endif
}

CriticalSectionPrivate::~CriticalSectionPrivate()
{
	//删除互斥对象
#ifdef WIN32
	DeleteCriticalSection(&m_critical);
#elif  QNX
	pthread_mutex_destroy(&m_critical);
#elif  VXWORKS
	semDelete(m_critical);
#endif
}
