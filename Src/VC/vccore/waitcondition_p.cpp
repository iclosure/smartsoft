#include "stdafx.h"
#include "waitcondition_p.h"
#include "waitcondition.h"
#include <stdio.h>

WaitEventPrivate::WaitEventPrivate(tstring name) :
	q_name(name + KUuid::guidBySting())
{
	q_priority = GetThreadPriority(GetCurrentThread());

	q_event = ::CreateEvent(NULL, TRUE, FALSE, q_name.c_str());
}

WaitEventPrivate::~WaitEventPrivate()
{
	::CloseHandle(q_event);
}

bool WaitEventPrivate::setEvent()
{
	_thrtrace((_T("set event [%s]\n"), q_name.c_str()));

	KMutexLocker locker(&q_mutex);
	return ::SetEvent(q_event) ? true : false;
}

bool WaitEventPrivate::resetEvent()
{
	_thrtrace((_T("reset event [%s]\n"), q_name.c_str()));

	KMutexLocker locker(&q_mutex);
	return ::ResetEvent(q_event) ? true : false;
}
