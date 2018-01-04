#ifndef KQUICKVIEW_H
#define KQUICKVIEW_H

#include <QQuickView>
#include "kwt_global.h"

class KWT_EXPORT KQuickView : public QQuickView
{
    Q_OBJECT
public:
    explicit KQuickView(QWindow *parent = 0);
    KQuickView(QQmlEngine* engine, QWindow *parent);
    KQuickView(const QUrl &source, QWindow *parent = 0);
    virtual ~KQuickView();

Q_SIGNALS:

public Q_SLOTS:

};

#endif // KQUICKVIEW_H
