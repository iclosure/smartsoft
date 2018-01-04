#include "precomp.h"
#include "kwaitcondition.h"

#if (QT_VERSION < 0x050000)
#include <Windows.h>
#endif

/////////////////////////////////////////////////////////////
/// WaitEventPrivate

class WaitEventPrivate : QObject
{
public:
    explicit WaitEventPrivate(QString name, QObject *parent = 0);
    virtual ~WaitEventPrivate(void);

    bool setEvent();
    bool resetEvent();

private:
    Q_DISABLE_COPY(WaitEventPrivate)

    int q_priority;
    HANDLE q_event;
    QMutex q_mutex;
    friend class KWaitEvent;
    friend class KWaitEventQueue;
};

WaitEventPrivate::WaitEventPrivate(QString name, QObject *parent) :
    QObject(parent), q_priority(0)
{
    Q_ASSERT(parent->inherits("KWaitEvent"));
    setObjectName(name + QUuid().createUuid().toString());

#ifndef Q_OS_WINRT
    q_priority = GetThreadPriority(GetCurrentThread());
#endif

#ifndef Q_OS_WINRT
    q_event = ::CreateEvent(NULL, TRUE, FALSE, objectName().toStdWString().c_str());
#else
    q_event = ::CreateEventEx(NULL, objectName().toStdWString().c_str(), CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
#endif
}

WaitEventPrivate::~WaitEventPrivate()
{
    ::CloseHandle(q_event);
}

bool WaitEventPrivate::setEvent()
{
    qDebug() << QString("set event [%1]").arg(objectName());

    QMutexLocker locker(&q_mutex);
    return ::SetEvent(q_event) ? true : false;
}

bool WaitEventPrivate::resetEvent()
{
    qDebug() << QString("reset event [%1]").arg(objectName());

    QMutexLocker locker(&q_mutex);
    return ::ResetEvent(q_event) ? true : false;
}

/////////////////////////////////////////////////////////////
/// KWaitEvent

KWaitEvent::KWaitEvent(QObject *parent) :
    QObject(parent)
{
    setObjectName("KWaitEvent");

    d = new WaitEventPrivate(objectName(), this);
}

KWaitEvent::KWaitEvent(QString name, QObject *parent) :
    QObject(parent)
{
    setObjectName("KWaitEvent");

    d = new WaitEventPrivate(name.isEmpty() ? objectName() : name, this);
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
#ifndef Q_OS_WINRT
    switch (::WaitForSingleObject(d->q_event, msecs)) {
#else
    switch (::WaitForSingleObjectEx(d->q_event, msecs, FALSE)) {
#endif
    case WAIT_OBJECT_0: return KWaitEvent::WaitObject0;
    case WAIT_TIMEOUT: return KWaitEvent::WaitTimeout;
    case WAIT_FAILED: return KWaitEvent::WaitFailed;
    default: return KWaitEvent::WaitInvalid;
    }
}

/////////////////////////////////////////////////////////////
/// KWaitEventQueue

KWaitEventQueue::KWaitEventQueue()
{
}

KWaitEventQueue::~KWaitEventQueue()
{
}

bool KWaitEventQueue::add(KWaitEvent * event)
{
    // insert 'event' into the queue (sorted by priority)
    int index = 0;
    for (; index < size(); index++) {
        if (at(index)->d->q_priority < event->d->q_priority) {
            break;
        }
    }
    insert(index, event);

    return true;
}

KWaitEvent::WaitResult KWaitEventQueue::waitQueue(WaitMode mode, unsigned long msecs)
{
    QVector<HANDLE> handleQueue;

    QListIterator<KWaitEvent*> iter(*this);
    while (iter.hasNext()) {
        handleQueue << iter.next()->d->q_event;
    }

    quint32 waitResult;
#ifndef Q_OS_WINRT
    waitResult = ::WaitForMultipleObjects(handleQueue.count(), handleQueue.constData(),
                                          (mode == KWaitEventQueue::WaitAll) ? TRUE : FALSE, msecs);
#else
    waitResult = ::WaitForMultipleObjectsEx(handleQueue.count(), handleQueue.constData(),
                                            (mode == KWaitEventQueue::WaitAll) ? TRUE : FALSE, msecs, FALSE);
#endif
    if (WAIT_OBJECT_0 > waitResult || waitResult >= (quint32)(WAIT_OBJECT_0 + handleQueue.count())) {
        switch (waitResult) {
        case WAIT_TIMEOUT: waitResult = KWaitEvent::WaitTimeout;
        case WAIT_FAILED: waitResult = KWaitEvent::WaitFailed;
        default: waitResult = KWaitEvent::WaitInvalid;
        }
    }

    return (KWaitEvent::WaitResult)waitResult;
}
