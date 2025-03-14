
// NOTE: No include guard
// #pragma once


#include "MElib/Core/Core.h"

#include <format>
#include <source_location>

#ifdef ME_ASSERT
	#undef ME_CORE_ASSERT_MESSAGE_INTERNAL
	#undef ME_ASSERT_MESSAGE_INTERNAL
	#undef ME_CORE_ASSERT
	#undef ME_ASSERT
#endif

#ifdef ME_VERIFY
	#define ME_CORE_VERIFY_MESSAGE_INTERNAL
	#define ME_VERIFY_MESSAGE_INTERNAL
	#define ME_CORE_VERIFY
	#define ME_VERIFY
#endif

namespace MElib {
	extern void PrintAssertMessage(std::string_view prefix, std::string_view condition, std::string_view message = {}, const std::source_location& location = std::source_location::current());
}

#if ENABLE_ASSERT
	#define ME_CORE_ASSERT_MESSAGE_INTERNAL(_condition, ...) ::MElib::PrintAssertMessage("Core Assertion Failed", _condition __VA_OPT__(,) __VA_OPT__(std::format(__VA_ARGS__)))
	#define ME_ASSERT_MESSAGE_INTERNAL(_condition, ...) ::MElib::PrintAssertMessage("Assertion Failed", _condition __VA_OPT__(,) __VA_OPT__(std::format(__VA_ARGS__)))

	#define ME_CORE_ASSERT(_condition, ...) if (!(_condition)) { ME_CORE_ASSERT_MESSAGE_INTERNAL(#_condition, __VA_ARGS__); ME_DEBUG_BREAK(); }
	#define ME_ASSERT(_condition, ...) if (!(_condition)) { ME_ASSERT_MESSAGE_INTERNAL(#_condition, __VA_ARGS__); ME_DEBUG_BREAK(); }
#else 
	#define ME_CORE_ASSERT(...) (void)0
	#define ME_ASSERT(...) (void)0
#endif

#ifdef ENABLE_VERIFY
	#define ME_CORE_VERIFY_MESSAGE_INTERNAL(...) ::MElib::PrintAssertMessage("Core Verify Failed" __VA_OPT__(,) __VA_OPT__(std::format(__VA_ARGS__)))
	#define ME_VERIFY_MESSAGE_INTERNAL(...) ::MElib::PrintAssertMessage("Verify Failed" __VA_OPT__(,) __VA_OPT__(std::format(__VA_ARGS__)))

	#define ME_CORE_VERIFY(_condition, ...) if (!(_condition)) { ME_CORE_VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); ME_DEBUG_BREAK(); }
	#define ME_VERIFY(_condition, ...) if (!(_condition)) { ME_VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); ME_DEBUG_BREAK(); }
#else
	#define ME_CORE_VERIFY(...) (void)0
	#define ME_VERIFY(...) (void)0
#endif
