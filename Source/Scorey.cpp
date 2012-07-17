/*
 *  Scorey.cpp
 *  Station05
 *
 */

#include "stdafx.h"
#include <windows.h>
#ifndef __gl_h_
#include "../tools/SDL/include/SDL_opengl.h"
#include "../tools/GL/include/glut.h"
#endif
#include "Scorey.h"
#include <math.h>


//----------------------------------------------
//----------------------------------------------

Scorey :: Scorey () :	ShipArchetype (), BodyWidth (3), BodyLength (8)
{
}

//----------------------------------------------

void			Scorey :: Rotate (RotationDir dir)
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

void			Scorey :: SetAngleOfRotation (Vector angle)
{
}

//----------------------------------------------

void			Scorey :: ApplyThrust ()
{
	Vector v;
	v.ConvertAngleInDegreesToXYComponents (Angle.z);
	Velocity += v;
	
	if (Velocity.Magnitude () > 8)
	{
		Velocity.Normalize ();
		Velocity *= 8;
	}
	IsApplyingThrust = true;
}

//----------------------------------------------

void			Scorey :: Update (GameData& GlobalGameData)
{
	// tending toward 0 although in space this doesn't really happen
	
	float Speed = Velocity.Magnitude ();
	if (Speed > 0)
	{
		float SlowingValue = Speed/100;
		if (SlowingValue < 0.2)
			SlowingValue = 0.2;
		Speed -= SlowingValue;// negative 10%, or a minimum value (Zenos paradox).
		if (Speed<SlowingValue)
			Speed = 0;
	}
	
	Velocity = Velocity.AsNormal () * Speed;
	
	Center += Velocity;
}

//----------------------------------------------

void			Scorey :: PostDrawCleanup ()
{
	IsApplyingThrust = false;
}

//----------------------------------------------
//----------------------------------------------

void			Scorey :: Setup ()
{
	DrawList =  glGenLists(1);
	glNewList (DrawList, GL_COMPILE);
	
	float Left = -BodyWidth*0.5, Top = -BodyLength*0.5;
	float Right = -Left, Bottom = -Top;
	float MidBottom = 0;
	float WideBody = 0.5;
	float Bevel = 0.3;
	float NearDepth = 0.0;
	float FarDepth = 0.6;
	float BevelDepth = 1.2;
	float WingWidth = 6.3;
	
	// the main body is composed of various quads
	SetMaterialsAndColorForShipBody ();
	
	DrawBody (Left, Right, Top, Bottom, WideBody, 
			  Bevel, MidBottom, NearDepth, FarDepth, BevelDepth);
	
	DrawWing (FrontRight, Right, Right+WingWidth, Top, Bottom, 
			  NearDepth, FarDepth, Bevel);
	
	DrawWing (FrontLeft, Left-WingWidth, Left, Top, Bottom, 
			  NearDepth, FarDepth, Bevel);
	
	SetMaterialsAndColorForCockpitWindows ();
	DrawCockpit (Left, Right, Top, MidBottom,
				 Bevel, NearDepth+0.3);
	
	DrawEngine (Left-WideBody, Right+WideBody, Bottom, Bottom+1);
	
	glEndList();
}

//----------------------------------------------

void			Scorey :: Draw ()
{
	glPushMatrix();		
	
	glTranslatef (Center.x, Center.y, Center.z);
	
	float Scale = 0.12;//1; //0.2
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

void			Scorey :: DrawThrust (float MaxLength)
{
	SetMaterialsAndColorForExhaust ();
	float TopMostPosition = ExhaustPosition.y;
	float NozzleHeight = TopMostPosition+MaxLength *0.25;// use 25% of the length for the initial nozzle exit
	float FlameBodyHeight = NozzleHeight+MaxLength *0.45;
	float FlameTipHeight = FlameBodyHeight+MaxLength * 0.30;
	
	float NozzleLeft = -BodyWidth*0.25;
	float NozzleRight = -NozzleLeft;	
	float FullLeft = -BodyWidth*0.75;
	float FullRight = -FullLeft;
	
	float RandomFlameCenter = (rand () % 100) / 100.0;
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

void			Scorey :: DrawBody (float Left, float Right, float Top, float Bottom, float WideBody, 
									 float Bevel, float MidBottom, 
									 float NearDepth, float FarDepth, float BevelDepth)
{
	// differing heights
	AddQuad (Left, Top, FarDepth,// left part
			 Left, Bottom, FarDepth,
			 Left+Bevel, MidBottom, NearDepth,
			 Left+Bevel, Top+Bevel, NearDepth);
	
	AddQuad (Right, Top, FarDepth,// left part
			 Right-Bevel, Top+Bevel, NearDepth,
			 Right-Bevel, MidBottom, NearDepth,
			 Right, Bottom, FarDepth);
	
	AddQuad (Left, Top, FarDepth,// nose tip
			 Left+Bevel, Top+Bevel, NearDepth,
			 Right-Bevel, Top+Bevel, NearDepth,
			 Right, Top, FarDepth);
	
	AddQuad ( //back part
			 Left+Bevel, MidBottom, NearDepth,
			 Left, Bottom, FarDepth,
			 Right, Bottom, FarDepth,
			 Right-Bevel, MidBottom, NearDepth);
	
	////////////////////////////////////////////////////
	// Top
	AddQuad (Left+Bevel, Top+Bevel, NearDepth,
			 Left+Bevel, MidBottom, NearDepth,
			 Right-Bevel, MidBottom, NearDepth,
			 Right-Bevel, Top+Bevel, NearDepth);

}

//----------------------------------------------

void			Scorey :: DrawWing (WingTypes type, float Left, float Right, 
									float Top, float Bottom, 
									float NearDepth, float FarDepth, float Bevel)
{
	float MidHorizontal = (Left+Right) / 2.0;
	float OneFourthDown = Top + (Bottom-Top) / 4.0;
	float MidVertical = Bottom - (Bottom - OneFourthDown) / 2.0;	
	
	if (type == FrontLeft)
	{
		
		AddQuad (Right, Top, FarDepth,
				 MidHorizontal, OneFourthDown, FarDepth,
				 MidHorizontal, MidVertical, FarDepth,
				 Right, Bottom, FarDepth);
		AddQuad (Left, Top, FarDepth,
				 Left, OneFourthDown, FarDepth,
				 MidHorizontal, MidVertical, FarDepth,
				 MidHorizontal, OneFourthDown, FarDepth);
	}
	else if (type == FrontRight)
	{
		AddQuad (Left, Top, FarDepth,
				 Left, Bottom, FarDepth,
				 MidHorizontal, MidVertical, FarDepth,
				 MidHorizontal, OneFourthDown, FarDepth);
		
		AddQuad (Right, Top, FarDepth,
				 MidHorizontal, OneFourthDown, FarDepth,
				 MidHorizontal, MidVertical, FarDepth,
				 Right, OneFourthDown, FarDepth );
	}
}

//----------------------------------------------

void			Scorey :: DrawCockpit (float Left, float Right, float Top, float Bottom,
										float Bevel, float Depth)
{
	float CockpitTop = Depth - 0.6F;
	float CockpitMidline = Top + (Bottom-Top)/3;
	float CockpitBackline = Bottom - (Bottom-Top)/2;
	float Bevels = Bevel*2;
	//float Bevels4 = Bevel*4;
	
	// Front Window
	AddQuad (Left+Bevel, Top+Bevels, Depth,
			 Left+Bevels, CockpitMidline, CockpitTop,
			 Right-Bevels, CockpitMidline, CockpitTop,
			 Right-Bevel, Top+Bevels, Depth);
	
	AddQuad (Left+Bevel, Top+Bevels, Depth,// left
			 Left+Bevel, Bottom-Bevel, Depth,
			 Left+Bevels, CockpitBackline, CockpitTop,
			 Left+Bevels, CockpitMidline, CockpitTop);
	AddQuad (Right-Bevel, Top+Bevels, Depth,// Right
			 Right-Bevels, CockpitMidline, CockpitTop,
			 Right-Bevels, CockpitBackline, CockpitTop,
			 Right-Bevel, Bottom-Bevel, Depth);
	
	AddQuad (Right-Bevel, Bottom-Bevel, Depth,// Back
			 Right-Bevels, CockpitBackline, CockpitTop,
			 Left+Bevels, CockpitBackline, CockpitTop,
			 Left+Bevel, Bottom-Bevel, Depth);
	
	AddQuad (Left+Bevels, CockpitMidline, CockpitTop,// top
			 Left+Bevels, CockpitBackline, CockpitTop,
			 Right-Bevels, CockpitBackline, CockpitTop,
			 Right-Bevels, CockpitMidline, CockpitTop);
}

//----------------------------------------------

void			Scorey :: DrawEngine (float Left, float Right, float Top, float Bottom)
{
	ExhaustPosition.Set ((Left+Right)/2, Bottom+0.3, 0);
	GLUquadric* quadric = gluNewQuadric ();
	
	float Radius = (Right-Left)/4;
	float Length = Bottom-Top;
	
	//////////////////////////////////////////////////////////
	
	glPushMatrix();
	glTranslatef (0, Top, Radius/1.2);
	glRotatef (-90, 1, 0, 0);
	gluCylinder (quadric, Radius,
				 Radius/2, 
				 Length, 20, 20);
	
	glPopMatrix();
	
	//////////////////////////////////////////////////////////
	gluDeleteQuadric (quadric);	
}

//----------------------------------------------

