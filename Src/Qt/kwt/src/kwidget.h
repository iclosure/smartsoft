#ifndef KWIDGET_H
#define KWIDGET_H

#include <QWidget>
#include "kwt_global.h"

class KWT_EXPORT KWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);

public:
    KWidget* parentWidget() const { return reinterpret_cast<KWidget*>(QWidget::parentWidget()); }
};

#endif // KWIDGET_H
