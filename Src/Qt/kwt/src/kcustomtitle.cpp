#include "precomp.h"
#include "kcustomtitle.h"

// KToolTitle

KCustomTitle::KCustomTitle(int height, TitleButtons titleButtons, QWidget *parent) :
    KTitle(height, titleButtons, parent)
{
    setObjectName("KCustomTitle");
    setFixedHeight( KTitle::height());
}
