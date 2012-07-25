/*
 *  ShipArchetype.cpp
 *  Station05
 */

#include "stdafx.h"
#include <windows.h>
#ifndef __gl_h_
#include "../tools/GL/include/glee.h"
#include "../tools/SDL/include/SDL_opengl.h"
#include "../tools/GL/include/glut.h"
#endif
#include <math.h>
#include "ShipArchetype.h"

//----------------------------------------------
//----------------------------------------------

ShipArchetype :: ShipArchetype () : IsApplyingThrust (false), StellarObject()
{
	ShieldLevel[0] = 3;
	ShieldLevel[1] = 3;
	memset( ResourceStorage, 0, sizeof( ResourceStorage ) );
}


//---------------------------------------------------------

int				ShipArchetype :: GetShieldLevel ( int which ) const// overall 0-100, not individually
{
	int value = 0;
	if( which == -1 )
		value = (ShieldLevel[0] + ShieldLevel[1]);
	if( which < NumShields )
		value = ShieldLevel[ which ];
	if (value > 100)
		value = 100;
	return value;
}

//----------------------------------------------

void			ShipArchetype :: SetCenter (Vector center)
{
	Center = center;
}

//----------------------------------------------

void			ShipArchetype :: SetAngle (Vector angle)
{
	Angle = angle;
}

//----------------------------------------------

void			ShipArchetype :: SetBodyColor (Vector color)
{
	BodyColor = color;
}

//----------------------------------------------

void			ShipArchetype :: SetCockpitColors (Vector FrameColor, Vector WindowColor)
{
	CockpitFrameColor = FrameColor;
	CockpitWindowColor = WindowColor;
}

//----------------------------------------------

void			ShipArchetype :: SetNationalColors (Vector* colors, int NumColors)
{
	NationalColors.clear ();
	for (int i=0; i<NumColors; i++)
	{
		NationalColors.push_back (colors[i]);
	}
}

//----------------------------------------------

void		ShipArchetype :: SetShieldColor (Vector color)
{
	ShieldColor = color;
}

//----------------------------------------------

void		ShipArchetype :: SetMaterialsAndColorForShipBody ()
{
	float Amb = 0.08f;
	float Specular = 0.04f;
	float low_shininess = 0.2f;
	
	float no_mat []  = {0.0f, 0.0f, 0.0f, 1.0f};
	float mat_ambient_color [] = {Amb, Amb, Amb, 1.0f};
	float mat_diffuse [] = {BodyColor.r, BodyColor.g, BodyColor.b, 1.0f};
	float mat_specular [] = {Specular, Specular, Specular, 1.0f};
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, low_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	
	glBlendColor (BodyColor.r, BodyColor.g, BodyColor.b, 1.0);
}

//----------------------------------------------

void	ShipArchetype :: SetMaterialsAndColorForCockpitFrame ()
{
	float Amb = 0.4f;
	float Specular = 0.6f;
	float shininess = 10.0f;
	
	float no_mat []  = {0.5f, 0.5f, 0.5f, 1.0f};
	float mat_ambient_color [] = {Amb, Amb, 0.8f, 1.0f};
	float mat_diffuse [] = {CockpitFrameColor.r, CockpitFrameColor.g, CockpitFrameColor.b, 1.0f};
	float mat_specular [] = {Specular, Specular, Specular, 1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	
	glBlendColor (CockpitFrameColor.r, CockpitFrameColor.g, CockpitFrameColor.b, 1.0);
}

//----------------------------------------------

void	ShipArchetype :: SetMaterialsAndColorForCockpitWindows ()
{
	float Amb = 0.4f;
	float Specular = 0.6f;
	float shininess = 5.0f;
	
	float no_mat []  = {0.0f, 0.0f, 0.0f, 1.0f};
	float mat_ambient_color [] = {Amb, Amb, Amb, 1.0f};
	float mat_diffuse [] = {CockpitWindowColor.r, CockpitWindowColor.g, CockpitWindowColor.b, 1.0f};
	float mat_specular [] = {Specular, Specular, Specular, 1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	
	glBlendColor (CockpitWindowColor.r, CockpitWindowColor.g, CockpitWindowColor.b, 1.0);
}

//----------------------------------------------

void	ShipArchetype :: SetMaterialsAndColorForShields ()
{
}

//----------------------------------------------

void	ShipArchetype :: SetMaterialsAndColorForExhaust ()
{
	float Amb = 0.0;
	float Specular = 0.0;
	float shininess = 0.0f;
	Vector Color (0.9f, 0.4f, 0.1f);
	
	float emission_mat []  = {1.0f, 0.0f, 0.0f, 1.0f};
	float mat_ambient_color [] = {Amb, Amb, Amb, 1.0f};
	float mat_diffuse [] = {Color.r, Color.g, Color.b, 1.0f};
	float mat_specular [] = {Specular, Specular, Specular, 1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission_mat);
	
	glBlendColor (Color.r, Color.g, Color.b, 1.0);
}

//----------------------------------------------

void	ShipArchetype :: AddQuad (float x0, float y0, float z0, 
								   float x1, float y1, float z1,
								   float x2, float y2, float z2,
								   float x3, float y3, float z3)
{
	Vector vertices[] =
	{
		Vector (x0, y0, z0), 
		Vector (x1, y1, z1), 
		Vector (x2, y2, z2), 
		Vector (x3, y3, z3)
	};
	AddQuad (vertices);
}

void	ShipArchetype :: AddQuad (Vector* corners)// calculates nomals too
{
	// use the cross product of the two vectors fromed by the first three vertices
	Vector vector1 = corners[0] - corners[1];
	Vector vector2 = corners[1] - corners[2];
	Vector Normal = -vector2.NormalizedCross (vector1);
	glBegin (GL_QUADS);		
	glNormal3fv ((float*)&Normal);
		glVertex3f (corners[0].x, corners[0].y, corners[0].z);
		glVertex3f (corners[1].x, corners[1].y, corners[1].z);
		glVertex3f (corners[2].x, corners[2].y, corners[2].z);
		glVertex3f (corners[3].x, corners[3].y, corners[3].z);
	glEnd ();
}

//----------------------------------------------

void	ShipArchetype :: AddTri (float x0, float y0, float z0, 
								  float x1, float y1, float z1,
								  float x2, float y2, float z2)
{
	Vector vertices[] =
	{
		Vector (x0, y0, z0), 
		Vector (x1, y1, z1), 
		Vector (x2, y2, z2)
	};
	AddTri (vertices);
}

void	ShipArchetype :: AddTri (Vector* corners)// calculates nomals too
{
	// use the cross product of the two vectors fromed by the first three vertices
	Vector vector1 = corners[0] - corners[1];
	Vector vector2 = corners[1] - corners[2];
	Vector Normal = -vector2.NormalizedCross (vector1);
	glBegin (GL_TRIANGLES);		
	glNormal3fv ((float*)&Normal);
		glVertex3f (corners[0].x, corners[0].y, corners[0].z);
		glVertex3f (corners[1].x, corners[1].y, corners[1].z);
		glVertex3f (corners[2].x, corners[2].y, corners[2].z);
	glEnd ();
}
