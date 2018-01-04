#include "precomp.h"
#include "kradiobutton.h"

KRadioButton::KRadioButton(QWidget *parent) :
    QRadioButton(parent),
    q_state(Normal)
{
    setObjectName("KRadioButton");
    setPixmap(":/kwt/images/default/radiobutton");
}

KRadioButton::KRadioButton(const QString &text, QWidget *parent) :
    QRadioButton(text, parent),
    q_state(Normal)
{
    setObjectName("KRadioButton");
    setPixmap(":/kwt/images/default/radiobutton");
}

KRadioButton::KRadioButton(const QPixmap& pixmap, const QString &text, QWidget *parent) :
    QRadioButton(text, parent),
    q_state(Normal)
{
    setObjectName("KRadioButton");
    setPixmap(pixmap);
}

void KRadioButton::setPixmap(const QString &name)
{
    q_pixmap.load(name);
}

void KRadioButton::setPixmap(const QPixmap &pixmap)
{
    q_pixmap = pixmap;
}

void KRadioButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int iconWidth = q_pixmap.width()/8;
    int iconHeight = q_pixmap.height();
    painter.drawPixmap(QRect(0, (height() - iconHeight) / 2, iconWidth, iconHeight), q_pixmap,
                       QRect(iconWidth * (q_state + (isChecked() ? 1 : 0)*ButtonStateCount), 0, iconWidth, iconHeight));
    painter.drawText(QRect(iconWidth + 5, 0, width() - iconWidth - 5, height()), Qt::AlignLeft|Qt::AlignVCenter, text());
}

void KRadioButton::enterEvent(QEvent *event)
{
    setCursor(Qt::ArrowCursor);
    q_state = Active;

    QRadioButton::enterEvent(event);
}

void KRadioButton::leaveEvent(QEvent *event)
{
    q_state = Normal;

    QRadioButton::leaveEvent(event);
}

void KRadioButton::mouseMoveEvent(QMouseEvent *event)
{
    if (rect().contains(event->pos())) {
        if (event->buttons() & Qt::LeftButton) {
            q_state = Selected;
        }
    } else if (q_state != Normal) {
        q_state = Normal;
    }
    QRadioButton::mouseMoveEvent(event);
}

void KRadioButton::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton) {
        q_state = Selected;
    }
    QRadioButton::mousePressEvent(event);
}

void KRadioButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(q_state == Selected) {
        q_state = Active;
    }
    QRadioButton::mouseReleaseEvent(event);
}
