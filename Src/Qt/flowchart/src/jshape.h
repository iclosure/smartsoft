#ifndef JSHAPE_H
#define JSHAPE_H

#include <QGraphicsItem>
#include "jflowchart_global.h"

class JView;
class JLinker;

class JShape : public QGraphicsItem
{
public:
    JShape(JView *view, QGraphicsItem *parent = 0);

    void addLinker(JLinker *linker);
    void removeLinker(JLinker *linker);
    QList<JLinker *> linkers() const;

    int type() const { return FlowChart::ItemShape; }
    Qt::ArrowType arrowType() const;
    void setArrowType(Qt::ArrowType type);
    qreal width() const;
    qreal height() const;

    QSizeF size() const;
    void setSize(const QSize &size);

    QRectF boundingRect() const;


protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    JView *q_view;
    Qt::ArrowType q_arrowType;
    QPointF q_newPos;
    QSizeF q_size;
    QList<JLinker *> q_linkers;
};

#endif // JSHAPE_H
