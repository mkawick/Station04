#pragma once

#include <memory>
#include <type_traits>
#include <Common/Function_Traits.h>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{
class Type;
typedef std::shared_ptr<const Type> shared_type;
typedef std::weak_ptr<const Type> weak_type;
typedef uint32_t Hash;

// --------------------------------------------------------------------------------------------------------------------
// Review(danc): should we create a 'Common\type_traits' for these?
template<typename T> struct remove_all_ptrs						{ typedef T type; };
template<typename T> struct remove_all_ptrs<T*>					{ typedef typename remove_all_ptrs<T>::type type; };
template<typename T> struct remove_all_ptrs<T* const>			{ typedef typename remove_all_ptrs<T>::type type; };
template<typename T> struct remove_all_ptrs<T* volatile>		{ typedef typename remove_all_ptrs<T>::type type; };
template<typename T> struct remove_all_ptrs<T* const volatile>	{ typedef typename remove_all_ptrs<T>::type type; };

// --------------------------------------------------------------------------------------------------------------------
// Review(danc): should we create a 'Common\type_traits' for these?
template<typename T> struct remove_all_cv						{ typedef typename std::remove_cv<T>::type type; };
template<typename T> struct remove_all_cv<T*>					{ typedef typename remove_all_cv<T>::type* type; };
template<typename T> struct remove_all_cv<T* const>				{ typedef typename remove_all_cv<T>::type* type; };
template<typename T> struct remove_all_cv<T* volatile>			{ typedef typename remove_all_cv<T>::type* type; };
template<typename T> struct remove_all_cv<T* const volatile>	{ typedef typename remove_all_cv<T>::type* type; };

// --------------------------------------------------------------------------------------------------------------------
template<typename T>
struct by_value
{
private:
	typedef typename std::remove_reference<T>::type					ref_removed;
	typedef typename remove_all_ptrs<typename ref_removed>::type	ref_ptr_removed;
	typedef typename std::remove_cv<typename ref_ptr_removed>::type	ref_ptr_cv_removed;
public:
	typedef typename ref_ptr_cv_removed type;
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T>
struct HasTypeInfo
{
	template<typename U, shared_type (U::*)() const> struct signature {};
	template<typename U> static int HasMethod(signature<U, &U::_TypeInfo>*);
	template<typename U> static char HasMethod(...);
	static const bool value = sizeof(HasMethod<T>(0)) != sizeof(char);
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T>
struct HasIndexer
{
	template<typename U, typename V, typename K, V (U::*)(K)> struct signature
	{
		typedef function_traits<V (U::*)(K)> FnTraits;
		typedef typename FnTraits::member_type container_type;
		typedef typename FnTraits::return_type value_type;
		typedef typename FnTraits::arg0_type key_type;
	};
	struct void_signature
	{
		typedef void container_type;
		typedef void value_type;
		typedef void key_type;
	};
	template<typename U, typename V, typename K> static signature<U, V, K, &U::operator[]>* IndexMethod(signature<U, V, K, &U::operator[]>*);
	template<typename U> static char IndexMethod(...);
	static const bool value = sizeof(IndexMethod<T>(0)) != sizeof(char);

	// C++11 required
	//typedef typename std::result_of< IndexMethod<T>(0) >::type result_type
	//typedef typename std::conditional<value, void_signature, typename result_type>::type signature_traits;
	//typedef typename signature_traits::container_type container_type;
	//typedef typename signature_traits::value_type value_type;
	//typedef typename signature_traits::key_type key_type;
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T>
struct HasEnumerator
{
	template<typename U, typename U::iterator (U::*)(), typename U::iterator (U::*)()> struct signature
	{
		typedef U container_type;
		typedef typename U::iterator iterator;
	};
	template<typename U> static signature<U, &U::begin, &U::end>* BeginEnd(signature<U, &U::begin, &U::end>*);
	template<typename U> static char BeginEnd(...);
	static const bool value = sizeof(BeginEnd<T>(0)) != sizeof(char);

	// C++11 required
	//typedef decltype(HasMethod<T>(0)) signature_type;
	//typedef signature_type::container_type container_type;
	//typedef signature_type::value_type value_type;
	//typedef signature_type::key_type key_type;
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T>
struct SupportTraits
{
	static const bool isExternal	= HasTypeInfo<T>::value;
	static const bool canEnumerate	= HasEnumerator<T>::value;	// supports begin/end iteration
	static const bool canIndex		= HasIndexer<T>::value;		// can be indexed by a key operator[]
};

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
