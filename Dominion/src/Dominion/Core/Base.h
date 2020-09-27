#pragma once

#ifdef DM_DEBUG
	#if defined(DM_PLATFORM_WINDOWS)
		#define DM_DEBUGBREAK() __debugbreak();
	#else
		#error "Platform does not support debugbreak yet!"
	#endif

	#define DM_ENABLE_ASSERTS
#else
	#define DM_DEBUGBREAK()
#endif

#ifdef DM_ENABLE_ASSERTS
	#define DM_ASSERT(x, ...) { if(!(x)) { DM_ERROR("Assertion Failed: {0}", __VA_ARGS__); DM_DEBUGBREAK(); } }
	#define DM_CORE_ASSERT(x, ...) { if(!(x)) { DM_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); DM_DEBUGBREAK(); } }
#else
	#define DM_ASSERT(x, ...)
	#define DM_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)