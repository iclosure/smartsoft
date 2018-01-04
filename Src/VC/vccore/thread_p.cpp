#include "stdafx.h"
#include "thread_p.h"
#include <assert.h>

#if defined(_DEBUG)
#include <stdio.h>
#endif  // _DEBUG

// 
// BaseThreadPrivate
// 

BaseThreadPrivate::BaseThreadPrivate(KObject *parent) :
	KObject(parent),running(false), finished(false),
	isInFinish(false), interruptionRequested(false),
	exited(false), returnCode(-1),
	stackSize(0), priority(KBaseThread::InheritPriority)
{
	handle = 0;
	id = 0;
	waiters = 0;
	terminationEnabled = true;
	terminatePending = false;
}

BaseThreadPrivate::~BaseThreadPrivate()
{
}

void BaseThreadPrivate::setPriority(KBaseThread::Priority priority)
{
	// copied from start() with a few modifications:
	
	int prio;
	this->priority = priority;
	switch (priority)
	{
	case KBaseThread::IdlePriority:
		prio = THREAD_PRIORITY_IDLE;
		break;

	case KBaseThread::LowestPriority:
		prio = THREAD_PRIORITY_LOWEST;
		break;

	case KBaseThread::LowPriority:
		prio = THREAD_PRIORITY_NORMAL;
		break;

	case KBaseThread::HighPriority:
		prio = THREAD_PRIORITY_ABOVE_NORMAL;
		break;

	case KBaseThread::HighestPriority:
		prio = THREAD_PRIORITY_HIGHEST;
		break;

	case KBaseThread::TimeCriticalPriority:
		prio = THREAD_PRIORITY_TIME_CRITICAL;
		break;

	case KBaseThread::InheritPriority:
	default:
		//assert(false);
		return;
	}

	if (!SetThreadPriority(handle, prio))
	{
		assert(false);
	}
}

unsigned int __stdcall BaseThreadPrivate::start(void *arg)
{
	KBaseThread* thr = reinterpret_cast<KBaseThread*>(arg);
	thr->run();
	return 0;
}

void BaseThreadPrivate::finish(void *arg, bool lockAnyway)
{
	KBaseThread* thr = reinterpret_cast<KBaseThread*>(arg);
	BaseThreadPrivate *d = thr->d;

	//KMutexLocker locker(lockAnyway ? &d->mutex : 0);
	d->isInFinish = true;
	d->priority = KBaseThread::InheritPriority;
	
	d->running = false;
	d->finished = true;
	d->isInFinish = false;
	d->interruptionRequested = false;

	if (!d->waiters)
	{
		::CloseHandle(d->handle);
		d->handle = 0;
	}

	d->id = 0;
}

// 
// ThreadPrivate
// 

ThreadPrivate::ThreadPrivate(tstring name, KObject *parent) :
	KBaseThread(parent), stoped(true)
{
	setObjectName(name + KUuid::guidBySting());
}

ThreadPrivate::~ThreadPrivate()
{
	this->stop();
}

bool ThreadPrivate::start(KBaseThread::Priority priority)
{
	{
		KMutexLocker locker(&mutex);

		if (KBaseThread::isRunning())
		{
			_thrtrace((_T("thread [%s] is running\n"), objectName().c_str()));
			return true;
		}

		stoped = false;
	}

	KBaseThread::start(priority);

	return true;
}

void ThreadPrivate::stop(bool wait)
{
	{
		KMutexLocker locker(&mutex);

		if (!KBaseThread::isRunning())
		{
			return;
		}

		stoped = true;
	}

	if (wait)
	{
		KBaseThread::wait();
	}
	else
	{
		KBaseThread::terminate();
	}
}

inline void ThreadPrivate::run()
{
	_thrtrace((_T("start thread [%s]\n"), objectName().c_str()));

	while(1)
	{
		{
			KMutexLocker locker(&mutex);
			if (stoped)
			{
				break;
			}
		}

		if (!routine(KBaseThread::parent()))
		{
			KMutexLocker locker(&d->mutex);
			d->running = false;
			break;
		}
	}

	_thrtrace((_T("stop thread [%s]\n"), objectName().c_str()));
}

// 
// KSuperThreadPrivate
//

SuperThreadPrivate::SuperThreadPrivate(tstring name, KObject *parent) :
	KBaseThread(parent), stoped(false)
{
	setObjectName(name + KUuid::guidBySting());
}

SuperThreadPrivate::~SuperThreadPrivate()
{
	this->stop();
}

bool SuperThreadPrivate::start(Priority priority)
{
	{
		KMutexLocker locker(&mutex);

		if (KBaseThread::isRunning())
		{
			_thrtrace((_T("thread [%s] is running\n"), objectName().c_str()));
			return true;
		}

		stoped = false;
	}

	waitEventQuit.resetEvent();

	KBaseThread::start(priority);

	return true;
}

void SuperThreadPrivate::stop(bool wait)
{
	{
		KMutexLocker locker(&mutex);

		if (!KBaseThread::isRunning())
		{
			return;
		}

		stoped = true;
	}

	waitEventQuit.setEvent();

	if (wait)
	{
		KBaseThread::wait();
	}
	else
	{
		KBaseThread::terminate();
	}
}

inline void SuperThreadPrivate::run()
{
	_thrtrace((_T("start thread [%s]\n"), objectName().c_str()));

	while(1) {
		{
			KMutexLocker locker(&mutex);
			if (stoped)
			{
				waitEventQuit.setEvent();
				break;
			}
		}

		if (!routine(parent()))
		{
			waitEventQuit.resetEvent();
			KMutexLocker locker(&d->mutex);
			d->running = false;
			break;
		}
	}

	_thrtrace((_T("stop thread [%s]\n"), objectName().c_str()));
}

// 
// KSingleThread
//  

KSingleThread::KSingleThread(_thread_routine routine, tstring name, KObject *parent) :
	KBaseThread(parent), stoped(true), routine(routine)
{
	setObjectName(name + KUuid::guidBySting());
}

KSingleThread::~KSingleThread()
{
	this->stop();
}

bool KSingleThread::start(Priority priority)
{
	{
		KMutexLocker locker(&q_mutex);

		if (KBaseThread::isRunning())
		{
			_thrtrace((_T("thread [%s] is running\n"), objectName().c_str()));
			return true;
		}

		stoped = false;
	}

	waitEventQuit.resetEvent();

	KBaseThread::start(priority);

	return true;
}

void KSingleThread::stop(bool wait)
{
	{
		KMutexLocker locker(&q_mutex);

		if (!KBaseThread::isRunning())
		{
			return;
		}

		stoped = true;
	}

	waitEventQuit.setEvent();

	if (!wait)
	{
		KBaseThread::terminate();
	}
}

inline void KSingleThread::run()
{
	_thrtrace((_T("start thread [%s]\n"), objectName().c_str()));
	
	while(1) 
	{
		{
			KMutexLocker locker(&q_mutex);
			if (stoped)
			{
				waitEventQuit.setEvent();
				break;
			}
		}

		if (!routine(parent()))
		{
			waitEventQuit.resetEvent();
			KMutexLocker locker(&d->mutex);
			d->running = false;
			break;
		}
	}

	_thrtrace((_T("stop thread [%s]\n"), objectName().c_str()));
}
