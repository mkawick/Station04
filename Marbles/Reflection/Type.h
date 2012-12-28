#pragma once

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{
class Object;
class Declaration;
class Member;

// --------------------------------------------------------------------------------------------------------------------
class Type
{
public:
	typedef std::vector<Member> MemberList;

	const MemberList&	Members() const;
	unsigned int		MemberIndex(const Hash& hashName) const;
	unsigned int		MemberIndex(const std::string& name) const;

private:
	// AssignFn
	// DestoryFn

	// CreateFn
	// ConvertFn
	// IndexFn
	// EnumeratorFn
	// AppendFn
};

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
