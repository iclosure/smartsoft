#ifndef KRIBBONTITLE_H
#define KRIBBONTITLE_H

#include "ktitle.h"

class KWT_EXPORT KRibbonTitle : public KTitle
{
    Q_OBJECT
public:
    explicit KRibbonTitle(int height, TitleButtons titleButtons = 0, QWidget *parent = 0);

private:
};

#endif // KRIBBONTITLE_H
