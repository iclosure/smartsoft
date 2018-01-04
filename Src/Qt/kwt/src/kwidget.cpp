#include "precomp.h"
#include "kwidget.h"

KWidget::KWidget(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f)
{
    setObjectName("KWidget");
}
