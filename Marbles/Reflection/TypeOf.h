// --------------------------------------------------------------------------------------------------------------------

#pragma once

#include <Reflection/Traits.h>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{

// --------------------------------------------------------------------------------------------------------------------
class Type;
typedef std::shared_ptr<const Type> shared_type;
typedef std::weak_ptr<const Type> weak_type;

// --------------------------------------------------------------------------------------------------------------------
template<typename T, bool hasMember = HasTypeInfo<T>::value> struct TypeOfT
{
	static const shared_type& DeclaredType();
	static const shared_type& DeclaredType(T*);
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T> const shared_type& TypeOf()
{
	typedef T Self;
	typedef std::remove_reference<typename Self>::type	NoRef;
	typedef remove_all_pointers<typename NoRef>::type	NoRefPointer;
	typedef std::remove_cv<typename NoRefPointer>::type	NoRefCVPointer;
	return TypeOfT<typename NoRefCVPointer>::DeclaredType();
}
template<typename T> const shared_type& TypeOf(T& obj)
{
	typedef T Self;
	typedef std::remove_reference<typename Self>::type	NoRef;
	typedef remove_all_pointers<typename NoRef>::type	NoRefPointer;
	typedef std::remove_cv<typename NoRefPointer>::type	NoRefCVPointer;
	return TypeOfT<typename NoRefCVPointer>::DeclaredType(&obj);
}
template<typename T> const shared_type& TypeOf(T** obj)
{
	return TypeOf(*obj);
}
template<typename T> const shared_type& TypeOf(T* obj)
{
	typedef T Self;
	typedef std::remove_reference<typename Self>::type	NoRef;
	typedef remove_all_pointers<typename NoRef>::type	NoRefPointer;
	typedef std::remove_cv<typename NoRefPointer>::type	NoRefCVPointer;
	return TypeOfT<typename NoRefCVPointer>::DeclaredType(obj);
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// --------------------------------------------------------------------------------------------------------------------
#define REFLECT_TYPE(T) \
	REFLECT_COMMON_TYPE(T)

#define REFLECT_EXTERNAL_TYPE(T) \
	REFLECT_COMMON_TYPE(T)

#define REFLECT_COMMON_TYPE(T) \
	template<> struct ::Marbles::Reflection::TypeOfT<T> \
	{ \
		static const shared_type& DeclaredType(); \
		static const shared_type& DeclaredType(T*); \

#define REFLECT_MEMBER(...) \

#define REFLECT_END() \
	};

// End of file --------------------------------------------------------------------------------------------------------
