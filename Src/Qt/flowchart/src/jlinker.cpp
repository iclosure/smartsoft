#include "precomp.h"
#include "jlinker.h"
#include "jshape.h"

JLinker::JLinker(JShape *fromShape, JShape *toShape, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , from(fromShape)
    , to(toShape)
    , q_arrowSize(10)
    , q_spacing(30)
{
    setZValue(-1);
    setAcceptedMouseButtons(0);
    from->addLinker(this);
    to->addLinker(this);
    adjust();
}

QRectF JLinker::boundingRect() const
{
    if (!from || !to) {
        return QRectF();
    }

    qreal penWidth = 1;
    qreal extra = (penWidth + q_arrowSize) / 2.0;

    return QRectF(q_fromPoint, QSizeF(q_toPoint.x() - q_fromPoint.x(),
                                      q_toPoint.y() - q_fromPoint.y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

void JLinker::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!from || !to) {
        return;
    }

    painter->setPen(QPen(QColor("darkblue"), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPath(q_linePath);
    painter->fillPath(q_arrowPath, QBrush(QColor("darkblue")));
}

JShape *JLinker::fromShape() const
{
    return from;
}

JShape *JLinker::toShape() const
{
    return to;
}

void JLinker::adjust()
{
    if (!from || !to) {
        return;
    }

    prepareGeometryChange();

    updateArrowPath();  //
}

Qt::ArrowType JLinker::updateToArrowType()
{
    Qt::ArrowType toArrowType = to->arrowType();
    Qt::ArrowType fromArrowType = from->arrowType();

    qreal dx = (to->x() - from->x()) + (to->width() - from->width())/2;
    qreal dy = (to->y() - from->y()) + (to->height() - from->height())/2;
    qreal intervalX = qAbs(dx) - (from->width() + to->width())/2;
    qreal intervalY = qAbs(dy) - (from->height() + to->height())/2;
    if (intervalX > 0) {  // 在from的X范围外
        if (intervalY > 0) {  // 在from的Y范围外
            if (qAbs(intervalX) > q_spacing) { // 在X外边角
                if (qAbs(intervalY) > q_spacing) { // 在Y外边角
                    switch(fromArrowType) {
                    case Qt::LeftArrow:
                        toArrowType = (dx > 0) ? ((dy > 0) ? Qt::UpArrow : Qt::DownArrow) : Qt::RightArrow;
                        break;
                    case Qt::UpArrow:
                        toArrowType = (dy > 0) ? Qt::UpArrow : Qt::DownArrow;
                        break;
                    case Qt::RightArrow:
                        toArrowType = (dx > 0) ? Qt::LeftArrow : ((dy > 0) ? Qt::UpArrow : Qt::DownArrow);
                        break;
                    case Qt::DownArrow:
                        toArrowType = (dy > 0) ? Qt::UpArrow : ((dx > 0) ? Qt::LeftArrow : Qt::RightArrow);
                        break;
                    default:
                        break;
                    }
                } else {  // 在X...
                    switch(fromArrowType) {
                    case Qt::LeftArrow:
                    case Qt::UpArrow:
                        toArrowType = (dx > 0) ? Qt::LeftArrow : Qt::RightArrow;
                        break;
                    case Qt::RightArrow:
                        toArrowType = (dx > 0) ? Qt::LeftArrow : ((dy > 0) ? Qt::UpArrow : Qt::DownArrow);
                        break;
                    case Qt::DownArrow:
                        toArrowType = (dx > 0) ? Qt::LeftArrow : Qt::RightArrow;
                        break;
                    default:
                        break;
                    }
                }
            } else {  // 在X...
                if (qAbs(intervalY) > q_spacing) { // 在Y外边角
                    toArrowType = (dy > 0) ? Qt::UpArrow : Qt::DownArrow;
                } else {  // 在X...
                    switch(fromArrowType) {
                    case Qt::LeftArrow:
                    case Qt::RightArrow:
                        toArrowType = (dy > 0) ? Qt::UpArrow : Qt::DownArrow;
                        break;
                    case Qt::UpArrow:
                    case Qt::DownArrow:
                        toArrowType = (dx > 0) ? Qt::LeftArrow : Qt::RightArrow;
                        break;
                    default:
                        break;
                    }
                }
            }
        } else {  // 在from的Y范围内
            if (qAbs(intervalX) > q_spacing) { // 在X外边角
                if (qAbs(intervalY) > q_spacing) { // 在Y外边角
                    toArrowType = (dx > 0) ? Qt::LeftArrow : Qt::RightArrow;
                } else {  // 在X...
                    switch(fromArrowType) {
                    case Qt::LeftArrow:
                    case Qt::RightArrow:
                        toArrowType = (dy > 0) ? Qt::UpArrow : Qt::DownArrow;
                        break;
                    case Qt::UpArrow:
                    case Qt::DownArrow:
                        toArrowType = (dx > 0) ? Qt::LeftArrow : Qt::RightArrow;
                        break;
                    default:
                        break;
                    }
                }
            } else {  // 在X...
                if (qAbs(dy) > q_spacing) { // 在Y外边角
                    switch(fromArrowType) {
                    case Qt::LeftArrow:
                    case Qt::RightArrow:
                        toArrowType = (dy > 0) ? Qt::UpArrow : Qt::DownArrow;
                        break;
                    case Qt::UpArrow:
                    case Qt::DownArrow:
                        toArrowType = (dx > 0) ? Qt::LeftArrow : Qt::RightArrow;
                        break;
                    default:
                        break;
                    }
                } else {  // 在X...
                    toArrowType = (dx > 0) ? Qt::LeftArrow : Qt::RightArrow;
                }
            }
        }
    } else {  // 在from的X范围内
        if (intervalY > 0) {  // 在from的Y范围外
            toArrowType = (dy > 0) ? Qt::UpArrow : Qt::DownArrow;
        } else {  // 在from的Y范围内
            if (qAbs(intervalX) > q_spacing) { // 在X外边角
                if (qAbs(intervalY) > q_spacing) { // 在Y外边角
                    switch(fromArrowType) {
                    case Qt::LeftArrow:
                    case Qt::RightArrow:
                        toArrowType = (dy > 0) ? Qt::UpArrow : Qt::DownArrow;
                        break;
                    case Qt::UpArrow:
                    case Qt::DownArrow:
                        toArrowType = (dx > 0) ? Qt::LeftArrow : Qt::RightArrow;
                        break;
                    default:
                        break;
                    }
                } else {  // 在X...
                    switch(fromArrowType) {
                    case Qt::LeftArrow:
                    case Qt::UpArrow:
                    case Qt::RightArrow:
                    case Qt::DownArrow:
                        toArrowType = (dy > 0) ? Qt::UpArrow : Qt::DownArrow;
                        break;
                    default: break;  // error, none.
                    }
                }
            } else {  // 在X...
                if (qAbs(intervalY) > q_spacing) { // 在Y外边角
                    toArrowType = (dx > 0) ? Qt::LeftArrow : Qt::RightArrow;
                } else {  // 在X...
                    switch(fromArrowType) {
                    case Qt::LeftArrow:
                    case Qt::RightArrow:
                        toArrowType = (dy > 0) ? Qt::UpArrow : Qt::DownArrow;
                        break;
                    case Qt::UpArrow:
                    case Qt::DownArrow:
                        toArrowType = (dx > 0) ? Qt::LeftArrow : Qt::RightArrow;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }

    return toArrowType;
}

void JLinker::updateArrowPath()
{
    //
    QPainterPath linePath;
    Qt::ArrowType toArrowType = updateToArrowType();
    QPointF sp, ep;

    switch(from->arrowType()) {
    case Qt::LeftArrow:
        sp = QPointF(from->x(), from->y() + from->height()/2);
        linePath.moveTo(sp);
        switch (toArrowType) {
        case Qt::LeftArrow:
            ep = QPointF(to->x(), to->y() + to->height()/2);
            linePath.lineTo(sp.x() + 20, sp.y());
            linePath.lineTo(sp.x() + 20, ep.y());
            linePath.lineTo(ep.x() - 15, ep.y());
            break;
        case Qt::UpArrow:
            ep = QPointF(to->x() + to->width()/2, to->y());
            if (sp.x() - ep.x() < 20) {
                linePath.lineTo(sp.x() - 20, sp.y());
                linePath.lineTo(sp.x() - 20, ep.y() - 25);
                linePath.lineTo(ep.x(), ep.y() - 25);
                linePath.lineTo(ep.x(), ep.y() - 15);
            } else {
                linePath.lineTo(ep.x(), sp.y());
                linePath.lineTo(ep.x(), ep.y() - 15);
            }
            break;
        case Qt::RightArrow:
            ep = QPointF(to->x() + to->width(), to->y() + to->height()/2);
            linePath.lineTo(sp.x() - 20, sp.y());
            linePath.lineTo(sp.x() - 20, ep.y());
            linePath.lineTo(ep.x() + 15, ep.y());
            break;
        case Qt::DownArrow:
            ep = QPointF(to->x() + to->width()/2, to->y() + to->height());
            if (sp.x() - ep.x() < 20) {
                linePath.lineTo(sp.x() - 20, sp.y());
                linePath.lineTo(sp.x() - 20, ep.y() + 25);
                linePath.lineTo(ep.x(), ep.y() + 25);
                linePath.lineTo(ep.x(), ep.y() + 15);
            } else {
                linePath.lineTo(ep.x(), sp.y());
                linePath.lineTo(ep.x(), ep.y() + 15);
            }
            break;
        default:
            break;
        }
        break;
    case Qt::UpArrow:
        sp = QPointF(from->x() + from->width()/2, from->y());
        linePath.moveTo(sp);
        switch (toArrowType) {
        case Qt::LeftArrow:
            ep = QPointF(to->x(), to->y() + to->height()/2);
            if (sp.y() - ep.y() < 20) {
                linePath.lineTo(sp.x(), sp.y() - 20);
                linePath.lineTo(ep.x() - 25, sp.y() - 20);
                linePath.lineTo(ep.x() - 25, ep.y());
                linePath.lineTo(ep.x() - 15, ep.y());
            } else {
                linePath.lineTo(sp.x(), ep.y());
                linePath.lineTo(ep.x() - 15, ep.y());
            }
            break;
        case Qt::UpArrow:
            ep = QPointF(to->x() + to->width()/2, to->y());
            linePath.lineTo(sp.x(), sp.y() - 20);
            linePath.lineTo(ep.x(), sp.y() - 20);
            linePath.lineTo(ep.x(), ep.y() - 15);
            break;
        case Qt::RightArrow:
            ep = QPointF(to->x() + to->width(), to->y() + to->height()/2);
            if (sp.y() - ep.y() < 20) {
                linePath.lineTo(sp.x(), sp.y() - 20);
                linePath.lineTo(ep.x() + 25, sp.y() - 20);
                linePath.lineTo(ep.x() + 25, ep.y());
                linePath.lineTo(ep.x() + 15, ep.y());
            } else {
                linePath.lineTo(sp.x(), ep.y());
                linePath.lineTo(ep.x() + 15, ep.y());
            }
            break;
        case Qt::DownArrow:
            ep = QPointF(to->x() + to->width()/2, to->y() + to->height());
            linePath.lineTo(sp.x(), sp.y() - 20);
            linePath.lineTo(ep.x(), sp.y() - 20);
            linePath.lineTo(ep.x(), ep.y() + 15);
            break;
        default:
            break;
        }
        break;
    case Qt::RightArrow:
        sp = QPointF(from->x() + from->width(), from->y() + from->height()/2);
        linePath.moveTo(sp);
        switch (toArrowType) {
        case Qt::LeftArrow:
            ep = QPointF(to->x(), to->y() + to->height()/2);
            linePath.lineTo(sp.x() + 20, sp.y());
            linePath.lineTo(sp.x() + 20, ep.y());
            linePath.lineTo(ep.x() - 15, ep.y());
            break;
        case Qt::UpArrow:
            ep = QPointF(to->x() + to->width()/2, to->y());
            if (ep.x() - sp.x() < 20) {
                linePath.lineTo(sp.x() + 20, sp.y());
                linePath.lineTo(sp.x() + 20, ep.y() - 25);
                linePath.lineTo(ep.x(), ep.y() - 25);
                linePath.lineTo(ep.x(), ep.y() - 15);
            } else {
                linePath.lineTo(ep.x(), sp.y());
                linePath.lineTo(ep.x(), ep.y() - 15);
            }
            break;
        case Qt::RightArrow:
            ep = QPointF(to->x() + to->width(), to->y() + to->height()/2);
            linePath.lineTo(sp.x() + 20, sp.y());
            linePath.lineTo(sp.x() + 20, ep.y());
            linePath.lineTo(ep.x() + 15, ep.y());
            break;
        case Qt::DownArrow:
            ep = QPointF(to->x() + to->width()/2, to->y() + to->height());
            if (ep.x() - sp.x() < 20) {
                linePath.lineTo(sp.x() + 20, sp.y());
                linePath.lineTo(sp.x() + 20, ep.y() + 25);
                linePath.lineTo(ep.x(), ep.y() + 25);
                linePath.lineTo(ep.x(), ep.y() + 15);
            } else {
                linePath.lineTo(ep.x(), sp.y());
                linePath.lineTo(ep.x(), ep.y() + 15);
            }
            break;
        default:
            break;
        }
        break;
    case Qt::DownArrow:
        sp = QPointF(from->x() + from->width()/2, from->y() + from->height());
        linePath.moveTo(sp);
        switch (toArrowType) {
        case Qt::LeftArrow:
            ep = QPointF(to->x(), to->y() + to->height()/2);
            if (ep.y() - sp.y() < 20) {
                linePath.lineTo(sp.x(), sp.y() + 20);
                linePath.lineTo(ep.x() - 25, sp.y() + 20);
                linePath.lineTo(ep.x() - 25, ep.y());
                linePath.lineTo(ep.x() - 15, ep.y());
            } else {
                linePath.lineTo(sp.x(), ep.y());
                linePath.lineTo(ep.x() - 15, ep.y());
            }
            break;
        case Qt::UpArrow:
            ep = QPointF(to->x() + to->width()/2, to->y());
            linePath.lineTo(sp.x(), sp.y() + 20);
            linePath.lineTo(ep.x(), sp.y() + 20);
            linePath.lineTo(ep.x(), ep.y() - 15);
            break;
        case Qt::RightArrow:
            ep = QPointF(to->x() + to->width(), to->y() + to->height()/2);
            if (ep.y() - sp.y() < 20) {
                linePath.lineTo(sp.x(), sp.y() + 20);
                linePath.lineTo(ep.x() + 25, sp.y() + 20);
                linePath.lineTo(ep.x() + 25, ep.y());
                linePath.lineTo(ep.x() + 15, ep.y());
            } else {
                linePath.lineTo(sp.x(), ep.y());
                linePath.lineTo(ep.x() + 15, ep.y());
            }
            break;
        case Qt::DownArrow:
            ep = QPointF(to->x() + to->width()/2, to->y() + to->height());
            linePath.lineTo(sp.x(), sp.y() + 20);
            linePath.lineTo(ep.x(), sp.y() + 20);
            linePath.lineTo(ep.x(), ep.y() + 15);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    // arrow
    QPainterPath arrowPath;
    arrowPath.moveTo(ep.x(), ep.y());
    switch(toArrowType) {
    case Qt::LeftArrow:
        arrowPath.lineTo(ep.x() - 15, ep.y() - 5);
        arrowPath.lineTo(ep.x() - 15, ep.y() + 5);
        break;
    case Qt::UpArrow:
        arrowPath.lineTo(ep.x() - 5, ep.y() - 15);
        arrowPath.lineTo(ep.x() + 5, ep.y() - 15);
        break;
    case Qt::RightArrow:
        arrowPath.lineTo(ep.x() + 15, ep.y() - 5);
        arrowPath.lineTo(ep.x() + 15, ep.y() + 5);
        break;
    case Qt::DownArrow:
        arrowPath.lineTo(ep.x() - 5, ep.y() + 15);
        arrowPath.lineTo(ep.x() + 5, ep.y() + 15);
        break;
    default:
        break;
    }

    //
    q_linePath = linePath;
    q_arrowPath = arrowPath;
}
