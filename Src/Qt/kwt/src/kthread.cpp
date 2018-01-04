#include "precomp.h"
#include "kthread.h"
#include "kthread_p.h"

//
// KThread
//

KThread::KThread(QObject *parent) :
    QObject(parent)
{
    setObjectName("KSuperThread");

    d = new ThreadPrivate(objectName(), this);
}

KThread::KThread(QString name, QObject *parent) :
    QObject(parent)
{
    setObjectName("KSuperThread");

    d = new ThreadPrivate(name.isEmpty() ? objectName() : name, this);
}

KThread::~KThread()
{
    delete d;
}

bool KThread::start(QThread::Priority priority)
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

QMutex * KThread::mutex()
{
    return &d->q_mutex;
}

//
// KKSuperThread
//

KSuperThread::KSuperThread(QObject *parent) :
    QObject(parent)
{
    setObjectName("KSuperThread");

    d = new SuperThreadPrivate(objectName(), this);
}

KSuperThread::KSuperThread(QString name, QObject *parent) :
    QObject(parent)
{
    setObjectName("KSuperThread");

    d = new SuperThreadPrivate(name.isEmpty() ? objectName() : name, this);
}

KSuperThread::~KSuperThread()
{
    delete d;
}

bool KSuperThread::start(QThread::Priority priority)
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
    if (!event) {
        return KWaitEvent::WaitFailed;
    }

    KWaitEventQueue waitEventQueue;

    waitEventQueue.add(&d->q_waitEventQuit);
    waitEventQueue.add(event);

    quint32 waitResult = waitEventQueue.waitQueue(KWaitEventQueue::WaitAny, msecs);
    switch(waitResult) {
    case KWaitEvent::WaitObject0: waitResult = KWaitEvent::WaitInternal; break;
    case KWaitEvent::WaitObject0 + 1: waitResult = KWaitEvent::WaitObject0; break;
    default: break;
    }

    return (KWaitEvent::WaitResult)waitResult;
}

KWaitEvent::WaitResult KSuperThread::waitObject(KWaitEventQueue *eventQueue, KWaitEventQueue::WaitMode mode, unsigned long msecs)
{
    if (!eventQueue || eventQueue->isEmpty()) {
        return KWaitEvent::WaitFailed;
    }

    KWaitEventQueue waitEventQueue;

    waitEventQueue.add(&d->q_waitEventQuit);

    QListIterator<KWaitEvent*> iter(*eventQueue);
    while (iter.hasNext()) {
        waitEventQueue.add(iter.next());
    }

    quint32 waitResult = waitEventQueue.waitQueue(KWaitEventQueue::WaitAny, msecs);
    if (waitResult == KWaitEvent::WaitObject0) {
        waitResult = KWaitEvent::WaitInternal;
    } else if ((mode == KWaitEventQueue::WaitAll) && (KWaitEvent::WaitObject0 < waitResult) &&
               waitResult < (quint32)(KWaitEvent::WaitObject0 + waitEventQueue.count())) {
        waitResult = eventQueue->waitQueue(mode, msecs);
    }

    return (KWaitEvent::WaitResult)waitResult;
}

//
// KKPairThread
//

KPairThread::KPairThread(QObject *parent) :
    QObject(parent)
{
    setObjectName("KPairThread");

    d = new PairThreadPrivate(objectName(), this);
}

KPairThread::KPairThread(QString name, QObject *parent) :
    QObject(parent)
{
    setObjectName("KPairThread");

    d = new PairThreadPrivate(name.isEmpty() ? objectName() : name, this);
}

KPairThread::~KPairThread()
{
    delete d;
}

bool KPairThread::start(PairType type, QThread::Priority priority)
{
    switch (type) {
    case PairFirst: {
        return d->first()->start(priority);
    }
    case PairSecond: {
        return d->second()->start(priority);
    }
    case PairAll: {
        if (d->first()->start(priority)) {
            return d->second()->start(priority);
        }
        break;
    }
    default:
        Q_ASSERT(false);
        break;
    }

    return false;
}

void KPairThread::stop(PairType type, bool wait)
{
    switch (type) {
    case PairFirst: {
        d->first()->stop(wait);
        if (wait) {
            d->first()->wait();
        }
        break;
    }
    case PairSecond: {
        d->second()->stop(wait);
        if (wait) {
            d->second()->wait();
        }
        break;
    }
    case PairAll: {
        d->first()->stop(wait);
        d->second()->stop(wait);
        if (wait) {
            d->first()->wait();
            d->second()->wait();
        }
        break;
    }
    default:
        Q_ASSERT(false);
        break;
    }
}

bool KPairThread::isRunning(PairType type) const
{
    switch (type) {
    case PairFirst: {
        d->first()->isRunning();
        break;
    }
    case PairSecond: {
        d->second()->isRunning();
        break;
    }
    case PairAll:
    default:
        Q_ASSERT(false);
        break;
    }
    return d->first()->isRunning() || d->second()->isRunning();
}

KPairThread::WaitResult KPairThread::waitObject(PairType type, KWaitEvent *event, unsigned long msecs)
{
    if (!event) {
        return WaitFailed;
    }

    KWaitEventQueue waitEventQueue;
    quint32 waitResult = WaitFailed;

    switch (type) {
    case PairFirst: {
        waitEventQueue.add(&d->first()->q_waitEventQuit);
        waitEventQueue.add(event);

        waitResult = waitEventQueue.waitQueue(KWaitEventQueue::WaitAny, msecs);
        switch(waitResult) {
        case KWaitEvent::WaitObject0: waitResult = WaitObjectFirst; break;
        case KWaitEvent::WaitObject0 + 1: waitResult = WaitObject0; break;
        default: break;
        }
        break;
    }
    case PairSecond: {
        waitEventQueue.add(&d->second()->q_waitEventQuit);
        waitEventQueue.add(event);

        waitResult = waitEventQueue.waitQueue(KWaitEventQueue::WaitAny, msecs);
        switch(waitResult) {
        case KWaitEvent::WaitObject0: waitResult = WaitObjectSecond; break;
        case KWaitEvent::WaitObject0 + 1: waitResult = WaitObject0; break;
        default: break;
        }
        break;
    }
    case PairAll: {
        waitEventQueue.add(&d->first()->q_waitEventQuit);
        waitEventQueue.add(&d->second()->q_waitEventQuit);
        waitEventQueue.add(event);

        waitResult = waitEventQueue.waitQueue(KWaitEventQueue::WaitAny, msecs);
        switch (waitResult) {
        case KWaitEvent::WaitObject0: waitResult = WaitObjectFirst; break;
        case KWaitEvent::WaitObject0 + 1: waitResult = WaitObjectSecond; break;
        case KWaitEvent::WaitObject0 + 2: waitResult = WaitObject0; break;
        default: break;
        }
        break;
    }
    default:
        Q_ASSERT(false);
        break;
    }

    return (WaitResult)waitResult;
}

KPairThread::WaitResult KPairThread::waitObject(PairType type, KWaitEventQueue * eventQueue, KWaitEventQueue::WaitMode mode, unsigned long msecs)
{
    if (!eventQueue || eventQueue->isEmpty()) {
        return WaitFailed;
    }

    KWaitEventQueue waitEventQueue;
    quint32 waitResult = WaitFailed;

    switch (type) {
    case PairFirst:
    {
        waitEventQueue.add(&d->first()->q_waitEventQuit);
        QListIterator<KWaitEvent*> iter(*eventQueue);
        while (iter.hasNext()) {
            waitEventQueue.add(iter.next());
        }

        waitResult = waitEventQueue.waitQueue(KWaitEventQueue::WaitAny, msecs);
        if (waitResult == KWaitEvent::WaitObject0) {
            waitResult = WaitObjectFirst;
        } else if ((mode == KWaitEventQueue::WaitAll) && (KWaitEvent::WaitObject0 < waitResult) &&
                   waitResult < (quint32)(KWaitEvent::WaitObject0 + waitEventQueue.count())) {
            waitResult = eventQueue->waitQueue(mode, msecs);
        }
        break;
    }
    case PairSecond:
    {
        waitEventQueue.add(&d->second()->q_waitEventQuit);
        QListIterator<KWaitEvent*> iter(*eventQueue);
        while (iter.hasNext()) {
            waitEventQueue.add(iter.next());
        }

        waitResult = waitEventQueue.waitQueue(KWaitEventQueue::WaitAny, msecs);
        if (waitResult == KWaitEvent::WaitObject0) {
            waitResult = WaitObjectSecond;
        } else if ((mode == KWaitEventQueue::WaitAll) && (KWaitEvent::WaitObject0 < waitResult) &&
                   waitResult < (quint32)(KWaitEvent::WaitObject0 + waitEventQueue.count())) {
            waitResult = eventQueue->waitQueue(mode, msecs);
        }
        break;
    }
    case PairAll:
    {
        waitEventQueue.add(&d->first()->q_waitEventQuit);
        waitEventQueue.add(&d->second()->q_waitEventQuit);
        QListIterator<KWaitEvent*> iter(*eventQueue);
        while (iter.hasNext()) {
            waitEventQueue.add(iter.next());
        }

        waitResult = waitEventQueue.waitQueue(KWaitEventQueue::WaitAny, msecs);
        if (waitResult == KWaitEvent::WaitObject0) {
            waitResult = WaitObjectFirst;
        } else if (waitResult == KWaitEvent::WaitObject0 + 1) {
            waitResult = WaitObjectSecond;
        } else if ((mode == KWaitEventQueue::WaitAll) && (KWaitEvent::WaitObject0 + 1 < waitResult) &&
                   waitResult < (quint32)(KWaitEvent::WaitObject0 + 1 + waitEventQueue.count())) {
            waitResult = eventQueue->waitQueue(mode, msecs);
        }
    }
    default:
        Q_ASSERT(false);
        break;
    }

    return (WaitResult)waitResult;
}
