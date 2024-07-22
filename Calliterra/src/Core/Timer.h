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
	inline static Timer* s_ApplicationTimer = nullptr;
};

template<typename... Params>
class CountdownTimer
{
public:
	CountdownTimer(float periodInMilliseconds, std::function<void(Params...)> callback)
		: m_Period(periodInMilliseconds), m_Callback(callback)
	{
		Reset();
	}

	CountdownTimer(float periodInMilliseconds)
		: m_Period(periodInMilliseconds), m_Callback(nullptr)
	{
		Reset();
	}

	void Update(Params... params)
	{
		if (GetElapsedInMilliseconds() >= m_Period)
		{
			Reset();
			if (m_Callback != nullptr)
			{
				m_Callback(params...);
			}
		}
	}

	void SetCallback(std::function<void(Params...)> callback)
	{
		m_Callback = callback;
	}

	float GetElapsedInMilliseconds()
	{
		return static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_Start).count());
	}

	float GetElapsedInSeconds()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() / 1e3f;
	}

private:
	void Reset()
	{
		m_Start = std::chrono::high_resolution_clock::now();
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	float m_Period;
	std::function<void(Params...)> m_Callback;

};
