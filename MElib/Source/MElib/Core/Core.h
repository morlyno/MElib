#pragma once

#if !ME_PLATFORM_WINDOWS
#error Platforms other than Windows are currently not supported!
#endif

#if defined(_MSC_VER)
	#define ME_COMPILER_MSVC 1
#else
	#error Unkown Compiler!
#endif

#ifdef ME_CONFIG_DEBUG
	#define ME_IF_DEBUG(x) x
#else
	#define ME_IF_DEBUG(...) (void)0
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
		static constexpr int8  Int8Min           = 0x80;
		static constexpr int16 Int16Min          = 0x8000;
		static constexpr int32 Int32Min          = 0x80000000;
		static constexpr int64 Int64Min          = 0x8000000000000000;

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
