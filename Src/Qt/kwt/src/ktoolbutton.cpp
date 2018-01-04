#include "precomp.h"
#include "ktoolbutton.h"

KToolButton::KToolButton(QWidget *parent) :
    QToolButton()
{
    KToolButton(QPixmap(), QString(), parent);
}

KToolButton::KToolButton(QPixmap &pixmap, const QString &text, QWidget *parent) :
    QToolButton(parent),
    q_mouseHover ( false ),
    q_mousePress ( false )
{
    setObjectName("KToolButton");
    //setAttribute(Qt::WA_TranslucentBackground);
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet("QToolButton#KToolButton { background:transparent; }");

    setText(text);

    //
    q_bkgPixmap.load(":/kwt/images/tool_button/bkg");

    // 设置文本颜色
    QPalette textPalette = palette();
    textPalette.setColor(QPalette::ButtonText, QColor(230, 230, 230));
    setPalette(textPalette);

    // 设置文本粗体
    const_cast<QFont &>(font()).setWeight(QFont::Bold);

    // 设置图标
    setIcon(pixmap);
    setIconSize(pixmap.size());

    // 设置大小
    //setFixedSize(pixmap.width() + 25, pixmap.height() + 27);
    setAutoRaise(true);
}

void KToolButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if(q_mouseHover)
    {
        painter.drawPixmap(rect(), q_bkgPixmap, QRect(0, 0, q_bkgPixmap.width() / 2, q_bkgPixmap.height()));
    } else if (q_mousePress) {
        painter.drawPixmap(rect(), q_bkgPixmap, QRect(q_bkgPixmap.width() / 2, 0,
                                                      q_bkgPixmap.width() / 2, q_bkgPixmap.height()));
    }

    QToolButton::paintEvent(event);
}

void KToolButton::enterEvent(QEvent *)
{
    q_mouseHover = true;
}

void KToolButton::leaveEvent(QEvent *)
{
    q_mouseHover = false;
}

void KToolButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        q_mouseHover = false;
        Q_EMIT clicked();
    }
}

void KToolButton::select(bool on)
{
    q_mousePress = on;
    update();
}

void KToolButton::mouseDoubleClickEvent(QMouseEvent *)
{

}
