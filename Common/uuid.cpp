/*
 *  uuid.cpp
 *  Station05
 */

#pragma warning ( disable: 4996 )
//#include "stdafx.h"
#include <windows.h>
#include "uuid.h"
#include <string.h>
#include <time.h>

#define CastToULong(a)	static_cast<unsigned long>(a)
//! Creates new UUID on every call.
UUID UuidGenerate()
{
	UUID uuid;
	uuid.Data1 = CastToULong( ::time(NULL) );
	uuid.Data2 = 0xa123;
	uuid.Data3 = 0xb123;
	uuid.Data4[0] = 'A';
	uuid.Data4[1] = 'B';
	uuid.Data4[2] = 'C';
	uuid.Data4[3] = 'D';
	uuid.Data4[4] = 'E';
	uuid.Data4[5] = 'F';
	uuid.Data4[6] = 'G';
	uuid.Data4[7] = 'A';
	
	return UUID (uuid);
}

//! Prints UUID value to the string in format "81BDCBCA-64D4-426d-AE8D-AD0147F4275C"
int UuidToString (const UUID& Uuid, std::string& StringOut)
{
	//AP_ASSERTMESSAGE(size >= sizeof("81BDCBCA-64D4-426d-AE8D-AD0147F4275C"));
	
	char Buffer[32];
	int result = 
	::sprintf(Buffer, 
			  "%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x",
			  static_cast<unsigned int>(Uuid.Data1), 
			  Uuid.Data2,
			  Uuid.Data3,
			  Uuid.Data4[0],
			  Uuid.Data4[1],
			  Uuid.Data4[2],
			  Uuid.Data4[3],
			  Uuid.Data4[4],
			  Uuid.Data4[5],
			  Uuid.Data4[6],
			  Uuid.Data4[7]);
	
	StringOut = Buffer;
	return result;
}
/*
bool	UUID::operator == (const UUID& uuid)
{
	if (memcmp (this, &uuid, sizeof (this)) == 0)
		return true;
	return false;
}*/
