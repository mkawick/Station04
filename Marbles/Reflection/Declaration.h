#pragma once

#include <type_traits>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{

// --------------------------------------------------------------------------------------------------------------------
// review(danc): This description brakes down the equivalent of a typedef, should we rename it?
class Declaration
{
public:
	shared_type		TypeInfo() const;
	shared_member	MemberInfo() const	{ return member; }
	inline bool		IsValid() const		{ return member; }
	inline bool		IsConstant() const	{ return is_constant; }
	inline bool		IsValue() const		{ return Value == semantic; }
	inline bool		IsCallable() const	{ return Function == semantic; }
	inline bool		IsReference() const	{ return !IsValue() & !IsCallable(); }
	inline bool		IsShared() const	{ return Shared == semantic; }
	inline bool		IsWeak() const		{ return Weak == semantic; }

	Declaration()
	: semantic(Value)
	, is_constant(false)
	, deref_count(0)
	//, reserved2(0)
	{
	}

	Declaration(shared_member member, bool constant = false)
	: member(member)
	, semantic(Value)
	, is_constant(constant)
	, deref_count(0)
	//, reserved2(0)
	{
	}

	Declaration(const Declaration& declaration, bool constant)
	: member(declaration.member)
	, semantic(declaration.semantic)
	, is_constant(constant)
	, deref_count(0)
	//, reserved2(0)
	{
	}

	Declaration(shared_member member, const Declaration& declaration)
	: member(member)
	, semantic(declaration.semantic)
	, is_constant(declaration.is_constant)
	, deref_count(0)
	//, reserved2(0)
	{
	}
protected:
	enum reference_semantic
	{
		Value = 0,	
		Function,		// Callable function
		Reference,		// Reference to raw value
		Shared,			// Reference to shared value
		Weak,			// Reference to weak value
	};
	shared_member		member;
	reference_semantic	semantic	: 3;
	bool				is_constant	: 1;
	uint8_t				deref_count	: 4;	// max of 2^4 - 1 or 15
	//uint8_t				deref_count;		// How many times must we dereference before finding the actual value
	//uint32_t			reserved2	: 16;
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T>
class DeclarationT : public Declaration
{
	template<typename T> struct get_semantic
	{ 
		static const reference_semantic value = 
			std::is_function<T>::value || std::is_member_function_pointer<T>::value ? Function : Value; 
	};
	template<typename T> struct get_semantic< T* > { static const reference_semantic value = Reference; };
	template<typename T> struct get_semantic< std::shared_ptr<T> > { static const reference_semantic value = Shared; };
	template<typename T> struct get_semantic< std::weak_ptr<T> > { static const reference_semantic value = Weak; };

	template<typename T> struct get_deref_count			{ static const int value = 0; };
	template<typename T> struct get_deref_count< T* >	{ static const int value = get_deref_count<T>::value + 1; };

public:
	DeclarationT()
	: Declaration(TypeOf<T>()->ValueDeclaration())
	{
		is_constant	= std::is_const<std::remove_reference<T>::type>::value;
		semantic = get_semantic<std::remove_const<T>::type>::value;
		deref_count = get_deref_count<std::remove_reference<T>::type>::value;
	}

	inline static void* Store(T*& obj)
	{
		return &obj;
	}
	inline static void* Store(T& obj)
	{
		typedef typename std::remove_const<T>::type* StoreType;
		return const_cast<StoreType>(&obj);
	}
};

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
