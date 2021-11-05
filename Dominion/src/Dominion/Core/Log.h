#pragma once

#include "Dominion/Core/Base.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

// This ignores all warnings raised inside external headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Dominion {

	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return sCoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return sClientLogger; }
	private:
		static Ref<spdlog::logger> sCoreLogger;
		static Ref<spdlog::logger> sClientLogger;
	};

}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

// Core log macros
#define DM_CORE_TRACE(...)    ::Dominion::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DM_CORE_INFO(...)     ::Dominion::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DM_CORE_WARN(...)     ::Dominion::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DM_CORE_ERROR(...)    ::Dominion::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DM_CORE_CRITICAL(...) ::Dominion::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define DM_TRACE(...)         ::Dominion::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DM_INFO(...)          ::Dominion::Log::GetClientLogger()->info(__VA_ARGS__)
#define DM_WARN(...)          ::Dominion::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DM_ERROR(...)         ::Dominion::Log::GetClientLogger()->error(__VA_ARGS__)
#define DM_CRITICAL(...)      ::Dominion::Log::GetClientLogger()->critical(__VA_ARGS__)
