#include "precomp.h"
#include "kline.h"

//
// class LineWidget
//

class LineWidget : public QWidget
{
    Q_DISABLE_COPY(LineWidget)
public:
    LineWidget(QWidget* parent = 0) :
        QWidget(parent),
        q_line(0, 0 ,0, 0)
    {
        setAttribute(Qt::WA_NoSystemBackground);
    }

protected:
    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);

        painter.drawLine(q_line);
    }

private:
    QLineF q_line;

    friend class KLine;
};

//
// class KLine
//

KLine::KLine(QGraphicsItem* parent) :
    QGraphicsProxyWidget(parent)
{
    q_lineWidget = new LineWidget();
    setWidget(q_lineWidget);
}

QPointF KLine::from() const
{
    return q_lineWidget->q_line.p1();
}

void KLine::setFrom(const QPointF& point)
{
    q_lineWidget->q_line.setP1(point);
    q_lineWidget->repaint();
}

QPointF KLine::to() const
{
    return q_lineWidget->q_line.p2();
}

void KLine::setTo(const QPointF& point)
{
    q_lineWidget->q_line.setP2(point);
    q_lineWidget->repaint();
}
