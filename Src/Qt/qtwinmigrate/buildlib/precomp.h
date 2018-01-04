#ifndef PRECOMP_H
#define PRECOMP_H

#include <QtCore>

#if QT_VERSION < 0x050000

#ifndef HWND_CAST
#define HWND_CAST(wid) (wid)
#endif

#ifndef WID_CAST
#define WID_CAST(hwnd) (hwnd)
#endif

#else

#include <QtWidgets/QWidget>

#ifndef HWND_CAST
#define HWND_CAST(wid) (HWND)(wid)
#endif

#ifndef WID_CAST
#define WID_CAST(hwnd) (WId)(hwnd)
#endif

#if QT_VERSION >= 0x050000
#define QT_WA(unicode, ansi) unicode
#endif

#endif

#endif // PRECOMP_H
