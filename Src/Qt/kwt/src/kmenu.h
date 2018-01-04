#ifndef KMENU_H
#define KMENU_H

#include <QMenu>
#include "kwt_global.h"

class KWT_EXPORT KMenu : public QMenu
{
    Q_OBJECT
public:
    explicit KMenu(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // KMENU_H
