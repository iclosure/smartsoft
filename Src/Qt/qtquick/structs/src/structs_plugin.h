#ifndef STRUCTS_PLUGIN_H
#define STRUCTS_PLUGIN_H

#include <QDeclarativeExtensionPlugin>

class StructsPlugin : public QDeclarativeExtensionPlugin
{
    //Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.project.Qt.QQmlExtensionInterface")
#endif

public:
    void registerTypes(const char *uri);
};

#endif // STRUCTS_PLUGIN_H
