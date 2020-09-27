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

#define BIT(x) (1 << x)