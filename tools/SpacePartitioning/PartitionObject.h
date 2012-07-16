#pragma once
// PartitionObject.h

#include "../../Common/struct.h"
class StellarObject;

enum CollisionFlags
{
	CollisionFlags_None = 0,
	CollisionFlags_Asteroid		= 1 <<  0,
	CollisionFlags_Ship			= 1 <<  1,
	CollisionFlags_Spacestation = 1 <<  2,
	CollisionFlags_Planet		= 1 <<  3,

	CollisionFlags_Missile		= 1 <<  8,
	CollisionFlags_PlasmaBurst	= 1 <<  9,
	CollisionFlags_Phaser		= 1 << 10,
	CollisionFlags_Drill		= 1 << 11,


	CollisionFlags_Resource		= 1 << 17,

	CollisionFlags_Mining = CollisionFlags_Asteroid | CollisionFlags_Drill,
};


class PartitionObject 
{
public:
	float					x1, y1, x2, y2;
	unsigned int			collisionFlag;
	StellarObject*			obj;

	PartitionObject( float x1, float y1, float x2, float y2 );
	PartitionObject();

	void SetObject( StellarObject* _obj ){ obj = _obj;}

	float cx() const { return ( x1 + x2 )*0.5f;}
	float cy() const { return ( y1 + y2 )*0.5f;}
};
