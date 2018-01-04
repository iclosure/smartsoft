#include "precomp.h"
#include "kmenu.h"

KMenu::KMenu(QWidget *parent) :
    QMenu(parent)
{
    setObjectName("KMenu");
}

void KMenu::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //painter.drawPixmap(rect(), QPixmap(":/kwt/images/default/menu/bkg"));

    QMenu::paintEvent(event);
}
