#pragma once

#include "Dix/Core/Base.h"
#include "Dix/Core/Log.h"
#include <filesystem>

#ifdef DIX_ENABLE_ASSERTS

	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define DIX_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { DIX##type##ERROR(msg, __VA_ARGS__); DIX_DEBUGBREAK(); } }
	#define DIX_INTERNAL_ASSERT_WITH_MSG(type, check, ...) DIX_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define DIX_INTERNAL_ASSERT_NO_MSG(type, check) DIX_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", DIX_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define DIX_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define DIX_INTERNAL_ASSERT_GET_MACRO(...) DIX_EXPAND_MACRO( DIX_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, DIX_INTERNAL_ASSERT_WITH_MSG, DIX_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define DIX_ASSERT(...) DIX_EXPAND_MACRO( DIX_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define DIX_CORE_ASSERT(...) DIX_EXPAND_MACRO( DIX_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define DIX_ASSERT(...)
	#define DIX_CORE_ASSERT(...)
#endif