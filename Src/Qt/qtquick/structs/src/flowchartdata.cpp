#include "flowchartdata.h"

#include <qdeclarative.h>

FlowChartData::FlowChartData(QDeclarativeItem *parent):
    QDeclarativeItem(parent)
{
    // By default, QDeclarativeItem does not draw anything. If you subclass
    // QDeclarativeItem to create a visual item, you will need to uncomment the
    // following line:

    // setFlag(ItemHasNoContents, false);

    connect(this, SIGNAL(titleChanged(QString)), this, SLOT(slotTitleChanged(QString)));
}

FlowChartData::~FlowChartData()
{
}

