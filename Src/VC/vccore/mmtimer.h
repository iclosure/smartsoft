#ifndef HR_MMTIMER_H
#define HR_MMTIMER_H

//////////////////////////////////////////////////////////////
// @file 		MmTimer.h
// @Copyright	Hi ROBOT@611-ChengDu-AVIC
// @key			Function：Windows多媒体定时器扩展类实现
// @remark		Remarks :
//				使用步骤：	1).创建CMmTimer对象;
//							2).调用Start()创建并启动定时器;
//							3).调用Stop()停止并释放定时器资源;
//							4).释放CMmTimer对象.
// @date			2012-11-20 16:14
//////////////////////////////////////////////////////////////

#include "vccore_global.h"
#include <Windows.h>
#include <mmsystem.h>

// 多媒体定时器简版接口回调函数定义
typedef void (CALLBACK *LPMMTIMERPROC)(void* owner);

class MmTimerPrivate;

class VCCORE_EXPORT MmTimer
{
public:
	explicit MmTimer(unsigned int interval = 0, int mode = TIME_PERIODIC);
	virtual ~MmTimer(void);

	bool start();
	bool stop();

	void set(unsigned int mode, unsigned int period, LPMMTIMERPROC evt, void* dwUser);
	void setTimerEvent(LPMMTIMERPROC event, void* owner);
	const unsigned int period() const;
	void setPeriod(unsigned int value);
	unsigned int mode() const;
	void setMode(unsigned int value);
	bool isActive() const;

private:
	HR_DISABLE_COPY(MmTimer)
	MmTimerPrivate* d;
};

#endif  // HR_MMTIMER_H
