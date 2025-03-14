#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////
///// Platform ////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(ME_PLATFORM_WINDOWS) // && ...
	#if defined(_WIN32) || defined(_WIN64)
		#define ME_PLATFORM_WINDOWS 1
	#else
		#error unknown platform
	#endif

#endif

#if (ME_PLATFORM_WINDOWS /* + ...*/) != 1
	#error Invalid platform configuration
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
///// Compiler ////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(ME_COMPILER_MSVC) // && ...
	#if defined(_MSC_VER)
		#define ME_COMPILER_MSVC 1
	#else
		#error Unkown Compiler!
	#endif
#endif

#if (ME_COMPILER_MSVC /* + ... */) != 1
	#error Invalid compiler detected
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ME_DEBUG_BREAK
	#ifdef ME_PLATFORM_WINDOWS
		#define ME_DEBUG_BREAK __debugbreak
	#endif
#endif


#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif

namespace MElib {

	inline namespace Types {
		using int8 = signed char;
		using int16 = signed short;
		using int32 = signed int;
		using int64 = signed long long;
		using uint8 = unsigned char;
		using uint16 = unsigned short;
		using uint32 = unsigned int;
		using uint64 = unsigned long long;
	}

	struct Numeric
	{
		static constexpr int8  Int8Min           = (int8 )0x80;
		static constexpr int16 Int16Min          = (int16)0x8000;
		static constexpr int32 Int32Min          = (int32)0x80000000;
		static constexpr int64 Int64Min          = (int64)0x8000000000000000;

		static constexpr int8  Int8Max           = 0x7f;
		static constexpr int16 Int16Max          = 0x7fff;
		static constexpr int32 Int32Max          = 0x7fffffff;
		static constexpr int64 Int64Max          = 0x7fffffffffffffff;

		static constexpr uint8  UInt8Min         = 0;
		static constexpr uint16 UInt16Min        = 0;
		static constexpr uint32 UInt32Min        = 0;
		static constexpr uint64 UInt64Min        = 0;

		static constexpr uint8  UInt8Max         = 0xff;
		static constexpr uint16 UInt16Max        = 0xffff;
		static constexpr uint32 UInt32Max        = 0xffffffff;
		static constexpr uint64 UInt64Max        = 0xffffffffffffffff;

		static constexpr float FloatMin          = 1.175494351e-38f;
		static constexpr float FloatMax          = 3.402823466e+38f;
		static constexpr float FloatEpsilon      = 1.192092896e-07F;
	};

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//// Configuration ////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

// 
// This guards the calls to Buffer.Write/Buffer.Read if a check and return.
// There is no notification in any way (unless ENABLE_VERIFY is defined)
// 
// #define ENABLE_SAFE_BUFFER_ACCESS
// 

// 
// This extends the behavior of ENABLE_SAFE_BUFFER_ACCESS by throwing an exception
// there is still a VERIFY before this if ENABLE_VERIFY is defined
// 
// #define ENABLE_SAFE_BUFFER_EXCEPTIONS
// 
