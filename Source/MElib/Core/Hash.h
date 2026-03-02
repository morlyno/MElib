#pragma once

#include "MElib/Core/Core.h"
#include "MElib/Memory/Buffer.h"

namespace MElib {

	class Hash
	{
	public:
		static uint64 GenerateFNV(const Buffer buffer);
	};

}
