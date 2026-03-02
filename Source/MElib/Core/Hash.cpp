#include "Hash.h"

namespace MElib {

	static constexpr uint64 s_FNV_Offset_Basis = 14695981039346656037;
	static constexpr uint64 s_FNV_Prime = 1099511628211;

	static uint64 FNV1aAppendBytes(uint64 hash, const uint8* first, uint64 bytes)
	{
		for (uint64 i = 0; i < bytes; i++)
		{
			hash ^= (uint64)first[i];
			hash *= s_FNV_Prime;
		}
		return hash;
	}

	uint64 Hash::GenerateFNV(const Buffer buffer)
	{
		return FNV1aAppendBytes(s_FNV_Offset_Basis, buffer.As<uint8>(), buffer.Size);
	}

}
