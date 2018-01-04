#pragma once

#ifdef WIN32
#include <Windows.h>
#elif  QNX
#include <pthread.h>
#elif  VXWORKS
#include <vxWorks.h>
#include <semLib.h>
#endif

#if defined(_DEBUG)
#include <stdio.h>
#endif // _DEBUG

#include "vccore_global.h"

/////////////////////////////////////////////////////////
/// CriticalSectionPrivate

class CriticalSectionPrivate
{
	friend class KCriticalSection;
public:
	explicit CriticalSectionPrivate();
	virtual ~CriticalSectionPrivate();

private:
	HR_DISABLE_COPY(CriticalSectionPrivate)

	unsigned int     m_lockCnt;  //现在进入lock的次数计数器，理论上不可能大于1

#ifdef WIN32
	CRITICAL_SECTION m_critical; //分派对象列表互斥区
#elif  QNX
	pthread_mutex_t  m_critical; //分派对象列表互斥区 
#elif  VXWORKS
	SEM_ID           m_critical; //vxworks下的一位信号量    
#endif
};
