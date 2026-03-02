#include "Buffer.h"

#include "MElib/Memory/Allocator.h"

#define ENABLE_VERIFY 1
#include "MElib/Core/Assert.h"

#define ME_BUFFER_ASSERT ME_CORE_ASSERT
#define ME_BUFFER_VERIFY ME_CORE_VERIFY


#if defined(ENABLE_SAFE_BUFFER_EXCEPTIONS) && !defined(ENABLE_SAFE_BUFFER_ACCESS)
	#define ENABLE_SAFE_BUFFER_ACCESS
#endif

#ifdef ENABLE_SAFE_BUFFER_ACCESS
	#ifdef ENABLE_SAFE_BUFFER_EXCEPTIONS
		#include <stdexcept>
		#define BUFFER_GUARD(_condition, _message, ...) if (!_condition) { throw std::out_of_range(_message); }
	#else
		#define BUFFER_GUARD(_condition, _message, ...) if (!_condition) { __VA_OPT__(__VA_ARGS__;)return; }
	#endif
#else
	#define BUFFER_GUARD(...) (void)0
#endif


namespace MElib {

	//////////////////////////////////////////////////////////////////////////
	//// Const Buffer
	//////////////////////////////////////////////////////////////////////////

	#pragma region ConstBuffer

	Buffer::Buffer(const MutableBuffer buffer)
		:
		Memory(buffer.Memory),
		Size(buffer.Size)
	{
	}

	Buffer::Buffer(Buffer&& other) noexcept
		:
		Memory(std::exchange(other.Memory, nullptr)),
		Size(std::exchange(other.Size, 0))
	{
	}

	Buffer& Buffer::operator=(Buffer&& other) noexcept
	{
		if (this == std::addressof(other))
			return *this;

		Memory = std::exchange(other.Memory, nullptr);
		Size = std::exchange(other.Size, 0);
		return *this;
	}

	#pragma endregion

	//////////////////////////////////////////////////////////////////////////
	//// Buffer
	//////////////////////////////////////////////////////////////////////////

	#pragma region Buffer

	MutableBuffer::MutableBuffer(MutableBuffer&& other) noexcept
		:
		Memory(std::exchange(other.Memory, nullptr)),
		Size(std::exchange(other.Size, 0))
	{
	}

	MutableBuffer& MutableBuffer::operator=(MutableBuffer&& other) noexcept
	{
		if (this == std::addressof(other))
			return *this;

		Memory = std::exchange(other.Memory, nullptr);
		Size   = std::exchange(other.Size, 0);
		return *this;
	}

	void MutableBuffer::Allocate(uint64 size)
	{
		Release();

		if (size)
		{
			Size = size;
			Memory = Allocator::Allocate(size);
		}
	}

	void MutableBuffer::Resize(uint64 newSize, bool canShrink)
	{
		if (newSize == Size || !canShrink && newSize < Size)
			return;

		Memory = Allocator::Reallocate(Memory, newSize);
		Size = newSize;
	}

	void MutableBuffer::Release()
	{
		if (!Memory)
			return;

		Allocator::Free(Memory);
		Memory = nullptr;
		Size = 0;
	}

	void MutableBuffer::WriteZero() const
	{
		memset(Memory, 0, Size);
	}

	void MutableBuffer::Write(const void* memory, uint64 size, uint64 offset) const
	{
		ME_BUFFER_VERIFY(size + offset <= Size, "[Buffer.Write] Out of range! Size:{} + Offset:{} is greater than Buffer Size {}", size, offset, Size);
	
		BUFFER_GUARD(size + offset <= Size, "[Buffer.Write] Out of range!");
		memcpy((uint8*)Memory + offset, memory, size);
	}

	void MutableBuffer::Read(void* targetMemory, uint64 size, uint64 offset) const
	{
		ME_BUFFER_VERIFY(size + offset <= Size, "[Buffer.Read] Out of range! Size:{} + Offset:{} is greater than buffer size {}", size, offset, Size);

		BUFFER_GUARD(size + offset <= Size, "[Buffer.Write] Out of range!");
		memcpy(targetMemory, (uint8*)Memory + offset, size);
	}

	MutableBuffer MutableBuffer::Copy(const Buffer data)
	{
		MutableBuffer result;
		result.Allocate(data.Size);
		result.Write(data.Memory, data.Size);
		return result;
	}

	MutableBuffer MutableBuffer::Copy(const void* data, uint64 size)
	{
		MutableBuffer result;
		result.Allocate(size);
		result.Write(data, size);
		return result;
	}

	#pragma endregion

	//////////////////////////////////////////////////////////////////////////
	//// Unique Buffer
	//////////////////////////////////////////////////////////////////////////

	#pragma region Unique Buffer

	UniqueBuffer::UniqueBuffer(MutableBuffer&& buffer)
		: MutableBuffer(std::exchange(buffer.Memory, nullptr),
						std::exchange(buffer.Size, 0))
	{
	}

	UniqueBuffer::~UniqueBuffer()
	{
		Release();
	}

	UniqueBuffer::UniqueBuffer(UniqueBuffer&& other) noexcept
		: MutableBuffer(std::exchange(other.Memory, nullptr),
						std::exchange(other.Size, 0))
	{
	}

	UniqueBuffer& UniqueBuffer::operator=(UniqueBuffer&& other) noexcept
	{
		if (this == std::addressof(other))
			return *this;

		Release();
		Memory = std::exchange(other.Memory, nullptr);
		Size   = std::exchange(other.Size, 0);
		return *this;
	}

	MutableBuffer UniqueBuffer::ExtractBuffer()
	{
		MutableBuffer result{ Memory, Size };
		Memory = nullptr;
		Size   = 0;
		return result;
	}

	Buffer UniqueBuffer::AsBuffer() const
	{
		return { Memory, Size };
	}

	#pragma endregion

}
