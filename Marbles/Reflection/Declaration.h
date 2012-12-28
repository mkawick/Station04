#pragma once

// --------------------------------------------------------------------------------------------------------------------
namespace Marbles
{
namespace Reflection
{
class Member;

// --------------------------------------------------------------------------------------------------------------------
class Declaration
{
public:
	bool is_constant	: 1;
	bool is_reference	: 1;
	bool is_function	: 1;
};

template<typename T>
class DeclarationT
{
public:
	DeclarationT()
	{
		is_constant	= std::is_const<std::remove_reference<T>::type>::value;
		is_reference = std::is_pointer<T>::value;
		is_function	= std::is_function<T>::value;
	}
};

// --------------------------------------------------------------------------------------------------------------------
} // namespace Reflection
} // namespace Marbles

// End of file --------------------------------------------------------------------------------------------------------
