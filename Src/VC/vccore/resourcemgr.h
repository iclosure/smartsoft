
#ifndef _HR_RESOURCE_MANAGER_H_
#define _HR_RESOURCE_MANAGER_H_

#include <Windows.h>
#include <conio.h>

#define SystemBasicInformation        0 
#define SystemPerformanceInformation  2 
#define SystemTimeInformation         3

typedef struct
{
	DWORD Unknown1;
	ULONG KeMaximumIncrement;
	ULONG PageSize;
	ULONG MmNumberOfPhysicalPages;
	ULONG MmLowestPhysicalPage;
	ULONG MmHighestPhysicalPage;
	ULONG AllocationGranularity;
	PVOID LowestUserAddress;
	PVOID MmHighestUserAddress;
	ULONG KeActiveProcessors;
	BYTE KeNumberProcessors;
	BYTE Unknown2;
	WORD Unknown3;
} SYSTEM_BASIC_INFORMATION;

typedef struct
{
	LARGE_INTEGER IdleTime;
	DWORD Spare[76];
} SYSTEM_PERFORMANCE_INFORMATION;

typedef struct
{
	LARGE_INTEGER KeBootTime;
	LARGE_INTEGER KeSystemTime;
	LARGE_INTEGER ExpTimeZoneBias;
	ULONG CurrentTimeZoneId;
	DWORD Reserved;
} SYSTEM_TIME_INFORMATION;

typedef LONG(WINAPI *PROCNTQSI)(UINT, PVOID, ULONG, PULONG);

class CpuRate
{
public:
	CpuRate()
	{
		Initialze();
	}

	~CpuRate()
	{
	}

	void Initialze()
	{
		m_oldIdleTime.QuadPart = 0;
		m_oldSystemTime.QuadPart = 0;
	}

	double GetCurrentRate()
	{
		LONG status;
		PROCNTQSI fnNtQuerySystemInformation
			= (PROCNTQSI)GetProcAddress(GetModuleHandle(TEXT("ntdll")), "NtQuerySystemInformation");
		if (!fnNtQuerySystemInformation)
		{
			return 0;
		}

		// get number of processors in the system
		SYSTEM_BASIC_INFORMATION sysBaseInfo;
		status = fnNtQuerySystemInformation(SystemBasicInformation,
			&sysBaseInfo, sizeof(SYSTEM_BASIC_INFORMATION), NULL);
		if (status != NO_ERROR)
		{
			return 0;
		}

		if (!_kbhit())
		{
			// get new system time 
			SYSTEM_TIME_INFORMATION sysTimeInfo;
			status = fnNtQuerySystemInformation(SystemTimeInformation,
				&sysTimeInfo, sizeof(SYSTEM_TIME_INFORMATION), 0);
			if (status != NO_ERROR)
				return 0;

			// get new CPU's idle time 
			SYSTEM_PERFORMANCE_INFORMATION sysPerfInfo;
			status = fnNtQuerySystemInformation(SystemPerformanceInformation,
				&sysPerfInfo, sizeof(SYSTEM_PERFORMANCE_INFORMATION), NULL);
			if (status != NO_ERROR)
			{
				return 0;
			}

			double dbIdleTime = 0, dbSystemTime = 0;

			// if it's a first call - skip it 
			if (m_oldIdleTime.QuadPart != 0)
			{
				// CurrentValue = NewValue - OldValue 
				dbIdleTime = (double)(sysPerfInfo.IdleTime.QuadPart - m_oldIdleTime.QuadPart);
				dbSystemTime = (double)(sysTimeInfo.KeSystemTime.QuadPart - m_oldSystemTime.QuadPart);

				// CurrentCpuIdle = IdleTime / SystemTime 
				dbIdleTime /= dbSystemTime;

				// CurrentCpuUsage% = 100 - (CurrentCpuIdle * 100) / NumberOfProcessors 
				dbIdleTime = 100.0 - dbIdleTime * 100.0 /
					(double)sysBaseInfo.KeNumberProcessors + 0.5;

				if (dbIdleTime < 0)
				{
					dbIdleTime = 0;
				}
			}

			// store new CPU's idle and system time 
			m_oldIdleTime = sysPerfInfo.IdleTime;
			m_oldSystemTime = sysTimeInfo.KeSystemTime;

			return dbIdleTime;
		}

		return 0;
	}

private:
	LARGE_INTEGER m_oldIdleTime;
	LARGE_INTEGER m_oldSystemTime;
};

#endif  // _HR_RESOURCE_MANAGER_H_
