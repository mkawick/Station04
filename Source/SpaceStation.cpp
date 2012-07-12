/*
 *  SpaceStation.cpp
 *  SDL04_Station
 *
 *  Created by Mickey Kawick on 21/11/08.
 *  Copyright 2008-2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include <windows.h>
#include "struct.h"
#include "SpaceStation.h"
#include <math.h>
#include <assert.h>
#include "mathfunctions.h"
#include "GlobalDefinitions.h"

const float SpaceStation :: CylinderMultiplier = 0.5f;
//---------------------------------------------------------

SpaceStation :: SpaceStation () : StellarObject ()
{
	ShieldLevel[0] = 3;
	ShieldLevel[1] = 3;
	ShieldColor.Set (0.8F, 0.6F, 0.4F);
	int NumResources = ResourceTypes_Count;
	for (int i=0; i<NumResources; i++)
	{
		//ResourcesStored[i].SetResource (static_cast <ResourceTypes> (i));
		ResourcesStored[i] = 0;//.SetQuantity (0);
		ResourceCapacity[i] = 500;
	}
	SetCurrentHitPoints (100);
}

//---------------------------------------------------------

int				SpaceStation :: GetShieldLevel () const// overall 0-100, not individually
{
	int value = (ShieldLevel[0] + ShieldLevel[1]) * 17;
	if (value > 100)
		value = 100;
	return value;
}

//---------------------------------------------------------

void			SpaceStation :: SetShieldLevel (int WhichSphere, int level)
{
	LIMIT (level, 0, 3);
	LIMIT (WhichSphere, 0, 1);
	
	ShieldLevel [WhichSphere] = level;
}

//---------------------------------------------------------

void	SpaceStation :: Setup ()
{
	DrawList =  glGenLists(1);
	glNewList (DrawList, GL_COMPILE);

		/////////////////////////////////////////////////////
		
	float SphereRadius = 2.0;
	float Spread = 3.0;
	GLUquadric* quadric = gluNewQuadric ();
		
		glPushMatrix();
			glTranslatef (-Spread/2, 0, 0);
			glRotatef (90, 0, 1, 0);
			SetMaterialsAndColorForStation ();
			gluCylinder (quadric, SphereRadius/2, SphereRadius/2, 
						 Spread, 16, 16);
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef (-Spread, 0, 0);
			CreateSpikeyBall (quadric, SphereRadius);
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef (Spread, 0, 0);
			CreateSpikeyBall (quadric, SphereRadius);
		glPopMatrix();
		
		glPushMatrix();
			CreateCentralHumanWindowsLongitudinally (SphereRadius/2, Spread);
		glPopMatrix();
		
		DrawShields (quadric, SphereRadius, Spread);
		//////////////////////////////////////////////////////////
	gluDeleteQuadric (quadric);	
	
	glEndList();
}

//---------------------------------------------------------

void	SpaceStation :: CreateSpikeyBall (GLUquadric* quadric, float radius)
{
	// first create 3 cones going all the way through
	
	float CylinderLength = radius*2.12F;
	float HalfRadius = CylinderLength/2;
	float CylinderRadius = radius/9;
	
	// move us back
	glTranslatef (0, HalfRadius, 0);
	
	SetMaterialsAndColorForShieldGenerators ();
	
	glPushMatrix();// up/down
		glTranslatef (0, -HalfRadius, 0);
		CreateCylinder (quadric, CylinderRadius, CylinderLength);// front to back
	glPopMatrix();
	
	glPushMatrix();// forward/back
		glTranslatef (0, -HalfRadius, 0);
		glRotatef (90, 1, 0, 0);
		CreateCylinder (quadric, CylinderRadius, CylinderLength);
	glPopMatrix();
	
	glPushMatrix();// left/right
		glTranslatef (0, -HalfRadius, 0);
		glRotatef (90, 0, 1, 0);
		CreateCylinder (quadric, CylinderRadius, CylinderLength);
	glPopMatrix();
	
	// we're slightly offcentrer now, so move us into position
	glTranslatef (0, -HalfRadius, 0);

	// draw the sphere
	glPushMatrix();
		glColor3f (0, 0, 1.0);
		SetMaterialsAndColorForStation ();
		gluSphere (quadric, radius, 16, 16);
	glPopMatrix();	
}

//---------------------------------------------------------

void	SpaceStation :: CreateCentralHumanWindowsLongitudinally (float Radius, float Length)
{
	float RectHeight = Radius/5.0F;
	
	float Top = -RectHeight/2;
	float Left = -Length/2;
	float Bottom = RectHeight/2;
	float Right = Length/2;
	
	float WidndowDistanceFromCenter =  -(Radius + 0.02F);
	SetMaterialsAndColorForWindows ();	
	
	glBegin (GL_QUADS);	// front
		glVertex3f (Left, Top, WidndowDistanceFromCenter);
		glVertex3f (Left, Bottom, WidndowDistanceFromCenter);
		glVertex3f (Right, Bottom, WidndowDistanceFromCenter);
		glVertex3f (Right, Top, WidndowDistanceFromCenter);
	glEnd ();
	
	glBegin (GL_QUADS);	// back
		glVertex3f (Right, Top, -WidndowDistanceFromCenter);
		glVertex3f (Right, Bottom, -WidndowDistanceFromCenter);
		glVertex3f (Left, Bottom, -WidndowDistanceFromCenter);
		glVertex3f (Left, Top, -WidndowDistanceFromCenter);
	glEnd ();
	
	SetMaterialsAndColorForStation ();
	
	float WindowStripSpread = Length/9;
	float z =  WidndowDistanceFromCenter-0.03F;
	for (float x1 = Left+WindowStripSpread, x2 = Right-WindowStripSpread; x1<= Right; x1 += WindowStripSpread, x2 -= WindowStripSpread)
	{
		DrawWindowStrip (x1, Top, Bottom, z);
		DrawWindowStrip (x2, Top, Bottom, -z);
	}
}

//---------------------------------------------------------

void	SpaceStation :: DrawWindowStrip (float LeftRightPos, float Top, float Bottom, float z)
{
	float Left = LeftRightPos-0.02F;
	float Right = LeftRightPos+0.02F;
	
	glBegin (GL_QUADS);	// back
		glVertex3f (Left, Top, z);
		glVertex3f (Left, Bottom, z);
		glVertex3f (Right, Bottom, z);
		glVertex3f (Right, Top, z);
	glEnd ();
}


//---------------------------------------------------------


void	SpaceStation :: DrawShields (GLUquadric* quadric, float Radius, float Spread)
{
	glEnable (GL_BLEND);
	SetMaterialsForShields ();
	
	for (int j=0; j<2; j++)
	{
		float WorkingRadius = Radius;
		float Position = Spread;
		if (j == 0)
			Position = -Spread;
		
		for (int i=0; i<ShieldLevel[j]; i++)
		{
			WorkingRadius += 0.3F;			
			SetColorForShields (i);
			glPushMatrix();
				glTranslatef (Position, 0, 0);
				gluSphere (quadric, WorkingRadius, 24, 24);
			glPopMatrix();
		}
	}
	
	float WorkingRadius = Radius * CylinderMultiplier;
	
	int AverageRadius = (ShieldLevel[0] + ShieldLevel[1])/2;
	
	for (int i=0; i<AverageRadius; i++)
	{
		WorkingRadius += 0.3F;
		SetColorForShields (i);
		glPushMatrix();
			glTranslatef (-Spread, 0, 0);
			glRotatef (90, 0, 1, 0);
			gluCylinder (quadric, WorkingRadius,
						 WorkingRadius, 
						 Spread*2, 20, 20);
		glPopMatrix();
	}
	
/*	glBlendFunc (GL_CONSTANT_ALPHA, GL_CONSTANT_ALPHA); 
	glBlendEquation (GL_CONSTANT_COLOR);*/
	glDisable (GL_BLEND);		
}

//---------------------------------------------------------

void	SpaceStation :: CreateCylinder (GLUquadric* quadric, float Radius, float Length)
{
	float HalfRadius = Length * CylinderMultiplier;
						
	glPushMatrix();
		glTranslatef (0, 0, -HalfRadius);
		gluCylinder (quadric, Radius, Radius, 
					 Length, 16, 16);
	glPopMatrix();	
	
	glPushMatrix();
		glTranslatef (0, 0, -HalfRadius);
		glRotatef (180, 1, 0, 0);
		gluDisk (quadric, 0, Radius, 16, 1);
	glPopMatrix();	
	
	glPushMatrix();	
		glTranslatef (0, 0, HalfRadius);
		gluDisk (quadric, 0, Radius, 16, 1);
	glPopMatrix();	
}

//---------------------------------------------------------
//---------------------------------------------------------

void	SpaceStation :: SetMaterialsAndColorForStation ()
{
	float Amb = 0.4F;
	//float Scale = 5.0;
	float Specular = 0.1F;
	float low_shininess = 5.0f;
	
	float no_mat []  = {0.0f, 0.0f, 0.0f, 1.0f};
	float mat_ambient_color [] = {Amb, Amb, 0.8F, 1.0f};
	float mat_diffuse [] = {Color.r, Color.g, Color.b, 1.0f};
	float mat_specular [] = {Specular, Specular, Specular, 1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, low_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	//glColor3f (Color.r, Color.g, Color.b);
	glBlendColor (Color.r, Color.g, Color.b, 1.0);
}

//---------------------------------------------------------

void	SpaceStation :: SetMaterialsAndColorForShieldGenerators ()
{
	float Amb = 0.4F;
	const float Diffuse = 0.6F;
	//float Scale = 5.0;
	float Specular = 0.06F;
	float low_shininess = 5.0f;
	
	float no_mat []  = {0.0f, 0.0f, 0.0f, 1.0f};
	float mat_ambient_color [] = {Amb, Amb, 0.2F, 1.0f};
	float mat_diffuse [] = {Diffuse, Diffuse, Diffuse, 1.0f};
	float mat_specular [] = {Specular, Specular, Specular, 1.0f};
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, low_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	//glColor3f (0.8, 0.8, 0.2);
	glBlendColor (0.8F, 0.8F, 0.2F, 1.0F);
}

//---------------------------------------------------------

void	SpaceStation :: SetMaterialsAndColorForWindows ()
{
	//float Amb = 0.4;
	//float Diffuse = 1.0;
	//float Scale = 5.0;
	float Specular = 0.1F;
	float low_shininess = 5.0f;
	
	float no_mat []  = {0.0f, 0.0f, 0.0f, 1.0f};
	//float mat_ambient_color [] = {Amb, Amb, 0.8, 1.0f};
	float mat_diffuse [] = {0.1f, 0.5f, 0.8f, 1.0f};
	float mat_specular [] = {Specular, Specular, Specular, 1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, low_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	//glColor3f (0, 0.1, 0.3);
	glBlendColor (0, 0.1F, 0.3F, 1.0F);
}

//---------------------------------------------------------

void	SpaceStation :: SetMaterialsForShields ()
{
	float Amb = 0.4F;
	float Specular = 0.1F;
	float low_shininess = 5.0f;
	
	float no_mat []  = {0.0f, 0.0f, 0.0f, 1.0f};
	float mat_ambient_color [] = {Amb, Amb, Amb, 1.0f};
	float mat_diffuse [] = {ShieldColor.r, ShieldColor.g, ShieldColor.b, 1.0f};
	float mat_specular [] = {Specular, Specular, Specular, 1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, low_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	
	glBlendFunc (GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA); 
	glBlendEquation (GL_FUNC_ADD);
}

//---------------------------------------------------------

void	SpaceStation :: SetColorForShields (int Level)
{
	if (Level == 0)
	{
		glBlendColor (ShieldColor.r, ShieldColor.g, ShieldColor.b, 0.18F);
	}
	else if (Level == 1)
	{
		glBlendColor (ShieldColor.r, ShieldColor.g, ShieldColor.b, 0.13F);
	}
	else if (Level == 2)
	{
		glBlendColor (ShieldColor.r, ShieldColor.g, ShieldColor.b, 0.05F);
	}
	
}
//---------------------------------------------------------
//---------------------------------------------------------
