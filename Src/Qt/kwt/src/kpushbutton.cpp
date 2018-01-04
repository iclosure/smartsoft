#include "precomp.h"
#include "kpushbutton.h"

KPushButton::KPushButton(QWidget *parent) :
    QPushButton(parent),
    q_state(Normal)
{
    setObjectName("KPushButton");
}

KPushButton::KPushButton(const QString &text, QWidget *parent) :
    QPushButton(text, parent),
    q_state(Normal)
{
    setObjectName("KPushButton");
}

KPushButton::KPushButton(const QIcon& icon, const QString &text, QWidget *parent) :
    QPushButton(icon, text, parent),
    q_state(Normal)
{
    setObjectName("KPushButton");
}

void KPushButton::setPixmap(const QString &name)
{
    q_pixmap.load(name);
    q_picWidth = q_pixmap.width() / 4;
    q_picHeight = q_pixmap.height();
    setFixedSize(q_picWidth, q_picHeight);
}

void KPushButton::setPixmap(QPixmap &pixmap)
{
    q_pixmap = pixmap;
    q_picWidth = q_pixmap.width() / 4;
    q_picHeight = q_pixmap.height();
    setFixedSize(q_picWidth, q_picHeight);
}

void KPushButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), q_pixmap, QRect(q_picWidth * q_state, 0, q_picWidth, q_picHeight));
}

void KPushButton::enterEvent(QEvent *event)
{
    setCursor(Qt::ArrowCursor);
    q_state = Active;

    QPushButton::enterEvent(event);
}

void KPushButton::leaveEvent(QEvent *event)
{
    q_state = Normal;

    QPushButton::leaveEvent(event);
}

void KPushButton::mouseMoveEvent(QMouseEvent *event)
{
    if (rect().contains(event->pos())) {
        if (event->buttons() & Qt::LeftButton) {
            q_state = Selected;
        }
    } else if (q_state != Normal) {
        q_state = Normal;
    }
    QPushButton::mouseMoveEvent(event);
}

void KPushButton::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton) {
        q_state = Selected;
    }
    QPushButton::mousePressEvent(event);
}

void KPushButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(q_state == Selected) {
        q_state = Active;
    } else {
        q_state = Normal;
    }
    QPushButton::mouseReleaseEvent(event);
}
