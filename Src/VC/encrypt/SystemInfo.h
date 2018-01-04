#pragma once

#include "encrypt_global.h"

namespace encrypt
{
	class CBiosInfo;
	class CCPUID;
	class CDiskSerial;
	class CEthernetMac;

	class DLLENCRYPT_API CSystemInfo
	{
	public:
		CSystemInfo(void);
		~CSystemInfo(void);

	public:
		CBiosInfo		m_biosInfo;
		CCPUID			m_CPUID;
		CDiskSerial		m_diskSerial;
		CEthernetMac	m_ethernetMac;
	};

}
