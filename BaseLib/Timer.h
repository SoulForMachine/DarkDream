
#ifndef _TIMER_H_
#define _TIMER_H_

#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")


class Timer
{
public:
	Timer() {}

	static void Init()
		{ timeBeginPeriod(1); }
	static void Deinit()
		{ timeEndPeriod(1); }
	static unsigned int GetTime()
		{ return timeGetTime(); }

	void Reset()
		{ m_prevTime = timeGetTime(); }

	float TimeSec() 
	{
		DWORD cur_time = timeGetTime();
		return (cur_time - m_prevTime) * 0.001f;
	}

	unsigned int TimeMsec()
	{
		unsigned int cur_time = timeGetTime();
		return cur_time - m_prevTime;
	}

private:
	unsigned int m_prevTime;
};



#endif // _TIMER_H_
