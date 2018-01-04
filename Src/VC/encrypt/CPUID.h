#pragma once

#include <Windows.h>
#include "encrypt_global.h"

namespace encrypt
{
	// CPU ID
	class DLLENCRYPT_API CCPUID
	{
	public:
		CCPUID(void);
		~CCPUID(void);

	public:
		BOOL GetFirstCPUID(LPSTR szCPUID);
		BOOL GetNextCPUID(LPSTR szCPUID);

	private:
		void GetCPUID(LPSTR szCPUID);

	private:
		HANDLE		m_hCurProcHandle;
		DWORD		m_dwProcAffinity;
		DWORD		m_dwSysAffinity;
		DWORD		m_dwAffinityMask;
	};
}
