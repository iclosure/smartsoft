
#include <Windows.h>
#include <encrypt.h>
#include "rmtlib.h"

using namespace encrypt;

CRmtLib::CRmtLib(void)
{

}

CRmtLib::~CRmtLib(void)
{
}

BOOL CRmtLib::License(const BYTE* strSN, UINT length)
{
	if (strSN == NULL || length == 0)
	{
		return FALSE;
	}

	CSystemInfo systemInfo;

	CHAR strBIOSInfo[1024] = {0};
	systemInfo.m_biosInfo.GetBiosID(strBIOSInfo);

	//CHAR strCPUID[1024] = {0};
	//systemInfo.m_CPUID.GetFirstCPUID(strCPUID);

	CHAR strDiskSerial[1024] = {0};
	systemInfo.m_diskSerial.GetFirstDiskSerial(strDiskSerial);

	CHAR strMac[1024] = {0};
	systemInfo.m_ethernetMac.GetFirstMac(strMac);

	// Encrypt BIOS INFO + Disk Serial Number + Mac Address
	std::string input = std::string(strBIOSInfo) + std::string(strDiskSerial) + std::string(strMac);

	MD5 md5(input);

	std::string output = md5.toString();

	// »°≈º–Ú¡–
	std::string output1;
	for (size_t i = 0; i < output.size() / 2; i++)
	{
		output1.append(1, output[i * 2 + 1]);
	}

	for (size_t i = 4; i < output1.size(); i+=5)
	{
		output1.insert(i, 1, '-');
	}

	std::string source;
	source.append((char*)strSN, output1.length());

	if (output1 == source)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
