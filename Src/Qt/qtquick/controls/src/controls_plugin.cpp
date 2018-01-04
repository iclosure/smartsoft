#include "precomp.h"
#include "controls_plugin.h"
#include "kline.h"

void ControlsPlugin::registerTypes(const char *uri)
{
    // @uri com.smartsoft.controls

    //
}

#if QT_VERSION < 0x050000

Q_EXPORT_PLUGIN2(qmlcontrolsplugin, ControlsPlugin)

#endif
