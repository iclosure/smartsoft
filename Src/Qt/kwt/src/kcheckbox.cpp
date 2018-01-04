#include "precomp.h"
#include "kcheckbox.h"

KCheckBox::KCheckBox(QWidget *parent) :
    QCheckBox(parent),
    q_state(Normal)
{
    setObjectName("KCheckBox");
    setPixmap(":/kwt/images/default/set_dlg_checkbox");
}

KCheckBox::KCheckBox(const QString &text, QWidget *parent) :
    QCheckBox(text, parent),
    q_state(Normal)
{
    setObjectName("KCheckBox");
    setPixmap(":/kwt/images/default/set_dlg_checkbox");
}

KCheckBox::KCheckBox(const QPixmap& pixmap, const QString &text, QWidget *parent) :
    QCheckBox(text, parent),
    q_state(Normal)
{
    setObjectName("KCheckBox");
    setPixmap(pixmap);
}

void KCheckBox::setPixmap(const QString &name)
{
    q_pixmap.load(name);
}

void KCheckBox::setPixmap(const QPixmap &pixmap)
{
    q_pixmap = pixmap;
}

void KCheckBox::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int iconWidth = q_pixmap.width()/12;
    int iconHeight = q_pixmap.height();
    painter.drawPixmap(QRect(0, (height() - iconHeight) / 2, iconWidth, iconHeight), q_pixmap,
                       QRect(iconWidth * (q_state + checkState()*CheckStateCount), 0, iconWidth, iconHeight));
    painter.drawText(QRect(iconWidth + 5, 0, width() - iconWidth - 5, height()), Qt::AlignLeft|Qt::AlignVCenter, text());
}

void KCheckBox::enterEvent(QEvent *event)
{
    setCursor(Qt::ArrowCursor);
    q_state = Active;

    QCheckBox::enterEvent(event);
}

void KCheckBox::leaveEvent(QEvent *event)
{
    q_state = Normal;

    QCheckBox::leaveEvent(event);
}

void KCheckBox::mouseMoveEvent(QMouseEvent *event)
{
    if (rect().contains(event->pos())) {
        if (event->buttons() & Qt::LeftButton) {
            q_state = Selected;
        }
    } else if (q_state != Normal) {
        q_state = Normal;
    }
    QCheckBox::mouseMoveEvent(event);
}

void KCheckBox::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton) {
        q_state = Selected;
    }
    QCheckBox::mousePressEvent(event);
}

void KCheckBox::mouseReleaseEvent(QMouseEvent *event)
{
    if(q_state == Selected) {
        q_state = Active;
    }
    QCheckBox::mouseReleaseEvent(event);
}
