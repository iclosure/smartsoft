#pragma once

#include <Windows.h>
#include <IPHlpApi.h>
#include "encrypt_global.h"

namespace encrypt
{
	class DLLENCRYPT_API CEthernetMac
	{
	public:
		CEthernetMac(void);
		~CEthernetMac(void);

	public:
		BOOL GetFirstMac(LPSTR szMac);
		BOOL GetNextMac(LPSTR szMac);

	private:
		IP_ADAPTER_INFO		m_adapterInfo[16];		// Allocate information
		PIP_ADAPTER_INFO	m_pAdapterInfo;			// Contains pointer to
	};
}