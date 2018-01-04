#include "precomp.h"
#include "jview.h"
#include "jshape.h"
#include "jlinker.h"

JView::JView(QWidget *parent)
    : QGraphicsView(parent)
{
    setObjectName("JView");
    setFrameShape(QFrame::NoFrame);

    setCacheMode(CacheBackground);
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(FullViewportUpdate);
    setTransformationAnchor(AnchorUnderMouse);
}

void JView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        break;
    case Qt::Key_Down:
        break;
    case Qt::Key_Left:
        break;
    case Qt::Key_Right:
        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        shuffle();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void JView::wheelEvent(QWheelEvent *event)
{
    scaleView(qPow((double)2, -event->delta() / 240.0));
}

void JView::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
}

void JView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor)
            .mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void JView::shuffle()
{
    Q_FOREACH (QGraphicsItem *item, scene()->items()) {
        if (qgraphicsitem_cast<JShape *>(item))
            item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
    }
}

void JView::zoomIn()
{
    scaleView(qreal(1.2));
}

void JView::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
