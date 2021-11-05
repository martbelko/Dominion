#pragma once

#include <memory>

#include "Dominion/Core/PlatformDetection.h"

#ifdef DM_DEBUG
	#if defined(DM_PLATFORM_WINDOWS)
		#define DM_DEBUGBREAK() __debugbreak()
	#elif defined(DM_PLATFORM_LINUX)
		#include <signal.h>
		#define DM_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define DM_ENABLE_ASSERTS
#else
	#define DM_DEBUGBREAK()
#endif

#define DM_EXPAND_MACRO(x) x
#define DM_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define DM_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Dominion {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#include "Dominion/Core/Log.h"
#include "Dominion/Core/Assert.h"
