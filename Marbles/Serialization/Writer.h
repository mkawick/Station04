// Writer.h
// --------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Serialization
{

// The writer should collect and then write.  for example
// Writer<TextFormat> writer ( root ); // if no sub call is made all will be serialized
// writer.Include( root.a ); // only sub elements serialized
// writer.Exclude( root.b.c.g );
// std::ostringstream oss;
// writer.write( oss );
// --------------------------------------------------------------------------------------------------------------------
template<typename F> 
class Writer
{
public:
					Writer(const Object& obj, bool endianSwap = false);

	void			Include(const Object& obj);
	void			Exclude(const Object& obj);
	bool			Write(std::ostream& os);

private:
	const ObjectList& Context() const;

	bool			Write(std::ostream& os, const Object& obj);
	template <typename T> bool Translate(std::ostream& os, const Object& obj);
	bool			WriteReference(std::ostream& os, const Object& obj);
	bool			WriteEnumerable(std::ostream& os, const Object& obj);
	bool			WriteMembers(std::ostream& os, const Object& obj);
	void			WriteNewLine(std::ostream& os) const;
	bool			ExtendPathTo(ObjectList& route, const Object& obj);

	typedef std::map<void*, std::string> PathMap;
	F				mFormat;
	ObjectList		mPath;
	PathMap			mWritten;
	ObjectList		mIncludes;
	ObjectMap		mExcludes; // Can this be done with mWritten?
};

// --------------------------------------------------------------------------------------------------------------------
template<typename F> 
Writer<F>::Writer(const Object& obj, bool endianSwap = false)
: mFormat(obj, endianSwap)
{
	mPath.push_back(obj);
	mWritten[NULL] = "0";
}

// --------------------------------------------------------------------------------------------------------------------
template<typename F> 
void Writer<F>::Include(const Object& obj)
{
	mIncludes.push_back(obj);
}

// --------------------------------------------------------------------------------------------------------------------
template<typename F> 
void Writer<F>::Exclude(const Object& obj)
{	// Review(danc): Can this be implemented using mWritten? ie do we need the mExcludes variable?
	mExcludes.push_back(obj); 
}

// --------------------------------------------------------------------------------------------------------------------
template<typename F> 
const ObjectList& Writer<F>::Context() const
{
	return mPath;
}

// --------------------------------------------------------------------------------------------------------------------
template<typename F> 
bool Writer<F>::WriteReference(std::ostream& os, const Object& obj)
{
	std::ios::pos_type pos = os.tellp();
	bool write = obj.IsValid() && obj.IsReference();
	if (write)
	{
		Object value = *obj;
		PathMap::const_iterator i = mWritten.find(value.Address());
		bool hasPath = i != mWritten.end();
		if (hasPath)
		{
			os << i->second;
		}
		else if (value.IsValid())
		{
			Write(os, value);
		}
	}
	return pos != os.tellp();
}

// --------------------------------------------------------------------------------------------------------------------
template<typename F> 
bool Writer<F>::WriteEnumerable(std::ostream& os, const Object& obj)
{
	bool write = obj.IsEnumerable();
	if (write)
	{
		mFormat.OpenEnumeration(os);
		std::ios::pos_type pos = os.tellp();
		mPath.push_back(obj);

		mPath.pop_back();
		if (pos != os.tellp())
		{
			WriteNewLine(os);
		}
		mFormat.CloseEnumeration(os);
	}
	return write;
}

// --------------------------------------------------------------------------------------------------------------------
template<typename F> 
bool Writer<F>::WriteMembers(std::ostream& os, const Object& obj)
{
	bool write = obj.IsValid() && 0 != obj.Members().size();
	if (write)
	{
		mFormat.OpenMap(os);
		std::ios::pos_type pos = os.tellp();
		
		bool first = true;
		Type::MemberList::const_iterator end = obj.Members().end();
		Type::MemberList::const_iterator begin = obj.Members().begin();
		for (Type::MemberList::const_iterator i = begin; i != end; ++i)
		{
			Object member(obj.At(*i));
			mPath.push_back(member);
			if (!first)
			{
				mFormat.Seperator(os);
			}
			first = false;
			WriteNewLine(os);
			mFormat.Label(os, member);
			Write(os, member);
			mPath.pop_back();
		}

		if (pos != os.tellp())
		{
			WriteNewLine(os);
		}
		mFormat.CloseMap(os);
	}
	return write;
}

// --------------------------------------------------------------------------------------------------------------------
template<typename F> 
void Writer<F>::WriteNewLine(std::ostream& os) const
{
	mFormat.NewLine(os);
	for (Type::MemberList::size_type i = 1; i < mPath.size(); ++i)
	{
		mFormat.Indent(os);
	}
}

// --------------------------------------------------------------------------------------------------------------------
template<typename F> 
bool Writer<F>::ExtendPathTo(ObjectList& path, const Object& obj)
{
	const Type::MemberList& members = obj.Members();
	ObjectList::size_type depth = path.size();
	for (	Type::MemberList::const_iterator i = members.begin();
			i != members.end() && !path.back().Identical(obj);
			++i)
	{
		path.push_back(path.back().At(*i));
		if (!ExtendPathTo(path, obj))
		{
			path.pop_back();
		}
	}
	return depth != path.size();
}

// --------------------------------------------------------------------------------------------------------------------
template<typename F> template <typename T> 
bool Writer<F>::Translate(std::ostream& os, const Object& obj)
{
	bool canWrite = TypeOf<T>() == obj.TypeInfo();
	if (canWrite)
	{
		mFormat.Write(os, obj.As<T>());
	}
	return canWrite;
}

// --------------------------------------------------------------------------------------------------------------------
template<typename F>
bool Writer<F>::Write(std::ostream& os)
{
	std::ios::pos_type pos = os.tellp();
	for(ObjectList::iterator i = mIncludes.begin(); i != mIncludes.end(); ++i)
	{
		PathMap::iterator written = mWritten.find(i->Address());
		if (mWritten.end() == written)
		{
			Write(os, *i);
		}
	}
	return pos != os.tellp();
}

// --------------------------------------------------------------------------------------------------------------------
template<typename F>
bool Writer<F>::Write(std::ostream& os, const Object& value)
{
	ASSERT(0 < mPath.size() && mPath.back().IsValid());
	std::ios::pos_type pos = os.tellp();
	if (1 == mPath.size() && !mPath.back().IsReference())
	{	// The first element should always output type info.
		mFormat.TypeInfo(os, mPath.back());
	}

	ObjectList::const_iterator start = mPath.begin();
	if (ExtendPathTo(mPath, value))
	{	// Write out path navigation to value
		for (ObjectList::const_iterator i = start; i != mPath.end(); ++i)
		{
			if (i->IsEnumerable())
			{
				mFormat.OpenEnumeration(os);
			}
			else if (0 != i->TypeInfo()->Members().size())
			{
				mFormat.OpenMap(os);
			}
		}
		Write(os, value);
		for (ObjectList::const_reverse_iterator i = mPath.rbegin() + 1; i != mPath.rend(); ++i)
		{
			if (i->IsEnumerable())
			{
				mFormat.CloseEnumeration(os);
			}
			else if (0 != i->TypeInfo()->Members().size())
			{
				mFormat.CloseMap(os);
			}
		}
	}
	else if (Translate<bool>(os, value))				{}
	else if (Translate<Marbles::uint8_t>(os, value))	{}
	else if (Translate<Marbles::uint16_t>(os, value))	{}
	else if (Translate<Marbles::uint32_t>(os, value))	{}
	else if (Translate<Marbles::uint64_t>(os, value))	{}
	else if (Translate<Marbles::int8_t>(os, value))		{}
	else if (Translate<Marbles::int16_t>(os, value))	{}
	else if (Translate<Marbles::int32_t>(os, value))	{}
	else if (Translate<Marbles::int64_t>(os, value))	{}
	else if (Translate<float>(os, value))				{}
	else if (Translate<double>(os, value))				{}
	else if (Translate<std::string>(os, value))			{}
	else if (WriteReference(os, value))					{}
	else if (WriteEnumerable(os, value))				{}
	else if (WriteMembers(os, value))					{}

	return pos != os.tellp();
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Serialization
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
