#pragma once

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{
class Object;
typedef std::vector<Object> ObjectList;
typedef std::map<hash_t, Object> ObjectMap;

#pragma warning(push)
#pragma warning(disable: 4521) // C4521 : multiple copy constructors specified

// --------------------------------------------------------------------------------------------------------------------
class Object
{
public:
	Object();
	~Object();
	Object(const Object& obj);
	Object(Object& obj); // Second copy constructor such that the template<> constructor does not apply
	Object(const Declaration& declaration, void* pointee);
	template<typename T>	explicit Object(T& obj);

	Object&					operator=(const Object& obj);
	//Object&					operator=(const Object& obj);
	template<typename T>	Object&	operator=(const T& obj);

	bool					IsValid() const;
	bool					IsEnumerable() const{ return false; }
	bool					IsCallable() const	{ return mInfo.IsCallable(); }
	bool					IsConstant() const	{ return mInfo.IsConstant(); }
	bool					IsValue() const		{ return mInfo.IsValue(); }
	bool					IsReference() const	{ return mInfo.IsReference(); }
	bool					IsShared() const	{ return mInfo.IsShared(); }
	bool					IsWeak() const		{ return mInfo.IsWeak(); }

	Object&					Swap(Object& obj);
	void*					Address() const		{ return mPointee.get(); }
	hash_t					HashName() const	{ return Type::Hash(this, sizeof(Object)); }

	shared_type				_TypeInfo() const	{ return mInfo.TypeInfo(); }
	shared_type				TypeInfo() const	{ return _TypeInfo(); }
	shared_member			MemberInfo() const	{ return mInfo.MemberInfo(); }
	shared_member			MemberInfo(const char* name) const;
	shared_member			MemberInfo(const std::string& name) const;
	shared_member			MemberInfo(const Path& route) const;
	shared_member			MemberInfo(hash_t hashName) const;
	const Type::MemberList& Members() const		{ return TypeInfo()->Members(); }

	template<typename T>	T& As()				{ return To<T>::From(*this); }
	template<typename T>	T& As() const		{ return To<T>::From(*const_cast<Object*>(this)); }

	Object					At(const char* name) const;
	Object					At(const std::string& name) const;
	Object					At(const Path& route) const;
	Object					At(const hash_t hashName) const;
	Object					At(const shared_member& member) const;
	Object					Append();
	Object					Append(const Object& obj);

	bool					Equal(const Object& obj) const;
	bool					Identical(const Object& obj) const;

	bool					operator==(const Object& obj) const { return Equal(obj); }
	bool					operator!=(const Object& obj) const { return !operator==(obj); }

	Object					operator*() const;
	//Object operator()() const;
	//Object operator()(Object );

	template<typename T>	static void Create(Object& obj);
private:
	template<typename T>	struct To;
	template<typename T>	struct Put;

	bool	_IsZero() const;
	template<typename T>
	Object& _AssignReference(const Object& rhs);
	Object& _AssignZero(const Object& zero);
	void	_SetAddress(void* p);

	Declaration				mInfo;		// Description of how to interpret mPointee
	std::shared_ptr<void>	mPointee;	// Generic reference to the object
};

#pragma warning(pop)

// --------------------------------------------------------------------------------------------------------------------
template<typename T> Object::Object(T& obj)
: mInfo(DeclarationT<T>())
{
	_SetAddress(DeclarationT<T>::Store(obj)); 
}

// --------------------------------------------------------------------------------------------------------------------
template<typename T> Object& Object::operator=(const T& value)
{
	Object rhs(value);
	return operator=(rhs);
}

// --------------------------------------------------------------------------------------------------------------------
template<typename T> struct Put
{
	static Object& Value(Object& obj, const T& value) 
	{ 
		return obj;
	}
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T> struct Object::To
{
	static T& From(const Object& obj) { return *To<T*>::From(obj); }
};

// --------------------------------------------------------------------------------------------------------------------
template<typename T> struct Object::To<T*>
{
	static T*& From(const Object& obj)
	{	// TODO: check readonly flag here
		typedef std::remove_cv<T>::type NoConstT;
		const void* address = &obj.mPointee;
		return const_cast<T*>(*reinterpret_cast<NoConstT* const *>(address));
	}
};

// --------------------------------------------------------------------------------------------------------------------
inline bool Object::IsValid() const
{
	return mPointee && mInfo.IsValid();
}

// --------------------------------------------------------------------------------------------------------------------
inline bool Object::Identical(const Object& obj) const
{
	return	obj.TypeInfo() == TypeInfo() &&
			obj.Address() == Address();
}

// --------------------------------------------------------------------------------------------------------------------
inline Object& Object::Swap(Object& obj)
{
	std::swap(obj.mInfo, mInfo);
	std::swap(obj.mPointee, mPointee);
	return *this;
}

// --------------------------------------------------------------------------------------------------------------------
inline shared_member Object::MemberInfo(const std::string& name) const
{
	return MemberInfo(name.c_str());
}

// --------------------------------------------------------------------------------------------------------------------
inline shared_member Object::MemberInfo(const char* name) const
{
	Path path(name);
	return MemberInfo(path);
}

// --------------------------------------------------------------------------------------------------------------------
inline shared_member Object::MemberInfo(hash_t hashName) const 
{
	const Type::MemberList& members = TypeInfo()->Members();
	const Type::MemberList::size_type index = TypeInfo()->MemberIndex(hashName); 
	return members[index];
}

// --------------------------------------------------------------------------------------------------------------------
inline Object Object::operator*() const 
{
	Object result;
	if (IsReference())
	{
		Declaration info(TypeInfo()->ValueDeclaration(), IsConstant());
		Object value(info, *reinterpret_cast<void**>(mPointee.get()));
		result.Swap(value);
	}
	return result; 
}

// --------------------------------------------------------------------------------------------------------------------
template<> inline void Object::Create<void>(Object& obj) { Object invalid; obj.Swap(invalid); }
template<typename T> inline void Object::Create(Object& obj)
{
	obj.mInfo = DeclarationT<T>();
	obj._SetAddress(new T());
}

// --------------------------------------------------------------------------------------------------------------------
inline bool Object::_IsZero() const
{
	return	IsReference() 
		? 0 == To<unsigned*>::From(*this)
		: 0 == To<unsigned>::From(*this);
}

// --------------------------------------------------------------------------------------------------------------------
inline void Object::_SetAddress(void* p) 
{	// std::shared_ptr<> does not allow us to set a raw pointer without allocating managed data.
	// Since std::shared_ptr<> uses only the allocated managed data to perform deletion operations 
	// setting the raw pointer does not put the std::shared_ptr<> into an invalid state.
	mPointee.reset();
	*reinterpret_cast<void**>(&mPointee) = p;
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

template<typename T> inline bool operator==(const Marbles::Reflection::Object& obj, const T& var)
{ return obj.TypeInfo()->Implements(Marbles::Reflection::TypeOf<T>()) && obj.As<T>() == var; }
template<typename T> inline bool operator==(const T& var, const Marbles::Reflection::Object& obj)
{ return obj.TypeInfo()->Implements(Marbles::Reflection::TypeOf<T>()) && var == obj.As<T>(); }
template<typename T> inline bool operator!=(const Marbles::Reflection::Object& obj, const T& var)
{ return !(obj == var); }
template<typename T> inline bool operator!=(const T& var, const Marbles::Reflection::Object& obj)
{ return !(var == obj); }

// End of file --------------------------------------------------------------------------------------------------------
