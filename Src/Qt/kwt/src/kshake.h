#ifndef KSHAKE_H
#define KSHAKE_H

#include <QObject>
#include <QPoint>
#include "kwt_global.h"

class QWidget;
class QTimer;

class KWT_EXPORT KShake : public QObject
{
    Q_OBJECT
public:
    explicit KShake(QObject *parent = 0);

    enum {
        MaxTimes    = 20,   // 抖动的次数
        MaxSpace    = 8,    // 抖动的幅度
        ShakeSpeed  = 30    // 触发抖动的时间间隔
    };

    void start();

private Q_SLOTS:
    void slotTimeout();

private:
    QWidget*    q_widget;
    QTimer*     q_timer;
    int         q_nPos;
    QPoint      q_curPos;
};

#endif // KSHAKE_H
