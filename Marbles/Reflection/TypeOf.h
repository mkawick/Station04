// --------------------------------------------------------------------------------------------------------------------

#pragma once

#include <Common/Hash.h>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{
// --------------------------------------------------------------------------------------------------------------------
class Type;
typedef std::shared_ptr<const Type> shared_type;
typedef std::weak_ptr<const Type> weak_type;

// --------------------------------------------------------------------------------------------------------------------
class Member;
typedef std::shared_ptr<const Member> shared_member;
typedef std::weak_ptr<const Member> weak_member;

// --------------------------------------------------------------------------------------------------------------------
template<typename T> struct InstanceT 
{ 
	static T& get() { static T s_instance; return s_instance; } 
private:
	static const T& s_reference; // Forces a 'get' call during static initialization
};

template<typename T> const T& InstanceT<T>::s_reference = InstanceT<T>::get();

// --------------------------------------------------------------------------------------------------------------------
template<typename T> struct TypeOfT
{ 
	static shared_type TypeInfo(); 
};
template<typename T, bool hasTypeInfo = HasTypeInfo<T>::value> struct ResolveT
{ 
	inline static shared_type TypeInfo()
	{
		return TypeOfT<T>::TypeInfo();
	}
	inline static shared_type TypeInfo(T*) 
	{ 
		return TypeOfT<T>::TypeInfo(); 
	} 
};
template<typename T> struct ResolveT<T, true> 
{
	inline static shared_type TypeInfo()
	{
		return TypeOfT<T>::TypeInfo();
	}
	inline static shared_type TypeInfo(T* p)
	{
		return NULL == p ? TypeOfT<T>::TypeInfo() : p->_TypeInfo();
	}
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T> shared_type TypeOf()	
{ 
	return ResolveT<typename by_value<T>::type>::TypeInfo(); 
}
template<typename T> shared_type TypeOf(T& obj)
{
	return ResolveT<typename by_value<T>::type>::TypeInfo(&obj);
}
template<typename T> shared_type TypeOf(T* obj)
{
	return ResolveT<typename by_value<T>::type>::TypeInfo(obj);
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// --------------------------------------------------------------------------------------------------------------------
#define REFLECT_TYPE(T,REFLECT_DEFINITION) \
	REFLECT_COMMON_TYPE(template<>,T,REFLECT_DEFINITION)

#define RFFLECT_COMMA ,
#define REFLECT_TEMPLATE_TYPE(_template,T,REFLECT_DEFINITION) \
	REFLECT_COMMON_TYPE(_template,T,REFLECT_DEFINITION)

// Todo: Ensure that builders are the only object that internally references a shared_ptr to a type!
#define REFLECT_COMMON_TYPE(_template,T,REFLECT_DEFINITION) \
	_template struct ::Marbles::Reflection::TypeOfT<T> \
	: public ::Marbles::Reflection::InstanceT< ::Marbles::Reflection::TypeOfT<T> > \
	{	\
		typedef T type; \
		static ::Marbles::Reflection::shared_type TypeInfo() \
		{ \
			::Marbles::Reflection::shared_type typeInfo = get().reflect_type.lock(); \
			if (!typeInfo) \
				typeInfo = get().Create(); \
			return typeInfo; \
		} \
	private: \
		::Marbles::Reflection::weak_type reflect_type; \
	public: \
		::Marbles::Reflection::shared_type Create() \
		{ \
			typedef T self_type; \
			::Marbles::Reflection::Type::Builder builder; \
			reflect_type = builder.Create<T>(#T); \
			REFLECT_DEFINITION \
			return builder.TypeInfo(); \
		} \
	}; \

#define REFLECT_CREATOR() \
			builder.SetCreator(&Object::Create<self_type>); \

#define REFLECT_MEMBER(...) \
			builder.AddMember(__VA_ARGS__); \

// End of file --------------------------------------------------------------------------------------------------------
