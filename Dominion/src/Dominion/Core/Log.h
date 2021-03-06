#pragma once

#include "Dominion/Core/Base.h"

#pragma warning(disable : 26812 6387 26495 26451) // Disable warnings from spdlog
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(default : 26812 6387 26495 26451) // Enable warnings to default

namespace Dominion {

	class Log
	{
	public:
		static void Init();
		static void Shutdown();
		static std::shared_ptr<spdlog::logger>& GetCoreLogger();
		static std::shared_ptr<spdlog::logger>& GetClientLogger();
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core logger macros
#if DM_ENABLE_CORE_LOGGER == 1
	#define DM_CORE_TRACE(...) ::Dominion::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define DM_CORE_INFO(...)  ::Dominion::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define DM_CORE_WARN(...)  ::Dominion::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define DM_CORE_ERROR(...) ::Dominion::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define DM_CORE_FATAL(...) ::Dominion::Log::GetCoreLogger()->critical(__VA_ARGS__)
#else
	#define DM_CORE_TRACE(...)
	#define DM_CORE_INFO(...)
	#define DM_CORE_WARN(...)
	#define DM_CORE_ERROR(...)
	#define DM_CORE_FATAL(...)
#endif

// Client logger macros
#if DM_ENABLE_CLIENT_LOGGER == 1
	#define DM_TRACE(...) ::Dominion::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define DM_INFO(...)  ::Dominion::Log::GetClientLogger()->info(__VA_ARGS__)
	#define DM_WARN(...)  ::Dominion::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define DM_ERROR(...) ::Dominion::Log::GetClientLogger()->error(__VA_ARGS__)
	#define DM_FATAL(...) ::Dominion::Log::GetClientLogger()->critical(__VA_ARGS__)
#else
	#define DM_TRACE(...)
	#define DM_INFO(...)
	#define DM_WARN(...)
	#define DM_ERROR(...)
	#define DM_FATAL(...)
#endif
