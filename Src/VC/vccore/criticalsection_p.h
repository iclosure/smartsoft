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

	unsigned int     m_lockCnt;  //���ڽ���lock�Ĵ����������������ϲ����ܴ���1

#ifdef WIN32
	CRITICAL_SECTION m_critical; //���ɶ����б�����
#elif  QNX
	pthread_mutex_t  m_critical; //���ɶ����б����� 
#elif  VXWORKS
	SEM_ID           m_critical; //vxworks�µ�һλ�ź���    
#endif
};
