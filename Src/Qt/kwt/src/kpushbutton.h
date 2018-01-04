#ifndef KPUSHBUTTON_H
#define KPUSHBUTTON_H

#include <QPushButton>
#include "kwt_global.h"

class QWidget;

class KWT_EXPORT KPushButton : public QPushButton
{
    Q_OBJECT
public:
    enum ButtonState {
        Normal,
        Active,
        Selected,
        Disabled,

        ButtonStateCount
    };
    Q_DECLARE_FLAGS(ButtonStates, ButtonState)

    explicit KPushButton(QWidget *parent=0);
    explicit KPushButton(const QString &text, QWidget *parent=0);
    KPushButton(const QIcon& icon, const QString &text, QWidget *parent=0);

    void setPixmap(const QString &name);
    void setPixmap(QPixmap &pixmap);

protected:
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    int             q_picWidth;
    int             q_picHeight;
    ButtonStates    q_state;
    QPixmap         q_pixmap;
};

#endif // KPUSHBUTTON_H
