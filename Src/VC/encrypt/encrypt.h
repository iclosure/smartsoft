
#ifndef ENCRYPT_HEADER_
#define ENVRYPT_HEADER_

#define _ENCRYPT_LIBNAME_1_	"encrypt"

#ifdef _UNICODE
#define _ENCRYPT_LIBNAME_2_	_ENCRYPT_LIBNAME_1_##"u"
#else
#define _ENCRYPT_LIBNAME_2_	_ENCRYPT_LIBNAME_1_
#endif

#ifdef _DEBUG
#define _ENCRYPT_LIBNAME_3_	_ENCRYPT_LIBNAME_2_##"d"
#else
#define _ENCRYPT_LIBNAME_3_	_ENCRYPT_LIBNAME_2_
#endif

#define _ENCRYPT_LIBNAME_		_ENCRYPT_LIBNAME_3_##".lib"

#pragma comment(lib, _ENCRYPT_LIBNAME_)

#ifndef _ENCRYPT_HIDE_AUTOLINK_OUTPUT_
#pragma comment (lib, _ENCRYPT_LIBNAME_)
#pragma message("Automatically linking with " _ENCRYPT_LIBNAME_)
#endif	// _ENCRYPT_HIDE_AUTOLINK_OUTPUT_

#include "des.h"
#include "BiosInfo.h"
#include "CPUID.h"
#include "DiskSerial.h"
#include "EthernetMac.h"
#include "SystemInfo.h"
#include "md5.h"

#endif	// ENCRYPT_HEADER_
