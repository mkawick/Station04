#pragma once

#include <type_traits>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{
class Object;
template<typename T> class MemberT;

// --------------------------------------------------------------------------------------------------------------------
class Member : public std::enable_shared_from_this<const Member>
{
public:
	Member(const std::string& name, const Declaration& declaration, const char* usage);
	Member(const std::string& name, const shared_type& type, const char* usage);

	const std::string&	Name() const		{ return mName; }
	hash_t				HashName() const	{ return mHashName; }
	Declaration			DeclareInfo() const	{ return Declaration(shared_from_this(), mDeclaration); }
	const char*			Usage() const		{ return mUsage; }

	virtual shared_type TypeInfo() const	{ return mType.lock(); }
	virtual Object		Assign(Object self, const Object& rhs) const;
	virtual Object		Dereference(const Object& self) const;
	virtual Object		Append(Object& self) const;

private:
	std::string			mName;
	hash_t				mHashName;
	Declaration			mDeclaration;
	weak_type			mType;
	const char*			mUsage;
};

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
