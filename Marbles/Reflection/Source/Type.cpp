// --------------------------------------------------------------------------------------------------------------------

#include <Reflection.h>
#include <Common/Common.h>
#include <Common/Hash.h>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{
namespace 
{
// --------------------------------------------------------------------------------------------------------------------
} // namespace <>

// --------------------------------------------------------------------------------------------------------------------
std::map<hash_t, shared_type> Type::sRegistrar;

// --------------------------------------------------------------------------------------------------------------------
Type::Type()
: mByValue()
, mCreateFn(NULL)
{
}

// --------------------------------------------------------------------------------------------------------------------
Type::~Type() 
{
}

// --------------------------------------------------------------------------------------------------------------------
const std::string& Type::Name() const
{ 
	return mByValue.MemberInfo()->Name(); 
}

// --------------------------------------------------------------------------------------------------------------------
hash_t Type::HashName() const
{ 
	return mByValue.MemberInfo()->HashName(); 
}

// --------------------------------------------------------------------------------------------------------------------
const bool Type::Implements(const shared_type& type) const
{
	bool implements = type && *this == *type;
	for(TypeList::const_iterator base = mImplements.begin(); 
		!implements && base != mImplements.end(); 
		++base)
	{
		implements = (*base)->Implements(type);
	}
	return implements;
}

// --------------------------------------------------------------------------------------------------------------------
Type::MemberList::size_type Type::MemberIndex(hash_t hashName) const
{	// A binary search would be good!
	MemberList::size_type i = 0; 
	while (i < mMembers.size() && hashName != mMembers[i]->HashName())
	{
		++i;
	}
	return i;
}

// --------------------------------------------------------------------------------------------------------------------
Object Type::Create(const char* name) const
{
	ASSERT(NULL == name); // Usage of name not implemented
	Object obj;
	if (mCreateFn)
	{
		(*mCreateFn)(obj);
	}
	return obj;
}

// --------------------------------------------------------------------------------------------------------------------
const bool Type::operator==(const Type& type) const
{
	return HashName() == type.HashName();
}

// --------------------------------------------------------------------------------------------------------------------
hash_t Type::Hash(const char* str)
{
	return Hash::djb2(str);
}

// --------------------------------------------------------------------------------------------------------------------
hash_t Type::Hash(const void* obj, size_t size)
{
	return Hash::djb2(obj, size);
}

// --------------------------------------------------------------------------------------------------------------------
shared_type	Type::Find(const char* name)
{
	return Find(Hash(name));
}

// --------------------------------------------------------------------------------------------------------------------
shared_type	Type::Find(hash_t hashName)
{
	std::map<hash_t, shared_type>::iterator i = sRegistrar.find(hashName);
	return i != sRegistrar.end() ? i->second : shared_type();
}

// --------------------------------------------------------------------------------------------------------------------
void Type::Clear()
{
	for(std::map<hash_t, shared_type>::iterator i = sRegistrar.begin();
		i != sRegistrar.end();
		++i)
	{
		i->second.reset();
	}
	sRegistrar.clear();
}

// --------------------------------------------------------------------------------------------------------------------
bool Type::Register(shared_type type)
{
	const bool registered = sRegistrar.end() != sRegistrar.find(type->HashName());
	if (!registered)
	{
		sRegistrar[type->HashName()] = type;
	}
	return !registered;
}

// --------------------------------------------------------------------------------------------------------------------
Type::Builder::Builder()
{
}

// --------------------------------------------------------------------------------------------------------------------
shared_type Type::Builder::Create(const char* name)
{
	std::shared_ptr<Type> candidate = std::shared_ptr<Type>(new Type());
	shared_type type = std::const_pointer_cast<const Type>(candidate);

	// Creates a circular reference Type->Member->Type
	std::shared_ptr<Member> member = std::make_shared<Member>(name, type, "Default by value type member");
	candidate->mByValue = std::const_pointer_cast<const Member>(member); 

	if (Type::Register(type))
	{
		mBuild.swap(candidate);
	}
	else
	{
		type.reset();
	}

	return type;
}

// --------------------------------------------------------------------------------------------------------------------
void Type::Builder::SetCreator(Type::CreateFn fn)
{
	ASSERT(mBuild);
	mBuild->mCreateFn = fn;
}

// --------------------------------------------------------------------------------------------------------------------
void Type::Builder::SetSize(size_t size)
{
	ASSERT(mBuild);
	mBuild->mSize = size;
}

// --------------------------------------------------------------------------------------------------------------------
void Type::Builder::SetAlignment(size_t alignment)
{
	ASSERT(mBuild);
	mBuild->mAlignment = 0;
	// Their is some bit twiddling way to do this
	while (1 < alignment)
	{	
		alignment >>= 1;
		++mBuild->mAlignment;
	}
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
