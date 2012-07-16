/*
 *  StarField.cpp
 *  Station05
 */

#include "stdafx.h"
#include <windows.h>
#define GLUT_BUILDING_LIB
#ifndef __gl_h_
#include <SDL/SDL_opengl.h>
#include "glut.h"
#endif
#include "StarField.h"
#include <math.h>

//---------------------------------------------------------

StarField :: StarField () : DrawList (0xFFFFFFFF)
{
	//int NumStars = NumStars;
	/*Vector colors[] = 
	{
		Vector(0.8, 0, 0.4), Vector (1, 1, 1), Vector (0, 0, 0.8),
		Vector (0, 0.7, 0.8), Vector (0, 0.8, 0.3), Vector (0.8, 0.8, 0),
		Vector (0.9, 1, 0.8), Vector (0, 1, 0), Vector (0, 0, 1),
		Vector (1, 0, 0.8)
	};*/
}

//---------------------------------------------------------

void	StarField :: SetCenter (Vector v)
{
	Center = v;
}

//---------------------------------------------------------

Star		StarField :: CreateStar ()
{
	float x = (rand ()%Range - (Range/2)) *1;
	float y = (rand ()%Range - (Range/2)) *1;
	float z = (rand ()% 200) -300;
	Vector Color; //= colors [rand ()%10];
	Color.r = static_cast <float> (rand () % 100)/200.0F + 0.4F;
	Color.g = static_cast <float> (rand () % 100)/200.0F + 0.4F;
	Color.b = static_cast <float> (rand () % 100)/200.0F + 0.4F;
	
	Star star;
	star.Position.Set (x, y, z);
	star.Color = Color;
	
	return star;
}
//---------------------------------------------------------

void	StarField :: Setup ()
{
	DrawList =  glGenLists(1);
	glNewList (DrawList, GL_COMPILE);
	
	glPushAttrib(GL_POINT);
	float Amb = 0.4;
	float Specular = 0.1;
	float low_shininess = 3.0f;
	

	float mat_ambient_color [] = {Amb, Amb, Amb, 1.0f};
	float mat_diffuse [] = {0.1f, 0.5f, 0.8f, 1.0f};// a little bluish
	float mat_specular [] = {Specular, Specular, Specular, 1.0f};
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, low_shininess);
	
	float PointSizes[2];
	float CurrentPointSize;
	bool IsAntiAliasingOn = glIsEnabled (GL_POINT_SMOOTH) ? true : false;
	glGetFloatv (GL_POINT_SIZE_RANGE, PointSizes);
	glGetFloatv (GL_POINT_SIZE, &CurrentPointSize);
	//glGetBooleanv (GL_POINT_SMOOTH, &IsAntiAliasingOn);
	
	//float Size = 63;
	glPointSize (1.2);
	glEnable (GL_POINT_SMOOTH);
	
	Top = -Range/2, Bottom=Range/2, Left = -Range/2, Right = Range/2;
	//float mat_emissive1 [] = { 1, 1, 1, 1.0f};
	//glMaterialfv (GL_FRONT, GL_EMISSION, mat_emissive1);
	
	//glBlendColor (1, 1, 1, 1.0);
	glBegin (GL_POINTS);
	for (int i=0; i<NumStars; i++)
	{
		Star star = CreateStar ();
		float mat_emissive [] = { star.Color.r, star.Color.g, star.Color.b, 1.0f};
		glMaterialfv (GL_FRONT, GL_EMISSION, mat_emissive);

		//glColor3f (1.0, 1.0, 0.0);
		//glBlendColor (star.Color.r, star.Color.g, star.Color.b, 1.0);
		glVertex3f (star.Position.x, star.Position.y, star.Position.z);// diamond
		
	}
	glEnd ();
	
	if (IsAntiAliasingOn == true)
	{
		glDisable (GL_POINT_SMOOTH);
	}
	glPointSize (CurrentPointSize);
	glPopAttrib ();
	glEndList();
}

//---------------------------------------------------------

void	StarField :: Update ()
{
}


//---------------------------------------------------------

void	StarField :: Draw ()
{
	const int HalfRange = Range/2;
	int Across = Center.x;
	Across %= HalfRange;
	int Vertical = Center.y;
	Vertical %= HalfRange;
	
	glPushMatrix();
		glTranslatef (Across, Vertical, 0);
		glCallList (DrawList);
	glPopMatrix();
	
	if (Across < -HalfRange*.8)// toward the left edge
	{
		glPushMatrix();
			glTranslatef (Across+HalfRange, 0, 0);
			glCallList (DrawList);
		glPopMatrix();
	}
	else if (Across > HalfRange*.8)// toward the right edge
	{
		glPushMatrix();
			glTranslatef (Across-HalfRange, 0, 0);
			glCallList (DrawList);
		glPopMatrix();
	}
	
	/*
	glPushMatrix ();
	glBegin (GL_POINTS);
		float mat_ambient_color [] = {0.4, 0.4, 0.4, 1.0f};
		float mat_diffuse [] = {0.1f, 0.5f, 0.8f, 1.0f};
		float mat_specular [] = {0.1, 0.1, 0.1, 1.0f};
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, 3.0);
		float mat_emissive [] = { 0, 1, 0, 1.0f};
		glMaterialfv (GL_FRONT, GL_EMISSION, mat_emissive);
	
		for (float i=-180; i<180; i++)
		{
			//Star star = CreateStar ();
			float x = ConvertToRadians (i);
			
			glVertex3f (sin (x)*6, cos (x)*6, 0.1);
		}
	glEnd ();
	glPopMatrix ();*/
}

//---------------------------------------------------------
