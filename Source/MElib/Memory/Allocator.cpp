#include "Allocator.h"
#include <stdlib.h>

namespace MElib {

	void* Memory::Allocator::Allocate(uint64 size)
	{
		return malloc(size);
	}

	void Memory::Allocator::Free(void* memory)
	{
		free(memory);
	}

	void* Memory::Allocator::Reallocate(void* memory, uint64 newSize)
	{
		return realloc(memory, newSize);
	}

}
