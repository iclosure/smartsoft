#pragma once

#include "mutex.h"
#include "thread.h"
#include <Windows.h>
#include <utility>	// use std::pair
#include <stack>
#include <vector>
#include <algorithm>
#include <assert.h>

#if defined(_DEBUG)
#include <stdio.h>
#endif  // _DEBUG

// 
// BaseThreadPrivate
// 

class BaseThreadPrivate : public KObject
{
public:
	explicit BaseThreadPrivate(KObject* parent);
	~BaseThreadPrivate();

	void setPriority(KBaseThread::Priority priority);

	KMutex mutex;

	bool running;
	bool finished;
	bool isInFinish; //when in BaseThreadPrivate::finish
	bool interruptionRequested;

	bool exited;
	int returnCode;

	unsigned int stackSize;
	KBaseThread::Priority priority;

	static KBaseThread *threadForId(int id);

	static unsigned int __stdcall start(void *arg);
	static void finish(void *arg, bool lockAnyway=true);

	HANDLE handle;
	unsigned int id;
	int waiters;
	bool terminationEnabled, terminatePending;

	friend class KBaseThread;
};

// 
// ThreadPrivate
// 

typedef bool (CALLBACK*_thread_routine)(void*);

class ThreadPrivate : public KBaseThread
{
public:
	explicit ThreadPrivate(tstring name, KObject *parent);
	~ThreadPrivate();

	bool start(Priority = NormalPriority);
	void stop(bool wait = true);

private:
	virtual void run();

	static bool __stdcall routine(void* param)
	{
		return static_cast<KThread*>(param)->work();
	}

private:
	HR_DISABLE_COPY(ThreadPrivate)

	volatile bool stoped;
	KMutex mutex;
	friend class ThreadWorker;
	friend class KThread;
};

// 
// SuperThreadPrivate
// 

class SuperThreadPrivate : public KBaseThread
{
public:
	explicit SuperThreadPrivate(tstring name, KObject* parent);
	~SuperThreadPrivate();

	bool start(Priority = InheritPriority);
	void stop(bool wait = true);

private:
	virtual void run();

	static bool __stdcall routine(void* param)
	{
		return static_cast<KSuperThread*>(param)->work();
	}

private:
	HR_DISABLE_COPY(SuperThreadPrivate)
	volatile bool stoped;
	KMutex mutex;
	KWaitEvent waitEventQuit;
	friend class KSuperThread;
};

// 
// KSingleThread
// 

class KSingleThread : public KBaseThread
{
public:
	explicit KSingleThread(_thread_routine routine, tstring name, KObject* parent);
	~KSingleThread();

	bool start(Priority = InheritPriority);
	void stop(bool wait = true);

	KMutex & mutex() { return q_mutex; }
	KWaitEvent & eventQuit() { return waitEventQuit; }

private:
	virtual void run();

private:
	HR_DISABLE_COPY(KSingleThread)
	volatile bool stoped;
	_thread_routine routine;
	KWaitEvent waitEventQuit;
	KMutex q_mutex;
	friend class KPairThread;
};

// 
// PairThreadPrivate
// 

class PairThreadPrivate
{
public:
	explicit PairThreadPrivate(tstring name, KObject* parent)
	{
		pair.first = new KSingleThread(routineFirst, name + _T(".first"), parent);
		pair.second = new KSingleThread(routineSecond, name + _T(".second"), parent);
	}

	virtual ~PairThreadPrivate()
	{
		delete pair.first;
		delete pair.second;
	}

	KSingleThread* first() const { return pair.first; }
	KSingleThread* second() const { return pair.second; }

private:
	static bool __stdcall routineFirst(void* param)
	{
		return static_cast<KPairThread*>(param)->workFirst();
	}

	static bool __stdcall routineSecond(void* param)
	{
		return static_cast<KPairThread*>(param)->workSecond();
	}

private:
	std::pair<KSingleThread*, KSingleThread*> pair;
	friend class KPairThread;
};
