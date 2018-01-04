#include "precomp.h"
#include "kpanetitle.h"

// KPaneTitle

KPaneTitle::KPaneTitle(int height, TitleButtons titleButtons, QWidget *parent) :
    KTitle(height, titleButtons, parent)
{
    setObjectName("KPaneTitle");

    q_widgetPane = new KWidget(this);
    q_widgetPane->setAttribute(Qt::WA_TranslucentBackground);
    q_widgetPane->setFixedHeight(90);
    setFixedHeight(KTitle::height() + q_widgetPane->height());

    q_horiLayoutMain = new QHBoxLayout(q_widgetPane);
    q_horiLayoutMain->setSpacing(2);
    q_horiLayoutMain->setContentsMargins(0, 0, 0, 0);
    q_horiLayoutMain->addStretch();

    KTitle::addWidget(q_widgetPane);
}

void KPaneTitle::addPaneWidget(QWidget *widget, Qt::Alignment alignment)
{
    Q_ASSERT(widget);

    if (alignment & Qt::AlignLeft)
    {
        q_listObject.append(widget);
        q_horiLayoutMain->insertWidget(q_listObject.count() - 1, widget, 0, alignment);
    }
    else if (alignment & Qt::AlignRight)
    {
        q_horiLayoutMain->insertWidget(q_listObject.count(), widget, 0, alignment);
    }
    else
    {
        Q_ASSERT(false);
    }
}

void KPaneTitle::removePaneWidget(QWidget *widget)
{
    Q_ASSERT(widget);
    q_listObject.removeOne(widget);
    q_horiLayoutMain->removeWidget(widget);
}

void KPaneTitle::addPaneLayout(QLayout *layout, Qt::Alignment alignment)
{
    Q_ASSERT(layout);

    if (alignment & Qt::AlignLeft)
    {
        q_listObject.append(layout);
        q_horiLayoutMain->insertLayout(q_listObject.count() - 1, layout);
    }
    else if (alignment & Qt::AlignRight)
    {
        q_horiLayoutMain->insertLayout(q_listObject.count(), layout);
    }
    else
    {
        Q_ASSERT(false);
    }
}

void KPaneTitle::removePaneLayout(QLayout *layout)
{
    Q_ASSERT(layout);
    q_listObject.removeOne(layout);
    q_horiLayoutMain->removeItem(layout);
}
