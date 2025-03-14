#pragma once

#include "MElib/Core/Core.h"
#include "MElib/Memory/Buffer.h"
#include <string_view>

namespace MElib {

	class Hash
	{
	public:
		static uint64 GenerateFNV(const std::string_view str);
		static uint64 GenerateFNV(Memory::BufferArg arg);
	};

}
