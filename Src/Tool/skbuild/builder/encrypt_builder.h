#pragma once
#include <direct.h>

int encrypt_builder(const tstring & keyString)
{
	DBG_PRINTF(("start encrypt bat file...\n"));

	tstring batDir;
	TCHAR *buffer;

	if ((buffer = _getcwd(NULL, 0)) == NULL)
	{
		assert(false);
		_tperror(_T("getcwd error"));
		return -1;
	}
	else
	{
		DBG_PRINTF(("buffer: %s\n", buffer));
		batDir = buffer;
		free(buffer);
	}

	tstring desDir(VCCore::toShortPath(VCCore::getEnv(ENV_SMARTKITS_HOME)));

	desDir.append(_T("Bin\\build\\"));

	batDir.append(_T("\\batfile\\"));
	if (_taccess_s(batDir.c_str(), 0))
	{
		assert(false);
		DBG_PRINTF((_T("directory '%s' is not exist!\n"), batDir.c_str()));
		return -1;
	}
	if (_taccess_s(desDir.c_str(), 0))
	{
		_tsystem((_T("@mkdir ") + desDir).c_str());
	}

	encrypt::CDES des;

	int result;

	// encrypt file build_driver.bat
	_tchmod((desDir + KUuid::toString(_v_guid_bat_build_driver)).c_str(), S_IWRITE);
	result = des.encrypt((batDir + "build_driver.bat").c_str(), keyString.c_str(), (desDir + KUuid::toString(_v_guid_bat_build_driver)).c_str());
	if (result != encrypt::DesResultSuccess)
	{
		assert(false);
		DBG_PRINTF(("encrypt file 'build_driver.bat' failed!"));
		return -1;
	}
	_tchmod((desDir + KUuid::toString(_v_guid_bat_build_driver)).c_str(), S_IREAD);

	// encrypt file build_kernel.bat
	_tchmod((desDir + KUuid::toString(_v_guid_bat_build_kernel)).c_str(), S_IWRITE);
	result = des.encrypt((batDir + "build_kernel.bat").c_str(), keyString.c_str(), (desDir + KUuid::toString(_v_guid_bat_build_kernel)).c_str());
	if (result != encrypt::DesResultSuccess)
	{
		assert(false);
		DBG_PRINTF(("encrypt file 'build_kernel.bat' failed!"));
		return -1;
	}
	_tchmod((desDir + KUuid::toString(_v_guid_bat_build_kernel)).c_str(), S_IREAD);

	// encrypt file build_smartkits.bat
	_tchmod((desDir + KUuid::toString(_v_guid_bat_build_smartkits)).c_str(), S_IWRITE);
	result = des.encrypt((batDir + "build_smartkits.bat").c_str(), keyString.c_str(), (desDir + KUuid::toString(_v_guid_bat_build_smartkits)).c_str());
	if (result != encrypt::DesResultSuccess)
	{
		assert(false);
		DBG_PRINTF(("encrypt file 'build_smartkits.bat' failed!"));
		return -1;
	}
	_tchmod((desDir + KUuid::toString(_v_guid_bat_build_smartkits)).c_str(), S_IREAD);

	return 0;
}

tstring decrypt_builder(const tstring & keyString, const TCHAR* fileName, const tstring& desDir)
{
	DBG_PRINTF(("start decrypt bat file '%s' ...\n", fileName));

	tstring srcPath;
	tstring desPath;
	int result;

	srcPath.append(VCCore::toShortPath(VCCore::getEnv(ENV_SMARTKITS_HOME))).append(_T("\\Bin\\build\\")).append(fileName);
	desPath.append(desDir).append(_T("\\")).append(KUuid::guidBySting()).append(_T(".bat"));

	DBG_PRINTF(("srcPath: %s\n", srcPath.c_str()));
	DBG_PRINTF(("desPath: %s\n", desPath.c_str()));

	if (_taccess_s(srcPath.c_str(), 0))
	{
		assert(false);
		DBG_PRINTF(("directory '%s' is not exist!\n", srcPath.c_str()));
		return tstring();
	}

	encrypt::CDES des;

	// decrypt file build_driver.bat
	DBG_PRINTF(("keyString: %s\n", keyString.c_str()));
	_tchmod(srcPath.c_str(), S_IWRITE);
	result = des.decrypt(srcPath.c_str(), keyString.c_str(), desPath.c_str());
	if (result != encrypt::DesResultSuccess)
	{
		assert(false);
		DBG_PRINTF(("encrypt file 'build_driver.bat' failed!"));
		return tstring();
	}
	_tchmod(desPath.c_str(), S_IREAD);

	return desPath;
}
