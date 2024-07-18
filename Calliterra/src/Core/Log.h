#pragma once

#define SPDLOG_WCHAR_TO_UTF8_SUPPORT
#include <spdlog/spdlog.h>

class Log
{
public:
	static void Init();

	static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger;  }

private:
	 inline static std::shared_ptr<spdlog::logger> s_Logger;
};

// Define the logging macros for debug builds
#ifdef DEBUG
	#define LOG_TRACE(...)		Log::GetLogger()->trace(__VA_ARGS__)
	#define LOG_DEBUG(...)		Log::GetLogger()->debug(__VA_ARGS__)
	#define LOG_INFO(...)		Log::GetLogger()->info(__VA_ARGS__)
	#define LOG_WARN(...)		Log::GetLogger()->warn(__VA_ARGS__)
	#define LOG_ERROR(...)		Log::GetLogger()->error(__VA_ARGS__)
	#define LOG_CRITICAL(...)	Log::GetLogger()->critical(__VA_ARGS__)
#else
// Remove the logging macros for non debug builds
	#define LOG_TRACE
	#define LOG_DEBUG
	#define LOG_INFO
	#define LOG_WARN
	#define LOG_ERROR
	#define LOG_CRITICAL
#endif
