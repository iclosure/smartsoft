#ifndef JLAMP_H
#define JLAMP_H

#include <QGraphicsItem>
#include "jflowchart_global.h"

class JLamp : public QGraphicsItem
{
public:
    explicit JLamp(QGraphicsItem *parent = 0);

    int type() const { return FlowChart::ItemLamp; }

    // QGraphicsItem interface
    QRectF boundingRect() const;
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


};

#endif // JLAMP_H
