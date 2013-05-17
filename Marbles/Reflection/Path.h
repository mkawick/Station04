#pragma once

#include <Reflection/Type.h>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{
// --------------------------------------------------------------------------------------------------------------------
template<typename _Hash, _Hash (*_HashFn)(const char*) = &Type::Hash, typename _Ax = std::allocator<_Hash> >
class PathT : public std::vector<_Hash, _Ax>
{
public:
	PathT(const char* path = "");
	PathT(const std::string& path);
	PathT(const PathT& path);

	_Hash HashName() const;

	void push_back(const _Hash& value);
	void push_back(const char* value);
	void push_back(const std::string& value);
	void push_back(const PathT& path);

	PathT& operator=(const _Hash& value);
	PathT& operator=(const char* path);
	PathT& operator=(const std::string& path);
	PathT& operator=(const PathT& path);

	static const char Seperator = '.';
private:
	typedef std::vector<_Hash, _Ax> base_type;
	static base_type Parse(const char* path);
};

// --------------------------------------------------------------------------------------------------------------------
template<typename _Hash, _Hash (*_HashFn)(const char*), typename _Ax>
const char PathT<_Hash, _HashFn, _Ax>::Seperator = '.';

// --------------------------------------------------------------------------------------------------------------------
template<typename _Hash, _Hash (*_HashFn)(const char*), typename _Ax>
PathT<_Hash, _HashFn, _Ax>::PathT(const char* path)
: base_type(Parse(path))
{
}

// --------------------------------------------------------------------------------------------------------------------
template<typename _Hash, _Hash (*_HashFn)(const char*), typename _Ax>
PathT<_Hash, _HashFn, _Ax>::PathT(const std::string& path)
: base_type(Parse(path.c_str()))
{
}

// --------------------------------------------------------------------------------------------------------------------
template<typename _Hash, _Hash (*_HashFn)(const char*), typename _Ax>
PathT<_Hash, _HashFn, _Ax>::PathT(const PathT& path)
: base_type(path)
{
}

template<typename _Hash, _Hash (*_HashFn)(const char*), typename _Ax>
_Hash PathT<_Hash, _HashFn, _Ax>::HashName() const
{
	_Hash hashName = 0;
	for (iterator i = begin(); i != end(); ++i)
	{
		hashName += *i;
	}
	return hashName;
}

// --------------------------------------------------------------------------------------------------------------------
template<typename _Hash, _Hash (*_HashFn)(const char*), typename _Ax>
void PathT<_Hash, _HashFn, _Ax>::push_back(const _Hash& value)
{
	base_type::push_back(value);
}

// --------------------------------------------------------------------------------------------------------------------
template<typename _Hash, _Hash (*_HashFn)(const char*), typename _Ax>
void PathT<_Hash, _HashFn, _Ax>::push_back(const char* value)
{
	base_type::push_back((*_HashFn)(value));
}

// --------------------------------------------------------------------------------------------------------------------
template<typename _Hash, _Hash (*_HashFn)(const char*), typename _Ax>
void PathT<_Hash, _HashFn, _Ax>::push_back(const std::string& value)
{
	base_type::push_back((*_HashFn)(value.c_str()));
}

// --------------------------------------------------------------------------------------------------------------------
template<typename _Hash, _Hash (*_HashFn)(const char*), typename _Ax>
void PathT<_Hash, _HashFn, _Ax>::push_back(const PathT<_Hash, _HashFn, _Ax>& path)
{
	reserve(size() + path.size());
	for (base_type::iterator i = path.begin(); i != path.end(); ++i)
	{
		base_type::push_back(*i);
	}
}

// --------------------------------------------------------------------------------------------------------------------
template<typename _Hash, _Hash (*_HashFn)(const char*), typename _Ax>
PathT<_Hash, _HashFn, _Ax>& PathT<_Hash, _HashFn, _Ax>::operator=(const _Hash& value)
{
	clear();
	base_type::push_back(value);
	return *this;
}

// --------------------------------------------------------------------------------------------------------------------
template<typename _Hash, _Hash (*_HashFn)(const char*), typename _Ax>
PathT<_Hash, _HashFn, _Ax>& PathT<_Hash, _HashFn, _Ax>::operator=(const char* path)
{
	base_type::operator=(Parse(path));
	return *this;
}

// --------------------------------------------------------------------------------------------------------------------
template<typename _Hash, _Hash (*_HashFn)(const char*), typename _Ax>
PathT<_Hash, _HashFn, _Ax>& PathT<_Hash, _HashFn, _Ax>::operator=(const std::string& path)
{
	base_type::operator=(Parse(path.c_str()));
	return *this;
}

// --------------------------------------------------------------------------------------------------------------------
template<typename _Hash, _Hash (*_HashFn)(const char*), typename _Ax>
PathT<_Hash, _HashFn, _Ax>& PathT<_Hash, _HashFn, _Ax>::operator=(const PathT<_Hash, _HashFn, _Ax>& path)
{
	base_type::operator=(path);
	return *this;
}

// --------------------------------------------------------------------------------------------------------------------
template<typename _Hash, _Hash (*_HashFn)(const char*), typename _Ax>
typename PathT<_Hash, _HashFn, _Ax>::base_type PathT<_Hash, _HashFn, _Ax>::Parse(const char* path)
{
	base_type out;
	char* end;
	char* begin = const_cast<char*>(path);

	do 
	{
		for(end = begin; '\0' != *end && Seperator != *end; ++end);
		if (Seperator == *end)
		{
			*end = '\0';						// modify end to hash the value
			out.push_back((*_HashFn)(begin));	// hash and store the value
			*end = Seperator;					// reset end back to original value
			begin = end + 1;					// A new beginning
			continue;
		}
		else
		{	// leaf name
			out.push_back(Type::Hash(begin));
			break;
		}
	} while ('\0' != *end);
	return out;
}

// --------------------------------------------------------------------------------------------------------------------
//inline Path HashPath(const Object& root, const Object& child)
//{
//	typedef Type::MemberList::const_iterator iterator;
//	typedef std::vector<std::pair<iterator, iterator> > Stack;
//	
//	Stack stack;
//	stack.push_back(std::make_pair(root.Members().begin(), root.Members().end()));
//	while (!stack.empty() && stack.back().first->Identical(child))
//	{
//		Type::MemberList& backMembers = stack.back().first->Members();
//		if (backMembers.empty())
//		{	// What about the indexable elements?
//			stack.pop_back();
//		}
//		else
//		{
//			stack.push_back(std::make_pair(backMembers.begin(), backMembers.end()));
//		}
//		++stack.back().first;
//		if (stack.back().first == stack.back().second)
//		{	// What about the indexable elements?
//			stack.pop_back();
//		}
//	}
//
//	Path path;
//	path.reserve(stack.size());
//	for (Stack::iterator i = stack.begin(); i != stack.end(); ++i)
//	{
//		path.push_back(i->first->HashName());
//	}
//	return path;
//
//	// From SerializationWrite.inl(142) -- ExtendPathTo
//	//Serializer::PathT::size_type depth = path.size();
//	//for (	Type::MemberList::const_iterator i = obj.Members().begin();
//	//		i != members.end() && !path.back().Identical(obj);
//	//		++i)
//	//{
//	//	path.push_back(path.back().At(*i));
//	//	if (!ExtendPathTo(path, obj))
//	//	{
//	//		path.pop_back();
//	//	}
//	//}
//	//return depth != path.size();
//}

typedef PathT<hash_t> Path;

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles
// End of file --------------------------------------------------------------------------------------------------------
