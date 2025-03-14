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

	uint64 Hash::GenerateFNV(const std::string_view str)
	{
		return FNV1aAppendBytes(s_FNV_Offset_Basis, (const uint8*)str.data(), str.size());
	}

	uint64 Hash::GenerateFNV(Memory::BufferArg arg)
	{
		auto buffer = arg.GetBuffer();
		FNV1aAppendBytes(s_FNV_Offset_Basis, buffer.As<uint8>(), buffer.Size);
	}

}
