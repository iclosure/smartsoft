#ifndef KDRAGPROXY_H
#define KDRAGPROXY_H

#include <QObject>
#include <QRect>
#include "kwt_global.h"

class QWidget;

class KWT_EXPORT KDragProxy : public QObject
{
    Q_OBJECT
public:
    explicit KDragProxy(QWidget *parent = 0);

public:
    void setBorder(int left, int top, int right, int bottom);

protected:
    enum WidgetRegion {
        TopLeft,
        Top,
        TopRight,
        Left,
        Inner,
        Right,
        BottomLeft,
        Bottom,
        BottomRight,
        Unknown
    };

    virtual bool eventFilter(QObject *obj, QEvent *event);

    void makeRegions();
    WidgetRegion hitTest(const QPoint& pos);
    void updateGeometry(int x, int y, int w, int h);
    void startCursorTimer();
    void stopCursorTimer();

private:
    int             q_leftWidth;
    int             q_topWidth;
    int             q_rightWidth;
    int             q_bottomWidth;
    QRect           q_regions[9];
    QPoint          q_originPosGlobal;
    QRect           q_originGeometry;
    bool            q_mousePressed;
    WidgetRegion    q_regionPressed;
    int             q_cursorTimerId;
    QWidget*        q_proxyWidget;
};

#endif // KDRAGPROXY_H
