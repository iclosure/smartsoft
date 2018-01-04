#ifndef JVIEW_H
#define JVIEW_H

#include <QGraphicsView>
#include "jflowchart_global.h"

class JView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit JView(QWidget *parent = 0);

Q_SIGNALS:

public Q_SLOTS:
    void shuffle();
    void zoomIn();
    void zoomOut();

protected:
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

    void scaleView(qreal scaleFactor);

private:
};

#endif // JVIEW_H
