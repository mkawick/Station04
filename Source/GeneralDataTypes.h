/*
 *  GeneralDataTypes.h
 *  Station05
 */

#pragma once
#include "../common/Math/Vector.h"
//----------------------------------------------------

struct ScreenRect
{
	Vector2D Corners [2];
	float&	operator[]	( int i );
	float&	operator[]	( unsigned int i );
};