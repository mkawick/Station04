// --------------------------------------------------------------------------------------------------------------------
#pragma once

#include <iosfwd>
#include <reflection.h>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Serialization
{
using namespace Reflection;

// --------------------------------------------------------------------------------------------------------------------
class Serializer
{
public:
	static bool Text(std::ostream& os, const Object& root);
	static bool Text(std::ostream& os, const Object& root, const Object& sub);
	//static bool Binary(std::ostream& os, const Object& root, const bool big_endian = PLATFORM_BIG_ENDIAN);
	//static bool Binary(std::ostream& os, const Object& root, const Object& sub, const bool big_endian = PLATFORM_BIG_ENDIAN);
	static bool From(std::istream& is, Object& root);

	template<typename T>				static bool Text(std::ostream& os, const T& root);
	template<typename T, typename S>	static bool Text(std::ostream& os, const T& root, const S& sub);
	//template<typename T>				static bool Binary(std::ostream& os, const T& root, const bool big_endian = PLATFORM_BIG_ENDIAN);
	//template<typename T, typename S>	static bool Binary(std::ostream& os, const T& root, const S& sub, const bool big_endian = PLATFORM_BIG_ENDIAN);
	template<typename T>				static bool From(std::istream& is, T& root);
};

// --------------------------------------------------------------------------------------------------------------------
inline bool Serializer::Text(std::ostream& os, const Object& root)
{
	return Text(os, root, root);
}

// --------------------------------------------------------------------------------------------------------------------
template<typename T> inline bool Serializer::Text(std::ostream& os, const T& root)
{
	Object rootobj(root);
	return Text(os, rootobj, rootobj);
}

// --------------------------------------------------------------------------------------------------------------------
template<typename T, typename S> inline bool Serializer::Text(std::ostream& os, const T& root, const S& sub)
{
	Object rootobj(root);
	Object subobj(sub);
	return Text(os, rootobj, subobj);
}

// --------------------------------------------------------------------------------------------------------------------
template<typename T> inline bool Serializer::From(std::istream& is, T& root)
{
	Object rootobj(root);
	return From(is, rootobj);
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Serialization
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
