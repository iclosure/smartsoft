#include "stdafx.h"
#include "mmtimer.h"

// ����Windows��ý���
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

	LPMMTIMERPROC q_timerEvent;	// ��ý�嶨ʱ���¼�����
	bool q_isActive;			// ����״̬
	unsigned int q_mode;        // ����ģʽ
	unsigned int q_resolution;  //
	unsigned int q_timerID;     // ��ý�嶨ʱ��ID��
	int q_errorCode;			// ���ڲ����Ĵ�����
	void* q_owner;				// �˱������ڶ�ý�嶨ʱ���¼�����_timerEvent���û������ݴ�
	unsigned int q_interval;    // �ڲ�ʱ��
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
*	������ý�嶨ʱ��
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
*	��ͣ��ý�嶨ʱ��
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
*ע����ý�嶨ʱ��
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

// �����Ͷ�ʱ���¼�
void WINAPI MmTimerPrivate::oneShotCallback(UINT uTimerID, UINT uMsg, DWORD_PTR owner, DWORD_PTR dw1, DWORD_PTR dw2)
{
	MmTimerPrivate* _this = reinterpret_cast<MmTimerPrivate*>(owner);
	_this->q_timerEvent(_this->q_owner);
	_this->stop();
}

// �����Ͷ�ʱ���¼�
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

/*���ò���*/
void MmTimer::set(unsigned int mode, unsigned int period, LPMMTIMERPROC evt, void* dwUser)
{
	setMode(mode);
	setPeriod(period);
	setTimerEvent(evt, dwUser);
}

/**
* @brief
*	���ö�ý�嶨ʱ���¼�������
* @param	event, �¼����.
*			dwUser, �¼������Ĳ���, ��event�������thisָ���ַ
* @return	none.
*/
void MmTimer::setTimerEvent(LPMMTIMERPROC event, void* owner)
{
	d->q_timerEvent = event;
	d->q_owner = owner;
}

// ��ȡ�ڲ�����ֵ
const unsigned int MmTimer::period() const
{
	return d->q_interval;
}

// �����ڲ�����ֵ(value, ʱ��, (units, ms))
void MmTimer::setPeriod(unsigned int value)
{
	d->q_interval = hr_min(hr_max(d->q_caps.wPeriodMin, value), d->q_caps.wPeriodMax);
	d->q_resolution = hr_min(hr_max(d->q_caps.wPeriodMin, 1), d->q_caps.wPeriodMax);
}

// ��ȡ��ʱ������ģʽ
unsigned int MmTimer::mode() const
{
	return d->q_mode;
}

// ����ģʽ(TIME_PERIODIC ���� TIME_ONESHOT)
void MmTimer::setMode(unsigned int value)
{
	d->q_mode = value;
}

// ��ȡ��ʱ������״̬
bool MmTimer::isActive() const
{
	return d->q_isActive;
}

#pragma endregion Properies

#undef hr_min
#undef hr_max
