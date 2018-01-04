#ifndef KWT_GLOBAL_H
#define KWT_GLOBAL_H

#include <qglobal.h>

// KWT_VERSION is (major << 16) + (minor << 8) + patch.

#define KWT_VERSION       0x030000
#define KWT_VERSION_STR   "3.0.0"

#if defined(Q_WS_WIN) || defined(Q_WS_S60)

#if defined(_MSC_VER) // MSVC Compiler
// template-class specialization 'identifier' is already instantiated
#pragma warning(disable: 4660)
#pragma warning(disable: 4819)  // warning : 936
#endif // _MSC_VER

#ifdef KWT_DLL

#if defined(KWT_MAKEDLL)     // create a Qwt DLL library
#define KWT_EXPORT  Q_DECL_EXPORT
#define KWT_TEMPLATEDLL
#else                        // use a Qwt DLL library
#define KWT_EXPORT  Q_DECL_IMPORT
#endif

#endif // KWT_DLL

#endif // Q_WS_WIN || Q_WS_S60

#ifndef KWT_EXPORT
#define KWT_EXPORT
#endif

#include <QString>
#include <QApplication>
#include <QVariant>

// define Q_NULLPTR
#ifndef Q_NULLPTR
#define Q_NULLPTR (0)
#endif  // Q_NULLPTR

// define Q_DECL_FINAL
#if QT_VERSION < 0x050000
#ifndef Q_DECL_FINAL
#define Q_DECL_FINAL
#endif
#endif

class KwtCorePrivate;
class KWT_EXPORT KwtCore
{
public:
    explicit KwtCore();
    ~KwtCore();

    bool load(const QString & systemName = QString());

private:
    Q_DISABLE_COPY(KwtCore)
    KwtCorePrivate* d;
};

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
    o->setProperty(name, QVariant::fromValue<void*>(t));
    return t;
}

template<typename T>
inline T qObjectProperity(QObject *o, const char* name) {
    Q_ASSERT(o && name);
    return reinterpret_cast<T>(o->property(name).value<void*>());
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

#endif // KWT_GLOBAL_H
