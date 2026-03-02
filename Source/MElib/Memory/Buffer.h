#pragma once

#include "MElib/Core/Core.h"

#include <ranges>

namespace MElib {

	class Buffer;
	class MutableBuffer;
	class UniqueBuffer;

	//////////////////////////////////////////////////////////////////////////
	//// Const Buffer
	//////////////////////////////////////////////////////////////////////////

	class Buffer
	{
	public:
		Buffer() = default;
		Buffer(std::nullptr_t) {}
		Buffer(const void* memory, uint64 size) : Memory(memory), Size(size) {}
		Buffer(const MutableBuffer buffer);
		~Buffer() = default;

		template<typename TRange>
			requires std::ranges::contiguous_range<TRange>
		Buffer(const TRange& data);

		template<typename TValue>
			requires (std::is_trivially_copyable_v<TValue> && !std::ranges::contiguous_range<TValue>)
		Buffer(const TValue& data);

		Buffer(Buffer&& other) noexcept;
		Buffer& operator=(Buffer&& other) noexcept;
		Buffer(const Buffer& other) = default;
		Buffer& operator=(const Buffer& other) = default;

		template<typename T>
		const T* As() const;

	public:
		const void* Memory = nullptr;
		uint64 Size = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	//// Buffer
	//////////////////////////////////////////////////////////////////////////

	class MutableBuffer
	{
	public:
		MutableBuffer() = default;
		MutableBuffer(std::nullptr_t) {}
		MutableBuffer(void* memory, uint64 size) : Memory(memory), Size(size) {}
		~MutableBuffer() = default;

		MutableBuffer(MutableBuffer&& other) noexcept;
		MutableBuffer& operator=(MutableBuffer&& other) noexcept;
		MutableBuffer(const MutableBuffer& other) = default;
		MutableBuffer& operator=(const MutableBuffer& other) = default;

	public:
		void Allocate(uint64 size);
		void Resize(uint64 newSize, bool canShrink = true);
		void Release();

		void WriteZero() const;
		void Write(const void* data, uint64 size, uint64 offset = 0) const;
		void Read(void* targetMemory, uint64 size, uint64 offset = 0) const;

		template<typename T>
		T* As();

	public:
		static MutableBuffer Copy(const Buffer data);
		static MutableBuffer Copy(const void* data, uint64 size);

	public:
		void* Memory = nullptr;
		uint64 Size = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	//// Unique Buffer
	//////////////////////////////////////////////////////////////////////////

	class UniqueBuffer : private MutableBuffer
	{
	public:
		UniqueBuffer() = default;
		UniqueBuffer(std::nullptr_t) {}
		UniqueBuffer(MutableBuffer&& buffer);
		~UniqueBuffer();

		UniqueBuffer(UniqueBuffer&& other) noexcept;
		UniqueBuffer& operator=(UniqueBuffer&& other) noexcept;
		UniqueBuffer(const UniqueBuffer&)            = delete;
		UniqueBuffer& operator=(const UniqueBuffer&) = delete;

		using MutableBuffer::Allocate;
		using MutableBuffer::Resize;
		using MutableBuffer::Release;

		using MutableBuffer::WriteZero;
		using MutableBuffer::Write;
		using MutableBuffer::Read;

		using MutableBuffer::As;

		MutableBuffer ExtractBuffer();
		Buffer AsBuffer() const;

	public:
		using MutableBuffer::Memory;
		using MutableBuffer::Size;
	};

	struct AsBuffer_t
	{
		Buffer operator()(const Buffer& buffer) const { return buffer; }
		Buffer operator()(const MutableBuffer& buffer) const { return buffer; }
		Buffer operator()(const UniqueBuffer& buffer) const { return buffer.AsBuffer(); }
	};

	static constexpr AsBuffer_t AsBuffer = {};

}

namespace MElib {

	template<typename TRange>
		requires std::ranges::contiguous_range<TRange>
	Buffer::Buffer(const TRange& data)
		:
		Memory(std::ranges::data(data)),
		Size(std::ranges::size(data) * sizeof(std::ranges::range_value_t<TRange>))
	{
	}

	template<typename TValue>
		requires (std::is_trivially_copyable_v<TValue> && !std::ranges::contiguous_range<TValue>)
	Buffer::Buffer(const TValue& data)
		:
		Memory(std::addressof(data)),
		Size(sizeof data)
	{
	}


	template<typename T>
	const T* Buffer::As() const
	{
		return static_cast<const T*>(Memory);
	}

	template<typename T>
	T* MutableBuffer::As()
	{
		return static_cast<T*>(Memory);
	}

}
