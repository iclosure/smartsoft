#ifndef PRECOMP_H
#define PRECOMP_H

#include <QtCore>
#include <QtGui>

#if (QT_VERSION < 0x050000)
#include <QWidget>
#else
#include <QtWidgets>
#endif

#ifndef JFLOWCHART_MAKEDLL
#include <jflowchart.h>
#endif

#endif // PRECOMP_H
