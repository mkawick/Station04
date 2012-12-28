#pragma once
#include "Vector3.h"

//---------------------------------------------------------------------------------------------------------------------------------

class CapsuleCollision
{
public:
	CapsuleCollision ();
	void		SetEnds (Vector3 v1, Vector3 v2);
	void		SetRadius (float r);
	//-------------------------------------------
	
	bool		Collides (const Vector3& point) const;
	bool		Collides (const Vector3& point, float radius) const;
	bool		Collides (const CapsuleCollision& other) const;// capsule	
	bool		Collides (const Vector3& r1, const Vector3& r2) const;// rect
	
protected:
	float		Radius;
	Vector3		pts[2];
};

//---------------------------------------------------------------------------------------------------------------------------------

