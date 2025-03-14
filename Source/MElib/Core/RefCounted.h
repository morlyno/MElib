#pragma once

#include "MElib/Core/Core.h"

#include <type_traits>
#include <atomic>


#define ME_REF_TRACK_ALIVE_INSTANCES 1
//#define ME_ENABLE_WEAK_OPERATORS

static_assert(ME_REF_TRACK_ALIVE_INSTANCES);

namespace MElib {

	class RefCounted
	{
	public:
		RefCounted() = default;
		RefCounted(const RefCounted&) = delete;
		RefCounted(RefCounted&&) = delete;
		RefCounted& operator=(const RefCounted&) = delete;
		RefCounted& operator=(RefCounted&&) = delete;
		virtual ~RefCounted() = default;

	public:
		uint64 GetRefCount() const { return m_Count.load(); }
		void IncreaseRefCount() const { ++m_Count; }
		void DecreaseRefCount() const { --m_Count; }
	private:
		mutable std::atomic<uint64> m_Count = 0;
	};

	namespace Internal::RefUtils {
		void AddAliveInstance(void* instance);
		void RemoveAliveInstance(void* instance);
		bool IsAlive(void* instance);
	}

	namespace Concepts {

		template<typename T>
		concept IsRefCounted = requires
		{
			std::is_base_of_v<RefCounted, T>;
		};

	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	class Ref
	{
	public:
		Ref();
		Ref(std::nullptr_t);
		Ref(T* instance);

		Ref(const Ref& other);
		Ref(Ref&& other);
		Ref& operator=(const Ref& other);
		Ref& operator=(Ref&& other);
		Ref& operator=(std::nullptr_t);
		~Ref();

		template<typename TOther> requires std::is_convertible_v<TOther*, T*> Ref(const Ref<TOther>& other);
		template<typename TOther> requires std::is_convertible_v<TOther*, T*> Ref(Ref<TOther>&& other);
		template<typename TOther> requires std::is_convertible_v<TOther*, T*> Ref& operator=(const Ref<TOther>& other);
		template<typename TOther> requires std::is_convertible_v<TOther*, T*> Ref& operator=(Ref<TOther>&& other);

		void Release();
		T* operator->() const;
		T& operator*() const;
		operator bool() const;

		bool operator==(std::nullptr_t) const;
		bool operator!=(std::nullptr_t) const;
		bool operator==(Ref other) const;
		bool operator!=(Ref other) const;

		T* Raw();
		const T* Raw() const;

		template<typename TTo>
			requires std::is_base_of_v<T, TTo> || std::is_base_of_v<TTo, T>
		Ref<TTo> As() const;
		
		template<typename TTo>
			requires std::is_base_of_v<T, TTo> || std::is_base_of_v<TTo, T>
		Ref<TTo> AsSave() const;

		template<typename... TArgs>
		static Ref Create(TArgs&&... args);

	private:
		void IncreaseRef();
		void DecreaseRef();

		void AssignSave(const Ref& other);
		void AssignSave(Ref&& other);

	private:
		T* m_Instance;

		template<typename TOther>
			requires Concepts::IsRefCounted<TOther>
		friend class Ref;
	};

	template<typename T>
	class Weak
	{
	public:
		Weak() = default;
		Weak(std::nullptr_t) {}

		Weak(Ref<T> ref);
		Weak(T* instance);

		const Weak& operator=(Ref<T> ref);
		const Weak& operator=(T* instance);

		Ref<T> GetRef() const;
		Ref<T> TryGetRef() const;

		bool Expired() const;
		bool Alive() const;
		operator bool() const;

#ifdef ME_ENABLE_WEAK_OPERATORS
		T* operator->() const { return GetRef().operator->(); }
		T& operator*() const { return GetRef().operator*(); }
#endif

	private:
		T* m_Instance = nullptr;
	};

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//// Template Implementation //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

namespace MElib {

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//// Ref Implementation ///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename T>
		requires Concepts::IsRefCounted<T>
	MElib::Ref<T>::Ref()
		: m_Instance(nullptr)
	{
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	MElib::Ref<T>::Ref(std::nullptr_t)
		: m_Instance(nullptr)
	{
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	Ref<T>::Ref(T* instance)
	{
		m_Instance = instance;
		IncreaseRef();
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	Ref<T>::Ref(const Ref& other)
	{
		m_Instance = other.m_Instance;
		IncreaseRef();
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	Ref<T>::Ref(Ref&& other)
	{
		m_Instance = other.m_Instance;
		other.m_Instance = nullptr;
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	Ref<T>& Ref<T>::operator=(const Ref& other)
	{
		AssignSave(other);
		return *this;
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	Ref<T>& Ref<T>::operator=(Ref&& other)
	{
		AssignSave(std::move(other));
		return *this;
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	Ref<T>& Ref<T>::operator=(std::nullptr_t)
	{
		DecreaseRef();
		m_Instance = nullptr;
		return *this;
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	Ref<T>::~Ref()
	{
		DecreaseRef();
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	template<typename TOther>
		requires std::is_convertible_v<TOther*, T*>
	Ref<T>::Ref(const Ref<TOther>& other)
	{
		m_Instance = other.m_Instance;
		IncreaseRef();
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	template<typename TOther>
		requires std::is_convertible_v<TOther*, T*>
	Ref<T>::Ref(Ref<TOther>&& other)
	{
		m_Instance = other.m_Instance;
		other.m_Instance = nullptr;
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	template<typename TOther>
		requires std::is_convertible_v<TOther*, T*>
	Ref<T>& Ref<T>::operator=(const Ref<TOther>& other)
	{
		AssignSave(other);
		return *this;
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	template<typename TOther>
		requires std::is_convertible_v<TOther*, T*>
	Ref<T>& Ref<T>::operator=(Ref<TOther>&& other)
	{
		AssignSave(std::move(other));
		return *this;
	}


	template<typename T>
		requires Concepts::IsRefCounted<T>
	void Ref<T>::Release()
	{
		DecreaseRef();
		m_Instance = nullptr;
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	T* Ref<T>::operator->() const
	{
		return m_Instance;
	}
	
	template<typename T>
		requires Concepts::IsRefCounted<T>
	T& Ref<T>::operator*() const
	{
		return *m_Instance;
	}
	
	template<typename T>
		requires Concepts::IsRefCounted<T>
	Ref<T>::operator bool() const
	{
		return m_Instance != nullptr;
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	bool Ref<T>::operator==(std::nullptr_t) const
	{
		return m_Instance == nullptr;
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	bool Ref<T>::operator!=(std::nullptr_t) const
	{
		return m_Instance != nullptr;
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	bool Ref<T>::operator==(Ref other) const
	{
		return m_Instance == other.m_Instance;
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	bool Ref<T>::operator!=(Ref other) const
	{
		return m_Instance != other.m_Instance;
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	T* Ref<T>::Raw()
	{
		return m_Instance;
	}
	
	template<typename T>
		requires Concepts::IsRefCounted<T>
	const T* Ref<T>::Raw() const
	{
		return m_Instance;
	}
	
	template<typename T>
		requires Concepts::IsRefCounted<T>
	template<typename TTo>
		requires std::is_base_of_v<T, TTo> || std::is_base_of_v<TTo, T>
	Ref<TTo> Ref<T>::As() const
	{
		return static_cast<TTo*>(m_Instance);
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	template<typename TTo>
		requires std::is_base_of_v<T, TTo> || std::is_base_of_v<TTo, T>
	Ref<TTo> Ref<T>::AsSave() const
	{
		return dynamic_cast<TTo*>(m_Instance);
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	template<typename... TArgs>
	Ref<T> Ref<T>::Create(TArgs&&... args)
	{
		return new T(std::forward<TArgs>(args)...);
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	void Ref<T>::IncreaseRef()
	{
		if (m_Instance)
		{
			m_Instance->IncreaseRefCount();
			Internal::RefUtils::AddAliveInstance(m_Instance);
		}
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	void Ref<T>::DecreaseRef()
	{
		if (m_Instance)
		{
			m_Instance->DecreaseRefCount();

			if (m_Instance->GetRefCount() == 0)
			{
				delete m_Instance;
				Internal::RefUtils::RemoveAliveInstance(m_Instance);
				m_Instance = nullptr;
			}
		}
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	void Ref<T>::AssignSave(const Ref& other)
	{
		if (this == std::addressof(other))
			return;

		DecreaseRef();
		m_Instance = other.m_Instance;
		IncreaseRef();
	}

	template<typename T>
		requires Concepts::IsRefCounted<T>
	void Ref<T>::AssignSave(Ref&& other)
	{
		if (this == std::addressof(other))
			return;

		m_Instance = other.m_Instance;
		other.m_Instance = nullptr;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	//// Weak Implementation //////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename T>
	Weak<T>::Weak(Ref<T> ref)
	{
		m_Instance = ref.Raw();
	}

	template<typename T>
	Weak<T>::Weak(T* instance)
	{
		m_Instance = instance;
	}

	template<typename T>
	const Weak<T>& Weak<T>::operator=(Ref<T> ref)
	{
		m_Instance = ref.Raw();
		return *this;
	}

	template<typename T>
	const Weak<T>& Weak<T>::operator=(T* instance)
	{
		m_Instance = instance;
	}

	template<typename T>
	Ref<T> Weak<T>::GetRef() const
	{
		return m_Instance;
	}

	template<typename T>
	Ref<T> Weak<T>::TryGetRef() const
	{
		if (Expired())
			return nullptr;
		return m_Instance;
	}

	template<typename T>
	bool Weak<T>::Expired() const
	{
		return !Internal::RefUtils::IsAlive(m_Instance);
	}

	template<typename T>
	bool MElib::Weak<T>::Alive() const
	{
		return Internal::RefUtils::IsAlive(m_Instance);
	}

	template<typename T>
	Weak<T>::operator bool() const
	{
		return Alive();
	}

}
