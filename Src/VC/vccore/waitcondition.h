#ifndef HR_WAIT_CONDITION_H
#define HR_WAIT_CONDITION_H

#include "vccore_global.h"
#include "kobject.h"
#include <list>

class WaitEventPrivate;

class VCCORE_EXPORT KWaitEvent : public KObject
{
public:
	enum WaitResult {
		WaitInternal = -4,
		WaitInvalid = -3,
		WaitTimeout = -2,
		WaitFailed = -1,
		WaitObject0 = 0,
	};

public:
	explicit KWaitEvent(tstring name = tstring(), KObject* parent = 0);
	virtual ~KWaitEvent(void);

	bool setEvent();
	bool resetEvent();
	WaitResult wait(unsigned long msecs = ULONG_MAX);

	HANDLE &handle() const;
	operator HANDLE() const;
	operator PHANDLE() const;

private:
	HR_DISABLE_COPY(KWaitEvent)

	WaitEventPrivate* d;
	friend class KWaitEventQueue;
};

#pragma warning(push)
#pragma warning(disable:4251)
class VCCORE_EXPORT KWaitEventQueue : public std::list<KWaitEvent*>
{
public:
	enum WaitMode {
		WaitAny,
		WaitAll
	};

public:
	explicit KWaitEventQueue();
	virtual ~KWaitEventQueue(void);

	bool add(KWaitEvent * event);
	KWaitEvent::WaitResult waitQueue(WaitMode mode = WaitAll, unsigned long msecs = ULONG_MAX);

private:
	HR_DISABLE_COPY(KWaitEventQueue)
};
#pragma warning(pop)

#endif  // HR_WAIT_CONDITION_H
