#ifndef KCUSTOMTITLE_H
#define KCUSTOMTITLE_H

#include "ktitle.h"

class KWT_EXPORT KCustomTitle : public KTitle
{
    Q_OBJECT
public:
    explicit KCustomTitle(int height, TitleButtons titleButtons = 0, QWidget *parent = 0);

private:
    int             q_titleHeight; //
};

#endif // KCUSTOMTITLE_H
