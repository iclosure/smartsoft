#include "precomp.h"
#include "kthread_p.h"

#if (QT_VERSION < 0x050000)
#include <Windows.h>
#endif

//
// KThread
//

//#if (QT_VERSION < 0x050000)

void KThread::sleep(unsigned long secs)
{
    ::Sleep(secs * 1000);
}

void KThread::msleep(unsigned long msecs)
{
    ::Sleep(msecs);
}

void KThread::usleep(unsigned long usecs)
{
    ::Sleep((usecs / 1000) + 1);
}

//#endif

//
// ThreadPrivate
//

ThreadPrivate::ThreadPrivate(QString name, QObject *parent) :
    QThread(parent), q_stoped(true)
{
    Q_ASSERT(parent->inherits("KThread"));
    setObjectName(name + QUuid().createUuid().toString());
}

ThreadPrivate::~ThreadPrivate()
{
    this->stop();
}

bool ThreadPrivate::start(Priority priority)
{
    {
        QMutexLocker locker(&q_mutex);

        if (QThread::isRunning()) {
            qWarning() << QString("thread [%1] is running").arg(objectName());
            return true;
        }

        q_stoped = false;
    }

    QThread::start(priority);

    return true;
}

void ThreadPrivate::stop(bool wait)
{
    {
        QMutexLocker locker(&q_mutex);

        if (!QThread::isRunning()) {
            return;
        }

        q_stoped = true;
    }

    if (wait) {
        QThread::wait();
    } else {
        QThread::terminate();
    }
}

inline void ThreadPrivate::run()
{
    qDebug() << QString("start thread [%1]").arg(objectName());

    Q_FOREVER {
        {
            QMutexLocker locker(&q_mutex);
            if (q_stoped) {
                break;
            }
        }

        if (!routine(QThread::parent())) {
            break;
        }
    }

    qDebug() << QString("stop thread [%1]").arg(objectName());
}

//
// SuperThreadPrivate
//

SuperThreadPrivate::SuperThreadPrivate(QString name, QObject *parent) :
    QThread(parent), q_stoped(true)
{
    Q_ASSERT(parent->inherits("KSuperThread"));
    setObjectName(name + QUuid().createUuid().toString());
}

SuperThreadPrivate::~SuperThreadPrivate()
{
    this->stop();
}

bool SuperThreadPrivate::start(Priority priority)
{
    {
        QMutexLocker locker(&q_mutex);

        if (QThread::isRunning()) {
            qWarning() << QString("thread [%1] is running").arg(objectName());
            return true;
        }

        q_stoped = false;
    }

    q_waitEventQuit.resetEvent();

    QThread::start(priority);

    return true;
}

void SuperThreadPrivate::stop(bool wait)
{
    {
        QMutexLocker locker(&q_mutex);

        if (!QThread::isRunning()) {
            return;
        }

        q_stoped = true;
    }

    q_waitEventQuit.setEvent();

    if (wait) {
        QThread::wait();
    } else {
        QThread::terminate();
    }
}

inline void SuperThreadPrivate::run()
{
    qDebug() << QString("start thread [%1]").arg(objectName());

    Q_FOREVER {
        {
            QMutexLocker locker(&q_mutex);
            if (q_stoped) {
                q_waitEventQuit.setEvent();
                break;
            }
        }

        if (!routine(QThread::parent())) {
            q_waitEventQuit.resetEvent();
            break;
        }
    }

    qDebug() << QString("stop thread [%1]").arg(objectName());
}

//
// KSingleThread
//

KSingleThread::KSingleThread(_thread_routine routine, QString name, QObject *parent) :
    QThread(parent), q_stoped(true), q_routine(routine)
{
    Q_ASSERT(parent->inherits("KPairThread"));
    setObjectName(name + QUuid().createUuid().toString());
}

KSingleThread::~KSingleThread()
{
    this->stop();
}

bool KSingleThread::start(Priority priority)
{
    {
        QMutexLocker locker(&q_mutex);

        if (QThread::isRunning()) {
            qWarning() << QString("thread [%1] is running").arg(objectName());
            return true;
        }

        q_stoped = false;
    }

    q_waitEventQuit.resetEvent();

    QThread::start(priority);

    return true;
}

void KSingleThread::stop(bool wait)
{
    {
        QMutexLocker locker(&q_mutex);

        if (!QThread::isRunning()) {
            return;
        }

        q_stoped = true;
    }

    q_waitEventQuit.setEvent();

    if (!wait) {
        QThread::terminate();
    }
}

inline void KSingleThread::run()
{
    qDebug() << QString("start thread [%1]").arg(objectName());

    Q_FOREVER {
        {
            QMutexLocker locker(&q_mutex);
            if (q_stoped) {
                q_waitEventQuit.setEvent();
                break;
            }
        }

        if (!q_routine(QThread::parent())) {
            q_waitEventQuit.resetEvent();
            break;
        }
    }

    qDebug() << QString("stop thread [%1]").arg(objectName());
}
