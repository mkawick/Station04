/*
 *  CapsuleCollision.h
 *  SDL04_Station
 *
 */
#pragma once
#include "Vector.h"

//---------------------------------------------------------------------------------------------------------------------------------

class CapsuleCollision
{
public:
	CapsuleCollision ();
	void		SetEnds (Vector v1, Vector v2);
	void		SetRadius (float r);
	//-------------------------------------------
	
	bool		Collides (const Vector& point) const;
	bool		Collides (const Vector& point, float radius) const;
	bool		Collides (const CapsuleCollision& other) const;// capsule	
	bool		Collides (const Vector& r1, const Vector& r2) const;// rect
	
protected:
	float		Radius;
	Vector		pts[2];
};

//---------------------------------------------------------------------------------------------------------------------------------

