/*
 *  GeneralDataTypes.cpp
 *  Station05
 *
 *  Created by Mickey Kawick on 16/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <assert.h>
#include <windows.h>
#include "GeneralDataTypes.h"

float&	ScreenRect::operator[]	( int i )
{
	assert( i>=0 && i<4 );
	switch( i )
	{
	case 0: return Corners[0].x;
	case 1: return Corners[0].y;
	case 2: return Corners[1].x;
	case 3: return Corners[1].y;
	}

	return Corners[0].x;
}

float&	ScreenRect::operator[]	( unsigned int i )
{
	return operator[]((int) i);
}