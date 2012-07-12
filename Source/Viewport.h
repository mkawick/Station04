/*
 *  Viewport.h
 *  SDL04_Station
 *
 *  Created by Mickey Kawick on 25/11/08.
 *  Copyright 2008-2012 __MyCompanyName__. All rights reserved.
 *
 */
#include "Vector.h"

class GameData;
//---------------------------------------------------------

class Viewport
{
public:
	Viewport ();
	
	void	SetNormalZDepth (float z);
	void	SetCameraPosition (Vector v);
	void	SetCameraAngle (Vector v);
	void	MoveCameraToPosition (Vector v, float Speed = 1.0, float Acceleration = 1.0);
	
	const Vector& GetCameraPosition () const {return Position;}
	
	void	Update (GameData& data);
	
	//------------------------------------------------
protected:
	float	NormalZDepth;
	Vector	Position;
	Vector	CameraAngle;
	Vector	Destination;
	Vector	Speed;
	Vector	Acceleration;
};

//---------------------------------------------------------
