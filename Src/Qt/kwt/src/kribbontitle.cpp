#include "precomp.h"
#include "kribbontitle.h"

// KRibbonTitle

KRibbonTitle::KRibbonTitle(int height, TitleButtons titleButtons, QWidget *parent) :
    KTitle(height, titleButtons, parent)
{
    setObjectName("KRibbonTitle");
    setFixedHeight(KTitle::height() + 80);
}
