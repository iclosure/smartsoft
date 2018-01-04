#include "precomp.h"
#include "ksystemmenu.h"
#include "kdialog.h"

KSystemMenu::KSystemMenu(KTitle::TitleButtons titleButtons, QWidget *parent) :
    QMenu(parent)
{
    setObjectName("KSystemMenu");

    if (titleButtons & KTitle::Maximized) {
        QAction* actionRestore = new QAction(QIcon(":/kwt/images/sysmenu/restore"), tr("&Restore"), this);
        addAction(actionRestore);
        actionRestore->setEnabled(parent->isMaximized());
        connect(actionRestore, SIGNAL(triggered()), this, SLOT(slotShowNormalParent()));
    }
    // TODO
    QAction* actionMove = new QAction(tr("&Move"), this);
    QAction* actionSize = new QAction(tr("&Size"), this);
    addAction(actionMove);
    addAction(actionSize);
    actionMove->setEnabled(false);
    actionSize->setEnabled(false);

    if (titleButtons & KTitle::Minimized) {
        QAction* actionMinimize = new QAction(QIcon(":/kwt/images/sysmenu/min"), tr("M&inimize"), this);
        addAction(actionMinimize);
        connect(actionMinimize, SIGNAL(triggered()), this, SLOT(slotShowMinimizeParent()));
    }

    if (titleButtons & KTitle::Maximized) {
        QAction* actionMaximize = new QAction(QIcon(":/kwt/images/sysmenu/max"), tr("M&aximize"), this);
        addAction(actionMaximize);
        actionMaximize->setEnabled(!parent->isMaximized());
        connect(actionMaximize, SIGNAL(triggered()), this, SLOT(slotShowMaximizeParent()));
    }

    if (titleButtons & KTitle::Close) {
        QAction* actionClose = new QAction(QIcon(":/kwt/images/sysmenu/close"), tr("Cl&ose"), this);
        addAction(actionClose);
        connect(actionClose, SIGNAL(triggered()), this, SLOT(slotCloseParent()));
    }
}

// slot functions

// show parent normal
void KSystemMenu::slotShowNormalParent()
{
    parent()->inherits("KDialog") ? reinterpret_cast<KDialog*>(parent())->showNormal()
                                : parentWidget()->showNormal();
}

// show parent minimize
void KSystemMenu::slotShowMinimizeParent()
{
    parent()->inherits("KDialog") ? reinterpret_cast<KDialog*>(parent())->showMinimized()
                                : parentWidget()->showMinimized();
}

// show parent maximize
void KSystemMenu::slotShowMaximizeParent()
{
    parent()->inherits("KDialog") ? Q_EMIT reinterpret_cast<KDialog*>(parent())->titleWidget()->showClicked()
                                : parentWidget()->showMaximized();
}

// close parent
void KSystemMenu::slotCloseParent()
{
    parent()->inherits("KDialog") ? reinterpret_cast<KDialog*>(parent())->close()
                                  : parentWidget()->close();
}
