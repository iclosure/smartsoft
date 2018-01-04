#ifndef JFLOWCHART_GLOBAL_H
#define JFLOWCHART_GLOBAL_H

#include <qglobal.h>
#include <QApplication>
#include <QGraphicsItem>
#include "jflowchart.h"

///////////////////////////////////////////////////////////////

// properity - QApplication

template<typename T>
inline T qAppSetProperity(T t, QString name = QString()) {
    Q_ASSERT(t && t->inherits("QObject"));
    qApp->setProperty((name.isEmpty() ? t->objectName() : name).toStdString().c_str(), QVariant::fromValue<T>(t));
    return t;
}

template<typename T>
inline T qAppProperity(const char* name) {
    Q_ASSERT(name);
    return qApp->property(name).value<T>();
}

inline bool qAppProperityIsValid(const char* name) {
    Q_ASSERT(name);
    return qApp->property(name).isValid();
}

// properity - QObject

template<typename T>
inline bool qObjectSetProperity(QObject *o, T t, const char* name) {
    Q_ASSERT(o && t && name);
    o->setProperty(name, QVariant::fromValue<T>(t));
    return t;
}

template<typename T>
inline T qObjectProperity(QObject *o, const char* name) {
    Q_ASSERT(o && name);
    return o->property(name).value<T>();
}

inline bool qObjectProperityIsValid(QObject *o, const char* name) {
    Q_ASSERT(o && name);
    return o->property(name).isValid();
}

///////////////////////////////////////////////////////////////

//
namespace klib
{
    static const double kDoubleEpsion = 1E-6;
    static const double kFDoubleEpsion = 1E-6f;

    // ==
    static inline bool fEqual(double a, double b)
    { return (a - b > -kDoubleEpsion || a - b < kDoubleEpsion) ? true : false; }
    static inline bool fEqual(float a, float b)
    { return (a - b > -kFDoubleEpsion || a - b < kFDoubleEpsion) ? true : false; }

    // >
    static inline bool fGreater(double a, double b)
    { return (a - b > kDoubleEpsion) ? true : false; }
    static inline bool fGreater(float a, float b)
    { return (a - b > kFDoubleEpsion) ? true : false; }

    // <
    static bool fLess(double a, double b)
    { return (a - b < -kDoubleEpsion) ? true : false; }
    static bool fLess(float a, float b)
    { return (a - b < -kFDoubleEpsion) ? true : false; }

    // >=
    static bool fGreaterOrEqual(double a, double b)
    { return ((a - b > kDoubleEpsion) ||  (a - b >= -kDoubleEpsion && a - b <= kDoubleEpsion)) ? true : false; }
    static bool fGreaterOrEqual(float a, float b)
    { return ((a - b > kFDoubleEpsion) ||  (a - b >= -kFDoubleEpsion && a - b <= kFDoubleEpsion)) ? true : false; }

    // <=
    static bool fLessOrEqual(double a, double b)
    { return ((a - b < -kDoubleEpsion) || (a - b >= -kDoubleEpsion && a - b <= kDoubleEpsion)) ? true : false; }
    static bool fLessOrEqual(float a, float b)
    { return ((a - b < -kFDoubleEpsion) || (a - b >= -kFDoubleEpsion && a - b <= kFDoubleEpsion)) ? true : false; }
}

///////////////////////////////////////////////////////////////

namespace FlowChart {

    //
    enum ItemType{
        ItemShape = QGraphicsItem::UserType + 1,  // JShape
        ItemLinker,  // JLinker
        ItemLamp,    // JLamp
    };
}

#endif // JFLOWCHART_GLOBAL_H
