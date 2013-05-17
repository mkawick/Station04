#pragma once

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{
// --------------------------------------------------------------------------------------------------------------------
template<typename T, typename C> 
class MemberT<T C::*> : public MemberT<T>
{
public:
	typedef T return_type;
	typedef C member_type;
	typedef T C::*field_type;

	MemberT(const std::string& name, field_type field, const char* usage)
	: MemberT<T>(name, DeclarationT<T>(), usage)
	, mField(field)
	{
	}

	virtual Object Dereference(const Object& self) const
	{
		ASSERT(self.TypeInfo()->Implements(TypeOf<typename member_type>()));
		return Object(DeclareInfo(), &(self.As<typename member_type*>()->*mField));
	}

private:
	field_type mField;
};

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
