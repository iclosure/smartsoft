#include "stdafx.h"
#include "precitimer.h"

PreciTimer::PreciTimer()
{
	initialize();
}


PreciTimer::~PreciTimer()
{
}

void PreciTimer::initialize()
{
	if (QueryPerformanceFrequency(&m_cpuFreq) && QueryPerformanceCounter(&m_orgCount)) {
		m_isValid = TRUE;
	}
	else {
		m_isValid = FALSE;
		m_cpuFreq.QuadPart = 0;
		m_orgCount.QuadPart = 0;
	}
}