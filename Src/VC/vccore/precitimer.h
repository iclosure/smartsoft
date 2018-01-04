
//! ���ܣ�ʹ��ϵͳ��ʱ��������õ���ȷ��ʱ��
//! �ṹ��
//! ˵����

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

	// ����CPU������ֵ(us)
	LONGLONG countUS() const
	{
		assert(m_isValid);

		LARGE_INTEGER curCount;

		QueryPerformanceCounter(&curCount);

		return curCount.QuadPart * 1000000UL / m_cpuFreq.QuadPart;
	}

	// ����CPU������ֵ(ms)
	LONGLONG counterMS() const
	{
		assert(m_isValid);

		LARGE_INTEGER curCount;

		QueryPerformanceCounter(&curCount);

		return curCount.QuadPart * 1000UL / m_cpuFreq.QuadPart;
	}

	// ����CPU������ֵ(us)
	double valueUS() const
	{
		assert(m_isValid);

		LARGE_INTEGER curCount;

		QueryPerformanceCounter(&curCount);

		return curCount.QuadPart * 1000000.0 / m_cpuFreq.QuadPart;
	}

	// ����CPU������ֵ(ms)
	double valueMS() const
	{
		assert(m_isValid);

		LARGE_INTEGER curCount;

		QueryPerformanceCounter(&curCount);

		return curCount.QuadPart * 1000.0 / m_cpuFreq.QuadPart;
	}

	// ���شӳ�ʼ�������ڵ�ʱ����(us)
	LONGLONG intervalUS() const
	{
		assert(m_isValid);

		LARGE_INTEGER curCount;

		QueryPerformanceCounter(&curCount);

		return (curCount.QuadPart - m_orgCount.QuadPart) * 1000000UL / m_cpuFreq.QuadPart;
	}

	// ���شӳ�ʼ�������ڵ�ʱ����(ms)
	LONGLONG intervalMS() const
	{
		assert(m_isValid);

		LARGE_INTEGER curCount;

		QueryPerformanceCounter(&curCount);

		return (curCount.QuadPart - m_orgCount.QuadPart) * 1000UL / m_cpuFreq.QuadPart;
	}

	BOOL isValid() { return m_isValid; }

private:
	BOOL			m_isValid;		// �����Ƿ��Ѿ���Ч��ʼ��
	LARGE_INTEGER	m_cpuFreq;		// CPUƵ��
	LARGE_INTEGER	m_orgCount;		// �����ʼ��ʱ��ʱ��
};

#endif  // _HR_PRECISION_TIME_H_
