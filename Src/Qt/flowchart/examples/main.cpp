#include "precomp.h"
#include <QTextCodec>
#include "flowchart.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
/*
    QFile file(":/qss/stylesheet-1");
    file.open(QFile::ReadOnly);
    app.setStyleSheet(file.readAll());
    app.setFont(QFont("微软雅黑", 10));
*/
    //
    JFlowChartCore flowChartCore;
    flowChartCore.load(QLocale::system().name().toStdString());
    QTranslator translator;
    translator.load(":/lang/" + QString("zh_CN"));
    app.installTranslator(&translator);

    //
    app.setOrganizationDomain("smartsoft.com");
    app.setOrganizationName("Smartsoft");
    app.setApplicationName("FlowChart Demo");
    app.setApplicationVersion(QString("%1.%2.%3")
                                   .arg(TARGET_VER_MAJ)
                                   .arg(TARGET_VER_MIN)
                                   .arg(TARGET_VER_PAT));
    //
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    FlowChart flowChart;
    flowChart.show();

    return app.exec();
}
