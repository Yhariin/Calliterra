#include "pch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

void Log::Init()
{
	spdlog::set_pattern("%^[%T] [%l]: %v%$");
	s_Logger = spdlog::stdout_color_mt("CORE");
	s_Logger->set_level(spdlog::level::trace);
}
