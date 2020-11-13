#pragma once

#include "Dominion/Core/PlatformDetection.h"

#include <memory>

#if defined(DM_DEBUG)
	#if defined(DM_PLATFORM_WINDOWS)
		#define DM_DEBUGBREAK() __debugbreak();
	#else
		#error "Platform does not support debugbreak yet!"
	#endif

	#define DM_ENABLE_ASSERTS
	#define DM_INCLUDE_IMGUI 1
	#define DM_ENABLE_CORE_LOGGER 1
	#define DM_ENABLE_CLIENT_LOGGER 1
	#define DM_PROFILE 0
#elif defined(DM_RELEASE)
	#define DM_DEBUGBREAK()
	#define DM_INCLUDE_IMGUI 1
	#define DM_ENABLE_CORE_LOGGER 1
	#define DM_ENABLE_CLIENT_LOGGER 1
	#define DM_PROFILE 0
#else
	#define DM_DEBUGBREAK()
	#define DM_INCLUDE_IMGUI 1 // TODO: Bug in the moment when set to 0
	#define DM_ENABLE_CORE_LOGGER 1
	#define DM_ENABLE_CLIENT_LOGGER 1
	#define DM_PROFILE 0
#endif

#define DM_EXPAND_MACRO(x) x
#define DM_STRINGIFY_MACRO(x) #x

#if defined(DM_DEBUG)
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#define new DEBUG_NEW
#endif

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
