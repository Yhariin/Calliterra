#pragma once
#include <chrono>

class DeltaTime
{
public:
	DeltaTime(double timeInMicroseconds = 0.0f)
		: m_DeltaTimeMicroseconds(timeInMicroseconds)
	{
	}

	static double GetCurrentTimeMilliseconds()
	{
		return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	}
	
	static double GetCurrentTimeMicroseconds()
	{
		return static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	}

	double GetMicroSeconds()
	{
		return m_DeltaTimeMicroseconds;
	}

	double GetMilliseconds()
	{
		return m_DeltaTimeMicroseconds / 1e3f;
	}

	double GetSeconds()
	{
		return m_DeltaTimeMicroseconds / 1e6f;
	}

private:
	double m_DeltaTimeMicroseconds;
};
