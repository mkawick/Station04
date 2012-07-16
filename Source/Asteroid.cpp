/*
 *  Asteroid.cpp
 *  Station05
 */

#include "stdafx.h"
#include <windows.h>
#include "Asteroid.h"
#ifndef __gl_h_
#import <Opengl/gl.h>
#import <GLUT/glut.h>
#endif

#include "GlobalDefinitions.h"
#include "GameEvent.h"
#include "GameFramework.h"

const float x = 0.525731112119133606F;
const float z = 0.850650808352039932F;
const int	NumVertices = 20;

static float vdata[12][3] = 
{
	{-x, 0,  z},
	{ x, 0,  z},
	{-x, 0, -z},
	{ x, 0, -z},
	{0,  z,  x},
	{0,  z, -x},
	{0, -z,  x},
	{0, -z, -x},
	{ z,  x, 0},
	{-z,  x, 0},
	{ z, -x, 0},
	{-z, -x, 0}
};

static GLint tindices [NumVertices][3] = 
{
	{1, 4, 0},	{4, 9, 0},	{4, 5, 9},	{8, 5, 4},	{1, 8, 4},
	{1, 10, 8}, {10, 3, 8}, {8, 3, 5},	{3, 2, 5},	{3, 7, 2},
	{3, 10, 7}, {10, 6, 7}, {6, 11, 7}, {6, 0, 11},	{6, 1, 0},
	{10, 1, 6}, {11, 0, 9}, {2, 11, 9}, {5, 2, 9},	{11, 2, 7}
};

//---------------------------------------------------------

Asteroid :: Asteroid () : SizeModifiers (1, 1, 1), size(1), shape(Smoothe), StellarObject()
{
	Reset (size, shape);
	Partitioning.collisionFlag = CollisionFlags_Mining;
}

void	Asteroid ::CalculateMaxAABB()
{
	Partitioning.x1 = Center.x - size;
	Partitioning.x2 = Center.x + size; 
	Partitioning.y1 = Center.y - size;
	Partitioning.y2 = Center.y + size;
}
//---------------------------------------------------------

Asteroid :: Asteroid (float _size, Shape _shape) : SizeModifiers (1, 1, 1), size(_size), shape(_shape), StellarObject()
{
	Reset (size, shape);
	Partitioning.collisionFlag = CollisionFlags_Asteroid;
}

//---------------------------------------------------------

void	Asteroid :: Reset (float size, Shape shape)
{
	if (DrawList != 0xFFFFFFFF)
	{
		glDeleteLists (DrawList, 1);
	}
	DrawList =  glGenLists(1);
	
		float Variance = 0.0;
		float ChanceOfVariance = 1.0;
		
		Points.clear ();
		
		switch (shape)// remember that the value is multiplied by roughly 0.5
		{
			case Smoothe:
				Variance = 0.01F; ChanceOfVariance = 1.0F;
				break;
			case Spikey:
				Variance = 2.0F; ChanceOfVariance = 1.0F;
				break;
			case HalfAndHalf:
				Variance = 0.09F; ChanceOfVariance = 0.5F;
				break;
			case MostlySmoothe:
				Variance = 0.01F; ChanceOfVariance = 0.2F;
				break;
			case MostlySpikey:
				Variance = 0.2F; ChanceOfVariance = 0.2F;
				break;
		}
		
		// randomize the points, but only a little.
		for (int i=0; i<NumVertices; i++)
		{
			Vector v (vdata[i][0]*SizeModifiers.x, vdata[i][1]*SizeModifiers.y, vdata[i][2]*SizeModifiers.z);		
			float IsVariedChance = static_cast <float> (rand () % 1000); 
			
			if (IsVariedChance*0.001f < ChanceOfVariance)// div by 1000
			{
				float Multiplier = static_cast <float> (rand ()% 100) -50;
				Multiplier *= 0.01F*Variance;
				v *= (size + Multiplier);
			}
			else
				v *= size;
			
			Points.push_back (v);
		}
	
	// Now create the OpenGL object
	glNewList (DrawList, GL_COMPILE);
	
		SetupMaterialsAndLighting ();
		
		// now we go through and add all triangles and normals.
		for (int i=0; i<NumVertices; i++)
		{
			GLint* index = tindices[i];
			int Vert1 = index[0];
			int Vert2 = index[1];
			int Vert3 = index[2];
			
			Vector v1 = Points[Vert1];
			Vector v2 = Points[Vert2];
			Vector v3 = Points[Vert3];
			AddTri (v1.x, v1.y, v1.z,
					v2.x, v2.y, v2.z,
					v3.x, v3.y, v3.z);
		}
		
	// lastly, we add these to our own drawlist.
	glEndList();
	
	// since we no longer need the points except for intersection testing
}


//---------------------------------------------------------

void	Asteroid :: RandomizeContent ()
{
}

//---------------------------------------------------------

bool	Asteroid :: MineResource( ResourceTypes attemptedType )
{
	switch( attemptedType )
	{
		case ResourceType_Iron:
		case ResourceType_DropiumCrystal:
		case ResourceType_Hydrogen:
		case ResourceType_Aluminium:
		case ResourceType_Copper:
			{
				Events::CreateResourceNodeEvent ResourceEvent( GetCenter(), ResourceType_DropiumCrystal, 3.0f );
				ResourceEvent.SetAsteroidIdFromWhichItCame( GetID() );
				GlobalGameFramework->SendMessages (ResourceEvent);
				return true;
			}
			break;
	}
	return false;
}

//---------------------------------------------------------

//---------------------------------------------------------
