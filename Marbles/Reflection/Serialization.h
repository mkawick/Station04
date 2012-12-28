// --------------------------------------------------------------------------------------------------------------------
#pragma once

#include <iosfwd>

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{
class Object;

// --------------------------------------------------------------------------------------------------------------------
class Serialize
{
public:
	static bool Text(std::ostream& os, const Object& in);
	static bool Text(std::ostream& os, const Object& in, const Object& root);
	//static bool Binary(std::ostream& os, const Object& in, const bool big_endian = PLATFORM_BIG_ENDIAN);
	//static bool Binary(std::ostream& os, const Object& in, const Object& root, const bool big_endian = PLATFORM_BIG_ENDIAN);
	static bool From(std::istream& is, Object& out);

	template<typename T>				static bool Text(std::ostream& os, const T& in);
	template<typename T, typename S>	static bool Text(std::ostream& os, const T& in, const S& root);
	//template<typename T>				static bool Binary(std::ostream& os, const T& in, const bool big_endian = PLATFORM_BIG_ENDIAN);
	//template<typename T, typename S>	static bool Binary(std::ostream& os, const T& in, const S& root, const bool big_endian = PLATFORM_BIG_ENDIAN);
	template<typename T>				static bool From(std::istream& is, T& out);
private:
	class Format;
	template<typename T> static bool	Write(std::ostream& os, const Format& format, const Object& obj);
	template<typename T> static bool	Read(std::istream& is, const Format& format, Object& obj);
};

// --------------------------------------------------------------------------------------------------------------------
inline bool Serialize::Text(std::ostream& os, const Object& in)
{
	return Text(os, in, in);
}

// --------------------------------------------------------------------------------------------------------------------
template<typename T> inline bool Serialize::Text(std::ostream& os, const T& in)
{
	Object inobj(in);
	return Text(os, inobj, inobj);
}

// --------------------------------------------------------------------------------------------------------------------
template<typename T, typename S> inline bool Serialize::Text(std::ostream& os, const T& in, const S& root)
{
	Object inobj(in);
	Object rootobj(in);
	return Text(os, inobj, rootobj);
}

// --------------------------------------------------------------------------------------------------------------------
template<typename T> inline bool Serialize::From(std::istream& is, T& out)
{
	Object outobj(out);
	return From(is, outobj);
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
