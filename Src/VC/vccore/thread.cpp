#include "stdafx.h"
#include "thread.h"
#include "thread_p.h"
#include <process.h>
#include <assert.h>
#if defined(_DEBUG)
#include <stdio.h>
#endif  // _DEBUG

// 
// KBaseThread
// 

KBaseThread::KBaseThread(KObject* parent) :
	KObject(parent)
{
	setObjectName(_T("KBaseThread"));

	d = new BaseThreadPrivate(this);
}

KBaseThread::~KBaseThread(void)
{
	delete d;
}

void KBaseThread::setPriority(Priority priority)
{
	KMutexLocker locker(&d->mutex);
	if (!d->running)
	{
		return;
	}
	d->setPriority(priority);
}

KBaseThread::Priority KBaseThread::priority() const
{
	KMutexLocker locker(&d->mutex);
	return d->priority;
}

void KBaseThread::setStackSize(unsigned int stackSize)
{
	KMutexLocker locker(&d->mutex);
	assert(!d->running);
	d->stackSize = stackSize;
}

unsigned int KBaseThread::stackSize() const
{
	KMutexLocker locker(&d->mutex);
	return d->stackSize;
}

void KBaseThread::start(Priority priority)
{
	KMutexLocker locker(&d->mutex);

	if (d->isInFinish)
	{
		locker.unlock();
		wait();
		locker.relock();
	}

	if (d->running)
	{
		return;
	}

	d->running = true;
	d->finished = false;
	d->exited = false;
	d->returnCode = 0;
	d->interruptionRequested = false;

	/*
	NOTE: we create the thread in the suspended state, set the
	priority and then resume the thread.

	since threads are created with normal priority by default, we
	could get into a case where a thread (with priority less than
	NormalPriority) tries to create a new thread (also with priority
	less than NormalPriority), but the newly created thread preempts
	its 'parent' and runs at normal priority.
	*/
	d->handle = (HANDLE)_beginthreadex(NULL, d->stackSize, BaseThreadPrivate::start,
		this, CREATE_SUSPENDED, &(d->id));

	if (!d->handle)
	{
		d->running = false;
		d->finished = true;
		return;
	}

	int prio;
	d->priority = priority;
	switch (d->priority)
	{
	case IdlePriority:
		prio = THREAD_PRIORITY_IDLE;
		break;

	case LowestPriority:
		prio = THREAD_PRIORITY_LOWEST;
		break;

	case LowPriority:
		prio = THREAD_PRIORITY_BELOW_NORMAL;
		break;

	case NormalPriority:
		prio = THREAD_PRIORITY_NORMAL;
		break;

	case HighPriority:
		prio = THREAD_PRIORITY_ABOVE_NORMAL;
		break;

	case HighestPriority:
		prio = THREAD_PRIORITY_HIGHEST;
		break;

	case InheritPriority:
	default:
		prio = GetThreadPriority(GetCurrentThread());
		break;
	}

	if (!SetThreadPriority(d->handle, prio))
	{
		_thrtrace((_T("KBaseThread::start() -> Failed to set thread priority\n")));
	}

	if (ResumeThread(d->handle) == (DWORD)-1)
	{
		_thrtrace((_T("KBaseThread::start() -> Failed to resume new thread\n")));
	}
}

void KBaseThread::terminate()
{
	KMutexLocker locker(&d->mutex);
	if (!d->running)
	{
		return;
	}
	if (!d->terminationEnabled)
	{
		d->terminationEnabled = true;
		return;
	}
	TerminateThread(d->handle, 0);
	BaseThreadPrivate::finish(this, false);
}

void KBaseThread::quit()
{
	exit();
}

void KBaseThread::exit(int returnCode)
{
	KMutexLocker locker(&d->mutex);
	d->exited = true;
	d->returnCode = returnCode;
}

void KBaseThread::run()
{
	(void)exec();
}

int KBaseThread::exec()
{
	KMutexLocker locker(&d->mutex);

	if (d->exited)
	{
		d->exited = false;
		return d->returnCode;
	}

	locker.unlock();

	//...

	locker.relock();

	d->exited = false;
	d->returnCode = -1;
	return d->returnCode;
}

void KBaseThread::requestInterruption()
{
	KMutexLocker locker(&d->mutex);
	if (!d->running || d->finished || d->isInFinish)
	{
		return;
	}
	d->interruptionRequested = true;
}

bool KBaseThread::isInterruptionRequested() const
{
	KMutexLocker locker(&d->mutex);
	if (!d->running || d->finished || d->isInFinish)
	{
		return false;
	}
	return d->interruptionRequested;
}

bool KBaseThread::isFinished() const
{
	KMutexLocker locker(&d->mutex);
	return d->finished && d->isInFinish;
}

bool KBaseThread::isRunning() const
{
	KMutexLocker locker(&d->mutex);
	return d->running && !d->isInFinish;
}

bool KBaseThread::wait(unsigned long time)
{
	KMutexLocker locker(&d->mutex);

	if (d->id == GetCurrentThreadId())
	{
		return false;
	}
	if (d->finished || !d->running)
	{
		return true;
	}

	++d->waiters;
	locker.mutex()->unlock();

	bool ret = false;
	switch (WaitForSingleObject(d->handle, time))
	{
	case WAIT_OBJECT_0:
		ret = true;
		break;

	case WAIT_FAILED:
		break;

	case WAIT_ABANDONED:
	case WAIT_TIMEOUT:
	default:
		break;
	}

	locker.mutex()->lock();
	--d->waiters;

	if (ret && !d->finished)
	{
		BaseThreadPrivate::finish(this, false);
	}
	if (d->finished && !d->waiters)
	{
		::CloseHandle(d->handle);
		d->handle = 0;
	}

	return ret;
}

void KBaseThread::sleep(unsigned long secs)
{
	::Sleep(secs * 1000);
}

void KBaseThread::msleep(unsigned long msecs)
{
	::Sleep(msecs);
}

void KBaseThread::usleep(unsigned long usecs)
{
	::Sleep((usecs / 1000) + 1);
}

// 
// KThread
// 

KThread::KThread(KObject *parent) :
	KObject(parent)
{
	setObjectName(_T("KThread"));

	d = new ThreadPrivate(objectName(), this);
}

KThread::KThread(tstring name, KObject *parent) :
	KObject(parent)
{
	setObjectName(_T("KThread"));

	d = new ThreadPrivate(name.empty() ? objectName() : name, this);
}

KThread::~KThread(void)
{
	delete d;
}

bool KThread::start(KBaseThread::Priority priority)
{
	return d->start(priority);
}

void KThread::stop(bool wait)
{
	d->stop(wait);
}

bool KThread::isRunning() const
{
	return d->isRunning();
}

KMutex * KThread::mutex()
{
	return &d->mutex;
}

// 
// KSuperThread
// 

KSuperThread::KSuperThread(KObject *parent) :
KObject(parent)
{
	setObjectName(_T("KSuperThread"));

	d = new SuperThreadPrivate(objectName(), this);
}

KSuperThread::KSuperThread(tstring name, KObject *parent) :
KObject(parent)
{
	setObjectName(_T("KSuperThread"));

	d = new SuperThreadPrivate(name.empty() ? objectName() : name, this);
}

KSuperThread::~KSuperThread()
{
	delete d;
}

bool KSuperThread::start(KBaseThread::Priority priority)
{
	return d->start(priority);
}

void KSuperThread::stop(bool wait)
{
	d->stop(wait);
}

bool KSuperThread::isRunning() const
{
	return d->isRunning();
}

KWaitEvent::WaitResult KSuperThread::waitObject(KWaitEvent *event, unsigned long msecs)
{
	if (!event)
	{
		return KWaitEvent::WaitFailed;
	}

	KWaitEventQueue waitEventQueue;

	waitEventQueue.add(&d->waitEventQuit);
	waitEventQueue.add(event);

	unsigned long waitResult = waitEventQueue.waitQueue(KWaitEventQueue::WaitAny, msecs);
	switch (waitResult)
	{
	case KWaitEvent::WaitObject0: waitResult = KWaitEvent::WaitInternal; break;
	case KWaitEvent::WaitObject0 + 1: waitResult = KWaitEvent::WaitObject0; break;
	default: break;
	}

	return (KWaitEvent::WaitResult)waitResult;
}

KWaitEvent::WaitResult KSuperThread::waitObject(KWaitEventQueue *eventQueue, KWaitEventQueue::WaitMode mode, unsigned long msecs)
{
	if (!eventQueue || eventQueue->empty())
	{
		return KWaitEvent::WaitFailed;
	}

	KWaitEventQueue waitEventQueue;

	waitEventQueue.add(&d->waitEventQuit);

	for (std::list<KWaitEvent*>::iterator iter = eventQueue->begin();
		iter != eventQueue->end(); iter++)
	{
		waitEventQueue.add(*iter);
	}

	unsigned long waitResult = waitEventQueue.waitQueue(KWaitEventQueue::WaitAny, msecs);
	if (waitResult == KWaitEvent::WaitObject0)
	{
		waitResult = KWaitEvent::WaitInternal;
	}
	else if ((mode == KWaitEventQueue::WaitAll) && (KWaitEvent::WaitObject0 < waitResult) &&
		waitResult < (unsigned long)(KWaitEvent::WaitObject0 + waitEventQueue.size()))
	{
		waitResult = eventQueue->waitQueue(mode, msecs);
	}

	return (KWaitEvent::WaitResult)waitResult;
}

// 
// KPairThread
// 

KPairThread::KPairThread(KObject *parent) :
	KObject(parent)
{
	setObjectName(_T("KPairThread"));

	d = new PairThreadPrivate(objectName(), this);
}

KPairThread::KPairThread(tstring name, KObject *parent) :
	KObject(parent)
{
	setObjectName(_T("KPairThread"));

	d = new PairThreadPrivate(name.empty() ? objectName() : name, this);
}

KPairThread::~KPairThread()
{
	delete d;
}

bool KPairThread::start(PairType type, KBaseThread::Priority priority)
{
	switch (type)
	{
	case PairFirst:
	{
		return d->first()->start(priority);
	}
	case PairSecond:
	{
		return d->second()->start(priority);
	}
	case PairAll:
	{
		if (d->first()->start(priority))
		{
			return d->second()->start(priority);
		}
		break;
	}
	default:
		assert(false);
		break;
	}

	return false;
}

void KPairThread::stop(PairType type, bool wait)
{
	switch (type)
	{
	case PairFirst:
	{
		d->first()->stop(wait);
		if (wait)
		{
			d->first()->wait();
		}
		break;
	}
	case PairSecond:
	{
		d->second()->stop(wait);
		if (wait)
		{
			d->second()->wait();
		}
		break;
	}
	case PairAll:
	{
		d->first()->stop(wait);
		d->second()->stop(wait);
		if (wait)
		{
			d->first()->wait();
			d->second()->wait();
		}
		break;
	}
	default:
		assert(false);
		break;
	}
}

bool KPairThread::isRunning(PairType type) const
{
	switch (type)
	{
	case PairFirst: return d->first()->isRunning();
	case PairSecond: return d->second()->isRunning();
	case PairAll: return d->first()->isRunning() && d->second()->isRunning();
	default:
		assert(false);
		break;
	}
	return false;
}

KPairThread::WaitResult KPairThread::waitObject(PairType type, KWaitEvent *event, unsigned long msecs)
{
	if (!event)
	{
		return WaitFailed;
	}

	KWaitEventQueue waitEventQueue;
	unsigned long waitResult = WaitFailed;

	switch (type)
	{
	case PairFirst:
	{
		waitEventQueue.add(&d->first()->waitEventQuit);
		waitEventQueue.add(event);

		waitResult = waitEventQueue.waitQueue(KWaitEventQueue::WaitAny, msecs);
		switch (waitResult)
		{
		case KWaitEvent::WaitObject0: waitResult = WaitObjectFirst; break;
		case KWaitEvent::WaitObject0 + 1: waitResult = WaitObject0; break;
		default: break;
		}
		break;
	}
	case PairSecond:
	{
		waitEventQueue.add(&d->second()->waitEventQuit);
		waitEventQueue.add(event);

		waitResult = waitEventQueue.waitQueue(KWaitEventQueue::WaitAny, msecs);
		switch (waitResult)
		{
		case KWaitEvent::WaitObject0: waitResult = WaitObjectSecond; break;
		case KWaitEvent::WaitObject0 + 1: waitResult = WaitObject0; break;
		default: break;
		}
		break;
	}
	case PairAll:
	{
		waitEventQueue.add(&d->first()->waitEventQuit);
		waitEventQueue.add(&d->second()->waitEventQuit);
		waitEventQueue.add(event);

		waitResult = waitEventQueue.waitQueue(KWaitEventQueue::WaitAny, msecs);
		switch (waitResult)
		{
		case KWaitEvent::WaitObject0: waitResult = WaitObjectFirst; break;
		case KWaitEvent::WaitObject0 + 1: waitResult = WaitObjectSecond; break;
		case KWaitEvent::WaitObject0 + 2: waitResult = WaitObject0; break;
		default: break;
		}
		break;
	}
	default:
		assert(false);
		break;
	}

	return (WaitResult)waitResult;
}

KPairThread::WaitResult KPairThread::waitObject(PairType type, KWaitEventQueue * eventQueue,
	KWaitEventQueue::WaitMode mode,unsigned long msecs)
{
	if (!eventQueue || eventQueue->empty())
	{
		return WaitFailed;
	}

	KWaitEventQueue waitEventQueue;
	unsigned long waitResult = WaitFailed;

	switch (type)
	{
	case PairFirst:
	{
		waitEventQueue.add(&d->first()->waitEventQuit);
		for (std::list<KWaitEvent*>::iterator iter = eventQueue->begin();
			iter != eventQueue->end(); iter++)
		{
			waitEventQueue.add(*iter);
		}

		waitResult = waitEventQueue.waitQueue(KWaitEventQueue::WaitAny, msecs);
		if (waitResult == KWaitEvent::WaitObject0)
		{
			waitResult = WaitObjectFirst;
		}
		else if ((mode == KWaitEventQueue::WaitAll) && (KWaitEvent::WaitObject0 < waitResult) &&
			waitResult < (unsigned long)(KWaitEvent::WaitObject0 + waitEventQueue.size()))
		{
			waitResult = eventQueue->waitQueue(mode, msecs);
		}
		break;
	}
	case PairSecond:
	{
		waitEventQueue.add(&d->second()->waitEventQuit);
		for (std::list<KWaitEvent*>::iterator iter = eventQueue->begin();
			iter != eventQueue->end(); iter++)
		{
			waitEventQueue.add(*iter);
		}

		waitResult = waitEventQueue.waitQueue(KWaitEventQueue::WaitAny, msecs);
		if (waitResult == KWaitEvent::WaitObject0)
		{
			waitResult = WaitObjectSecond;
		}
		else if ((mode == KWaitEventQueue::WaitAll) && (KWaitEvent::WaitObject0 < waitResult) &&
			waitResult < (unsigned long)(KWaitEvent::WaitObject0 + waitEventQueue.size()))
		{
			waitResult = eventQueue->waitQueue(mode, msecs);
		}
		break;
	}
	case PairAll:
	{
		waitEventQueue.add(&d->first()->waitEventQuit);
		waitEventQueue.add(&d->second()->waitEventQuit);
		for (std::list<KWaitEvent*>::iterator iter = eventQueue->begin();
			iter != eventQueue->end(); iter++)
		{
			waitEventQueue.add(*iter);
		}

		waitResult = waitEventQueue.waitQueue(KWaitEventQueue::WaitAny, msecs);
		if (waitResult == KWaitEvent::WaitObject0)
		{
			waitResult = WaitObjectFirst;
		}
		else if (waitResult == KWaitEvent::WaitObject0 + 1)
		{
			waitResult = WaitObjectSecond;
		}
		else if ((mode == KWaitEventQueue::WaitAll) && (KWaitEvent::WaitObject0 + 1 < waitResult) &&
			waitResult < (unsigned long)(KWaitEvent::WaitObject0 + 1 + waitEventQueue.size()))
		{
			waitResult = eventQueue->waitQueue(mode, msecs);
		}
		break;
	}
	default:
		assert(false);
		break;
	}

	return (WaitResult)waitResult;
}
