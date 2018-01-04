
#include "precomp.h"

NTSTATUS UnicodeStringDupString(
	OUT LPWSTR *strDest,
	IN LPWSTR strSrc,
	IN POOL_TYPE PoolType,
	IN BOOLEAN MultiStr)
{
	LPWSTR strTemp = strSrc;
	ULONG length;
	
	if (MultiStr)
	{
		do {
			while (*(strTemp++)){ ; }
		} while (*(strTemp++));
	}
	else
	{
		while (*(strTemp++)){ ; }
	}

	length = (ULONG)(strTemp - strSrc) * sizeof(WCHAR);
	strTemp = HR_ALLOCATE_POOL(PoolType, length);

	if (!strTemp)
	{
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	RtlCopyMemory(strTemp, strSrc, length);
	*strDest = strTemp;

	return STATUS_SUCCESS;
}

VOID UnicodeStringAppendString(
	PNTSTATUS pStatus,
	IN OUT PUNICODE_STRING  pDest,
	IN PWCHAR pSrc,
	IN USHORT lenSrc)
{
	UNICODE_STRING old;
	NTSTATUS status;
	SIZE_T newLength;

	status = *pStatus;

	if (!NT_SUCCESS(status) || !pSrc || !lenSrc)
	{
		return;
	}

	old = *pDest;

	RtlZeroMemory(pDest, sizeof(UNICODE_STRING));

	newLength = (SIZE_T)old.Length + (SIZE_T)lenSrc;

	if ((USHORT)newLength == newLength)   /* checking for overflow */
	{
		pDest->MaximumLength = (USHORT)newLength;
		pDest->Buffer = HR_ALLOCATE_POOL(NonPagedPool, pDest->MaximumLength + sizeof(WCHAR));
	}

	if (pDest->Buffer != NULL)
	{
		RtlZeroMemory(pDest->Buffer, pDest->MaximumLength + sizeof(WCHAR));
		status = RtlAppendUnicodeStringToString(pDest, &old);
		if (NT_SUCCESS(status))
		{
			PWCHAR pSrc0 = HR_ALLOCATE_POOL(NonPagedPool, lenSrc + sizeof(WCHAR));
			if (pSrc0 != NULL)
			{
				RtlZeroMemory(pSrc0, lenSrc + sizeof(WCHAR));
				RtlCopyMemory(pSrc0, pSrc, lenSrc);

				status = RtlAppendUnicodeToString(pDest, pSrc0);

				HR_FREE_POOL(pSrc0);
			}
			else
			{
				status = STATUS_INSUFFICIENT_RESOURCES;
			}
		}
	}
	else
		status = STATUS_INSUFFICIENT_RESOURCES;

	UnicodeStringFreeBad(status, pDest);

	if (old.Buffer)
	{
		HR_FREE_POOL(old.Buffer);
	}

	*pStatus = status;
}

VOID UnicodeStringAppendUnicodeString(
	IN OUT PNTSTATUS pStatus,
	IN OUT PUNICODE_STRING pDest,
	IN PUNICODE_STRING pSrc)
{
	UnicodeStringAppendString(pStatus, pDest, pSrc->Buffer, pSrc->Length);
}

VOID UnicodeStringAppendString0(PNTSTATUS pStatus, IN OUT PUNICODE_STRING  pDest, IN PWCHAR pSrc)
{
	UnicodeStringAppendString(pStatus, pDest, pSrc, (USHORT)(wcslen(pSrc) * sizeof(WCHAR)));
}

VOID UnicodeStringAppendData(
	PNTSTATUS pStatus,
	IN OUT PUNICODE_STRING pDest,
	IN ULONG data,
	IN ULONG base)
{
	UNICODE_STRING numStr;
	WCHAR numStrBuf[20];

	if (!NT_SUCCESS(*pStatus))
	{
		return;
	}

	RtlInitUnicodeString(&numStr, NULL);
	numStr.MaximumLength = sizeof(numStrBuf);
	numStr.Buffer = numStrBuf;
	*pStatus = RtlIntegerToUnicodeString(data, base, &numStr);

	if (UnicodeStringFreeBad(*pStatus, pDest))
	{
		return;
	}

	UnicodeStringAppendString(pStatus, pDest, numStr.Buffer, numStr.Length);
}

VOID UnicodeStringFree(IN OUT PUNICODE_STRING  pDest)
{
	if (pDest->Buffer)
	{
		HR_FREE_POOL(pDest->Buffer);
	}

	RtlZeroMemory(pDest, sizeof(UNICODE_STRING));
}

BOOLEAN UnicodeStringFreeBad(NTSTATUS status, IN OUT PUNICODE_STRING  pDest)
{
	if (!NT_SUCCESS(status))
	{
		UnicodeStringFree(pDest);
		return TRUE;
	}

	return FALSE;
}

VOID UnicodeStringAppendDeviceProperty(
	IN OUT PNTSTATUS pStatus,
	IN OUT PUNICODE_STRING pDest,
	IN PDEVICE_OBJECT pDevObj,
	IN DEVICE_REGISTRY_PROPERTY deviceProperty)
{
	NTSTATUS status;
	ULONG len;

	status = *pStatus;
	if (!NT_SUCCESS(status))
	{
		return;
	}

	status = IoGetDeviceProperty(pDevObj, deviceProperty, 0, NULL, &len);
	if (status == STATUS_BUFFER_TOO_SMALL && len) 
	{
		PWCHAR pStrTmp = (PWCHAR)HR_ALLOCATE_POOL(NonPagedPool, len);
		if (pStrTmp != NULL)
		{
			status = IoGetDeviceProperty(pDevObj, deviceProperty, len, pStrTmp, &len);
			if (NT_SUCCESS(status))
			{
				UnicodeStringAppendString0(&status, pDest, pStrTmp);
			}

			HR_FREE_POOL(pStrTmp);
		}
		else
		{
			status = STATUS_INSUFFICIENT_RESOURCES;
		}
	}

	UnicodeStringFreeBad(status, pDest);

	*pStatus = status;
}
