#pragma once
#include <chrono>

class DeltaTime
{
	friend class Application;
public:
	static double GetCurrentTimeMilliseconds()
	{
		return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
	}
	
	static double GetCurrentTimeMicroseconds()
	{
		return static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
	}

	double GetMicroSeconds()
	{
		return m_DeltaTimeMicroseconds;
	}

	double GetMilliseconds()
	{
		return m_DeltaTimeMicroseconds / 1e3;
	}

	double GetSeconds()
	{
		return m_DeltaTimeMicroseconds / 1e6;
	}

	static DeltaTime& Get()
	{
		return *s_Instance;
	}

private:
	DeltaTime(double timeInMicroseconds = 0.0f)
		: m_DeltaTimeMicroseconds(timeInMicroseconds)
	{
		s_Instance = this;
	}

	double m_DeltaTimeMicroseconds;
	inline static DeltaTime* s_Instance = nullptr;
};
