#ifndef HR_THREAD_H
#define HR_THREAD_H

#include "vccore_global.h"
#include "kobject.h"
#include "waitcondition.h"
#include <windows.h>

// 
// KBaseThread
// 

class BaseThreadPrivate;

class VCCORE_EXPORT KBaseThread : public KObject
{
public:
	explicit KBaseThread(KObject *parent = 0);
	~KBaseThread();

	enum Priority {
		IdlePriority,

		LowestPriority,
		LowPriority,
		NormalPriority,
		HighPriority,
		HighestPriority,

		TimeCriticalPriority,

		InheritPriority
	};

	void setPriority(Priority priority);
	Priority priority() const;
	void setStackSize(unsigned int stackSize);
	unsigned int stackSize() const;

	void start(Priority = InheritPriority);
	void terminate();
	void quit();
	void exit(int returnCode = 0);

	bool isFinished() const;
	bool isRunning() const;
	void requestInterruption();
	bool isInterruptionRequested() const;

	// default argument causes thread to block indefinitely
	bool wait(unsigned long time = ULONG_MAX);

	static void sleep(unsigned long secs);
	static void msleep(unsigned long msecs);
	static void usleep(unsigned long usecs);

protected:
	virtual void run();
	int exec();

private:
	HR_DISABLE_COPY(KBaseThread)
	BaseThreadPrivate* d;
	friend class BaseThreadPrivate;
	friend class ThreadPrivate;
	friend class SuperThreadPrivate;
	friend class KSingleThread;
};

// 
// KThread
// 

class KMutex;
class ThreadPrivate;

class VCCORE_EXPORT KThread : public KObject
{
public:
	explicit KThread(KObject *parent = 0);
	explicit KThread(tstring name, KObject *parent = 0);
	~KThread();

	bool start(KBaseThread::Priority = KBaseThread::InheritPriority);
	void stop(bool wait = true);
	bool isRunning() const;

	virtual bool work(void) = 0;

	KMutex * mutex();

private:
	HR_DISABLE_COPY(KThread)
	ThreadPrivate* d;
};

// 
// KSuperThread
// 

class SuperThreadPrivate;

class VCCORE_EXPORT KSuperThread : public KObject
{
public:
	explicit KSuperThread(KObject *parent = 0);
	explicit KSuperThread(tstring name, KObject *parent = 0);
	~KSuperThread();

	bool start(KBaseThread::Priority = KBaseThread::InheritPriority);
	void stop(bool wait = true);
	bool isRunning() const;
	KWaitEvent::WaitResult waitObject(KWaitEvent *, unsigned long msecs = ~0UL);
	KWaitEvent::WaitResult waitObject(KWaitEventQueue *, KWaitEventQueue::WaitMode mode = KWaitEventQueue::WaitAll,
		unsigned long msecs = ~0UL);

	virtual bool work(void) = 0;

private:
	HR_DISABLE_COPY(KSuperThread)
	SuperThreadPrivate* d;
};

// 
// KPairThread
// 

class PairThreadPrivate;

class VCCORE_EXPORT KPairThread : public KObject
{
public:
	enum WaitResult {
		WaitObjectFirst = KWaitEvent::WaitInvalid - 2,
		WaitObjectSecond = KWaitEvent::WaitInvalid - 1,
		WaitInvalid = KWaitEvent::WaitInvalid,
		WaitTimeout = KWaitEvent::WaitTimeout,
		WaitFailed = KWaitEvent::WaitFailed,
		WaitObject0 = KWaitEvent::WaitObject0,
	};
	enum PairType {
		PairFirst,
		PairSecond,
		PairAll
	};

public:
	explicit KPairThread(KObject *parent = 0);
	explicit KPairThread(tstring name, KObject *parent = 0);
	virtual ~KPairThread();

	bool start(PairType = PairAll, KBaseThread::Priority = KBaseThread::InheritPriority);
	void stop(PairType = PairAll, bool wait = true);
	bool isRunning(PairType) const;
	WaitResult waitObject(PairType, KWaitEvent *, unsigned long msecs = ~0UL);
	WaitResult waitObject(PairType, KWaitEventQueue *, KWaitEventQueue::WaitMode mode = KWaitEventQueue::WaitAll,
		unsigned long msecs = ~0UL);

	virtual bool workFirst() = 0;
	virtual bool workSecond() = 0;

private:
	HR_DISABLE_COPY(KPairThread)
	PairThreadPrivate* d;
};

#endif  // HR_THREAD_H
