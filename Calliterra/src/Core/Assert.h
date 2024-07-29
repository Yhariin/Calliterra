#pragma once

#ifdef DEBUG
	#define DEBUGBREAK() __debugbreak()
	#define ENABLE_ASSERTS
	#include <winerror.h>
	#include <comdef.h>

#else
	#define DEBUGBREAK()
#endif

#if defined DEBUG || RELEASE
	// Strips the function from non-debug builds
	#define STRIP_DEBUG(x) x
#else
	#define STRIP_DEBUG(x)
#endif


#ifdef ENABLE_ASSERTS
	#define EXPAND(x) x

	#define INTERNAL_ASSERT_IMPL(condition, msg, ...) { if(!(condition)) { LOG_ERROR(msg, __VA_ARGS__); DEBUGBREAK(); }}
	#define INTERNAL_ASSERT_WITH_MSG(condition, ...) INTERNAL_ASSERT_IMPL(condition, "Assertion '{0}' failed at {1}:{2}", __VA_ARGS__, __FILE__, __LINE__)
	#define INTERNAL_ASSERT_NO_MSG(condition) INTERNAL_ASSERT_IMPL(condition, "Assertion '{0}' failed at {1}:{2}", #condition, __FILE__, __LINE__)

	#define INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define INTERNAL_ASSERT_GET_MACRO(...) EXPAND( INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, INTERNAL_ASSERT_WITH_MSG, INTERNAL_ASSERT_NO_MSG) )

	// HRESULT specific macros
	#define INTERNAL_ASSERT_HR_IMPL(condition, msg, ...) { if(!(SUCCEEDED(condition))) { _com_error err(condition); LPCTSTR errMsg = err.ErrorMessage(); LOG_ERROR(msg, __VA_ARGS__); LOG_ERROR(errMsg); DEBUGBREAK(); }}
	#define INTERNAL_ASSERT_HR_WITH_MSG(condition, ...) INTERNAL_ASSERT_HR_IMPL(condition, "HRESULT Assertion '{0}' failed at {1}:{2}", __VA_ARGS__, __FILE__, __LINE__)
	#define INTERNAL_ASSERT_HR_NO_MSG(condition) INTERNAL_ASSERT_HR_IMPL(condition, "HRESULT Assertion '{0}' failed at {1}:{2}", #condition, __FILE__, __LINE__)

	#define INTERNAL_ASSERT_HR_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define INTERNAL_ASSERT_HR_GET_MACRO(...) EXPAND( INTERNAL_ASSERT_HR_GET_MACRO_NAME(__VA_ARGS__, INTERNAL_ASSERT_HR_WITH_MSG, INTERNAL_ASSERT_HR_NO_MSG) )

	// =======================These are the assert macros you should be using==================================
	#define ASSERT(...) EXPAND( INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(__VA_ARGS__) )

	// Use ASSERT_VERIFY if you want to test a function inside the assert
	#define ASSERT_VERIFY(...) EXPAND( INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(__VA_ARGS__) )

	// This assert should be used for winAPI functions that return an HRESULT
	#define ASSERT_HR(...) EXPAND( INTERNAL_ASSERT_HR_GET_MACRO(__VA_ARGS__)(__VA_ARGS__) )

#else
	#define ASSERT(x,...)
	#define ASSERT_VERIFY(x, ...) x
	#define ASSERT_HR(x, ...) x
#endif
