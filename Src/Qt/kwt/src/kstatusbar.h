#ifndef KSTATUS_H
#define KSTATUS_H

#include <QStatusBar>

class KStatusBar : public QStatusBar
{
    Q_OBJECT
public:
    explicit KStatusBar(QWidget *parent = 0);

private:
};

#endif // KSTATUS_H
