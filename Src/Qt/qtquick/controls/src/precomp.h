#ifndef PRECOMP_H
#define PRECOMP_H

#include <QtCore>
#include <QtGui>

#if (QT_VERSION < 0x050000)

#include <QWidget>

#ifdef QT_WIDGETS_LIB
#include <QtWidgets>
#endif

#endif

#ifdef QT_DECLARATIVE_LIB
#include <QtDeclarative>
#endif

#endif // PRECOMP_H
