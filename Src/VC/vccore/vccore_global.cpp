#include "stdafx.h"
#include "vccore_global.h"
#include <ObjBase.h>
#include <WinError.h>
#include <algorithm>
#include <assert.h>

#if defined(_DEBUG)

int __stdcall __HrTraceA(__in const char* Format, ...)
{
	va_list ap;
	char* szData = NULL;
	int nRetVal = -1;

	va_start(ap, Format);

	for (int nBufferSize = 1024; nRetVal < 0; nBufferSize *= 2)
	{
		if (szData != NULL)
		{
			free(szData);
		}
		szData = (char*)malloc((nBufferSize + 1) * sizeof(char));
		nRetVal = _vsnprintf_s(szData, nBufferSize, _TRUNCATE, Format, ap);
	}

	va_end(ap);

	OutputDebugStringA(szData);
	free(szData);

	return nRetVal;
}

int __stdcall __HrTraceW(__in const wchar_t* Format, ...)
{
	va_list ap;
	wchar_t* szData = NULL;
	int nRetVal = -1;

	va_start(ap, Format);

	for (int nBufferSize = 1024; nRetVal < 0; nBufferSize *= 2)
	{
		if (szData != NULL)
		{
			free(szData);
		}
		szData = (wchar_t*)malloc((nBufferSize + 1) * sizeof(wchar_t));
		nRetVal = _vsnwprintf_s(szData, nBufferSize, _TRUNCATE, Format, ap);
	}

	va_end(ap);

	OutputDebugStringW(szData);
	free(szData);

	return nRetVal;
}

#endif  // _DEBUG

/*
* KUuid
*/

GUID KUuid::guid()
{
	GUID guid;

	if (S_OK == ::CoCreateGuid(&guid))
	{
		return guid;
	}
	else
	{
		assert(false);
		return guid;
	}
}

tstring KUuid::guidBySting()
{
	return toString(KUuid::guid());
}

tstring KUuid::toString(const GUID &guid)
{
	TCHAR buffer[35];

	_stprintf_s(buffer, _T("{%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x}"),
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	return buffer;
}

/*
* VCCore
*/

tstring __stdcall VCCore::getEnv(const TCHAR* envvar)
{
	TCHAR* libvar;
	size_t requiredSize;

	errno_t err = _tgetenv_s(&requiredSize, NULL, 0, envvar);
	if (err || requiredSize == 0)
	{
		return tstring();
	}

	libvar = new TCHAR[requiredSize];
	if (!libvar)
	{
		return tstring();
	}

	err = _tgetenv_s(&requiredSize, libvar, requiredSize, envvar);
	if (err || requiredSize == 0)
	{
		delete[] libvar;
		return tstring();
	}

	tstring ret(libvar);

	delete[] libvar;

	return ret;
}

tstring __stdcall VCCore::getEnv(const tstring& envvar)
{
	return getEnv(envvar.c_str());
}

tstring __stdcall VCCore::toShortPath(const TCHAR* path)
{
	if (tstring(path).find(_T(' '), 0) == tstring::npos)
	{
		return path;
	}

	DWORD count = GetShortPathName(path, NULL, 0);

	if (count == 0)
	{
		return path;
	}

	TCHAR * buffer = new TCHAR[(count + 1) * sizeof(TCHAR)];

	count = GetShortPathName(path, buffer, count);

	tstring result(buffer);

	std::transform(result.begin(), result.end(), result.begin(), tolower); 

	delete[] buffer;

	return result;
}

tstring __stdcall VCCore::toShortPath(const tstring & path)
{
	return toShortPath(path.c_str());
}
