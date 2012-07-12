/*
 *  Viewport.cpp
 *  SDL04_Station
 */

#include "stdafx.h"
#include <windows.h>
#ifndef __gl_h_
#include <SDL/SDL_opengl.h>
#include "glut.h"
#endif

#include "Viewport.h"
#include "GameData.h"
#include "player.h"
#include "playerdatabase.h"

//---------------------------------------------------------

Viewport :: Viewport ()
{
	NormalZDepth = 0;
}

//---------------------------------------------------------

void	Viewport :: SetNormalZDepth (float z)
{
	NormalZDepth = z;
}

//---------------------------------------------------------

void	Viewport :: SetCameraPosition (Vector v)
{
	Position = v;
}

//---------------------------------------------------------

void	Viewport :: SetCameraAngle (Vector v)
{
	CameraAngle = v;
}

//---------------------------------------------------------

void	Viewport :: MoveCameraToPosition (Vector v, float s, float a)
{
	Destination = v;
	Vector MovementVector = Destination - Position;
	Speed = MovementVector.AsNormal () * s;
	Acceleration = MovementVector.AsNormal () * a;
}

//---------------------------------------------------------

void	Viewport :: Update (GameData& data)
{
	PlayerDatabase* playerdb = data.GetPlayerDatabase();
	if (playerdb != NULL)
	{
		UUID CurrentPLayerID  = playerdb->GetCurrentPlayerID();
		Player* player = playerdb->GetPlayer (CurrentPLayerID);
		
		Vector v = player->GetViewFocus ();
		v.z += NormalZDepth;
		SetCameraPosition(v);
	}
	
	float	ViewAngle = 0.11;// simply for setting the proper frustum
	glFrustum (-ViewAngle, ViewAngle, -ViewAngle, ViewAngle, 0.1, 9000);
	
	// I can't express how important setting the Frustrum before rotating and translating is.
	// We will end up with very strange clipping in 3-space if we don't keep these
	// ordered thusly.
	Vector LookAt = -CameraAngle;
	glRotated (LookAt.pitch, 1.0, 0, 0);
	glRotated (LookAt.yaw, 0, 1.0, 0);
	glRotated (LookAt.roll, 0, 0, 1.0);		
	
	glTranslated (-Position.x, -Position.y, -Position.z);
	
}

//---------------------------------------------------------
