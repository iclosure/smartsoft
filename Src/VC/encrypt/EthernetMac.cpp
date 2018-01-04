
#include "EthernetMac.h"
#include <stdio.h>

#pragma comment (lib, "IPHlpApi")

#pragma warning (disable : 4996)

namespace encrypt
{
	CEthernetMac::CEthernetMac(void)
	{
	}

	CEthernetMac::~CEthernetMac(void)
	{
	}

	BOOL CEthernetMac::GetFirstMac(LPSTR szMac)
	{
		DWORD dwBufLen = sizeof(m_adapterInfo);		// Save memory size of buffer
		DWORD dwStatus = GetAdaptersInfo(m_adapterInfo, &dwBufLen);
		if (dwStatus != ERROR_SUCCESS)
		{
			return FALSE;
		}

		m_pAdapterInfo = m_adapterInfo;

		return GetNextMac(szMac);
	}

	BOOL CEthernetMac::GetNextMac(LPSTR szMac)
	{
		if (NULL != m_pAdapterInfo)
		{
			// 把网卡MAC地址格式化成常用的16进制形式，例如0010-A4E4-5802
			sprintf(szMac, "%02X%02X-%02X%02X-%02X%02X",
				m_pAdapterInfo->Address[0],
				m_pAdapterInfo->Address[1],
				m_pAdapterInfo->Address[2],
				m_pAdapterInfo->Address[3],
				m_pAdapterInfo->Address[4],
				m_pAdapterInfo->Address[5]);
			m_pAdapterInfo = m_pAdapterInfo->Next;	// Progress through linked list
			return TRUE;
		}

		return FALSE;
	}
}
