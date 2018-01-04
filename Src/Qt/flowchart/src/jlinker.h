#ifndef JLINKER_H
#define JLINKER_H

#include <QGraphicsItem>
#include "jflowchart_global.h"

class JShape;

class JLinker : public QGraphicsItem
{
public:
    JLinker(JShape *fromShape, JShape *toShape, QGraphicsItem *parent = 0);

    JShape *fromShape() const;
    JShape *toShape() const;

    void adjust();

    int type() const { return FlowChart::ItemLinker; }

    // QGraphicsItem interface
    QRectF boundingRect() const;
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

private:
    Qt::ArrowType updateToArrowType();  //
    void updateArrowPath();  //

private:
    QPointF q_fromPoint;
    QPointF q_toPoint;
    qreal q_arrowSize;
    QPainterPath q_linePath;
    QPainterPath q_arrowPath;
    JShape *from, *to;
    int q_spacing;
};

#endif // JLINKER_H
