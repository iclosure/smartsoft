#ifndef KSYSTEMMENU_H
#define KSYSTEMMENU_H

#include <QMenu>
#include "ktitle.h"

class KSystemMenu : public QMenu
{
    Q_OBJECT
public:
    explicit KSystemMenu(KTitle::TitleButtons titleButtons, QWidget *parent = 0);

private Q_SLOTS:
    void slotShowNormalParent();
    void slotShowMinimizeParent();
    void slotShowMaximizeParent();
    void slotCloseParent();
};

#endif // KSYSTEMMENU_H
