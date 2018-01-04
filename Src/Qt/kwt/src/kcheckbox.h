#ifndef KCHECKBOX_H
#define KCHECKBOX_H

#include <QCheckBox>
#include "kwt_global.h"

class KWT_EXPORT KCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    enum CheckState {
        Normal,
        Active,
        Selected,
        Disabled,

        CheckStateCount
    };
    Q_DECLARE_FLAGS(CheckStates, CheckState)
public:
    explicit KCheckBox(QWidget *parent = 0);
    explicit KCheckBox(const QString &text, QWidget *parent=0);
    KCheckBox(const QPixmap& pixmap, const QString &text, QWidget *parent=0);

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
    CheckStates     q_state;
    QPixmap         q_pixmap;
};

#endif // KCHECKBOX_H
