#include "stdafx.h"
#include "waitcondition.h"
#include "waitcondition_p.h"
#include <vector>

// 
// KWaitEvent
// 

KWaitEvent::KWaitEvent(tstring name, KObject* parent) :
	KObject(parent)
{
	setObjectName(name.empty() ? _T("KWaitEvent") : name);
	d = new WaitEventPrivate(objectName());
}

KWaitEvent::~KWaitEvent()
{
	delete d;
}

bool KWaitEvent::setEvent()
{
	return d->setEvent();
}

bool KWaitEvent::resetEvent()
{
	return d->resetEvent();
}

KWaitEvent::WaitResult KWaitEvent::wait(unsigned long msecs)
{
	unsigned long waitResult = ::WaitForSingleObject(d->q_event, msecs);
	switch (waitResult) {
	case WAIT_OBJECT_0: return KWaitEvent::WaitObject0;
	case WAIT_TIMEOUT: return KWaitEvent::WaitTimeout;
	case WAIT_FAILED: return KWaitEvent::WaitFailed;
	default: return KWaitEvent::WaitInvalid;
	}
}

HANDLE& KWaitEvent::handle() const
{
	return d->q_event;
}

KWaitEvent::operator HANDLE() const
{
	return d->q_event;
}

KWaitEvent::operator PHANDLE() const
{
	return &d->q_event;
}

// 
// KWaitEventQueue
// 

KWaitEventQueue::KWaitEventQueue()
{
}

KWaitEventQueue::~KWaitEventQueue()
{
}

bool KWaitEventQueue::add(KWaitEvent * event)
{
	// insert 'event' into the queue (sorted by priority)
	iterator iter;
	for (iter = begin(); iter != end(); iter++) {
		if ((*iter)->d->q_priority < event->d->q_priority) {
			break;
		}
	}

	insert(iter, event);

	return true;
}

KWaitEvent::WaitResult KWaitEventQueue::waitQueue(WaitMode mode, unsigned long msecs)
{
	std::vector<HANDLE> handleQueue;
	for (iterator iter = begin(); iter != end(); iter++) {
		handleQueue.push_back((*iter)->d->q_event);
	}

	unsigned long waitResult = ::WaitForMultipleObjects(handleQueue.size(),
		&handleQueue[0], (mode == KWaitEventQueue::WaitAll) ? TRUE : FALSE, msecs);
	if (WAIT_OBJECT_0 > waitResult || waitResult >= (unsigned long)(WAIT_OBJECT_0 + handleQueue.size())) {
		switch (waitResult) {
		case WAIT_TIMEOUT: waitResult = KWaitEvent::WaitTimeout; break;
		case WAIT_FAILED: waitResult = KWaitEvent::WaitFailed; break;
		default: waitResult = KWaitEvent::WaitInvalid; break;
		}
	}

	return (KWaitEvent::WaitResult)waitResult;
}
