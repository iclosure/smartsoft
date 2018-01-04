#pragma once

#include "encrypt_builder.h"

int build_target(int argc, const TCHAR* const argv[], const tstring & keyString, const GUID &guidName)
{
	int result;

	// 
	tstring desDir(VCCore::toShortPath(VCCore::getEnv(_T("TEMP"))) + _T("\\"));

	desDir.append(KUuid::toString(_v_guid_bat_temp_folder)).append(_T(".tmp\\")).append(KUuid::guidBySting());
	if (_access_s(desDir.c_str(), 0))
	{
		_tsystem((_T("@mkdir ") + desDir).c_str());
	}

	// 
	const tstring batPath = decrypt_builder(keyString, KUuid::toString(guidName).c_str(), desDir);
	if (batPath.empty())
	{
		return -1;
	}

	// 
	tstring cmd(_T("call ") + batPath);

	for (int i = 1; i < argc; i++)
	{
		cmd.append(_T(" ")).append(argv[i]);
	}

	//DBG_PRINTF(("cmd: %s\n", cmd.c_str()));

	result = _tsystem(cmd.c_str());
	DBG_PRINTF(("call::result: %d\n", result));

	// delete destination folder

	if (!_taccess_s(desDir.c_str(), 0))
	{
		result = _tsystem((_T("@rmdir /s /q ") + desDir).c_str());
		DBG_PRINTF(("rmdir::result: %d\n", result));
	}

	return result;
}

int build_driver(int argc, const TCHAR* const argv[], const tstring & keyString)
{
	return build_target(argc, argv, keyString, _v_guid_bat_build_driver);
}

int build_kernel(int argc, const TCHAR* const argv[], const tstring & keyString)
{
	return build_target(argc, argv, keyString, _v_guid_bat_build_kernel);
}

int build_smartkits(int argc, const TCHAR* const argv[], const tstring & keyString)
{
	return build_target(argc, argv, keyString, _v_guid_bat_build_smartkits);
}
