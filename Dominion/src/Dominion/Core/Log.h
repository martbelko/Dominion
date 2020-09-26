#pragma once

#include "Dominion/Core/Base.h"
#include "spdlog/spdlog.h"

namespace Dominion {

	class Log
	{
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger>& GetCoreLogger();
		static std::shared_ptr<spdlog::logger>& GetClientLogger();
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

#define DM_CORE_TRACE(...) ::Dominion::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DM_CORE_INFO(...)  ::Dominion::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DM_CORE_WARN(...)  ::Dominion::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DM_CORE_ERROR(...) ::Dominion::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DM_CORE_FATAL(...) ::Dominion::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define DM_TRACE(...) ::Dominion::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DM_INFO(...)  ::Dominion::Log::GetClientLogger()->info(__VA_ARGS__)
#define DM_WARN(...)  ::Dominion::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DM_ERROR(...) ::Dominion::Log::GetClientLogger()->error(__VA_ARGS__)
#define DM_FATAL(...) ::Dominion::Log::GetClientLogger()->critical(__VA_ARGS__)
