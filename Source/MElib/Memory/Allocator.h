#pragma once

#include "MElib/Core/Core.h"

namespace MElib::Memory {

	class Allocator
	{
	public:
		static void* Allocate(uint64 size);
		static void* Reallocate(void* memory, uint64 newSize);
		static void Free(void* memory);
	};

}
