
#ifndef _PERFTIMER_H_
#define _PERFTIMER_H_


class PerfTimer
{
public:
	PerfTimer() {}

	static void Init()
	{
		LARGE_INTEGER freq;
		BOOL result = QueryPerformanceFrequency(&freq);
		m_ticktime = result? 1.0 / double(freq.QuadPart): 0.0;
	}

	void Reset()
	{
		QueryPerformanceCounter(&m_ticks);
	}

	double Time()
	{
		LARGE_INTEGER cur_ticks;
		QueryPerformanceCounter(&cur_ticks);
		return double(cur_ticks.QuadPart - m_ticks.QuadPart) * m_ticktime;
	}

	__int64 TimeTicks()
	{
		LARGE_INTEGER cur_ticks;
		QueryPerformanceCounter(&cur_ticks);
		return cur_ticks.QuadPart - m_ticks.QuadPart;
	}

private:
	double m_ticktime;
	LARGE_INTEGER m_ticks;
};


#endif // _PERFTIMER_H_

