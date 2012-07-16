/*
 *  uuid.h
 *  Station05
 *
 */

#pragma once


#include <string>
/*
#if !defined( UUID )
	class UUID  
		{
		public:
			unsigned long	Data1;
			unsigned short	Data2;
			unsigned short	Data3;
			unsigned char	Data4[8];
			
			bool	operator == (const UUID& uuid);
		} ;
#endif*/
	
	//! Creates new UUID on every call.
	UUID	UuidGenerate ();
	
	//! Prints UUID value to the string in format "81BDCBCA-64D4-426d-AE8D-AD0147F4275C". 
	//! Returns size of resulting string representation.
	int		UuidToString (const UUID& Uuid, std::string& StringOut);
