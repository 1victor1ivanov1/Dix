#pragma once

#include "Dix/Core/PlatformDetection.h"

#include <memory>

#ifdef DIX_DEBUG
	#ifdef DIX_PLATFORM_WINDOWS
		#define DIX_DEBUGBREAK() __debugbreak()
	#else
		#error "Platform doesn`t support debugbreak yet!"
	#endif

	#define DIX_ENABLE_ASSERTS
#else
	#define DIX_DEBUGBREAK()
#endif

#define BITS(x) 1 << x

#define DIX_EXPAND_MACRO(x) x
#define DIX_STRINGIFY_MACRO(x) #x

#define DIX_BIND_EVENT_CALLBACK(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Dix
{
	template<typename T>
	using UniquePtr = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr UniquePtr<T> MakeUnique(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using SharedPtr = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr SharedPtr<T> MakeShared(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "Dix/Core/Assert.h"
#include "Dix/Core/Log.h"