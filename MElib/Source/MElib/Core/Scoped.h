#pragma once

#include "MElib/std/Include.h"

namespace MElib {

	template<typename T>
	class Scope
	{
	public:
		Scope();
		Scope(std::nullptr_t);
		explicit Scope(T* instance);

		Scope(Scope&& other);
		Scope& operator=(Scope&& other);
		Scope(const Scope&) = delete;
		Scope& operator=(Scope& other) = delete;

		~Scope();

		template<typename TOther> requires std::is_convertible_v<TOther*, T*> Scope(Scope<TOther>&& other);
		template<typename TOther> requires std::is_convertible_v<TOther*, T*> Scope& operator=(Scope<TOther>&& other);

		void Release();
		T& operator*() const;
		T* operator->() const;
		operator bool() const;
		bool operator==(const Scope& other);
		bool operator!=(const Scope& other);

		T* Raw();
		const T* Raw() const;

		template<typename TAs> TAs* ViewAs() const;
		template<typename TAs> TAs* ViewAsSafe() const;
		template<typename TAs> Scope<TAs> As() const;
		template<typename TAs> Scope<TAs> AsSafe() const;

		template<typename... TArgs>
		static Scope Create(TArgs&&... args);

	private:
		T* m_Instance;

		template<typename TOther>
		friend class Scope;
	};

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//// Template Implementation //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

namespace MElib {

	template<typename T>
	Scope<T>::Scope()
		: m_Instance(nullptr)
	{
	}

	template<typename T>
	Scope<T>::Scope(std::nullptr_t)
		: m_Instance(nullptr)
	{
	}

	template<typename T>
	Scope<T>::Scope(T* instance)
		: m_Instance(instance)
	{
	}

	template<typename T>
	Scope<T>::Scope(Scope&& other)
	{
		m_Instance = other.m_Instance;
		other.m_Instance = nullptr;
	}

	template<typename T>
	Scope<T>& Scope<T>::operator=(Scope&& other)
	{
		if (this == std::addressof(other))
			return *this;

		m_Instance = other.m_Instance;
		other.m_Instance = nullptr;
		return *this;
	}

	template<typename T>
	Scope<T>::~Scope()
	{
		Release();
	}

	template<typename T>
	template<typename TOther>
		requires std::is_convertible_v<TOther*, T*>
	Scope<T>::Scope(Scope<TOther>&& other)
	{
		m_Instance = other.m_Instance;
		other.m_Instance = nullptr;
	}

	template<typename T>
	template<typename TOther> requires std::is_convertible_v<TOther*, T*>
	Scope<T>& Scope<T>::operator=(Scope<TOther>&& other)
	{
		if (this == std::addressof(other))
			return *this;

		m_Instance = other.m_Instance;
		other.m_Instance = nullptr;
		return *this;
	}

	template<typename T>
	void Scope<T>::Release()
	{
		delete m_Instance;
		m_Instance = nullptr;
	}

	template<typename T>
	T& Scope<T>::operator*() const
	{
		return *m_Instance;
	}

	template<typename T>
	T* Scope<T>::operator->() const
	{
		return m_Instance;
	}

	template<typename T>
	Scope<T>::operator bool() const
	{
		return m_Instance != nullptr;
	}

	template<typename T>
	bool Scope<T>::operator==(const Scope& other)
	{
		return m_Instance == other.m_Instance;
	}

	template<typename T>
	bool Scope<T>::operator!=(const Scope& other)
	{
		return m_Instance != other.m_Instance;
	}

	template<typename T>
	T* Scope<T>::Raw()
	{
		return m_Instance;
	}

	template<typename T>
	const T* Scope<T>::Raw() const
	{
		return m_Instance;
	}

	template<typename T>
	template<typename TAs>
	TAs* Scope<T>::ViewAs() const
	{
		return static_cast<TAs*>(m_Instance);
	}

	template<typename T>
	template<typename TAs>
	TAs* Scope<T>::ViewAsSafe() const
	{
		return dynamic_cast<TAs*>(m_Instance);
	}

	template<typename T>
	template<typename TAs>
	Scope<TAs> Scope<T>::As() const
	{
		return Scope<TAs>(std::move(*this));
	}

	template<typename T>
	template<typename TAs>
	Scope<TAs> Scope<T>::AsSafe() const
	{
		TAs* instance = dynamic_cast<TAs>(m_Instance);
		m_Instance = nullptr;
		return Scope(instance);
	}

	template<typename T>
	template<typename... TArgs>
	Scope<T> MElib::Scope<T>::Create(TArgs&&... args)
	{
		return Scope(new T(std::forward<TArgs>(args)...));
	}

}
