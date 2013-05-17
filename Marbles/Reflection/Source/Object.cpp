// --------------------------------------------------------------------------------------------------------------------

#include <Reflection.h>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{

// --------------------------------------------------------------------------------------------------------------------
Object::~Object()
{
}

// --------------------------------------------------------------------------------------------------------------------
Object::Object() 
{}

// --------------------------------------------------------------------------------------------------------------------
Object::Object(const Object& obj)
: mInfo(obj.mInfo)
, mPointee(obj.mPointee)
{}

// --------------------------------------------------------------------------------------------------------------------
Object::Object(Object& obj)
: mInfo(obj.mInfo)
, mPointee(obj.mPointee) 
{}

// --------------------------------------------------------------------------------------------------------------------
Object::Object(const Declaration& declaration, void* pointee)
: mInfo(declaration)
{
	_SetAddress(pointee);
}

// --------------------------------------------------------------------------------------------------------------------
Object&	Object::operator=(const Object& rhs)
{
	// if rhs is not valid then lhs becomes not valid
	// if lhs is not valid then rhs is cloned
	// if rhs implements lhs it is valid to do a formal assignment
	// if rhs is zero then reset lhs approprately
	shared_type typeInfo = TypeInfo();
	if (!IsValid() || !rhs.IsValid())
	{	// Clone the object
		Object clone(rhs);
		Swap(clone);
	}
	else if (mInfo.IsShared() || mInfo.IsWeak())
	{
		if (rhs.TypeInfo()->Implements(typeInfo))
		{
			MemberInfo()->Assign(*this, rhs);
		}
		else if (rhs.TypeInfo()->Implements(typeInfo->Parameters()[0]))
		{

		}
		_AssignReference< std::shared_ptr<void> >(rhs);
	}
	else if (rhs.TypeInfo()->Implements(typeInfo))
	{
		if (mInfo.IsShared() || mInfo.IsWeak())
		{
			MemberInfo()->Assign(*this, rhs);
		}
		else if (mInfo.IsReference())
		{	// Assign the values reference address
			void* address = rhs.Address();
			if (rhs.IsReference())
			{	
				address = (*rhs).Address();
			}
			*To<void**>::From(*this) = address;
		}
		else
		{
			MemberInfo()->Assign(*this, rhs);
		}
	}
	else if (rhs._IsZero())
	{
		_AssignZero(rhs);
	}
	else
	{
		ASSERT(!"Unable to do assignment.");
	}
	return *this;
}

// --------------------------------------------------------------------------------------------------------------------
shared_member Object::MemberInfo(const Path& route) const
{
	shared_member memberInfo;
	shared_type typeInfo = TypeInfo();
	for (Path::const_iterator i = route.begin(); typeInfo && i != route.end(); ++i)
	{
		Type::MemberList::size_type location = typeInfo->MemberIndex(*i);
		if ((0 <= location) & (location < typeInfo->Members().size()))
		{
			memberInfo = typeInfo->Members()[location];
			typeInfo = memberInfo->TypeInfo();
		}
		else
		{
			memberInfo.reset();
			typeInfo.reset();
		}
	}
	return memberInfo;
}

// --------------------------------------------------------------------------------------------------------------------
Object Object::At(const char* name) const
{
	return At(Type::Hash(name));
}

// --------------------------------------------------------------------------------------------------------------------
Object Object::At(const hash_t hashName) const
{
	Object obj;
	if (IsValid())
	{
		const Type::MemberList& members = Members();
		const Type::MemberList::size_type index = TypeInfo()->MemberIndex(hashName);
		if (index < members.size())
		{
			Object member(At(members[index]));
			obj.Swap(member);
		}
	}
	return obj;
}

// --------------------------------------------------------------------------------------------------------------------
Object Object::At(const shared_member& member) const
{
	ASSERT(member);
	return member->Dereference(*this);
}

// --------------------------------------------------------------------------------------------------------------------
Object Object::Append()
{
	return MemberInfo()->Append(*this);
}

// --------------------------------------------------------------------------------------------------------------------
Object Object::Append(const Object& obj)
{
	Object appended = Append();
	appended = obj;
	return appended;
}

// --------------------------------------------------------------------------------------------------------------------
bool Object::Equal(const Object& obj) const
{
	// TODO: Member by member comparison is required to determine if two objects are equivalant
	return Identical(obj);
}

// --------------------------------------------------------------------------------------------------------------------
template<typename T>
Object& Object::_AssignReference(const Object& rhs)
{
	T& self_ptr = *To<T*>::From(*this);
	if (rhs.mInfo.IsShared())
	{
		self_ptr = *To<std::shared_ptr<void>*>::From(rhs);
	}
	else if (rhs.mInfo.IsWeak())
	{
		self_ptr = To<std::weak_ptr<void>*>::From(rhs)->lock();
	}
	else if (rhs.mInfo.IsReference())
	{   // not good wrong deleter!
		self_ptr = std::shared_ptr<void>(*reinterpret_cast<void**>(rhs.Address()));
	}
	return *this;
}

// --------------------------------------------------------------------------------------------------------------------
inline Object& Object::_AssignZero(const Object& zero)
{
	if (mInfo.IsShared())
	{
		To<std::shared_ptr<void>*>::From(*this)->reset();
	}
	else if (mInfo.IsWeak())
	{
		To<std::weak_ptr<void>*>::From(*this)->reset();
	}
	else if (mInfo.IsReference())
	{
		*To<void**>::From(*this) = 0;
	}
	else
	{	// This case should never be executed, it will be treated as a normal assignment by operator=()
		ASSERT(zero.TypeInfo()->Implements(TypeInfo()));
		MemberInfo()->Assign(*this, zero);
	}
	return *this;
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
