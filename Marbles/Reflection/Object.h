#pragma once

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{
#pragma warning(push)
#pragma warning(disable: 4521) // C4521 : multiple copy constructors specified
// --------------------------------------------------------------------------------------------------------------------
class Object
{
public:
	Object(const Object& obj);
	Object(Object& obj);
	template<typename T> Object(T& obj);
private:
};
#pragma warning(pop)

// --------------------------------------------------------------------------------------------------------------------
template<typename T> Object::Object(T& obj)
{
}

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
