/*
 *  CapsuleCollision.cpp
 *  Station05
 */

#include <windows.h>
#include "CapsuleCollision.h"
#include <math.h>
#include "MathFunctions.h"

//---------------------------------------------------------------------------------------------------------------------------------

CapsuleCollision :: CapsuleCollision () : Radius (0)
{
}

//-------------------------------------------

void		CapsuleCollision :: SetEnds (Vector v1, Vector v2)
{
	pts[0] = v1, pts[1] = v2;
}

//-------------------------------------------

void		CapsuleCollision :: SetRadius (float r)
{
	Radius = r;
}

//-------------------------------------------

bool		CapsuleCollision :: Collides (const Vector& pt) const// point
{
	float Dist = GetSquareDistanceFromPointToLine (pts[0], pts[1], pt);
	
	if (Dist <= Radius*Radius) return true;
	return false;
}

//-------------------------------------------

bool		CapsuleCollision :: Collides (const Vector& pt, float radius) const
{
	float Dist = GetSquareDistanceFromPointToLine (pts[0], pts[1], pt);
	
	float test = Radius + radius;
	if (Dist <= test*test) return true;
	return false;
}

//-------------------------------------------

bool		CapsuleCollision :: Collides (const CapsuleCollision& other) const// capsule
{
	return false;
}

//-------------------------------------------

bool		CapsuleCollision :: Collides (const Vector& r1, const Vector& r2) const// rect
{
	return false;
}

//---------------------------------------------------------------------------------------------------------------------------------

