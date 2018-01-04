// skbuild.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <stdio.h>
#include <sys\stat.h>
#include <io.h>
#include <process.h>
#include <direct.h>
#include <assert.h>
#include <list>

// builder
#include "builder/build_target.h"

// infmaster
#include "infmaster/infmaster.h"

///
/// argv[0]: execute name
/// argv[1]: BuildEvent
/// argv[2]: SolutionDir
/// argv[3]: ProjectDir
/// argv[4]: SourceDir
/// argv[5]: ...
/// ...
/// 

int main(int argc, _TCHAR* argv[])
{
	tstring keyString = KUuid::toString(_v_guid_bat_key);

	if (argc == 1)  // no argument
	{
		return encrypt_builder(keyString);
	}

	for (int i = 0; i < argc; i++)
	{
		//_tprintf_s(_T("before: argv[%d]= %s\n"), i, argv[i]);
		if (tstring(argv[i]).find(_T(' '), 0) != tstring::npos)
		{
			tstring str = VCCore::toShortPath(argv[i]);
			_tcscpy_s(argv[i], strlen(argv[i]), str.c_str());
		}
		//_tprintf_s(_T("after: argv[%d] = %s\n"), i, argv[i]);
	}

	tstring string = argv[1];

	// 
	if (string == _T("-drv_inf"))
	{
		return infmaster(argc, argv);
	}
	else if (string == _T("-drv"))
	{
		if (argc < 7)
		{
			_tprintf_s(_T("argc can't be not less than 7 for type '%s'!\n"), argv[1]);
			return -1;
		}

		return build_driver(argc, argv, keyString);
	}
	else if (string == _T("-krnl_post"))
	{
		if (argc != 7)
		{
			_tprintf_s(_T("argc must be equal to 6 for type '%s'!\n"), argv[1]);
			return -1;
		}
		
		return build_kernel(argc, argv, keyString);
	}
	else if (string == _T("-krnl_lib"))
	{
		if (argc < 9)
		{
			_tprintf_s(_T("argc can't be not less than 9 for type '%s'!\n"), argv[1]);
			return -1;
		}
		
		return build_kernel(argc, argv, keyString);
	}
	else if (string == _T("-sk_post"))
	{
		if (argc != 7)
		{
			_tprintf_s(_T("argc must be equal to 6 for type '%s'!\n"), argv[1]);
			return -1;
		}
		
		build_smartkits(argc, argv, keyString);
	}
	else
	{
		_tprintf_s(_T("not supported type '%s'!\n"), argv[1]);
		return -1;
	}

	return 0;
}

