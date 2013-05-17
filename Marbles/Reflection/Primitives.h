#pragma once

// --------------------------------------------------------------------------------------------------------------------
REFLECT_TYPE(Marbles::Reflection::Type, REFLECT_CREATOR())

// --------------------------------------------------------------------------------------------------------------------
REFLECT_TYPE(bool,				REFLECT_CREATOR())
REFLECT_TYPE(Marbles::uint8_t,	REFLECT_CREATOR())
REFLECT_TYPE(Marbles::uint16_t,	REFLECT_CREATOR())
REFLECT_TYPE(Marbles::uint32_t,	REFLECT_CREATOR())
REFLECT_TYPE(Marbles::uint64_t,	REFLECT_CREATOR())
REFLECT_TYPE(Marbles::int8_t,	REFLECT_CREATOR())
REFLECT_TYPE(Marbles::int16_t,	REFLECT_CREATOR())
REFLECT_TYPE(Marbles::int32_t,	REFLECT_CREATOR())
REFLECT_TYPE(Marbles::int64_t,	REFLECT_CREATOR())
REFLECT_TYPE(float,				REFLECT_CREATOR())
REFLECT_TYPE(double,			REFLECT_CREATOR())
REFLECT_TYPE(std::string,		REFLECT_CREATOR())

// --------------------------------------------------------------------------------------------------------------------
REFLECT_TEMPLATE_TYPE(template<typename T>, std::char_traits<T>,)
REFLECT_TEMPLATE_TYPE(template<typename T>, std::allocator<T>,	REFLECT_CREATOR())
REFLECT_TEMPLATE_TYPE(template<typename T>, std::shared_ptr<T>,	REFLECT_CREATOR())
REFLECT_TEMPLATE_TYPE(template<typename T>, std::weak_ptr<T>,	REFLECT_CREATOR())

// --------------------------------------------------------------------------------------------------------------------
REFLECT_TEMPLATE_TYPE(template<typename T RFFLECT_COMMA typename A>, 
					  std::vector<T RFFLECT_COMMA A>, 
					  REFLECT_CREATOR())

// End of file --------------------------------------------------------------------------------------------------------
