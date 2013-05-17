#pragma once

#include <Common/Function_traits.h> 
#include <Common/Hash.h> 

#include <sstream>
#include <map>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{
class Object;

// --------------------------------------------------------------------------------------------------------------------
class Type
{
public:
	typedef std::vector<shared_type>	TypeList;
	typedef std::vector<shared_member>	MemberList;
	typedef std::vector<Declaration>	DeclarationList;
	class Builder;

	Type();
	~Type();

	const std::string&		Name() const;
	hash_t					HashName() const;
	size_t					Size() const						{ return mSize; }
	size_t					Alignment() const					{ return static_cast<size_t>(1) << mAlignment; }
	const Declaration&		ValueDeclaration() const			{ return mByValue; }
	const DeclarationList&	Parameters() const					{ return mParameters; }
	const MemberList&		Members() const						{ return mMembers; }
	MemberList::size_type	MemberIndex(const char* name) const	{ return MemberIndex(Hash(name)); }
	MemberList::size_type	MemberIndex(hash_t hashName) const;

	const bool				Implements(const shared_type& type) const;
	Object					Create(const char* name = NULL) const;

	const bool				operator==(const Type& type) const;

	static hash_t			Hash(const char* str);
	static hash_t			Hash(const void* obj, size_t size);
	static shared_type		Find(const char* name);
	static shared_type		Find(hash_t hashName);
	static void				Clear();

private:
	static bool				Register(shared_type type);

	Declaration				mByValue;
	MemberList				mMembers;
	DeclarationList			mParameters;
	TypeList				mImplements;
	size_t					mSize;
	unsigned char			mAlignment; // Stored as an exponent to a power of two

	typedef void (*CreateFn)(Object& );

	// AssignFn
	// DestoryFn
	CreateFn mCreateFn;

	// ConvertFn
	// IndexFn
	// EnumeratorFn
	// AppendFn

	static std::map<hash_t, shared_type> sRegistrar;
};

// --------------------------------------------------------------------------------------------------------------------
class Type::Builder
{
public:
	Builder();
	shared_type TypeInfo() const { return mBuild; }
	shared_type Create(const char* name);
	
	void SetCreator(Type::CreateFn fn);
	// void SetAppend(Type::AppendFn fn);
	// void SetEnumerator();

	template<typename T> shared_type Create(const char* name);
	template<typename T> void AddMember(const char* name, const char* description = NULL);
	template<typename T> void AddMember(const char* name, T member, const char* description = NULL);
	void AddMember(const char* name, shared_type type, const char* description = NULL);

private:
	void SetAlignment(size_t alignment);
	void SetSize(size_t size);

	template<typename T> struct template_traits;
	template<typename T> friend struct TypeOfT;

	std::shared_ptr<Type> mBuild;
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T> struct Type::Builder::template_traits
{
	static unsigned NumberOfParameters()				{ return 0; }
	static Declaration ParameterAt(unsigned /*index*/)	{ return Declaration(); }
	static std::string TypeName(const char* name)		{ return std::string(name); }
};

// Review(danc): This only works for type parameters not numeric template parameters
#define BUILD_TEMPLATE_TRAITS_PREFIX ::Marbles::Reflection::TypeOf<A
#define BUILD_TEMPLATE_TRAITS_POSTFIX >()
#define BUILD_TEMPLATE_TRAITS_TYPENAME ->Name()
#define BUILD_TEMPLATE_TRAITS_SEP << ',' <<
#define BUILD_TEMPLATE_TRAITS_LIST(N) FN_LIST(N,BUILD_TEMPLATE_TRAITS_PREFIX,BUILD_TEMPLATE_TRAITS_POSTFIX BUILD_TEMPLATE_TRAITS_TYPENAME,BUILD_TEMPLATE_TRAITS_SEP)
#define BUILD_TEMPLATE_PARAMETERS_PREFIX ::Marbles::Reflection::DeclarationT<A
#define BUILD_TEMPLATE_PARAMETERS_POSTFIX >()
#define BUILD_PARAMETER_TYPE_LIST(N) FN_LIST(N,BUILD_TEMPLATE_PARAMETERS_PREFIX,BUILD_TEMPLATE_PARAMETERS_POSTFIX,FN_COMMA)
#define BUILD_TEMPLATE_TRAITS(N) \
	template<template<FN_LIST(N,typename B,,FN_COMMA)> class T, FN_TYPENAME(N)> \
	struct Type::Builder::template_traits< T<FN_TYPES(N)> > \
	{ \
		static unsigned NumberOfParameters() { return N; } \
		static Declaration ParameterAt(unsigned index) \
		{ \
			Declaration parameters[] = { BUILD_PARAMETER_TYPE_LIST(N) }; \
			return (0 <= index && index < N) ? parameters[index] : Declaration(); \
		} \
		static std::string TypeName(const char* name) \
		{ \
			std::stringstream ss; \
			int max_chars = 256; \
			while ('\0' != *name && '<' != *name && max_chars--) \
			{ \
				ss << *(name++); \
			} \
			ss << '<' << BUILD_TEMPLATE_TRAITS_LIST(N) << '>'; \
			return ss.str(); \
		} \
	}

BUILD_TEMPLATE_TRAITS(1);
BUILD_TEMPLATE_TRAITS(2);
BUILD_TEMPLATE_TRAITS(3);
BUILD_TEMPLATE_TRAITS(4);
BUILD_TEMPLATE_TRAITS(5);
BUILD_TEMPLATE_TRAITS(6);
BUILD_TEMPLATE_TRAITS(7);
BUILD_TEMPLATE_TRAITS(8);
BUILD_TEMPLATE_TRAITS(9);
BUILD_TEMPLATE_TRAITS(10);
BUILD_TEMPLATE_TRAITS(11);
BUILD_TEMPLATE_TRAITS(12);

#undef BUILD_TEMPLATE_TRAITS_PREFIX 
#undef BUILD_TEMPLATE_TRAITS_POSTFIX
#undef BUILD_TEMPLATE_TRAITS_TYPENAME
#undef BUILD_TEMPLATE_TRAITS_SEP
#undef BUILD_TEMPLATE_TRAITS_LIST
#undef BUILD_PARAMETER_TYPE_LIST
#undef BUILD_TEMPLATE_TRAITS

// --------------------------------------------------------------------------------------------------------------------
template<typename T> 
shared_type Type::Builder::Create(const char* name)
{	
	std::shared_ptr<Type> candidate = std::shared_ptr<Type>(new Type());
	shared_type type = std::const_pointer_cast<const Type>(candidate);

	unsigned numberOfParameters = template_traits<typename by_value<T>::type>::NumberOfParameters(); 
	candidate->mParameters.reserve(numberOfParameters);
	for(unsigned i = 0; i < numberOfParameters; ++i)
	{
		candidate->mParameters.push_back(template_traits<typename by_value<T>::type>::ParameterAt(i)); 
	}
	std::string fullName = template_traits<typename by_value<T>::type>::TypeName(name);
	std::shared_ptr< MemberT<T> > member = std::make_shared< MemberT<T> >(fullName, type, "Default value type member.");
	candidate->mByValue = Declaration(std::static_pointer_cast<Member>(member)); 

	if (Type::Register(type))
	{
		mBuild.swap(candidate);
	}
	else
	{
		type.reset();
	}

	if (mBuild)
	{
		SetCreator(&Object::Create<void>);
		SetAlignment(std::alignment_of<T>::value);
		SetSize(sizeof(T));
	}

	return type;
}

// --------------------------------------------------------------------------------------------------------------------
template<typename T> void Type::Builder::AddMember(const char* name, const char* description)
{
	AddMember(name, TypeOf<T>(), description);
}

// --------------------------------------------------------------------------------------------------------------------
template<typename T> void Type::Builder::AddMember(const char* name, T member, const char* description)
{
	shared_member memberInfo = std::make_shared< MemberT<T> >(name, member, description);
	if (memberInfo)
	{
		mBuild->mMembers.push_back(memberInfo);
	}
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
