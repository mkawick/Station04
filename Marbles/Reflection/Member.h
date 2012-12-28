#pragma once

#include <type_traits>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{

// --------------------------------------------------------------------------------------------------------------------
class Member
{
public:
	const std::string&	Name() const;
	Hash				HashName() const;

	virtual shared_type	Type() const;
	virtual bool		Constant() const;
	virtual bool		Reference() const;
	virtual bool		Function() const;

	virtual Object		Dereference(Object ref) = 0;
private:
	std::string			mName;
	Hash				mHashName;

	virtual bool		Constant() const		{ return std::is_const<T>::value; }
	virtual bool		Reference() const		{ return std::is_pointer<T>::value; }
	virtual bool		Function() const		{ return false; }
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T> 
class MemberT : public Member
{
public:
	shared_type			Type() const			{ return TypeOf<T>(); }
	virtual bool		Constant() const		{ return std::is_const<T>::value; }
	virtual bool		Reference() const		{ return std::is_pointer<T>::value; }
	virtual bool		Function() const		{ return false; }

	virtual Object		Dereference(Object ref)	{ return Object(); }
private:
};

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
