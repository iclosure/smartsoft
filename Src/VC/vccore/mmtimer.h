#ifndef HR_MMTIMER_H
#define HR_MMTIMER_H

//////////////////////////////////////////////////////////////
// @file 		MmTimer.h
// @Copyright	Hi ROBOT@611-ChengDu-AVIC
// @key			Function��Windows��ý�嶨ʱ����չ��ʵ��
// @remark		Remarks :
//				ʹ�ò��裺	1).����CMmTimer����;
//							2).����Start()������������ʱ��;
//							3).����Stop()ֹͣ���ͷŶ�ʱ����Դ;
//							4).�ͷ�CMmTimer����.
// @date			2012-11-20 16:14
//////////////////////////////////////////////////////////////

#include "vccore_global.h"
#include <Windows.h>
#include <mmsystem.h>

// ��ý�嶨ʱ�����ӿڻص���������
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
