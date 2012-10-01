// --------------------------------------------------------------------------------------------------------------------

#pragma once

class Type;
typedef shared_ptr<Type> shared_type;
typedef weak_ptr<Type> weak_type;

// --------------------------------------------------------------------------------------------------------------------
template<typename T> struct TypeOfT
{
	static const shared_type& DeclaredType();
	static const shared_type& DeclaredType(T*);
};

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
#define REFLECT_EXTERNAL_START(T) \
	REFLECT_COMMON_START(T)

#define REFLECT_START(T) \
	REFLECT_COMMON_START(T)

#define REFLECT_COMMON_START(T) \
	template<> const ::Marbles::Reflection::Type& ::Marbles::Reflection::TypeOfT<T>::DeclareType() \
	{ \
		typedef T Self; \

#define REFLECT_END() \
	};
	
// End of file --------------------------------------------------------------------------------------------------------
