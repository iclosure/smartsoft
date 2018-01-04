
#include "BiosInfo.h"

#pragma warning (disable : 4996)

namespace encrypt
{
	typedef struct _UNICODE_STRING {
		USHORT		Length;			// ����
		USHORT		MaxLen;			// ��󳤶�
		LPSTR		Buffer;			// ����ָ�룬���������ַ�ڴ�ʱ���˴�ָ��UNICODE�ַ���"\device\physicalmemory"
	} UNICODE_STRING, * PUNICODE_STRING;

	typedef struct _OBJECT_ATTRIBUTES {
		ULONG				Length;			// ���� 18h
		HANDLE				RootDirectory;	// 00000000
		PUNICODE_STRING		ObjectName;		// ָ���������ָ��
		ULONG				Attributes;		// ��������00000040H
		PVOID				SecurityDescriptor;			// Points to type POBJECT_ATTRIBUTOR, 0
		PVOID				SecurityQualityOfService;	// Points to type SECURITY_QUALITY_OF_SERVICE, 0
	} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

	typedef DWORD (WINAPI *ZWOS)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES);
	typedef DWORD (WINAPI *ZWMV)(HANDLE, HANDLE, PVOID, ULONG, ULONG, PLARGE_INTEGER, PSIZE_T, DWORD, ULONG, ULONG);

	// Default Construct
	CBiosInfo::CBiosInfo(void)
		: m_lpBiosInfo ( NULL )
	{
	}

	CBiosInfo::~CBiosInfo(void)
	{
	}
	
	BOOL CBiosInfo::Initialize()
	{
		if (NULL != m_lpBiosInfo)
		{
			return TRUE;
		}

		// ����ntdll.dll���õ�������ַ
		HMODULE hInstLib;
		ZWOS ZWopenS;
		ZWMV ZWmapV;

		hInstLib = LoadLibraryA("ntdll.dll");
		if (NULL == hInstLib)
		{
			return FALSE;
		}

		ZWopenS = (ZWOS) GetProcAddress(hInstLib, "ZwOpenSection");
		ZWmapV = (ZWMV) GetProcAddress(hInstLib, "ZwMapViewOfSection");
		if (NULL == ZWopenS || NULL == ZWmapV)
		{
			return FALSE;
		}

		// ���ú������������ڴ����ӳ��
		wchar_t strPH[] = L"\\device\\physicalmemory";	// *
		UNICODE_STRING strUnicodePH;
		strUnicodePH.Buffer = (LPSTR)strPH;
		strUnicodePH.Length = 0x2c;	// ע���С�ǰ��ֽ��㣬˫�ֽ�һ���ַ�
		strUnicodePH.MaxLen = 0x2e;	// Ҳ�ǰ��ֽ���
		OBJECT_ATTRIBUTES obj_ar;
		obj_ar.Attributes = 64;	// ���ԣ�40H
		obj_ar.Length = 24;		// OBJECT_ATTRIBUTES���͵ĳ���: 18H
		obj_ar.ObjectName = &strUnicodePH;
		obj_ar.RootDirectory = NULL;
		obj_ar.SecurityDescriptor = NULL;
		obj_ar.SecurityQualityOfService = NULL;
		HANDLE hSection;
		if (FALSE != ZWopenS(&hSection, 4, &obj_ar))
		{
			return FALSE;
		}

		DWORD ba = 0;		// ��ϵ��Ļ���ַ�������ﷵ��
		LARGE_INTEGER so;
		SIZE_T ssize;
		so.LowPart = 0x000f0000;		// �����ڴ��ַ������f000:0000
		so.HighPart = 0x00000000;		// 
		ssize = 0xffff;
		if (FALSE != ZWmapV((HANDLE) hSection, (HANDLE) 0xffffffff, &ba, 0, 0xffff, &so, &ssize, 1, 0, 2))
		{
			return FALSE;
		}

		m_lpBiosInfo = (LPSTR) ba;

		return TRUE;
	}

	BOOL CBiosInfo::GetBiosDate(LPSTR szDate)
	{
		if (0 == isalnum(*(m_lpBiosInfo + 0xfff5)))
		{
			return FALSE;
		}

		strcpy(szDate, m_lpBiosInfo + 0xfff5);

		return TRUE;
	}

	BOOL CBiosInfo::GetAwardBiosID(LPSTR szBiosID)
	{
// 		if (0 == strstr(m_lpBiosInfo + 0xe061, "Award"))
// 		{
// 			return FALSE;
// 		}

		if (0 == isalnum(*(m_lpBiosInfo + 0xe061)) || 0 == isalnum(*(m_lpBiosInfo + 0xec71)))
		{
			return FALSE;
		}

		strcpy(szBiosID, m_lpBiosInfo + 0xec71);

		return TRUE;
	}

	BOOL CBiosInfo::GetAMIBiosID(LPSTR szBiosID)
	{
// 		if (0 != strnicmp(m_lpBiosInfo + 0xf400, "AMIBIOS", lstrlen("AMIBIOS")))
// 		{
// 			return FALSE;
// 		}
		
		if (0 == isalnum(*(m_lpBiosInfo + 0xf400)) || 0 == isalnum(*(m_lpBiosInfo + 0xf478)))
		{
			return FALSE;
		}

		strcpy(szBiosID, m_lpBiosInfo + 0xf478);

		return TRUE;
	}

	BOOL CBiosInfo::GetBiosID(LPSTR szBiosID)
	{
		if (! Initialize())
		{
			*szBiosID = '\0';
			return FALSE;
		}

		if (! GetAwardBiosID(szBiosID))
		{
			if (! GetAMIBiosID(szBiosID))
			{
				if (! GetBiosDate(szBiosID))
				{
					*szBiosID = '\0';
					return FALSE;
				}
			}
		}

		return TRUE;
	}
}
