#include "precomp.h"
#include "kdragproxy.h"

KDragProxy::KDragProxy(QWidget *parent) :
    QObject(parent),
    q_leftWidth (0),
    q_topWidth (0),
    q_rightWidth (0),
    q_bottomWidth (0),
    q_mousePressed (false),
    q_regionPressed (Unknown),
    q_cursorTimerId (0)
{
    setObjectName("KDragProxy");
    q_proxyWidget = parent;
    q_proxyWidget->setMouseTracking(true);
    q_proxyWidget->installEventFilter(this);
}

void KDragProxy::setBorder(int left, int top, int right, int bottom)
{
    q_leftWidth = left;
    q_topWidth = top;
    q_rightWidth = right;
    q_bottomWidth = bottom;
}

void KDragProxy::updateGeometry(int x, int y, int w, int h)
{
    if (w < q_proxyWidget->minimumWidth()
            || w > q_proxyWidget->maximumWidth()
            || h < q_proxyWidget->minimumHeight()
            || h > q_proxyWidget->maximumHeight())
    {
        return ;
    }

    q_proxyWidget->setGeometry(x, y, w, h);
}

bool KDragProxy::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type()) {
    case QEvent::MouseMove:
    {
        QMouseEvent* mouseEvent = reinterpret_cast<QMouseEvent*>(event);
        QPoint curPosLocal = mouseEvent->pos();
        WidgetRegion regionType = hitTest(curPosLocal);
        QPoint curPosGlobal = q_proxyWidget->mapToGlobal(curPosLocal);

        if (q_mousePressed)
        {
            switch (q_regionPressed) {
            case TopLeft:
            {
                QPoint dPos = curPosGlobal - q_originPosGlobal;
                updateGeometry(q_originGeometry.x() + dPos.x(), q_originGeometry.y() + dPos.y(),
                               q_originGeometry.width() - dPos.x(), q_originGeometry.height() - dPos.y());
            }
            case Top:
            {
                int dY = curPosGlobal.y() - q_originPosGlobal.y();
                updateGeometry(q_originGeometry.x(), q_originGeometry.y() + dY,
                               q_originGeometry.width(), q_originGeometry.height() - dY);
                break;
            }
            case TopRight:
            {
                QPoint dPos =  curPosGlobal - q_originPosGlobal;
                updateGeometry(q_originGeometry.x(), q_originGeometry.y() + dPos.y(),
                               q_originGeometry.width() + dPos.x(), q_originGeometry.height() - dPos.y());
                break;
            }
            case Left:
            {
                int dX = curPosGlobal.x() - q_originGeometry.x();
                updateGeometry(q_originGeometry.x() + dX, q_originGeometry.y(),
                               q_originGeometry.width() - dX, q_originGeometry.height());
                break;
            }
            case Inner:
            {
                q_proxyWidget->move(q_originGeometry.topLeft() + curPosGlobal - q_originPosGlobal);
                break;
            }
            case Right:
            {
                int dX = curPosGlobal.x() - q_originPosGlobal.x();
                updateGeometry(q_originGeometry.x(), q_originGeometry.y(),
                               q_originGeometry.width() + dX, q_originGeometry.height());
                break;
            }
            case BottomLeft:
            {
                QPoint dPos = curPosGlobal - q_originPosGlobal;
                updateGeometry(q_originGeometry.x() + dPos.x(), q_originGeometry.y(),
                               q_originGeometry.width() - dPos.x(), q_originGeometry.height() + dPos.y());
                break;
            }
            case Bottom:
            {
                int dY = curPosGlobal.y() - q_originPosGlobal.y();
                updateGeometry(q_originGeometry.x(), q_originGeometry.y(),
                               q_originGeometry.width(), q_originGeometry.height() + dY);
                break;
            }
            case BottomRight:
            {
                QPoint dPos = curPosGlobal - q_originPosGlobal;
                updateGeometry(q_originGeometry.x(), q_originGeometry.y(),
                               q_originGeometry.width() + dPos.x(), q_originGeometry.height() + dPos.y());
                break;
            }
            default:
                break;
            }
        } else {
            switch (regionType) {
            case Top:
            case Bottom:
                q_proxyWidget->setCursor(Qt::SizeVerCursor);
                break;
            case TopRight:
            case BottomLeft:
                q_proxyWidget->setCursor(Qt::SizeBDiagCursor);
                break;
            case Left:
            case Right:
                q_proxyWidget->setCursor(Qt::SizeHorCursor);
                break;
            case TopLeft:
            case BottomRight:
                q_proxyWidget->setCursor(Qt::SizeFDiagCursor);
                break;
            default:
                q_proxyWidget->setCursor(Qt::ArrowCursor);
                break;
            }

            startCursorTimer();
        }
        break;
    }
    case QEvent::MouseButtonPress:
    {
        QMouseEvent* mouseEvent = reinterpret_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            q_mousePressed = true;
            QPoint curPos = mouseEvent->pos();
            q_regionPressed = hitTest(curPos);
            q_originPosGlobal = q_proxyWidget->mapToGlobal(curPos);
            q_originGeometry = q_proxyWidget->geometry();
            stopCursorTimer();
        }
        break;
    }
    case QEvent::MouseButtonRelease:
    {
        q_mousePressed = false;
        q_regionPressed = Unknown;
        q_proxyWidget->setCursor(Qt::ArrowCursor);
        break;
    }
    case QEvent::Resize:
    {
        makeRegions();
        break;
    }
    case QEvent::Leave:
    {
        q_proxyWidget->setCursor(Qt::ArrowCursor);
        stopCursorTimer();
        break;
    }
    case QEvent::Timer:
    {
        QTimerEvent* timerEvent = reinterpret_cast<QTimerEvent*>(event);
        if (timerEvent->timerId() == q_cursorTimerId)
        {
            if (q_regions[Inner].contains(q_proxyWidget->mapToGlobal(QCursor::pos())))
            {
                q_proxyWidget->setCursor(Qt::ArrowCursor);
                stopCursorTimer();
            }
        }
        break;
    }
    default:
        break;
    }

    return QObject::eventFilter(obj, event);
}

void KDragProxy::startCursorTimer()
{
    stopCursorTimer();
    q_cursorTimerId = q_proxyWidget->startTimer(50);
}

void KDragProxy::stopCursorTimer()
{
    if (q_cursorTimerId != 0)
    {
        q_proxyWidget->killTimer(q_cursorTimerId);
        q_cursorTimerId = 0;
    }
}

void KDragProxy::makeRegions()
{
    int w = q_proxyWidget->width();
    int h = q_proxyWidget->height();

    q_regions[TopLeft]      = QRect(0, 0, q_leftWidth, q_topWidth);
    q_regions[Top]          = QRect(q_leftWidth, 0, w - q_leftWidth - q_rightWidth, q_topWidth);
    q_regions[TopRight]     = QRect(w - q_rightWidth, 0, q_rightWidth, q_topWidth);
    q_regions[Left]         = QRect(0, q_topWidth, q_leftWidth, h - q_topWidth - q_bottomWidth);
    q_regions[Inner]        = QRect(q_leftWidth, q_topWidth, w - q_leftWidth - q_rightWidth, h - q_topWidth - q_bottomWidth);
    q_regions[Right]        = QRect(w - q_rightWidth, q_topWidth, q_rightWidth, h - q_topWidth - q_bottomWidth);
    q_regions[BottomLeft]   = QRect(0, h - q_bottomWidth, q_leftWidth, q_bottomWidth);
    q_regions[Bottom]       = QRect(q_leftWidth, h - q_bottomWidth, w - q_leftWidth - q_rightWidth, q_bottomWidth);
    q_regions[BottomRight]  = QRect(w - q_rightWidth, h - q_bottomWidth, q_rightWidth, q_bottomWidth);
}

KDragProxy::WidgetRegion KDragProxy::hitTest(const QPoint &pos)
{
    for (int i = 0; i < 9; i++)
    {
        const QRect rect = q_regions[i];
        if (rect.contains(pos))
        {
            return KDragProxy::WidgetRegion(i);
        }
    }

    return Unknown;
}
