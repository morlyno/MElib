#pragma once

#include "MElib/Core/Core.h"
#include "MElib/Memory/Buffer.h"

namespace MElib {

	namespace Internal {

		template<typename T>
		struct FunctionTraits
		{
			static constexpr bool IsMember = false;
			static constexpr bool IsConst = false;
		};

		template<typename TReturn, typename... TArgs>
		struct FunctionTraits<TReturn(*)(TArgs...)>
		{
			using Return = TReturn;

			static constexpr bool IsMember = false;
			static constexpr bool IsConst = false;
		};

		template<typename TReturn, typename TClass, typename... TArgs>
		struct FunctionTraits<TReturn(TClass::*)(TArgs...)>
		{
			using Return = TReturn;
			using Class = TClass;

			static constexpr bool IsMember = true;
			static constexpr bool IsConst = false;
		};

		template<typename TReturn, typename TClass, typename... TArgs>
		struct FunctionTraits<TReturn(TClass::*)(TArgs...) const>
		{
			using Return = TReturn;
			using Class = TClass;

			static constexpr bool IsMember = true;
			static constexpr bool IsConst = true;
		};

	}

	template<typename TReturn, typename... TArgs>
	class Delegate;

	class DelegateID
	{
	public:
		DelegateID(const void* stub, const void* object) : m_Stub(stub), m_Object(object) {}

		bool IsBound() const { return m_Stub != nullptr; }
		bool HasObject() const { return m_Object != nullptr; }

		bool operator==(const DelegateID& other) const { return m_Stub == other.m_Stub && m_Object == other.m_Object; }
		bool operator!=(const DelegateID& other) const { return !(*this == other); }

		template<typename TReturn, typename... TArgs>
		bool Equals(const Delegate<TReturn, TArgs...>& delegate) const
		{
			return *this == delegate.GetBoundID();
		}

	private:
		const void* m_Stub = nullptr;
		const void* m_Object = nullptr;
	};

	template<typename TReturn, typename... TArgs>
	class Delegate<TReturn(TArgs...)>
	{
	private:
		using TObjectPtr = void*;
		using TInvokationStub = TReturn(*)(TObjectPtr, TArgs&&...);
		using TDestructorStub = void(*)(TObjectPtr);

	public:
		Delegate() = default;

		template<auto TFunction>
			requires std::is_invocable_r_v<TReturn, decltype(TFunction), TArgs...>
		void Bind();

		template<auto TFunction, typename TClass>
			requires std::is_invocable_r_v<TReturn, decltype(TFunction), TClass*, TArgs...>
		void Bind(TClass* object);

		template<typename TCallable>
			requires std::invocable<TCallable, TArgs...>
		void Bind(TCallable&& callable);

		void Unbind();
		TReturn Invoke(TArgs&&... args) const;
		TReturn operator()(TArgs&&... args) const { return Invoke(std::forward<TArgs>(args)...); }

		bool IsBound() const { return m_Stub != nullptr; }
		operator bool() const { return IsBound; }

		DelegateID GetBoundID() const;

	private:
		template<auto TFunction>
		static TReturn FunctionStub(TObjectPtr object, TArgs&&... args)
		{
			using Traits = Internal::FunctionTraits<decltype(TFunction)>;
			if constexpr (Traits::IsMember && Traits::IsConst)
			{
				return (static_cast<const Traits::Class*>(object)->*TFunction)(std::forward<TArgs>(args)...);
			}
			else if constexpr (Traits::IsMember && !Traits::IsConst)
			{
				return (static_cast<Traits::Class*>(object)->*TFunction)(std::forward<TArgs>(args)...);
			}
			else if constexpr (!Traits::IsMember)
			{
				return TFunction(std::forward<TArgs>(args)...);
			}
			else
			{
				static_assert(false, "Unkown Function Type");
			}
		}

		template<typename TCallable>
		static TReturn CallableStub(TObjectPtr object, TArgs&&... args)
		{
			if constexpr (sizeof(TCallable) <= sizeof(TObjectPtr))
			{
				auto& callable = reinterpret_cast<TCallable&>(object);
				return callable(std::forward<TArgs>(args)...);
			}
			else
			{
				auto* callable = static_cast<TCallable*>(object);
				return (*callable)(std::forward<TArgs>(args)...);
			}
		}

		template<typename TCallable>
		static void DestructorStub(TObjectPtr object)
		{
			static constexpr bool allocated = sizeof(TCallable) > sizeof(TObjectPtr);

			if constexpr (!std::is_trivially_destructible_v<std::decay_t<TCallable>>)
			{
				if constexpr (allocated)
					static_cast<TCallable*>(object)->~TCallable();
				else
					reinterpret_cast<TCallable&>(object).~TCallable();
			}

			if constexpr (allocated)
			{
				operator delete(object);
			}
		}

	private:
		TObjectPtr m_Object = nullptr;
		TInvokationStub m_Stub = nullptr;

		TDestructorStub m_DestructorStub = nullptr;
	};

}

namespace MElib {

	template<typename TReturn, typename... TArgs>
	template<auto TFunction>
		requires std::is_invocable_r_v<TReturn, decltype(TFunction), TArgs...>
	void Delegate<TReturn(TArgs...)>::Bind()
	{
		Unbind();
		m_Object = nullptr;
		m_Stub = FunctionStub<TFunction>;
	}

	template<typename TReturn, typename... TArgs>
	template<auto TFunction, typename TClass>
		requires std::is_invocable_r_v<TReturn, decltype(TFunction), TClass*, TArgs...>
	void Delegate<TReturn(TArgs...)>::Bind(TClass* object)
	{
		Unbind();
		m_Object = object;
		m_Stub = FunctionStub<TFunction>;
	}

	template<typename TReturn, typename... TArgs>
	template<typename TCallable>
		requires std::invocable<TCallable, TArgs...>
	void Delegate<TReturn(TArgs...)>::Bind(TCallable&& callable)
	{
		Unbind();

		using CallableType = std::decay_t<TCallable>;
		static constexpr bool smallCallable = sizeof(CallableType) <= sizeof(TObjectPtr);

		if constexpr (!std::is_trivially_destructible_v<CallableType> || !smallCallable)
			m_DestructorStub = DestructorStub<CallableType>;

		if constexpr (smallCallable)
		{
			m_Stub = CallableStub<CallableType>;
			new (&m_Object) CallableType(std::forward<TCallable>(callable));

			return;
		}

		void* storage = operator new(sizeof(CallableType));
		new (storage) CallableType(std::forward<TCallable>(callable));

		m_Object = storage;
		m_Stub = CallableStub<CallableType>;
	}

	template<typename TReturn, typename... TArgs>
	void Delegate<TReturn(TArgs...)>::Unbind()
	{
		if (m_DestructorStub)
			std::invoke(m_DestructorStub, m_Object);

		m_DestructorStub = nullptr;
		m_Stub = nullptr;
		m_Object = nullptr;
	}

	template<typename TReturn, typename... TArgs>
	TReturn Delegate<TReturn(TArgs...)>::Invoke(TArgs&&... args) const
	{
		return std::invoke(m_Stub, m_Object, std::forward<TArgs>(args)...);
	}

	template<typename TReturn, typename... TArgs>
	DelegateID Delegate<TReturn(TArgs...)>::GetBoundID() const
	{
		return DelegateID(m_Stub, m_Object);
	}

}
