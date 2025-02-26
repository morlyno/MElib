#pragma once

#include "MElib/Core/Core.h"
#include "MElib/Core/Logging.h"

#ifdef ME_PLATFORM_WINDOWS
	#define ME_DEBUG_BREAK __debugbreak
#endif

#if ENABLE_ASSERT
	#define ME_CORE_ASSERT_MESSAGE_INTERNAL(...) ::MElib::Logging::PrintAssertMessage("Core Assertion Failed" __VA_OPT__(,) __VA_ARGS__)
	#define ME_ASSERT_MESSAGE_INTERNAL(...) ::MElib::Logging::PrintAssertMessage("Assertion Failed" __VA_OPT__(,) __VA_ARGS__)

	#define ME_CORE_ASSERT(_condition, ...) if (!(_condition)) { ME_CORE_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); ME_DEBUG_BREAK(); }
	#define ME_ASSERT(_condition, ...) if (!(_condition)) { ME_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); ME_DEBUG_BREAK(); }
#else 
	#define ME_CORE_ASSERT(...) (void)0
	#define ME_ASSERT(...) (void)0
#endif

#ifdef ENABLE_ASSERT
	#define ME_CORE_VERIFY_MESSAGE_INTERNAL(...) ::MElib::Logging::PrintAssertMessage("Core Verify Failed" __VA_OPT__(,) __VA_ARGS__)
	#define ME_VERIFY_MESSAGE_INTERNAL(...) ::MElib::Logging::PrintAssertMessage("Verify Failed" __VA_OPT__(,) __VA_ARGS__)

	#define ME_CORE_VERIFY(_condition, ...) if (!(_condition)) { ME_CORE_VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); ME_DEBUG_BREAK(); }
	#define ME_VERIFY(_condition, ...) if (!(_condition)) { ME_VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); ME_DEBUG_BREAK(); }
#else
	#define ME_CORE_VERIFY(...) (void)0
	#define ME_VERIFY(...) (void)0
#endif
