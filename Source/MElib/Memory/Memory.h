#pragma once

#include "MElib/Core/Core.h"
#include "MElib/Memory/Buffer.h"

#include <vector>

namespace MElib {

	enum class ResizePolicy
	{
		Exact, Geometric
	};

	class Memory
	{
	public:
		static void Write(void* destination, const void* source, uint64 byteSize);
		static void Write(void* destination, const Buffer source)															{ Write(destination, source.Memory, source.Size); }
		static void Write(void* destination, uint64 offset, const void* source, uint64 byteSize);
		static void Write(void* destination, uint64 offset, const Buffer source)											{ Write(destination, offset, source.Memory, source.Size); }
		static void WriteZero(void* destination, uint64 byteSize);
		static void WriteZero(void* destination, uint64 byteSize, uint64 byteOffset);

		static void Read(void* destination, uint64 byteCount, const void* memory, uint64 byteOffset);
		static void Read(const MutableBuffer destination, const void* memory, uint64 byteOffset)							{ Read(destination.Memory, destination.Size, memory, byteOffset); }

		//
		// Write (UniqueBuffer)
		//
		// These functions will resize the given buffer
		// by calculating a geometric growth
		// when the size is to small
		//

		static void Write(ResizePolicy policy, UniqueBuffer& destination, const void* source, uint64 byteSize);
		static void Write(ResizePolicy policy, UniqueBuffer& destination, const Buffer source)								{ Write(policy, destination, source.Memory, source.Size); }
		static void Write(ResizePolicy policy, UniqueBuffer& destination, uint64 offset, const void* source, uint64 byteSize);
		static void Write(ResizePolicy policy, UniqueBuffer& destination, uint64 offset, const Buffer source)				{ Write(policy, destination, offset, source.Memory, source.Size); }

		//
		// Write (std::vector)
		//
		// These functions call resize on the given vector
		// when the size is to small
		//

		template<typename T>
		static void Write(std::vector<T>& destination, const void* source, uint64 byteSize);
		template<typename T>
		static void Write(std::vector<T>& destination, const Buffer source);
		template<typename T>
		static void Write(std::vector<T>& destination, uint64 elementOffset, const void* source, uint64 byteSize);
		template<typename T>
		static void Write(std::vector<T>& destination, uint64 elementOffset, const Buffer source);

		template<typename T>
		static void WriteZero(T& destination);

		template<typename T>
		static void Read(T& destination, const void* memory, uint64 byteOffset);
		template<typename T>
		static T    Read(const void* memory, uint64 byteOffset);
	};

}

namespace MElib {

	template<typename T>
	void Memory::Write(std::vector<T>& destination, const void* source, uint64 byteSize)
	{
		const uint64 count = (byteSize + sizeof(T) - 1) / sizeof(T);
		if (destination.size() < count)
			destination.resize(count);

		Write(destination.data(), source, byteSize);
	}

	template<typename T>
	void Memory::Write(std::vector<T>& destination, const Buffer source)
	{
		Write(destination, source.Memory, source.Size);
	}

	template<typename T>
	void Memory::Write(std::vector<T>& destination, uint64 elementOffset, const void* source, uint64 byteSize)
	{
		const uint64 count = (byteSize + sizeof(T) - 1) / sizeof(T);
		if (destination.size() < elementOffset + count)
			destination.resize(elementOffset + count);

		Write(destination.data(), elementOffset * sizeof(T), source, byteSize);
	}

	template<typename T>
	void Memory::Write(std::vector<T>& destination, uint64 elementOffset, const Buffer source)
	{
		Write(destination, elementOffset, source.Memory, source.Size);
	}

	template<typename T>
	void Memory::WriteZero(T& destination)
	{
		WriteZero(&destination, sizeof(T));
	}

	template<typename T>
	void Memory::Read(T& destination, const void* memory, uint64 byteOffset)
	{
		Read(&destination, sizeof(T), memory, byteOffset);
	}

	template<typename T>
	T Memory::Read(const void* memory, uint64 byteOffset)
	{
		T result;
		Read(result, memory, byteOffset);
		return result;
	}

}