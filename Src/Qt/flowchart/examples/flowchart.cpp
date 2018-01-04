#include "precomp.h"
#include "flowchart.h"

FlowChart::FlowChart(QWidget *parent)
    : QWidget(parent)
{
    resize(800, 480);

    JFlowChart *flowChart = new JFlowChart();
    flowChart->setParent((long)winId());
    flowChart->addListener(this);

    QHBoxLayout* horiLayoutMain = new QHBoxLayout(this);
    horiLayoutMain->setContentsMargins(0, 0, 0, 0);
    horiLayoutMain->addWidget(QWidget::find((WId)flowChart->winId()));
}

bool FlowChart::onLampClicked(const std::string &shapeId, const std::string &lampId)
{
    Q_UNUSED(shapeId);
    Q_UNUSED(lampId);

    return true;
}

bool FlowChart::onShapeClicked(const std::string &shapeId)
{
    Q_UNUSED(shapeId);

    return true;
}
