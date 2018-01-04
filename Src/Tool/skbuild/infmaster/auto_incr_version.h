
#pragma once

#include <Windows.h>
#include <string>

int auto_incr_version(
	const _TCHAR* baseDir,
	const _TCHAR* targetDir,
	const _TCHAR* targetName,
	const _TCHAR* configType)
{
	std::basic_string<_TCHAR> inxPath(baseDir);
	std::basic_string<_TCHAR> infPath(targetDir);

	inxPath.append(1, _T('\\'));
	inxPath.append(targetName);
	inxPath.append(_T(".inx"));

	infPath.append(1, _T('\\'));
	infPath.append(targetName);
	infPath.append(_T(".inf"));

	DBG_PRINTF((_T("\nINFMASTER: inx file: %s\n"), inxPath.c_str()));
	DBG_PRINTF((_T("\nINFMASTER: inf file: %s\n"), infPath.c_str()));

	_TCHAR str[50];
	DWORD status;

	status = GetPrivateProfileString(_T("Version"), _T("DriverVer"), _T(""), str, 50, inxPath.c_str());

	DBG_PRINTF((_T("\nINFMASTER: GetPrivateProfileString return: %08X\n"), status));

	_tprintf_s(_T("\nINFMASTER: Last version: %s\n"), str);

	struct {
		WORD wYear;
		WORD wMonth;
		WORD wDay;
		DWORD dwMajor;
		DWORD dwMinor;
		DWORD dwPatch;
		DWORD dwBuild;
	} version = { 0 };

	_stscanf_s(str, _T("%02d/%02d/%04d,%d.%d.%d.%d"),
		&version.wMonth,
		&version.wDay,
		&version.wYear,
		&version.dwMajor,
		&version.dwMinor,
		&version.dwPatch,
		&version.dwBuild);

	////////////////////////////////////////////////////////////////////////////

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	version.wYear = sysTime.wYear;
	version.wMonth = sysTime.wMonth;
	version.wDay = sysTime.wDay;

	if (std::basic_string<_TCHAR>(configType).compare(_T("chk")) == 0)
	{
		version.dwBuild++;
		if (version.dwBuild > 99999)
		{
			version.dwBuild = 0;
			version.dwPatch++;

			if (version.dwPatch > 9999)
			{
				version.dwPatch = 0;
				version.dwMinor++;

				if (version.dwMinor > 100)
				{
					version.dwMinor = 0;
					version.dwMajor++;
				}
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
	
	_stprintf_s(str, 50, _T("%02d/%02d/%04d,%d.%d.%04d.%05d"),
		version.wMonth,
		version.wDay,
		version.wYear,
		version.dwMajor,
		version.dwMinor,
		version.dwPatch,
		version.dwBuild);

	status = WritePrivateProfileString(_T("Version"), _T("DriverVer"), str, inxPath.c_str());

	DBG_PRINTF((_T("\nINFMASTER: WritePrivateProfileString return: %08X\n"), status));

	_tprintf_s(_T("\nINFMASTER: Curr version: %s\n"), str);

	status = WritePrivateProfileString(_T("Version"), _T("DriverVer"), str, infPath.c_str());

	return 0;
}
