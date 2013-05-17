// --------------------------------------------------------------------------------------------------------------------

#pragma once

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{

// --------------------------------------------------------------------------------------------------------------------
template<typename T> 
class MemberT : public Member
{
public:
	MemberT(const std::string& name, const shared_type& type, const char* usage)
	: Member(name, type, usage) {}
	MemberT(const std::string& name, const Declaration& declaration, const char* usage)
	: Member(name, declaration, usage) {}

	virtual shared_type	DeclaredType() const { return TypeOf<T>(); }
	virtual Object		Assign(Object self, const Object& rhs) const;
	virtual Object		Dereference(const Object& /*self*/) const;
	virtual Object		Append(Object& self) const;
private:
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T> 
Object MemberT<T>::Assign(Object self, const Object& rhs) const
{
	ASSERT(self.IsValid());
	ASSERT(self.TypeInfo()->Implements(rhs.TypeInfo()));
	if (self.IsValue())
	{
		self.As<T>() = rhs.As<T>();
	}
	//else if (self.IsShared())
	//{
	//	self.As< std::shared_ptr<T> >() = rhs.As<T>();
	//}
	//else if (self.IsWeak())
	//{
	//	self.As< std::weak_ptr<T> >() = rhs.As<T>();
	//}
	else if (self.IsReference())
	{
		self.As<T*>() = rhs.As<T*>();
	}
	return self;
}

// --------------------------------------------------------------------------------------------------------------------
template<typename T> 
Object MemberT<T>::Dereference(const Object& /*self*/) const
{ 
	ASSERT(!"Not implemented");
	return Object(); 
}

// --------------------------------------------------------------------------------------------------------------------
template<typename T> 
Object MemberT<T>::Append(Object& /*self*/) const
{
	ASSERT(!"Not implemented");
	return Object();
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
