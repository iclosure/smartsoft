#ifndef KDECLARATIVEVIEW_H
#define KDECLARATIVEVIEW_H

#include <QDeclarativeView>
#include "kwt_global.h"

class KWT_EXPORT KDeclarativeView : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit KDeclarativeView(QWidget *parent = 0);

Q_SIGNALS:

public Q_SLOTS:

};

#endif // KDECLARATIVEVIEW_H
