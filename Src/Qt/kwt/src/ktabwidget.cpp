#include "precomp.h"
#include "ktabwidget.h"

KTabWidget::KTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    setObjectName("KTabWidget");
    setStyleSheet("QTabWidget#KTabWidget { background:transparent; }");
}
