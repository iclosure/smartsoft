#ifndef KTHREAD_H
#define KTHREAD_H

#include <QThread>
#include "kwt_global.h"
#include "kwaitcondition.h"

//
// KThread
//

class QMutex;
class ThreadPrivate;

class KWT_EXPORT KThread : public QObject
{
    Q_OBJECT
public:
    explicit KThread(QObject *parent = 0);
    explicit KThread(QString name, QObject *parent = 0);
    virtual ~KThread();

    bool start(QThread::Priority = QThread::InheritPriority);
    void stop(bool wait = true);
    bool isRunning() const;

    virtual bool work(void) = 0;

    QMutex * mutex();

    static void sleep(unsigned long secs);
    static void msleep(unsigned long msecs);
    static void usleep(unsigned long usecs);

private:
    Q_DISABLE_COPY(KThread)
    ThreadPrivate* d;
};

//
// KSuperThread
//

class SuperThreadPrivate;

class KWT_EXPORT KSuperThread : public QObject
{
    Q_OBJECT
public:
    explicit KSuperThread(QObject *parent = 0);
    explicit KSuperThread(QString name, QObject *parent = 0);
    virtual ~KSuperThread();

    bool start(QThread::Priority = QThread::InheritPriority);
    void stop(bool wait = true);
    bool isRunning() const;
    KWaitEvent::WaitResult waitObject(KWaitEvent *, unsigned long msecs = ~0UL);
    KWaitEvent::WaitResult waitObject(KWaitEventQueue *, KWaitEventQueue::WaitMode mode = KWaitEventQueue::WaitAll,
        unsigned long msecs = ~0UL);

    virtual bool work(void) = 0;

private:
    Q_DISABLE_COPY(KSuperThread)
    SuperThreadPrivate* d;
};

//
// KPairThread
//

class PairThreadPrivate;

class KWT_EXPORT KPairThread : public QObject
{
    Q_OBJECT
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
    explicit KPairThread(QObject *parent = 0);
    explicit KPairThread(QString name, QObject *parent = 0);
    virtual ~KPairThread();

    bool start(PairType = PairAll, QThread::Priority = QThread::InheritPriority);
    void stop(PairType = PairAll, bool wait = true);
    bool isRunning(PairType) const;
    WaitResult waitObject(PairType, KWaitEvent *, unsigned long msecs = ~0UL);
    WaitResult waitObject(PairType, KWaitEventQueue *, KWaitEventQueue::WaitMode mode = KWaitEventQueue::WaitAll,
        unsigned long msecs = ~0UL);

    virtual bool workFirst() = 0;
    virtual bool workSecond() = 0;

private:
    Q_DISABLE_COPY(KPairThread)
    PairThreadPrivate* d;
};

#endif // KTHREAD_H
