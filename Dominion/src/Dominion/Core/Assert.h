#pragma once

#include <filesystem>

#include "Dominion/Core/Base.h"
#include "Dominion/Core/Log.h"

#ifdef DM_ENABLE_ASSERTS

	#define DM_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { DM##type##ERROR(msg, __VA_ARGS__); DM_DEBUGBREAK(); } }
	#define DM_INTERNAL_ASSERT_WITH_MSG(type, check, ...) DM_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define DM_INTERNAL_ASSERT_NO_MSG(type, check) DM_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", DM_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define DM_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define DM_INTERNAL_ASSERT_GET_MACRO(...) DM_EXPAND_MACRO( DM_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, DM_INTERNAL_ASSERT_WITH_MSG, DM_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define DM_ASSERT(...) DM_EXPAND_MACRO( DM_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define DM_CORE_ASSERT(...) DM_EXPAND_MACRO( DM_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define DM_ASSERT(...)
	#define DM_CORE_ASSERT(...)
#endif
