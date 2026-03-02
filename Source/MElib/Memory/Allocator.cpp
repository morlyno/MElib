#include "Allocator.h"
#include <stdlib.h>

namespace MElib {

	void* Allocator::Allocate(uint64 size)
	{
		return malloc(size);
	}

	void Allocator::Free(void* memory)
	{
		free(memory);
	}

	void* Allocator::Reallocate(void* memory, uint64 newSize)
	{
		return realloc(memory, newSize);
	}

}
