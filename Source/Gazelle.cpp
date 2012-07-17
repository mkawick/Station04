/*
 *  Gazelle.cpp
 *  Station05
 *
 */

#include "stdafx.h"
#include <windows.h>
#ifndef __gl_h_
#include <SDL/SDL_opengl.h>
#endif
#include "Gazelle.h"
#include "GameData.h"
#include <math.h>


//----------------------------------------------
//----------------------------------------------

Gazelle :: Gazelle () :	ShipArchetype (), BodyWidth (2), BodyLength (10)
{
}

//----------------------------------------------

void			Gazelle :: Rotate (RotationDir dir)
{
	Vector v;
	float AngleAdd = 4;
	if (dir == CW)
	{
		v.Set (0, 0, AngleAdd);
	}
	else
	{
		v.Set (0, 0, -AngleAdd);
	}
	
	Angle += v;
}

//----------------------------------------------

void			Gazelle :: SetAngleOfRotation (Vector angle)
{
}

//----------------------------------------------

void			Gazelle :: ApplyThrust ()
{
	Vector v;
	v.ConvertAngleInDegreesToXYComponents (Angle.z);
	v *= 100;
	Velocity += v;
	
	if (Velocity.Magnitude () > 80)
	{
		Velocity.Normalize ();
		Velocity *= 80;
	}
	IsApplyingThrust = true;
}

//----------------------------------------------

void			Gazelle :: Update (GameData& GlobalGameData)
{
	// tending toward 0 although in space this doesn't really happen
	
	float Speed = Velocity.Magnitude () * GlobalGameData.GetTimeElapsedInMS() / 1000;
	if (Speed > 0)
	{
		float SlowingValue = Speed/100;
		if (SlowingValue < 0.002F)
			SlowingValue = 0.002F;
		Speed -= SlowingValue;// negative 10%, or a minimum value (Zenos paradox).
		if (Speed<SlowingValue)
			Speed = 0;
	}
	
	Velocity = Velocity.AsNormal () * Speed;
	
	Center += Velocity;
}

//----------------------------------------------

void			Gazelle :: PostDrawCleanup ()
{
	IsApplyingThrust = false;
}

//----------------------------------------------
//----------------------------------------------

void			Gazelle :: Setup ()
{
	DrawList =  glGenLists(1);
	glNewList (DrawList, GL_COMPILE);
	
	float Left = -BodyWidth*0.5F, Top = -BodyLength*0.5F;
	float Right = -Left, Bottom = -Top;
	float MidBottom = 0;
	float WideBody = 0.5F;
	float Bevel = 0.3F;
	float NearDepth = 0.0F;
	float FarDepth = 0.6F;
	float BevelDepth = 1.2F;
	float WingWidth = 4.3F;
	
		// the main body is composed of various quads
		SetMaterialsAndColorForShipBody ();
		
		DrawBody (Left, Right, Top, Bottom, WideBody, 
				  Bevel, MidBottom,  NearDepth, FarDepth, BevelDepth);
		
		DrawWing (RearRight, Right+WideBody, Right+WingWidth, MidBottom, Bottom, 
				  NearDepth, FarDepth, Bevel);
		
		DrawWing (RearLeft, Left-WingWidth, Left-WideBody, MidBottom, Bottom, 
				  NearDepth, FarDepth, Bevel);
		
		SetMaterialsAndColorForCockpitWindows ();
		DrawCockpit (Left-WideBody, Right+WideBody, MidBottom, Bottom,
					 Bevel, NearDepth);
	
		DrawEngine (Left-WideBody, Right+WideBody, Bottom, Bottom+2);
	
	glEndList();
}

//----------------------------------------------

void			Gazelle :: Draw ()
{
	glPushMatrix();		
	
		glTranslatef (Center.x, Center.y, Center.z);
		
		float Scale = 0.12F;//1; //0.2
		glScalef (Scale, Scale, Scale);// order is critical here, we must scale after rotating
		glRotatef (Angle.x+180, 1, 0, 0);// flip front to back
		glRotatef (Angle.y, 0, 1, 0);
		glRotatef (Angle.z, 0, 0, 1);
		
		glCallList (DrawList);
	
		if (IsApplyingThrust)
		{
			float MaxExhaustLength = 3;
			DrawThrust (MaxExhaustLength);
		}
	glPopMatrix();
}

//----------------------------------------------

void			Gazelle :: DrawThrust (float MaxLength)
{
	SetMaterialsAndColorForExhaust ();
	float TopMostPosition = ExhaustPosition.y;
	float NozzleHeight = TopMostPosition+MaxLength *0.25F;// use 25% of the length for the initial nozzle exit
	float FlameBodyHeight = NozzleHeight+MaxLength *0.45F;
	float FlameTipHeight = FlameBodyHeight+MaxLength * 0.30F;
	
	float NozzleLeft = -BodyWidth*0.25F;
	float NozzleRight = -NozzleLeft;	
	float FullLeft = -BodyWidth*0.75F;
	float FullRight = -FullLeft;
	
	float RandomFlameCenter = (rand () % 100) / 100.0F;
	RandomFlameCenter *= NozzleLeft-NozzleRight;
	RandomFlameCenter -= NozzleLeft;
	
	float FlameTipLeft = NozzleLeft + RandomFlameCenter;
	float FlameTipRight = NozzleRight + RandomFlameCenter; 
	
	glPushMatrix();	
	AddQuad (NozzleLeft, TopMostPosition, 0,// initial Nozzle exit
			 FullLeft, NozzleHeight, 0,
			 FullRight, NozzleHeight, 0,
			 NozzleRight, TopMostPosition, 0);
	
	AddQuad (FullLeft, NozzleHeight, 0,// main flame body
			 FlameTipLeft, FlameBodyHeight, 0,
			 FlameTipRight, FlameBodyHeight, 0,
			 FullRight, NozzleHeight, 0);
	
	AddTri (FlameTipLeft, FlameBodyHeight, 0,
			RandomFlameCenter, FlameTipHeight, 0,
			FlameTipRight, FlameBodyHeight, 0);
	glPopMatrix();
}

//----------------------------------------------

void			Gazelle :: DrawBody (float Left, float Right, float Top, float Bottom, float WideBody, 
									float Bevel, float MidBottom, 
									float NearDepth, float FarDepth, float BevelDepth)
{
	// differing heights
	AddQuad (Left, Top, FarDepth,
			 Left-WideBody, MidBottom, NearDepth,
			 Right+WideBody, MidBottom, NearDepth,
			 Right, Top, FarDepth);
	
	AddQuad (Left-Bevel, Top-Bevel, BevelDepth,// nose tip
			 Left, Top, FarDepth,
			 Right, Top, FarDepth,
			 Right+Bevel, Top-Bevel, BevelDepth);
	
	AddQuad (Left-Bevel, Top-Bevel, BevelDepth,// left side of ship
			 Left-Bevel-WideBody, MidBottom, BevelDepth,
			 Left-WideBody, MidBottom, NearDepth,
			 Left, Top, FarDepth);
	
	AddQuad (Right+Bevel, Top-Bevel, BevelDepth,// right side of ship
			 Right, Top, FarDepth,
			 Right+WideBody, MidBottom, NearDepth,
			 Right+WideBody+Bevel, MidBottom, BevelDepth);
	
	////////////////////////////////////////////////////
	// mid back
	AddQuad (Left-WideBody, MidBottom, NearDepth,
			 Left-WideBody, Bottom, NearDepth,
			 Right+WideBody, Bottom, NearDepth,
			 Right+WideBody, MidBottom, NearDepth);
	
	AddQuad (Left-Bevel-WideBody, MidBottom, BevelDepth,// left side of ship
			 Left-Bevel-WideBody, Bottom, BevelDepth,
			 Left-WideBody, Bottom, NearDepth,
			 Left-WideBody, MidBottom, NearDepth);
	
	AddQuad (Right+Bevel+WideBody, MidBottom, BevelDepth,// right side of ship
			 Right+WideBody, MidBottom, NearDepth,
			 Right+WideBody, Bottom, NearDepth,
			 Right+WideBody+Bevel, Bottom, BevelDepth);
}

//----------------------------------------------

void			Gazelle :: DrawWing (WingTypes type, float Left, float Right, float Top, float Bottom, 
						  float NearDepth, float FarDepth, float Bevel)
{
	if (type == RearRight)
	{
		AddQuad (Left, Top, FarDepth,
				 Left, Bottom, NearDepth,
				 Right, Bottom, NearDepth,
				 Right, (Top+Bottom)/2, FarDepth);
		AddQuad (Right, (Top+Bottom)/2, FarDepth,
				 Right, Bottom, NearDepth,
				 Right+Bevel, Bottom, FarDepth,
				 Right, (Top+Bottom)/2, FarDepth);
	}
	else if (type == RearLeft)
	{
		AddQuad (Right, Top, FarDepth,
				 Left, (Top+Bottom)/2, FarDepth,
				 Left, Bottom, NearDepth,
				 Right, Bottom, NearDepth);
		AddQuad (Left, (Top+Bottom)/2, FarDepth,
				 Left-Bevel, Bottom, FarDepth,
				 Left, Bottom, NearDepth,
				 Left, (Top+Bottom)/2, FarDepth);
	}
}

//----------------------------------------------

void			Gazelle :: DrawCockpit (float Left, float Right, float Top, float Bottom,
							 float Bevel, float Depth)
{
	float CockpitTop = Depth - 0.6F;
	float CockpitMidline = (Top+Bottom)/2;
	float Bevels = Bevel*2;
	float Bevels4 = Bevel*4;
	
	// Front Window
	AddQuad (Left+Bevel, Top+Bevel, Depth,
			 Left+Bevels, CockpitMidline, CockpitTop,
			 Right-Bevels, CockpitMidline, CockpitTop,
			 Right-Bevel, Top+Bevel, Depth);
	
	// top
	AddQuad (Left+Bevels, CockpitMidline, CockpitTop,
			 Left+Bevels, Bottom-Bevels4, CockpitTop,
			 Right-Bevels, Bottom-Bevels4, CockpitTop,
			 Right-Bevels, CockpitMidline, CockpitTop);
	
	AddQuad (Left+Bevel, Top+Bevel, Depth,// left
			 Left+Bevel, Bottom-Bevel, Depth,
			 Left+Bevels, Bottom-Bevels4, CockpitTop,
			 Left+Bevels, CockpitMidline, CockpitTop);
	AddQuad (Right-Bevel, Top+Bevel, Depth,// Right
			 Right-Bevels, CockpitMidline, CockpitTop,
			 Right-Bevels, Bottom-Bevels4, CockpitTop,
			 Right-Bevel, Bottom-Bevel, Depth);
	
	AddQuad (Right-Bevel, Bottom-Bevel, Depth,// Back
			 Right-Bevels, Bottom-Bevels4, CockpitTop,
			 Left+Bevels, Bottom-Bevels4, CockpitTop,
			 Left+Bevel, Bottom-Bevel, Depth);
}

//----------------------------------------------

void			Gazelle :: DrawEngine (float Left, float Right, float Top, float Bottom)
{
	ExhaustPosition.Set ((Left+Right)/2, Bottom+0.3F, 0);
	GLUquadric* quadric = gluNewQuadric ();
	
	float Radius = (Right-Left)/4;
	float Length = Bottom-Top;
	
	//////////////////////////////////////////////////////////
	
	glPushMatrix();
		glTranslatef (0, Top, Radius/1.2F);
		glRotatef (-90, 1, 0, 0);
		gluCylinder (quadric, Radius,
					 Radius/2, 
					 Length, 20, 20);
		
	glPopMatrix();
	
	//////////////////////////////////////////////////////////
	gluDeleteQuadric (quadric);	
}

//----------------------------------------------

