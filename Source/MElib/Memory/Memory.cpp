#include "Memory.h"

#include "MElib/Core/Assert.h"

namespace MElib {

	namespace utils {

		static uint64 CalculateGrowth(uint64 capacity, uint64 newSize)
		{
			const uint64 geometric = capacity + capacity / 2;

			if (geometric < newSize)
				return newSize;

			return geometric;
		}

		static uint64 CalculateGrowth(ResizePolicy policy, uint64 capacity, uint64 newSize)
		{
			switch (policy)
			{
				case ResizePolicy::Exact:
				{
					return newSize;
				}
				case ResizePolicy::Geometric:
				{
					const uint64 geometric = capacity + capacity / 2;

					if (geometric < newSize)
						return newSize;

					return geometric;
				}
			}

			ME_CORE_ASSERT(false, "Unknown ResizePolicy");
			return newSize;
		}

	}

	void Memory::Write(void* destination, const void* source, uint64 byteSize)
	{
		memcpy(destination, source, byteSize);
	}

	void Memory::Write(void* destination, uint64 offset, const void* source, uint64 byteSize)
	{
		memcpy(static_cast<uint8*>(destination) + offset, source, byteSize);
	}

	void Memory::WriteZero(void* destination, uint64 byteSize)
	{
		memset(destination, 0, byteSize);
	}

	void Memory::WriteZero(void* destination, uint64 byteSize, uint64 byteOffset)
	{
		memset(static_cast<uint8*>(destination) + byteOffset, 0, byteSize);
	}

	void Memory::Read(void* destination, uint64 byteCount, const void* memory, uint64 byteOffset)
	{
		memcpy(destination, static_cast<const uint8*>(memory) + byteOffset, byteCount);
	}

	void Memory::Write(ResizePolicy policy, UniqueBuffer& destination, const void* source, uint64 byteSize)
	{
		if (destination.Size < byteSize)
			destination.Resize(utils::CalculateGrowth(policy, destination.Size, byteSize));

		Write(destination.Memory, source, byteSize);
	}

	void Memory::Write(ResizePolicy policy, UniqueBuffer& destination, uint64 offset, const void* source, uint64 byteSize)
	{
		if (destination.Size < offset + byteSize)
			destination.Resize(utils::CalculateGrowth(policy, destination.Size, offset + byteSize));

		Write(destination.Memory, offset, source, byteSize);
	}

}
