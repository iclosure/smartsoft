#ifndef KTHREAD_P_H
#define KTHREAD_P_H

#include <QThread>
#include "kthread.h"

typedef bool (__stdcall*_thread_routine)(void*);

//
// ThreadPrivate
//

typedef bool (__stdcall*_thread_routine)(void*);

class ThreadPrivate : public QThread
{
public:
    explicit ThreadPrivate(QString name, QObject *parent);
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
    Q_DISABLE_COPY(ThreadPrivate)

    volatile bool q_stoped;
    QMutex q_mutex;
    friend class KThread;
};

//
// SuperThreadPrivate
//

class SuperThreadPrivate : public QThread
{
public:
    explicit SuperThreadPrivate(QString name, QObject* parent);
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
    Q_DISABLE_COPY(SuperThreadPrivate)
    volatile bool q_stoped;
    QMutex q_mutex;
    KWaitEvent q_waitEventQuit;
    friend class KSuperThread;
};

//
// KSingleThread
//

class KSingleThread : public QThread
{
public:
    explicit KSingleThread(_thread_routine routine, QString name, QObject* parent);
    ~KSingleThread();

    bool start(Priority = InheritPriority);
    void stop(bool wait = true);

    QMutex & mutex() { return q_mutex; }
    KWaitEvent & eventQuit() { return q_waitEventQuit; }

private:
    virtual void run();

private:
    Q_DISABLE_COPY(KSingleThread)
    volatile bool q_stoped;
    _thread_routine q_routine;
    KWaitEvent q_waitEventQuit;
    QMutex q_mutex;
    friend class KPairThread;
};

//
// PairThreadPrivate
//

class PairThreadPrivate
{
public:
    explicit PairThreadPrivate(QString name, QObject* parent)
    {
        q_pair.first = new KSingleThread(routineFirst, name + ".first", parent);
        q_pair.second = new KSingleThread(routineSecond, name + ".second", parent);
    }

    virtual ~PairThreadPrivate()
    {
        delete q_pair.first;
        delete q_pair.second;
    }

    KSingleThread* first() const { return q_pair.first; }
    KSingleThread* second() const { return q_pair.second; }

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
    QPair<KSingleThread*, KSingleThread*> q_pair;
    friend class KPairThread;
};

#endif // KTHREAD_P_H
