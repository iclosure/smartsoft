#include "precomp.h"
#include "kquickview.h"

KQuickView::KQuickView(QWindow *parent) :
    QQuickView(parent)
{
    setFlags(Qt::FramelessWindowHint);
    setColor(QColor(Qt::transparent));
}

KQuickView::KQuickView(QQmlEngine* engine, QWindow *parent) :
    QQuickView(engine, parent)
{
}

KQuickView::KQuickView(const QUrl &source, QWindow *parent) :
    QQuickView(source, parent)
{
}


KQuickView::~KQuickView()
{

}
