
#include "CPUID.h"
#include <stdio.h>

#pragma warning (disable : 4996)

namespace encrypt
{
	CCPUID::CCPUID(void)
	{
	}

	CCPUID::~CCPUID(void)
	{
	}

	BOOL CCPUID::GetFirstCPUID(LPSTR szCPUID)
	{
		m_hCurProcHandle = GetCurrentProcess();
		GetProcessAffinityMask(m_hCurProcHandle, &m_dwProcAffinity, &m_dwSysAffinity);

		// Check if available process affinity mask is equal to the
		// available system affinity mask.
		if (m_dwProcAffinity != m_dwSysAffinity)
		{
			return FALSE;
		}

		m_dwAffinityMask = 1;
		
		return GetNextCPUID(szCPUID);
	}

	BOOL CCPUID::GetNextCPUID(LPSTR szCPUID)
	{
		if (m_dwAffinityMask != 0 && m_dwAffinityMask <= m_dwProcAffinity)
		{
			// Check if this CPU is available.
			if (m_dwAffinityMask & m_dwProcAffinity)
			{
				if (SetProcessAffinityMask(m_hCurProcHandle, m_dwAffinityMask))
				{
					Sleep(0);		// Give OS time to switch CPU.
				}
			}

			GetCPUID(szCPUID);

			m_dwAffinityMask <<= 1;

			return TRUE;
		}
		else
		{
			// Reset the Process affinity.
			SetProcessAffinityMask(m_hCurProcHandle, m_dwProcAffinity);
			return FALSE;
		}
	}

	void CCPUID::GetCPUID(LPSTR szCPUID)
	{
		DWORD x[] = {0, 0, 0};
		
		_asm {
			mov eax, 1
			cpuid
			mov dword ptr x, eax
			mov eax, 3
			cpuid
			mov x + 4, edx
			mov x + 8, ecx
		}

		sprintf(szCPUID, "%08X-%08X-%08X", x[0], x[1], x[2]);
	}
}
