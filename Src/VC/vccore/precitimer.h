
//! 功能：使用系统的时间计数器得到精确的时间
//! 结构：
//! 说明：

#ifndef _HR_PRECISION_TIME_H_
#define _HR_PRECISION_TIME_H_

#include "vccore_global.h"
#include <Windows.h>
#include <assert.h>

class VCCORE_EXPORT PreciTimer
{
public:
	explicit PreciTimer(void);
	~PreciTimer(void);

	void initialize();

	// 返回CPU计数器值(us)
	LONGLONG countUS() const
	{
		assert(m_isValid);

		LARGE_INTEGER curCount;

		QueryPerformanceCounter(&curCount);

		return curCount.QuadPart * 1000000UL / m_cpuFreq.QuadPart;
	}

	// 返回CPU计数器值(ms)
	LONGLONG counterMS() const
	{
		assert(m_isValid);

		LARGE_INTEGER curCount;

		QueryPerformanceCounter(&curCount);

		return curCount.QuadPart * 1000UL / m_cpuFreq.QuadPart;
	}

	// 返回CPU计数器值(us)
	double valueUS() const
	{
		assert(m_isValid);

		LARGE_INTEGER curCount;

		QueryPerformanceCounter(&curCount);

		return curCount.QuadPart * 1000000.0 / m_cpuFreq.QuadPart;
	}

	// 返回CPU计数器值(ms)
	double valueMS() const
	{
		assert(m_isValid);

		LARGE_INTEGER curCount;

		QueryPerformanceCounter(&curCount);

		return curCount.QuadPart * 1000.0 / m_cpuFreq.QuadPart;
	}

	// 返回从初始化到现在的时间间隔(us)
	LONGLONG intervalUS() const
	{
		assert(m_isValid);

		LARGE_INTEGER curCount;

		QueryPerformanceCounter(&curCount);

		return (curCount.QuadPart - m_orgCount.QuadPart) * 1000000UL / m_cpuFreq.QuadPart;
	}

	// 返回从初始化到现在的时间间隔(ms)
	LONGLONG intervalMS() const
	{
		assert(m_isValid);

		LARGE_INTEGER curCount;

		QueryPerformanceCounter(&curCount);

		return (curCount.QuadPart - m_orgCount.QuadPart) * 1000UL / m_cpuFreq.QuadPart;
	}

	BOOL isValid() { return m_isValid; }

private:
	BOOL			m_isValid;		// 对象是否已经有效初始化
	LARGE_INTEGER	m_cpuFreq;		// CPU频率
	LARGE_INTEGER	m_orgCount;		// 对象初始化时的时间
};

#endif  // _HR_PRECISION_TIME_H_
