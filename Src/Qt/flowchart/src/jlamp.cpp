#include "precomp.h"
#include "jlamp.h"

JLamp::JLamp(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
}

QRectF JLamp::boundingRect() const
{
    return QRectF(0, 0, 10, 10);
}

void JLamp::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
