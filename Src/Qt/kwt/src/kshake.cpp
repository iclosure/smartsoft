#include "precomp.h"
#include "kshake.h"

KShake::KShake(QObject *parent) :
    QObject(parent),
    q_widget(reinterpret_cast<QWidget*> (parent)),
    q_timer (new QTimer(this)),
    q_nPos  ( MaxTimes ),
    q_curPos(QPoint())
{
    setObjectName("KShake");

    ///////////////////////////////////////////////////////////////////////

    connect(q_timer, SIGNAL(timeout()), SLOT(slotTimeout()));
}

void KShake::start()
{
    if (q_nPos < MaxTimes)
    {
        return ;
    }

    q_nPos = 1;
    q_curPos = q_widget->pos();
    q_timer->start(ShakeSpeed);
}

void KShake::slotTimeout()
{
    if(q_nPos < MaxTimes)
    {
        q_nPos ++;
        switch(q_nPos % 4) {
        case 1:
        {
            q_widget->move(q_curPos.x(), q_curPos.y() - MaxSpace);
            break;
        }
        case 2:
        {
            q_widget->move(q_curPos.x() - MaxSpace, q_curPos.y() - MaxSpace);
            break;
        }
        case 3:
        {
            q_widget->move(q_curPos.x() - MaxSpace, q_curPos.y());
            break;
        }
        case 0:
        default:
            q_widget->move(q_curPos);
            break;
        }
    } else {
        q_timer->stop();
    }
}
