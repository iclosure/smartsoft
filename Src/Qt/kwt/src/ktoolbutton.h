#ifndef KTOOLBUTTON_H
#define KTOOLBUTTON_H

#include <QToolButton>
#include "kwt_global.h"

class KWT_EXPORT KToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit KToolButton(QWidget *parent = 0);
    explicit KToolButton(QPixmap &pixmap, const QString &text = QString(), QWidget *parent = 0);

public:
    void select(bool on = true);

protected:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *);

private:
    bool            q_mouseHover;       // 鼠标是否移过
    bool            q_mousePress;       // 鼠标是否按下
    QPixmap         q_bkgPixmap;        // 按钮背景图片
};

#endif // KTOOLBUTTON_H
