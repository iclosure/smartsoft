#ifndef FLOWCHART_H
#define FLOWCHART_H

#include <QWidget>
#include <jflowchart.h>

class FlowChart
        : public QWidget
        , public JFlowChartListener
{
public:
    FlowChart(QWidget *parent = 0);

    // JFlowChartListener interface
public:
    virtual bool onLampClicked(const std::string &shapeId, const std::string &lampId);
    virtual bool onShapeClicked(const std::string &shapeId);
};

#endif // FLOWCHART_H
