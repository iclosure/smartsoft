#pragma once

#include "auto_incr_version.h"

///
/// argv[0]: execute name
/// argv[1]: BuildEvent
/// argv[2]: -ver/...
/// argv[3]: basic direction
/// argv[4]: output name
/// argv[5]: target name
/// argv[6]: configuration type, eg: -chk
/// 

int infmaster(int argc, const _TCHAR* const argv[])
{
	_tprintf_s(_T("\nINFMASTER: START\n"));

	DBG_PRINTF((_T("\nINFMASTER: argc: %d\n"), argc));

	if (argc < 7)
	{
		DBG_PRINTF((_T("\nINFMASTER: argc must be 6 (%d)\n"), argc - 1));
		DBG_PRINTF((_T("\nINFMASTER: FINISH\n")));
		return -1;
	}

#if defined(_DEBUG)
	for (int i = 1; i < argc; i++)
	{
		DBG_PRINTF((_T("\nINFMASTER: argv[%d]=%s\n"), i, argv[i]));
	}
#endif  // _DEBUG

	if (std::basic_string<_TCHAR>(argv[2]).compare(_T("-ver")) == 0)
	{
		auto_incr_version(argv[3], argv[4], argv[5], argv[6]);
	}

	_tprintf_s(_T("\nINFMASTER: FINISH\n\n"));

	return 0;
}
