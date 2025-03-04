#pragma once

#include "MElib/Core/Core.h"
#include <span>

namespace MElib::Memory {
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//// Buffer ///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////

	class Buffer
	{
	public:
		Buffer() = default;
		Buffer(std::nullptr_t) {}
		Buffer(void* memory, uint64 size) : Memory(memory), Size(size) {}
		~Buffer() = default;

		void Allocate(uint64 size);
		void Resize(uint64 newSize, bool canShrink = true);
		void Release();

		void WriteZero() const;
		void Write(const void* data, uint64 size, uint64 offset = 0) const;
		void Read(void* targetMemory, uint64 size, uint64 offset = 0) const;

	public:
		static Buffer Copy(Buffer other);
		static Buffer Copy(const void* data, uint64 size);

		template<typename TRange>
		static Buffer FromRange(const TRange& range)
			requires std::ranges::contiguous_range<TRange>;

	public:
		void* Memory = nullptr;
		uint64 Size = 0;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//// Owning Buffer ////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////

	class OwningBuffer
	{
	public:
		OwningBuffer() = default;
		OwningBuffer(std::nullptr_t) {}
		OwningBuffer(OwningBuffer&& other) noexcept;
		OwningBuffer& operator=(OwningBuffer&& other) noexcept;
		~OwningBuffer();

		void Allocate(uint64 size);
		void Resize(uint64 newSize, bool canShrink = true);
		void Release();

		void WriteZero() const;
		void Write(const void* data, uint64 size, uint64 offset = 0) const;
		void Read(void* targetMemory, uint64 size, uint64 offset = 0) const;

		Buffer ExtractBuffer();
		Buffer GetBuffer() const;

	public:
		static OwningBuffer AdoptMemory(void* memory, uint64 size);
		static OwningBuffer AdoptMemory(Buffer buffer);

	private:
		void* m_Memory = nullptr;
		uint64 m_Size = 0;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//// Buffer Arg ///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////

	class BufferArg
	{
	public:
		BufferArg() = default;
		BufferArg(BufferArg&& other) noexcept;
		BufferArg& operator=(BufferArg&& other) noexcept;
		~BufferArg();

		void Release();

		Buffer& GetBuffer();
		Buffer GetBuffer() const;

		static BufferArg Adopt(Buffer buffer);
		static BufferArg Borrow(Buffer buffer);
		
		static BufferArg Adopt(OwningBuffer& buffer);
		static BufferArg Borrow(OwningBuffer buffer);

		template<typename TRange>
		static BufferArg Borrow(const TRange& range)
			requires std::ranges::contiguous_range<TRange>;

	private:
		BufferArg(const BufferArg& other) = delete;
		BufferArg& operator=(const BufferArg& other) = delete;

	private:
		Buffer m_Buffer;
		bool m_Owns = false;
	};

}

namespace MElib::Memory {

	template<typename TRange>
	static Buffer Buffer::FromRange(const TRange& range)
		requires std::ranges::contiguous_range<TRange>
	{
		std::span span = range;
		return Buffer((void*)span.data(), span.size_bytes());
	}

	template<typename TRange>
	static BufferArg BufferArg::Borrow(const TRange& range)
		requires std::ranges::contiguous_range<TRange>
	{
		return Borrow(Buffer::FromRange(range));
	}

}
