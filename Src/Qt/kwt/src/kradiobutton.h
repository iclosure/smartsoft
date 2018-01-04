#ifndef KRADIOBUTTON_H
#define KRADIOBUTTON_H

#include <QRadioButton>
#include "kwt_global.h"

class KWT_EXPORT KRadioButton : public QRadioButton
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
public:
    explicit KRadioButton(QWidget *parent = 0);
    explicit KRadioButton(const QString &text, QWidget *parent=0);
    KRadioButton(const QPixmap& pixmap, const QString &text, QWidget *parent=0);

    void setPixmap(const QString &name);
    void setPixmap(const QPixmap &pixmap);

protected:
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    ButtonStates    q_state;
    QPixmap         q_pixmap;

};

#endif // KRADIOBUTTON_H
