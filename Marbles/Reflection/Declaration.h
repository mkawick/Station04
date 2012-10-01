#pragma once

// --------------------------------------------------------------------------------------------------------------------
struct Declaration
{
	shared_type type;
	bool is_constant : 1;
	bool is_reference : 1;
	bool is_function : 1;
	bool is_collection : 1;
	uint32_t reserved : 28;
};

template<typename T>
struct DeclarationT
{
	DeclarationT()
	{
		type = TypeOf<T>();
		
	}
}

// End of file --------------------------------------------------------------------------------------------------------
