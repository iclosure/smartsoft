#ifndef WAITCONDITION_H
#define WAITCONDITION_H

#include <QObject>
#include <QList>
#include "kwt_global.h"

class WaitEventPrivate;

class KWT_EXPORT KWaitEvent : public QObject
{
    Q_OBJECT
public:
    enum WaitResult {
        WaitInternal = -4,
        WaitInvalid = -3,
        WaitTimeout = -2,
        WaitFailed = -1,
        WaitObject0 = 0,
    };

public:
    explicit KWaitEvent(QObject * parent = 0);
    explicit KWaitEvent(QString name, QObject * parent = 0);
    virtual ~KWaitEvent(void);

    bool setEvent();
    bool resetEvent();
    WaitResult wait(unsigned long msecs = ~0UL);

private:
    Q_DISABLE_COPY(KWaitEvent)

    WaitEventPrivate* d;
    friend class KWaitEventQueue;
};

class KWT_EXPORT KWaitEventQueue : public QList<KWaitEvent*>
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
    KWaitEvent::WaitResult waitQueue(WaitMode mode = WaitAll, unsigned long msecs = ~0UL);

private:
};

#endif // WAITCONDITION_H
