#include "precomp.h"
#include "kstatusbar.h"
#include "kshake.h"

KStatusBar::KStatusBar(QWidget *parent) :
    QStatusBar(parent)
{
    setObjectName("KStatusBar");
    setFixedHeight(height());
}
