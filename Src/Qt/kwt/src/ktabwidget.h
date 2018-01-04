#ifndef KTABWIDGET_H
#define KTABWIDGET_H

#include <QTabWidget>
#include "kwt_global.h"

class KWT_EXPORT KTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit KTabWidget(QWidget *parent = 0);
};

#endif // KTABWIDGET_H
