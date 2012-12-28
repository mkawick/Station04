/*
 *  CapsuleCollision.cpp
 *  Station05
 */

#include "Math/CapsuleCollision.h"
#include "Math/MathFunctions.h"
// #include <cmath>

//---------------------------------------------------------------------------------------------------------------------------------

CapsuleCollision :: CapsuleCollision () : Radius (0)
{
}

//-------------------------------------------

void		CapsuleCollision :: SetEnds (Vector3 v1, Vector3 v2)
{
	pts[0] = v1, pts[1] = v2;
}

//-------------------------------------------

void		CapsuleCollision :: SetRadius (float r)
{
	Radius = r;
}

//-------------------------------------------

bool		CapsuleCollision :: Collides (const Vector3& pt) const// point
{
	float Dist = GetSquareDistanceFromPointToLine (pts[0], pts[1], pt);
	
	if (Dist <= Radius*Radius) return true;
	return false;
}

//-------------------------------------------

bool		CapsuleCollision :: Collides (const Vector3& pt, float radius) const
{
	float Dist = GetSquareDistanceFromPointToLine (pts[0], pts[1], pt);
	
	float test = Radius + radius;
	if (Dist <= test*test) return true;
	return false;
}

//-------------------------------------------

bool		CapsuleCollision :: Collides (const CapsuleCollision& other) const// capsule
{
	(void)other;
	return false;
}

//-------------------------------------------

bool		CapsuleCollision :: Collides (const Vector3& r1, const Vector3& r2) const// rect
{
	(void) r1; (void)r2;
	return false;
}

//---------------------------------------------------------------------------------------------------------------------------------

