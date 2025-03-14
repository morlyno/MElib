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


namespace MElib::Memory {

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//// Buffer ///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////

	void Buffer::Allocate(uint64 size)
	{
		Release();

		if (size)
		{
			Size = size;
			Memory = Allocator::Allocate(size);
		}
	}

	void Buffer::Resize(uint64 newSize, bool canShrink)
	{
		if (newSize == Size || !canShrink && newSize < Size)
			return;

		Memory = Allocator::Reallocate(Memory, newSize);
		Size = newSize;
	}

	void Buffer::Release()
	{
		if (!Memory)
			return;

		Allocator::Free(Memory);
		Memory = nullptr;
		Size = 0;
	}

	void Buffer::WriteZero() const
	{
		memset(Memory, 0, Size);
	}

	void Buffer::Write(const void* memory, uint64 size, uint64 offset) const
	{
		ME_BUFFER_VERIFY(size + offset <= Size, "[Buffer.Write] Out of range! Size:{} + Offset:{} is greater than Buffer Size {}", size, offset, Size);
	
		BUFFER_GUARD(size + offset <= Size, "[Buffer.Write] Out of range!");
		memcpy((uint8*)Memory + offset, memory, size);
	}

	void Buffer::Read(void* targetMemory, uint64 size, uint64 offset) const
	{
		ME_BUFFER_VERIFY(size + offset <= Size, "[Buffer.Read] Out of range! Size:{} + Offset:{} is greater than buffer size {}", size, offset, Size);

		BUFFER_GUARD(size + offset <= Size, "[Buffer.Write] Out of range!");
		memcpy(targetMemory, (uint8*)Memory + offset, size);
	}

	Buffer Buffer::Copy(Buffer other)
	{
		Buffer result;
		result.Allocate(other.Size);
		result.Write(other.Memory, other.Size);
		return result;
	}

	Buffer Buffer::Copy(const void* data, uint64 size)
	{
		Buffer result;
		result.Allocate(size);
		result.Write(data, size);
		return result;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//// Owning Buffer ////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////

	OwningBuffer::~OwningBuffer()
	{
		Release();
	}

	OwningBuffer::OwningBuffer(OwningBuffer&& other) noexcept
	{
		m_Memory = other.m_Memory;
		m_Size = other.m_Size;
		other.m_Memory = nullptr;
		other.m_Size = 0;
	}

	OwningBuffer& OwningBuffer::operator=(OwningBuffer&& other) noexcept
	{
		if (this == std::addressof(other))
			return *this;

		Release();
		m_Memory = other.m_Memory;
		m_Size = other.m_Size;
		other.m_Memory = nullptr;
		other.m_Size = 0;
		return *this;
	}

	void OwningBuffer::Allocate(uint64 size)
	{
		Release();

		if (size)
		{
			m_Size = size;
			m_Memory = Allocator::Allocate(size);
		}
	}

	void OwningBuffer::Resize(uint64 newSize, bool canShrink)
	{
		if (newSize == m_Size || !canShrink && newSize < m_Size)
			return;

		m_Memory = Allocator::Reallocate(m_Memory, newSize);
		m_Size = newSize;
	}

	void OwningBuffer::Release()
	{
		if (!m_Memory)
			return;

		Allocator::Free(m_Memory);
		m_Memory = nullptr;
		m_Size = 0;
	}

	void OwningBuffer::WriteZero() const
	{
		memset(m_Memory, 0, m_Size);
	}

	void OwningBuffer::Write(const void* data, uint64 size, uint64 offset) const
	{
		ME_BUFFER_VERIFY(size + offset <= m_Size, "[OwningBuffer.Write] Out of range! Size:{} + Offset:{} is greater than Buffer Size {}", size, offset, m_Size);

		BUFFER_GUARD(size + offset <= m_Size, "[OwningBuffer.Write] Out of range!");
		memcpy((uint8*)m_Memory + offset, data, size);
	}

	void OwningBuffer::Read(void* targetMemory, uint64 size, uint64 offset) const
	{
		ME_BUFFER_VERIFY(size + offset <= m_Size, "[OwningBuffer.Read] Out of range! Size:{} + Offset:{} is greater than buffer size {}", size, offset, m_Size);

		BUFFER_GUARD(size + offset <= m_Size, "[OwningBuffer.Write] Out of range!");
		memcpy(targetMemory, (uint8*)m_Memory + offset, size);
	}

	OwningBuffer OwningBuffer::AdoptMemory(void* memory, uint64 size)
	{
		OwningBuffer result;
		result.m_Memory = memory;
		result.m_Size = size;
		return result;
	}

	OwningBuffer OwningBuffer::AdoptMemory(Buffer buffer)
	{
		OwningBuffer result;
		result.m_Memory = buffer.Memory;
		result.m_Size = buffer.Size;
		return result;
	}

	Buffer OwningBuffer::ExtractBuffer()
	{
		Buffer result{ m_Memory, m_Size };
		m_Memory = nullptr;
		m_Size = 0;
		return result;
	}

	Buffer OwningBuffer::GetBuffer() const
	{
		return { m_Memory, m_Size };
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//// Buffer Arg ///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////

	BufferArg::BufferArg(BufferArg&& other) noexcept
	{
		m_Buffer = other.m_Buffer;
		m_Owns = other.m_Owns;
		other.m_Buffer = {};
		other.m_Owns = false;
	}

	BufferArg& BufferArg::operator=(BufferArg&& other) noexcept
	{
		if (this == std::addressof(other))
			return *this;

		Release();
		m_Buffer = other.m_Buffer;
		m_Owns = other.m_Owns;
		other.m_Buffer = {};
		other.m_Owns = false;
		return *this;
	}

	BufferArg::~BufferArg()
	{
		Release();
	}

	void BufferArg::Release()
	{
		if (m_Owns)
		{
			m_Buffer.Release();
		}

		m_Buffer = {};
		m_Owns = false;
	}

	Buffer& BufferArg::GetBuffer()
	{
		return m_Buffer;
	}

	Buffer BufferArg::GetBuffer() const
	{
		return m_Buffer;
	}

	BufferArg BufferArg::Adopt(Buffer buffer)
	{
		BufferArg arg;
		arg.m_Buffer = buffer;
		arg.m_Owns = true;
		return arg;
	}

	BufferArg BufferArg::Borrow(Buffer buffer)
	{
		BufferArg arg;
		arg.m_Buffer = buffer;
		arg.m_Owns = false;
		return arg;
	}

	BufferArg BufferArg::Adopt(OwningBuffer& buffer)
	{
		BufferArg arg;
		arg.m_Buffer = buffer.ExtractBuffer();
		arg.m_Owns = true;
		return arg;
	}

	BufferArg BufferArg::Borrow(OwningBuffer buffer)
	{
		BufferArg arg;
		arg.m_Buffer = buffer.GetBuffer();
		arg.m_Owns = false;
		return arg;
	}

}
