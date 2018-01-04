#pragma once

#include <Windows.h>
#include "encrypt_global.h"

namespace encrypt
{
	// Bios INFO
	class DLLENCRYPT_API CBiosInfo
	{
	public:
		CBiosInfo(void);
		virtual ~CBiosInfo(void);

	public:
		BOOL GetBiosID(LPSTR szBiosID);

	private:
		BOOL Initialize();
		BOOL GetAwardBiosID(LPSTR szBiosID);
		BOOL GetAMIBiosID(LPSTR szBiosID);
		BOOL GetBiosDate(LPSTR szDate);

	private:
		LPSTR	m_lpBiosInfo;
	};
}
