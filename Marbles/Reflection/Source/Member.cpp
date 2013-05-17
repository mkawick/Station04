// --------------------------------------------------------------------------------------------------------------------

#include <Reflection.h>
#include <Common/Common.h>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{
// --------------------------------------------------------------------------------------------------------------------
shared_type	Declaration::TypeInfo() const 
{ 
	return member->TypeInfo(); 
}

// --------------------------------------------------------------------------------------------------------------------
Member::Member(const std::string& name, const Declaration& declaration, const char* usage)
: mName(name)
, mType(declaration.TypeInfo())
, mUsage(usage)
, mDeclaration(declaration)
{
	mHashName = Type::Hash(mName.c_str());
}

// --------------------------------------------------------------------------------------------------------------------
Member::Member(const std::string& name, const shared_type& type, const char* usage)
: mName(name)
, mType(type)
, mUsage(usage)
{
	mHashName = Type::Hash(mName.c_str());
}

// --------------------------------------------------------------------------------------------------------------------
Object Member::Assign(Object self, const Object& rhs) const
{
	ASSERT(self.IsValid());
	ASSERT(rhs.TypeInfo()->Implements(self.TypeInfo()));
	const Type::MemberList& members = self.TypeInfo()->Members();
	for(Type::MemberList::const_iterator i = members.begin(); i < members.end(); ++i)
	{
		self.At(*i) = rhs.At(*i);
	}
	return self;
}

// --------------------------------------------------------------------------------------------------------------------
Object Member::Dereference(const Object& /*self*/) const
{
	return Object();
}

// --------------------------------------------------------------------------------------------------------------------
Object Member::Append(Object& /*self*/) const
{
	return Object();
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
