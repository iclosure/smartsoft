#include "precomp.h"
#include "jview.h"
#include "jshape.h"
#include "jlinker.h"

JShape::JShape(JView *view, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , q_view(view)
    , q_arrowType(Qt::RightArrow)
    , q_size(120, 50)
{
    setFlags(ItemIsMovable
             | ItemIsSelectable
             | ItemIsFocusable
             | ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setAcceptHoverEvents(true);
}

void JShape::addLinker(JLinker *linker)
{
    q_linkers << linker;
    linker->adjust();
}

void JShape::removeLinker(JLinker *linker)
{
    q_linkers.removeOne(linker);
}

QList<JLinker *> JShape::linkers() const
{
    return q_linkers;
}

Qt::ArrowType JShape::arrowType() const
{
    return q_arrowType;
}

void JShape::setArrowType(Qt::ArrowType type)
{
    q_arrowType = type;
}

qreal JShape::width() const
{
    return q_size.width();
}

qreal JShape::height() const
{
    return q_size.height();
}

QSizeF JShape::size() const
{
    return q_size;
}

void JShape::setSize(const QSize &size)
{
    q_size = size;
}

QRectF JShape::boundingRect() const
{
    return QRectF(QPointF(0, 0), q_size);
}

void JShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QRectF rect = boundingRect();

    QLinearGradient gradient(0.0, 0.0, 0.0, rect.height());
    gradient.setColorAt(0.0, QColor(100, 100, 100, 254));
    gradient.setColorAt(0.6, QColor(100, 100, 100, 220));
    gradient.setColorAt(1.0, QColor(100, 100, 100, 100));

    QPainterPath path;
    path.addRoundedRect(boundingRect(), 6, 6);
    painter->fillPath(path, gradient);
}

QVariant JShape::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    qDebug() << change << value;
    switch (change) {
    case ItemPositionHasChanged:
        Q_FOREACH (JLinker *linker, q_linkers) {
            linker->adjust();
        }
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void JShape::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);
    QGraphicsItem::mousePressEvent(event);
}

void JShape::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    unsetCursor();
    QGraphicsItem::mouseReleaseEvent(event);
}

void JShape::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::OpenHandCursor);
    QGraphicsItem::hoverMoveEvent(event);
}

void JShape::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);
    QGraphicsItem::mouseMoveEvent(event);
}
