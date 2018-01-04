#ifndef JSCENE_H
#define JSCENE_H

#include <QGraphicsScene>
#include "jflowchart_global.h"

class JScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit JScene(QObject *parent = 0);

Q_SIGNALS:

public Q_SLOTS:

    // QGraphicsScene interface
protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect);
    virtual void drawForeground(QPainter *painter, const QRectF &rect);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    virtual void wheelEvent(QGraphicsSceneWheelEvent *event);
};

#endif // JSCENE_H
