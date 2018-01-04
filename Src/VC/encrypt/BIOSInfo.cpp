
#include "BiosInfo.h"

#pragma warning (disable : 4996)

namespace encrypt
{
	typedef struct _UNICODE_STRING {
		USHORT		Length;			// 长度
		USHORT		MaxLen;			// 最大长度
		LPSTR		Buffer;			// 缓存指针，访问物理地址内存时，此处指向UNICODE字符串"\device\physicalmemory"
	} UNICODE_STRING, * PUNICODE_STRING;

	typedef struct _OBJECT_ATTRIBUTES {
		ULONG				Length;			// 长度 18h
		HANDLE				RootDirectory;	// 00000000
		PUNICODE_STRING		ObjectName;		// 指向对象名的指针
		ULONG				Attributes;		// 对象属性00000040H
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

		// 读入ntdll.dll，得到函数地址
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

		// 调用函数，对物理内存进行映射
		wchar_t strPH[] = L"\\device\\physicalmemory";	// *
		UNICODE_STRING strUnicodePH;
		strUnicodePH.Buffer = (LPSTR)strPH;
		strUnicodePH.Length = 0x2c;	// 注意大小是按字节算，双字节一个字符
		strUnicodePH.MaxLen = 0x2e;	// 也是按字节算
		OBJECT_ATTRIBUTES obj_ar;
		obj_ar.Attributes = 64;	// 属性：40H
		obj_ar.Length = 24;		// OBJECT_ATTRIBUTES类型的长度: 18H
		obj_ar.ObjectName = &strUnicodePH;
		obj_ar.RootDirectory = NULL;
		obj_ar.SecurityDescriptor = NULL;
		obj_ar.SecurityQualityOfService = NULL;
		HANDLE hSection;
		if (FALSE != ZWopenS(&hSection, 4, &obj_ar))
		{
			return FALSE;
		}

		DWORD ba = 0;		// 联系后的基地址将在这里返回
		LARGE_INTEGER so;
		SIZE_T ssize;
		so.LowPart = 0x000f0000;		// 物理内存地址，就是f000:0000
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
