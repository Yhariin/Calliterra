#pragma once

#ifdef _DEBUG
	#define DEBUGBREAK() __debugbreak()
	#define ENABLE_ASSERTS
#else
	#define DEBUGBREAK()
#endif

#ifdef ENABLE_ASSERTS
	#define ASSERT(x,...) { if(!(x)) { LOG_ERROR("Assertion '{0}' failed at {1}:{2}", __VA_ARGS__, __FILE__, __LINE__); DEBUGBREAK();}}
	#define ASSERT_VERIFY(x,...) { if(!(x)) { LOG_ERROR("Assertion '{0}' failed at {1}:{2}", __VA_ARGS__, __FILE__, __LINE__); DEBUGBREAK();}}
#else
	#define ASSERT(x,...)
	#define ASSERT_VERIFY(x, ...) x
#endif
