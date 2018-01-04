#ifndef KSTYLESHEET_H
#define KSTYLESHEET_H

#include <QObject>
#include "kwt_global.h"

class QWidget;

class KWT_EXPORT KStyleSheet : public QObject
{
    Q_OBJECT
public:
    explicit KStyleSheet(QObject *parent = 0);

    static void setButtonStyle(QWidget* widget, const QString &image, int  cutSec, int width = -1, int height = -1);
};

#endif // KSTYLESHEET_H
