#ifndef KPANETITLE_H
#define KPANETITLE_H

#include "ktitle.h"
#include <QList>

class QLayout;
class QHBoxLayout;

// KPaneTitle class
class KWT_EXPORT KPaneTitle : public KTitle
{
    Q_OBJECT
public:
    explicit KPaneTitle(int height, TitleButtons titleButtons = 0, QWidget *parent = 0);

    void addPaneWidget(QWidget *widget, Qt::Alignment alignment = Qt::AlignLeft) Q_DECL_FINAL;
    void removePaneWidget(QWidget *widget) Q_DECL_FINAL;
    void addPaneLayout(QLayout *layout, Qt::Alignment alignment = Qt::AlignLeft) Q_DECL_FINAL;
    void removePaneLayout(QLayout *layout) Q_DECL_FINAL;

private:
    QHBoxLayout*            q_horiLayoutMain;   //
    QList<QObject*>         q_listObject;       //
    KWidget*                q_widgetPane;       //
};

#endif // KPANETITLE_H
