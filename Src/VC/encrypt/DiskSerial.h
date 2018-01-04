#pragma once

#include <Windows.h>
#include "encrypt_global.h"

namespace encrypt
{
	class DLLENCRYPT_API CDiskSerial
	{
	public:
		CDiskSerial(void);
		~CDiskSerial(void);

	public:
		BOOL GetFirstDiskSerial(LPSTR szDiskSerial);
		BOOL GetNextDiskSerial(LPSTR szDiskSerial);

	private:
		BOOL ReadDrivePortsInWin9x();
		BOOL ReadPhysicalDriveInNTWithAdminRights();
		BOOL ReadIdeDriveAsScsiDriveNT();
		BOOL ReadPhysicalDriveNTWithZeroRights();

		void ConvertToString(LPSTR szDiskSerial, DWORD dwDiskData[], int nSize);
		void FlipAndCodeBytes(LPSTR szResult, LPCSTR cszStr);
		void FlipBlankSpace(LPSTR szResult, LPCSTR cszStr);

	private:
		int		m_nSerialCount;
		int		m_nReadCount;
		CHAR	m_szSerials[32][64];
	};
}