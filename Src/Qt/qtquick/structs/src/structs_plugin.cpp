#include "structs_plugin.h"
#include "flowchartdata.h"

#include <qdeclarative.h>

void StructsPlugin::registerTypes(const char *uri)
{
    // @uri com.smartsoft.structs
    qmlRegisterType<FlowChartData>(uri, 1, 1, "FlowChartData");

    //
}

#if QT_VERSION < 0x050000

Q_EXPORT_PLUGIN2(qmlstructsplugin, StructsPlugin)

#endif
