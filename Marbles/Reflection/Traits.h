#pragma once

#include <memory>
#include <type_traits>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{

class Type;
typedef std::shared_ptr<const Type> shared_type;
typedef std::weak_ptr<const Type> weak_type;
typedef unsigned int Hash;

// --------------------------------------------------------------------------------------------------------------------
// Review(danc): should we create a 'Common\type_traits' for this?
template<typename T> struct remove_all_pointers
{
	typedef T type;
};
template<typename T> struct remove_all_pointers<T*>
{
	typedef typename remove_all_pointers<T>::type type;
};
template<typename T> struct remove_all_pointers<T* const>
{
	typedef typename remove_all_pointers<T>::type type;
};
template<typename T> struct remove_all_pointers<T* volatile>
{
	typedef typename remove_all_pointers<T>::type type;
};
template<typename T> struct remove_all_pointers<T* const volatile>
{
	typedef typename remove_all_pointers<T>::type type;
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T> struct remove_all_cv
{
	typedef typename std::remove_cv<T>::type type;
};
template<typename T> struct remove_all_cv<T*>
{
	typedef typename remove_all_cv<T>::type* type;
};
template<typename T> struct remove_all_cv<T* const>
{
	typedef typename remove_all_cv<T>::type* type;
};
template<typename T> struct remove_all_cv<T* volatile>
{
	typedef typename remove_all_cv<T>::type* type;
};
template<typename T> struct remove_all_cv<T* const volatile>
{
	typedef typename remove_all_cv<T>::type* type;
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T>
struct HasTypeInfo
{
	template<typename U, shared_type (U::*)() const> struct signature {};
	template<typename U> static int HasMethod(signature<U, &U::TypeInfo>*);
	template<typename U> static char HasMethod(...);
	static const bool value = sizeof(HasMethod<T>(0)) != sizeof(char);
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T>
struct HasConverter
{
	template<typename U, typename V, V (U::*)()> struct signature
	{
		typedef U container_type;
		typedef V convert_type;
	};
	template<typename U, typename V> signature<U, V, &U::operator*>* HasMethod(signature<U, V, &U::operator*>*);
	template<typename U> char HasMethod(...);
	static const bool value = sizeof(HasMethod<T>(0)) != sizeof(char);

	// C++11 required
	//typedef decltype(HasMethod<T>(0)) signature_type;
	//typedef signature_type::container_type container_type;
	//typedef signature_type::convert_type convert_type;
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T>
struct HasIndexer
{
	template<typename U, typename V, typename K, V (U::*)(K)> struct signature 
	{
		typedef U container_type;
		typedef V value_type;
		typedef K key_type;
	};
	template<typename U, typename V, typename K> signature<U, V, K, &U::operator[]>* IndexMethod(signature<U, V, K, &U::operator[]>*);
	template<typename U> char IndexMethod(...);
	static const bool value = sizeof(IndexMethod<T>(0)) != sizeof(char);

	// C++11 required
	//typedef decltype(HasMethod<T>(0)) signature_type;
	//typedef signature_type::container_type container_type;
	//typedef signature_type::value_type value_type;
	//typedef signature_type::key_type key_type;
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T>
struct HasEnumerator
{
	template<typename U, typename I, I (U::*)(), I (U::*)()> struct fncheck
	{
		typedef U value_type;
		typedef I iterator;
	};
	template<typename U, typename I> fncheck<U, I, &U::begin, &U::end>* BeginEnd(fncheck<U, I, &U::begin, &U::end>*);
	template<typename U> char BeginEnd(...);
	static const bool value = sizeof(BeginEnd<T>(0)) == sizeof(int);
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T>
struct SupportTraits
{
	static const bool isExternal = HasReflectType<T>::value;
	static const bool canConvert = HasConverter<T>::value;		// supports dereferencing operation
	static const bool canEnumerate = CanEnumerate<T>::value;	// supports begin/end iteration
	static const bool canIndex = HasIndexer<T>::value;			// can be indexed by a key operator[]
	//static const bool canCreate;	// has a default public constructor
};

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
