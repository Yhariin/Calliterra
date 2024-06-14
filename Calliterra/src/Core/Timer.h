#pragma once
#include <chrono>

class Timer
{
	friend class Application;
public:
	Timer()
	{
		Reset();
	}

	void Reset()
	{
		m_Start = std::chrono::high_resolution_clock::now();
	}

	double GetElapsedInMicroseconds()
	{
		return static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_Start).count());
	}

	double GetElapsedInMilliseconds()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() / 1e3;
	}

	double GetElapsedInSeconds()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() / 1e6;
	}

	static Timer& GetApplicationTimer()
	{
		return *s_ApplicationTimer;
	}

private:
	static void SetApplicationTimer(Timer* timer)
	{
		s_ApplicationTimer = timer;
	}

	std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	static Timer* s_ApplicationTimer;
};
