#include "stdafx.h"
#include "mmtimer.h"

// 导入Windows多媒体库
#pragma comment (lib, "winmm.lib")

//
//  Define standard min and max macros
//

#ifndef hr_max
#define hr_max(a,b)            (((a) > (b)) ? (a) : (b))
#endif  // hr_max

#ifndef hr_min
#define hr_min(a,b)            (((a) < (b)) ? (a) : (b))
#endif  // hr_min

/////////////////////////////////////////////////////////
/// MmTimerPrivate

class MmTimerPrivate
{
	friend class MmTimer;
public:
	explicit MmTimerPrivate(unsigned int interval, int mode);
	virtual ~MmTimerPrivate();

	bool start();
	bool stop();
	void dispose();

private:
	inline static void WINAPI oneShotCallback(UINT uTimerID, UINT uMsg, DWORD_PTR owner, DWORD_PTR dw1, DWORD_PTR dw2);
	inline static void WINAPI periodicCallback(UINT uTimerID, UINT uMsg, DWORD_PTR owner, DWORD_PTR dw1, DWORD_PTR dw2);

private:
	HR_DISABLE_COPY(MmTimerPrivate)

	LPMMTIMERPROC q_timerEvent;	// 多媒体定时器事件函数
	bool q_isActive;			// 运行状态
	unsigned int q_mode;        // 运行模式
	unsigned int q_resolution;  //
	unsigned int q_timerID;     // 多媒体定时器ID号
	int q_errorCode;			// 串口操作的错误码
	void* q_owner;				// 此变量用于多媒体定时器事件函数_timerEvent的用户参数暂存
	unsigned int q_interval;    // 内部时间
	TIMECAPS q_caps;			//
};

MmTimerPrivate::MmTimerPrivate(unsigned int interval, int mode) :
	q_timerEvent(0),
	q_isActive(false),
	q_mode(mode),
	q_resolution(0),
	q_timerID(0),
	q_errorCode(TIMERR_NOERROR),
	q_owner(0)
{
	if (TIMERR_NOERROR == timeGetDevCaps(&q_caps, sizeof(TIMECAPS)))
	{
		q_interval = hr_min(hr_max(q_caps.wPeriodMin, interval), q_caps.wPeriodMax);
		q_resolution = hr_min(hr_max(q_caps.wPeriodMin, 1), q_caps.wPeriodMax);
	}
}

MmTimerPrivate::~MmTimerPrivate()
{
	dispose();
}

/**
* @brief
*	启动多媒体定时器
* @param	none.
* @return	none.
*/
bool MmTimerPrivate::start()
{
	if (!q_isActive)
	{
		if (TIME_PERIODIC == q_mode)
		{
			q_timerID = timeSetEvent(q_interval, q_resolution, periodicCallback, (DWORD_PTR)this, q_mode);
		}
		else
		{
			q_timerID = timeSetEvent(q_interval, q_resolution, oneShotCallback, (DWORD_PTR)this, q_mode);
		}

		if (0 == q_timerID)
		{
			q_errorCode = q_timerID;
			return false;
		}

		q_isActive = true;
	}

	return true;
}

/**
* @brief
*	暂停多媒体定时器
* @param	none
* @return	none.
*/
bool MmTimerPrivate::stop()
{
	if (q_isActive)
	{
		q_errorCode = timeKillEvent(q_timerID);
		if (q_errorCode != TIMERR_NOERROR)
		{
			return false;
		}

		q_isActive = false;
		q_timerID = 0;
	}

	return true;
}

/**
* @brief
*注销多媒体定时器
* @param	none
* @return	none.
*/
void MmTimerPrivate::dispose()
{
	if (q_isActive)
	{
		timeKillEvent(q_timerID);
		q_isActive = false;
		q_timerID = 0;
	}
}

// 触发型定时器事件
void WINAPI MmTimerPrivate::oneShotCallback(UINT uTimerID, UINT uMsg, DWORD_PTR owner, DWORD_PTR dw1, DWORD_PTR dw2)
{
	MmTimerPrivate* _this = reinterpret_cast<MmTimerPrivate*>(owner);
	_this->q_timerEvent(_this->q_owner);
	_this->stop();
}

// 周期型定时器事件
void WINAPI MmTimerPrivate::periodicCallback(UINT uTimerID, UINT uMsg, DWORD_PTR owner, DWORD_PTR dw1, DWORD_PTR dw2)
{
	MmTimerPrivate* _this = reinterpret_cast<MmTimerPrivate*>(owner);
	_this->q_timerEvent(_this->q_owner);
}

/////////////////////////////////////////////////////////
/// MmTimer

MmTimer::MmTimer(unsigned int interval, int mode)
{
	d = new MmTimerPrivate(interval, mode);
}

MmTimer::~MmTimer(void)
{
	delete d;
}

bool MmTimer::start()
{
	return d->start();
}

bool MmTimer::stop()
{
	return d->stop();
}

#pragma region Properies

/*设置参数*/
void MmTimer::set(unsigned int mode, unsigned int period, LPMMTIMERPROC evt, void* dwUser)
{
	setMode(mode);
	setPeriod(period);
	setTimerEvent(evt, dwUser);
}

/**
* @brief
*	设置多媒体定时器事件处理句柄
* @param	event, 事件句柄.
*			dwUser, 事件函数的参数, 即event所属类的this指针地址
* @return	none.
*/
void MmTimer::setTimerEvent(LPMMTIMERPROC event, void* owner)
{
	d->q_timerEvent = event;
	d->q_owner = owner;
}

// 获取内部周期值
const unsigned int MmTimer::period() const
{
	return d->q_interval;
}

// 设置内部周期值(value, 时间, (units, ms))
void MmTimer::setPeriod(unsigned int value)
{
	d->q_interval = hr_min(hr_max(d->q_caps.wPeriodMin, value), d->q_caps.wPeriodMax);
	d->q_resolution = hr_min(hr_max(d->q_caps.wPeriodMin, 1), d->q_caps.wPeriodMax);
}

// 获取定时器运行模式
unsigned int MmTimer::mode() const
{
	return d->q_mode;
}

// 设置模式(TIME_PERIODIC 或者 TIME_ONESHOT)
void MmTimer::setMode(unsigned int value)
{
	d->q_mode = value;
}

// 获取定时器运行状态
bool MmTimer::isActive() const
{
	return d->q_isActive;
}

#pragma endregion Properies

#undef hr_min
#undef hr_max
