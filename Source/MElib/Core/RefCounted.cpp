#include "RefCounted.h"

#include <set>
#include <mutex>

#ifndef ENABLE_ASSERT
	#define ENABLE_ASSERT 1
#endif

#include "MElib/Core/Assert.h"

namespace MElib {

	static std::mutex s_Mutex;
	static std::set<void*> s_AliveInstances;

	void Internal::RefUtils::AddAliveInstance(void* instance)
	{
		ME_CORE_ASSERT(instance != nullptr);
		std::scoped_lock lock(s_Mutex);
		s_AliveInstances.insert(instance);
	}

	void Internal::RefUtils::RemoveAliveInstance(void* instance)
	{
		ME_CORE_ASSERT(instance != nullptr);
		std::scoped_lock lock(s_Mutex);
		s_AliveInstances.erase(instance);
	}

	bool Internal::RefUtils::IsAlive(void* instance)
	{
		std::scoped_lock lock(s_Mutex);
		return s_AliveInstances.contains(instance);
	}

}
