#pragma once

#include <memory>
#include <functional>
#include <algorithm>
#include <vector>
#include <array>
#include <limits>
#include <cassert>

// We should not make these global includes if possible
#include <boost\cstdint.hpp>
#include <boost\interprocess\detail\atomic.hpp>
namespace boost
{
	using namespace interprocess::ipcdetail;
}

#define TO_STRING2(constant) #constant
#define TO_STRING(constant) TO_STRING2(constant)
#define Location(label) __FILE__ "(" TO_STRING(__LINE__) ") " label ": "
#define TODO(txt) Location("Todo") txt
#define Message(label, msg) message (Location(label) msg)

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{

// --------------------------------------------------------------------------------------------------------------------
#define STATIC_ASSERT BOOST_STATIC_ASSERT
#define ASSERT assert

// --------------------------------------------------------------------------------------------------------------------
template <typename T> 
inline size_t AlignOf() 
{ 
	return __alignof(T); // This is win32 specific
}

// --------------------------------------------------------------------------------------------------------------------
template <typename T> 
inline size_t AlignTo(size_t value) 
{ 
	return ((value + (AlignOf<T>() - 1)) & ~AlignOf<T>()); 
}

// --------------------------------------------------------------------------------------------------------------------
template <typename A>
inline const A& Max(const A& a, const A& b) { return a > b ? a : b; }
template <typename A>
inline const A& Min(const A& a, const A& b) { return a < b ? a : b; }

// --------------------------------------------------------------------------------------------------------------------
template<typename T> struct Expired 
{ inline bool operator()(const std::tr1::weak_ptr<T>& p) const { return p.expired(); } };

// --------------------------------------------------------------------------------------------------------------------
template<typename T> inline void Destruct(void* p) { p->~T(); }
template<typename T> inline void Delete(void* p) { delete reinterpret_cast<T*>(p); }
template<typename T> inline void Empty(T* ) { }

template<typename T>
inline T* Construct() { return new T(); }
template<typename T>
inline T* Construct(void* p) { return new (p) T(); }

template<typename T, typename A0>
inline T* Construct(A0& a0) { return new T(a0); }
template<typename T, typename A0>
inline T* Construct(void* p, A0& a0) { return new (p) T(a0); }

template<typename T, typename A0, typename A1>
inline T* Construct(A0& a0, A1& a1) { return new T(a0, a1); }
template<typename T, typename A0, typename A1>
inline T* Construct(void* p, A0& a0, A1& a1) { return new (p) T(a0, a1); }

template<typename T, typename A0, typename A1, typename A2>
inline T* Construct(A0& a0, A1& a1, A2& a2) { return new T(a0, a1, a2); }
template<typename T, typename A0, typename A1, typename A2>
inline T* Construct(void* p, A0& a0, A1& a1, A2& a2) { return new (p) T(a0, a1, a2); }

template<typename T, typename A0, typename A1, typename A2, typename A3>
inline T* Construct(A0& a0, A1& a1, A2& a2, A3& a3) { return new T(a0, a1, a2, a3); }
template<typename T, typename A0, typename A1, typename A2, typename A3>
inline T* Construct(void* p, A0& a0, A1& a1, A2& a2, A3& a3) { return new (p) T(a0, a1, a2, a3); }

template<typename T, typename A0, typename A1, typename A2, typename A3, typename A4>
inline T* Construct(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4) { return new T(a0, a1, a2, a3, a4); }
template<typename T, typename A0, typename A1, typename A2, typename A3, typename A4>
inline T* Construct(void* p, A0& a0, A1& a1, A2& a2, A3& a3, A4& a4) { return new (p) T(a0, a1, a2, a3, a4); }

template<typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
inline T* Construct(A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5) { return new T(a0, a1, a2, a3, a4, a5); }
template<typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
inline T* Construct(void* p, A0& a0, A1& a1, A2& a2, A3& a3, A4& a4, A5& a5) { return new (p) T(a0, a1, a2, a3, a4, a5); }

// --------------------------------------------------------------------------------------------------------------------
// to be replaced by std::unique_ptr<>
template<typename T> class unique_ptr : public std::auto_ptr<T>
{
public:
	explicit unique_ptr(T* p = 0) 
		:  std::auto_ptr<T>(p)
		{	// construct from object pointer
		}

	unique_ptr(const unique_ptr<T>& rhs) 
		:  std::auto_ptr<T>(const_cast<unique_ptr<T>&>(rhs))
		{	
		}

	unique_ptr(const std::auto_ptr_ref<T> rhs) 
		: std::auto_ptr<T>(rhs)
		{	
		}
};

// --------------------------------------------------------------------------------------------------------------------
// to be replaced by std::atomic<>
template<typename T>
class atomic
{
	typedef T					value_type;
	typedef atomic<value_type>	self_type;
	typedef value_type&			reference;
public:
	atomic() {}
	atomic(value_type value) : mValue(value) {}
	atomic(const self_type& rhs) : mValue(boost::atomic_read32(&rhs.mValue)) {}

	inline self_type& operator=(value_type value) 
	{ boost::atomic_write32(&mValue, static_cast<boost::uint32_t>(value)); return *this; }
	inline self_type& operator=(const self_type& rhs) 
	{ boost::atomic_write32(&mValue, boost::atomic_read32(&rhs.mValue)); return *this; }

	inline value_type get() 
	{ return static_cast<value_type>(boost::atomic_read32(&mValue)); }
	inline value_type get() const 
	{ return const_cast<self_type*>(this)->get(); }

	inline value_type compare_exchange(value_type value, value_type compare) 
	{ return static_cast<value_type>(boost::atomic_cas32(&mValue, static_cast<boost::uint32_t>(value), static_cast<boost::uint32_t>(compare))); }

	// value_type operator++() { return boost::atomic_increment(&mValue); }
private:
	mutable volatile boost::uint32_t mValue;
};

template<typename T>
class atomic<T*>
{
	typedef T*					value_type;
	typedef atomic<value_type>	self_type;
	typedef value_type&			reference;
public:
	atomic() {}
	atomic(value_type value) : mValue(reinterpret_cast<boost::uint32_t>(value)) {}
	atomic(const self_type& rhs) : mValue(boost::atomic_read32(&rhs.mValue)) {}
	
	inline self_type& operator=(value_type value) 
	{ Set(value); return *this; }
	inline self_type& operator=(const self_type& rhs) 
	{ Set(rhs); return *this; }

	// Set the value and return the previous value
	inline value_type& set(value_type value) 
	{ return reinterpret_cast<value_type>(boost::atomic_write32(&mValue, reinterpret_cast<boost::uint32_t>(value))); }
	inline value_type& set(const self_type& rhs) 
	{ return reinterpret_cast<value_type>(boost::atomic_write32(&mValue, boost::atomic_read32(&rhs.mValue))); }

	// Get the value 
	inline value_type get() 
	{ return reinterpret_cast<value_type>(boost::atomic_read32(&mValue)); }
	inline value_type get() const 
	{ return const_cast<self_type*>(this)->get(); }

	// if value equal to 'compare' then set to 'newValue', always returning the previous value
	inline value_type compare_exchange(value_type newValue, value_type compare) 
	{ 
		return reinterpret_cast<value_type>(boost::atomic_cas32(
			&mValue, 
			reinterpret_cast<boost::uint32_t>(newValue), 
			reinterpret_cast<boost::uint32_t>(compare))); 
	}

	// Increment/Decrement operators
	inline value_type operator++() 
	{ return boost::atomic_inc32(&mValue); }
	inline value_type operator--() 
	{ return boost::atomic_dec32(&mValue); }
	inline value_type operator++(int) 
	{ return boost::atomic_inc32(&mValue) + 1; }
	inline value_type operator--(int) 
	{ return boost::atomic_dec32(&mValue) - 1; }
private:
	volatile boost::uint32_t mValue;
};

// --------------------------------------------------------------------------------------------------------------------
} // namespace Marbles

// --------------------------------------------------------------------------------------------------------------------
namespace std { using namespace std::tr1; }

// End of file --------------------------------------------------------------------------------------------------------
