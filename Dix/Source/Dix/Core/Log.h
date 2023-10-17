#pragma once

#include "Dix/Core/Base.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace Dix
{
	class Log
	{
	public:
		static void Init();

		static SharedPtr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static SharedPtr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static SharedPtr<spdlog::logger> s_CoreLogger;
		static SharedPtr<spdlog::logger> s_ClientLogger;
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

//Log macros
#define DIX_CORE_TRACE(...)    ::Dix::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DIX_CORE_INFO(...)     ::Dix::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DIX_CORE_WARN(...)     ::Dix::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DIX_CORE_ERROR(...)    ::Dix::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DIX_CORE_CRITICAL(...) ::Dix::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define DIX_TRACE(...)    ::Dix::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DIX_INFO(...)     ::Dix::Log::GetClientLogger()->info(__VA_ARGS__)
#define DIX_WARN(...)     ::Dix::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DIX_ERROR(...)    ::Dix::Log::GetClientLogger()->error(__VA_ARGS__)
#define DIX_CRITICAL(...) ::Dix::Log::GetClientLogger()->critical(__VA_ARGS__)