
#ifndef _HR_PRECISION_TIME_H_
#define _HR_PRECISION_TIME_H_

HR_EXTERN_C_START

#include <ntdef.h>
#include "dbgprint.h"

FORCEINLINE VOID __CRTDECL HrQueryCurrentTime()
{
	LARGE_INTEGER CurrentTime;
	LARGE_INTEGER LocalTime;
	TIME_FIELDS   TimeFiled;

	HR_PRINT_METHOD_BEGIN();

	// ����õ�����ʵ�Ǹ�������ʱ��
	KeQuerySystemTime(&CurrentTime);
	HrKdPrint(("CurrentTime: %I64u\n", CurrentTime.QuadPart));
	// ת���ɱ���ʱ��
	ExSystemTimeToLocalTime(&CurrentTime, &LocalTime);
	HrKdPrint(("LocalTime: %I64u\n", LocalTime.QuadPart));
	// ��ʱ��ת��Ϊ����������ʽ
	RtlTimeToTimeFields(&LocalTime, &TimeFiled);

	HrKdPrint(("[TimeTest] NowTime : %4d-%2d-%2d %2d:%2d:%2d", 
		TimeFiled.Year, TimeFiled.Month, TimeFiled.Day, 
		TimeFiled.Hour, TimeFiled.Minute, TimeFiled.Second));

	HR_PRINT_METHOD_END();
}

FORCEINLINE VOID __CRTDECL HrQuerySystemStartupTime(
	OUT PLARGE_INTEGER StartupTime)
{
	LARGE_INTEGER systemTime;
	LARGE_INTEGER localTime;
	LARGE_INTEGER cpuFreq;
	LARGE_INTEGER counter;

	KeQuerySystemTime(&systemTime);
	HrKdPrint(("systemTime: %I64u\n", systemTime.QuadPart));
	counter = KeQueryPerformanceCounter(&cpuFreq);
	counter.QuadPart = counter.QuadPart * 10000000 / cpuFreq.QuadPart;	// ��λ��100ns

	ExSystemTimeToLocalTime(&systemTime, &localTime);
	StartupTime->QuadPart = localTime.QuadPart - counter.QuadPart;

	HrQueryCurrentTime();
}

FORCEINLINE VOID __CRTDECL HrQuerySystemStartupTimePrecise(
	OUT PLARGE_INTEGER StartupTime)
{
	//LARGE_INTEGER systemTime;
	//LARGE_INTEGER localTime;
	//LARGE_INTEGER counter;
	/*
	KeQuerySystemTimePrecise(&systemTime);
	counter = KeQueryPerformanceCounter(NULL);
	ExSystemTimeToLocalTime(&systemTime, &localTime);

	StartupTime->QuadPart = localTime.QuadPart - counter.QuadPart;*/
	StartupTime->QuadPart = 0;
}

HR_EXTERN_C_END

#endif  // _HR_PRECISION_TIME_H_
