#pragma once

#include <Common\Platform.h>
#include <memory>

template <typename T> 
inline size_t AlignTo(size_t value) 
{ 
	return ((value + (__alignof(T) - 1)) & ~__alignof(T)); 
}

// --------------------------------------------------------------------------------------------------------------------
template<typename T> class shared_ptr;
template<typename T> class weak_ptr;
 
// --------------------------------------------------------------------------------------------------------------------
template<typename T> class shared_ptr : public std::tr1::shared_ptr<T>
{
	shared_ptr() {}
	template<class _Ux>
	explicit shared_ptr(_Ux *_Px) : std::tr1::shared_ptr(_Px) {}

	template<class _Ux, class _Dx>
	shared_ptr(_Ux *_Px, _Dx _Dt) : std::tr1::shared_ptr(_Px, _Dt) {}

	template<class _Ux, class _Dx, class _Alloc>
	shared_ptr(_Ux *_Px, _Dx _Dt, _Alloc _Ax) : std::tr1::shared_ptr(_Px, _Dt, _Ax) {}

	template<class _Ty2>
	shared_ptr(const shared_ptr<_Ty2>& _Other) : std::tr1::shared_ptr(_Other) {}

	template<class _Ty2>
	explicit shared_ptr(const std::tr1::weak_ptr<_Ty2>& _Other, bool _Throw = true) : std::tr1::shared_ptr(_Other, _Throw) {}
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T> class weak_ptr : public std::tr1::weak_ptr<T>
{
	weak_ptr() {}
	template<class _Ux>
	explicit weak_ptr(_Ux *_Px) : std::tr1::weak_ptr(_Px) {}

	template<class _Ux, class _Dx>
	weak_ptr(_Ux *_Px, _Dx _Dt) : std::tr1::weak_ptr(_Px, _Dt) {}

	template<class _Ux, class _Dx, class _Alloc>
	weak_ptr(_Ux *_Px, _Dx _Dt, _Alloc _Ax) : std::tr1::weak_ptr(_Px, _Dt, _Ax) {}

	template<class _Ty2>
	weak_ptr(const weak_ptr<_Ty2>& _Other) : std::tr1::weak_ptr(_Other) {}

	template<class _Ty2>
	explicit weak_ptr(const std::tr1::shared_ptr<_Ty2>& _Other, bool _Throw = true) : std::tr1::weak_ptr(_Other, _Throw) {}
};

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
