#ifndef JFLOWCHART_P_H
#define JFLOWCHART_P_H

#include "jflowchart.h"
#include "jflowchart_global.h"
#include "jview.h"

class QGraphicsView;

class JFlowChartPrivate : public JView
{
    Q_OBJECT
public:
    explicit JFlowChartPrivate(QWidget *parent = 0);

Q_SIGNALS:

public Q_SLOTS:

private:
    QVector<JFlowChartListener*> listeners;
    friend class JFlowChart;
};

#endif // JFLOWCHART_P_H
