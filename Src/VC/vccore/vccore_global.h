#ifndef HR_VCCORE_GLOBAL_H
#define HR_VCCORE_GLOBAL_H

// VCCORE_VERSION is (major << 16) + (minor << 8) + patch.

#define VCCORE_VERSION       0x010100
#define VCCORE_VERSION_STR   "1.1.0"

#if defined(Q_WS_WIN) || defined(Q_WS_S60)

#include <qglobal.h>

#endif // Q_WS_WIN || Q_WS_S60

#if defined(_MSC_VER) /* MSVC Compiler */
/* template-class specialization 'identifier' is already instantiated */
#pragma warning(disable: 4660)
#endif // _MSC_VER

#ifdef VCCORE_DLL
/*
#if defined(VCCORE_MAKEDLL)     // create a VCCORE dll library
#define VCCORE_EXPORT  __declspec(dllexport)
#define VCCORE_TEMPLATEDLL
#else                        // use a VCCORE dll library
#define VCCORE_EXPORT  __declspec(dllimport)
#endif
*/
#endif // VCCORE_DLL

#ifndef VCCORE_EXPORT
#define VCCORE_EXPORT
#endif

#include <stddef.h>
#define hr_offsetof(s, m) offsetof(s, m)
#define hr_sizeof(s, m) sizeof(((s *)0)->m)

#ifndef HR_TSTRING
#include <string>
#include <tchar.h>
#define HR_TSTRING
typedef std::basic_string<TCHAR> tstring;
#endif  // HR_TSTRING

#ifndef DBG_PRINTF
#if defined(_DEBUG)
#define DBG_PRINTF(_x_) _tprintf_s _x_
#else
#define DBG_PRINTF(_x_)
#endif  // !_DEBUG
#endif  // DBG_PRINTF

#if defined(_DEBUG)

int __stdcall __HrTraceA(__in const char* Format, ...);
int __stdcall __HrTraceW(__in const wchar_t* Format, ...);
#define hrtrace(_x_) __HrTraceA _x_ 
#define whrtrace(_x_) __HrTraceW _x_ 

#ifdef UNICODE
#define _thrtrace(_x_) whrtrace(_x_)
#else
#define _thrtrace(_x_) hrtrace(_x_)
#endif  // !UNICODE

#else

#define hrtrace(_x_)
#define whrtrace(_x_)
#define _thrtrace(_x_)

#endif  // !_DEBUG

/*
   Some classes do not permit copies to be made of an object. These
   classes contains a private copy constructor and assignment
   operator to disable copying (the compiler gives an error message).
*/
#ifndef HR_DISABLE_COPY
#define HR_DISABLE_COPY(Class) \
	Class(const Class &); \
	Class& operator=(const Class &);
#endif  // HR_DISABLE_COPY

/*
* KUuid
*/

#include <guiddef.h>

class VCCORE_EXPORT KUuid
{
public:

	static GUID guid();
	static tstring guidBySting();
	static tstring toString(const GUID &guid);

private:
	HR_DISABLE_COPY(KUuid);
};

/*
* VCCore
*/

class VCCORE_EXPORT VCCore
{
public:

	static tstring __stdcall getEnv(const TCHAR* envvar);
	static tstring __stdcall getEnv(const tstring & envvar);

	static tstring __stdcall toShortPath(const TCHAR* path);
	static tstring __stdcall toShortPath(const tstring & path);

private:
	HR_DISABLE_COPY(VCCore);
};

#endif // HR_VCCORE_GLOBAL_H
