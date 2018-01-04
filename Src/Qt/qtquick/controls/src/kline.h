#ifndef KLINE_H
#define KLINE_H

#include <QDeclarativeItem>
#include <QGraphicsProxyWidget>

class LineWidget;

class KLine : public QGraphicsProxyWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(KLine)
    Q_PROPERTY(QPointF from READ from WRITE setFrom)
    Q_PROPERTY(QPointF to READ to WRITE setTo)

public:
    explicit KLine(QGraphicsItem* parent = 0);

    QPointF from() const;
    void setFrom(const QPointF&);

    QPointF to() const;
    void setTo(const QPointF&);

Q_SIGNALS:

private:
    LineWidget *q_lineWidget;
};

#endif // KLINE_H
