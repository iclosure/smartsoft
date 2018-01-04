#ifndef HR_MUTEX_H
#define HR_MUTEX_H

#include "vccore_global.h"

// 
// KMutex
// 

class MutexPrivate;

class VCCORE_EXPORT KMutex
{
public:
	explicit KMutex(void);
	virtual ~KMutex(void);

	void lock();
	void unlock();

private:
	HR_DISABLE_COPY(KMutex)
	MutexPrivate* d;
};

// 
// KMutexLocker
// 

class VCCORE_EXPORT KMutexLocker
{
public:
	inline explicit KMutexLocker(KMutex* m) : q_mutex(m)
	{
		q_mutex->lock();
	}
	inline ~KMutexLocker(void) { q_mutex->unlock(); }

	inline void unlock()
	{
		q_mutex->unlock();
	}

	inline void relock()
	{
		q_mutex->lock();
	}

	inline KMutex *mutex()
	{
		return q_mutex;
	}

private:
	HR_DISABLE_COPY(KMutexLocker)
	KMutex* q_mutex;
};

#endif  // HR_MUTEX_H
