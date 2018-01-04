#include "precomp.h"
#include "jflowchart.h"
#include "jflowchart_p.h"


/////////////////////////////////////////////////////////
/// global JFlowChartCorePrivate

class JFlowChartCorePrivate : public QObject
{
    friend class JFlowChartCore;
public:
    explicit JFlowChartCorePrivate(QObject *parent = 0);
    ~JFlowChartCorePrivate();

private:
    QVector<QTranslator*> translators;
};

JFlowChartCorePrivate::JFlowChartCorePrivate(QObject *parent)
    : QObject(parent)
{
}

JFlowChartCorePrivate::~JFlowChartCorePrivate()
{
    if (!qApp) {
        return;
    }

    Q_FOREACH (QTranslator *translator, translators) {
        qApp->removeTranslator(translator);
        delete translator;
    }
}

/////////////////////////////////////////////////////////
/// global JFlowChartCore

JFlowChartCore::JFlowChartCore()
{
    Q_INIT_RESOURCE(jflowchart_images);
    Q_INIT_RESOURCE(jflowchart_lang);
    Q_INIT_RESOURCE(jflowchart_qss);

    d = new JFlowChartCorePrivate();
}

JFlowChartCore::~JFlowChartCore()
{
    Q_CLEANUP_RESOURCE(jflowchart_images);
    Q_CLEANUP_RESOURCE(jflowchart_lang);
    Q_CLEANUP_RESOURCE(jflowchart_qss);

    delete d;
}

bool JFlowChartCore::load(const std::string &systemName)
{
    if (!qApp) {
        return false;
    }

    //
    QFile file(":/jflowchart/qss/stylesheet_1.qss");
    file.open(QFile::ReadOnly);
    qApp->setStyleSheet(file.readAll());
    qApp->setFont(QFont("微软雅黑", 10));

    QString name = systemName.empty() ? QLocale::system().name() : systemName.c_str();

    //
    d->translators.append(new QTranslator());
    bool result = d->translators.last()->load(":/jflowchart/lang/qt_" + name);
    if (result) {
        qApp->installTranslator(d->translators.last());
    }

    //
    d->translators.append(new QTranslator());
    result = d->translators.last()->load(":/jflowchart/lang/" + name);
    if (result) {
        qApp->installTranslator(d->translators.last());
    }

    return true;
}

/////////////////////////////////////////////////////////
/// class JFlowChart

JFlowChart::JFlowChart()
{
    d = new JFlowChartPrivate();
}

JFlowChart::~JFlowChart()
{
    delete d;
}

void JFlowChart::addListener(JFlowChartListener *listener)
{
    if (!listener) {
        return;
    }
    if (d->listeners.indexOf(listener) == -1) {
        d->listeners.append(listener);
    }
}

void JFlowChart::removeListener(JFlowChartListener *&listener)
{
    if (!listener) {
        return;
    }
    int index = d->listeners.indexOf(listener);
    if (index != -1) {
        d->listeners.remove(index);
    }
}

long JFlowChart::winId() const
{
    return (long)d->winId();
}

bool JFlowChart::setParent(long winId)
{
    QWidget* widget = QWidget::find((WId)winId);
    if (!widget) {
        return false;
    }

    d->setParent(widget);

    return true;
}
