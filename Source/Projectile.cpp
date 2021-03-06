/*
 *  Projectile.cpp
 *  Station05
 */

#include "stdafx.h"
#include <windows.h>
#ifndef __gl_h_
#include "../tools/GL/include/glee.h"
#include "../tools/SDL/include/SDL_opengl.h"
#include "../tools/GL/include/glut.h"
#endif

#include "GameFramework.h"
#include "Projectile.h"
//#include "Gazelle.h"
#include "scorey.h"

//------------------------------------------

BasicProjectile :: BasicProjectile () : LifeSpan (0), ProjectileLength(40), ScaleFactor(1)
{
	Partitioning.collisionFlag = CollisionFlags_None;
}

//------------------------------------------

void			BasicProjectile :: SetColor (Vector color)
{
	Color = color;
}

//------------------------------------------

void			BasicProjectile :: SetCenter (Vector center)
{
	Center = center;

	float length = ProjectileLength * ScaleFactor * 0.5f;
	Partitioning.x1 = center.x - length;// a simple rect enclosing the largest possible area is sufficient for a first-pass test.
	Partitioning.y1 = center.y - length;
	Partitioning.x2 = center.x + length;
	Partitioning.y2 = center.y + length;
}

//------------------------------------------

void			BasicProjectile :: SetAngle (Vector angle)
{
	Angle = angle;
}

//------------------------------------------

void			BasicProjectile :: SetVelocity (Vector v)
{
	Velocity = v;
}

//------------------------------------------

void			BasicProjectile :: SetAcceleration (Vector a)
{
	Acceleration = a;
}

//------------------------------------------

void			BasicProjectile :: SetLifeSpan (float life)
{
	LifeSpan = (int) life;
}

void	BasicProjectile :: CalculateMaxAABB()
{
	float actualLength = ProjectileLength * 0.5f * ScaleFactor;

	Partitioning.x1 = Center.x - actualLength;
	Partitioning.y1 = Center.y - actualLength;
	Partitioning.x2 = Center.x + actualLength;
	Partitioning.y2 = Center.y + actualLength;
}

void	BasicProjectile :: CalculateMaxAABB( float& _x1, float& _y1, float& _x2, float& _y2 )
{
	float actualLength = ProjectileLength * 0.5f * ScaleFactor;

	//cos( -Angle.z );
	//float 
	// Angle.z
	//Center.x, Center.y
	_x1 = Center.x - actualLength;
	_y1 = Center.y - actualLength;
	_x2 = Center.x + actualLength;
	_y2 = Center.y + actualLength;
}
//------------------------------------------

PhaseBolt :: PhaseBolt () : BasicProjectile ()
{
	ProjectileLength = 20.0f;
	ScaleFactor = 0.12f;
	Color.Set (0.9f, 0.2f, 0);
	Partitioning.collisionFlag = CollisionFlags_Phaser;
}

//------------------------------------------

void	PhaseBolt :: Setup ()
{
}


//------------------------------------------

void	PhaseBolt :: Draw ()
{
	if (LifeSpan > 0)
	{
		//
		SetMaterialsAndColor ();
		Vector BoltVector = Center;
		float Left = -1, Right = 1;
		float Top = -ProjectileLength*0.5f, Bottom = ProjectileLength*0.5f;
		
		glPushMatrix();
		
			glTranslatef (Center.x, Center.y, Center.z);
			glScalef (ScaleFactor, ScaleFactor, ScaleFactor);
			glRotatef (-Angle.z, 0, 0, 1);
		
			glBegin (GL_QUADS);	/// cw
				glVertex3f (Left, Top, 0);
				glVertex3f (Right, Top, 0);
				glVertex3f (Right, Bottom, 0);
				glVertex3f (Left, Bottom, 0);			
			glEnd ();
		
			glEnable (GL_BLEND);
			SetMaterialsAndColorForFuzzyOutline ();
			glBegin (GL_QUADS);	/// cw
				glVertex3f (Left-1, Top-1, 0);
				glVertex3f (Right+1, Top-1, 0);
				glVertex3f (Right+1, Bottom+1, 0);
				glVertex3f (Left-1, Bottom+1, 0);			
			glEnd ();
			glDisable (GL_BLEND);
		
		glPopMatrix();
		//

		//Partitioning.x1
	}
}

//------------------------------------------

void	PhaseBolt :: Update ()
{
	// phase bolts always move, so update is always necessary.
	if (LifeSpan > 0)
	{
		Center += Velocity;
		Velocity += Acceleration;
		
		LifeSpan --;

		//CalculateMaxAABB();

		list<PartitionObject*> listOfPossibleCollisionObjects;
		
		//GlobalGameFramework->GetSpacePartition().GetObjectsAt( listOfPossibleCollisionObjects, Center.x, Center.y, 2.0f, CollisionFlags_Mining );
		GlobalGameFramework->GetSpacePartition().GetObjectsAtMin( listOfPossibleCollisionObjects, Center.x, Center.y, CollisionFlags_Mining );

		wchar_t string[256];
		wsprintf( string, L"Num partition objects = %d\n", listOfPossibleCollisionObjects.size() );
		OutputDebugString( string );

		list<PartitionObject*>::iterator it = listOfPossibleCollisionObjects.begin();
		if( it != listOfPossibleCollisionObjects.end() )
		{
			float actualLength = ProjectileLength * 0.5f * ScaleFactor;
			float fudgeFactor = 1.2f;// given the way that the missiles travel, we need a bit of overlap
			// on the interection tests.
			float lenSquared = actualLength * actualLength * fudgeFactor;
			float cx = Center.x, cy = Center.y;

			while( it != listOfPossibleCollisionObjects.end() )
			{
				PartitionObject* obj = *it++;

				float radiusSquared = ABS( obj->cx() - obj->x1 );
				radiusSquared *= radiusSquared;
				
				float dist = DIST_SQUARED( obj->cx(), obj->cy(), cx, cy );
				if( dist - ( radiusSquared + lenSquared ) < 0 )
				{
					// intersection probably. Tell the Asteroid to do something.
					if( obj->obj )
					{
						Asteroid* pAsteroid = reinterpret_cast<Asteroid*>( obj->obj );
						if( pAsteroid )// this must be improved.
						{
							int whichResource = rand() % (ResourceTypes_Count - 1) + 1;
							if( pAsteroid->MineResource( static_cast<ResourceTypes> (whichResource) ) )
							{
								//pAsteroid->Remove();
							}
						}
					}
				}
			}
		}
	}
}

//------------------------------------------

void	PhaseBolt :: PostDrawCleanup ()
{
}

//------------------------------------------

void	PhaseBolt :: Setup (const ShipArchetype& gazelle, UUID& WhoFired)
{
	Vector v = gazelle.GetCenter();
	Vector a, b;
	b.ConvertAngleInDegreesToXYComponents (gazelle.GetAngle ().z);
	a = gazelle.GetAngle ();
	
	float Length = GetProjectileLength () * GetScaleFactor ();
	// fudge factor pushes the bolt away from the nose a bit
	Vector StartPosition = v + b * Length * 1.2f; 
	SetCenter (StartPosition);
	SetAngle (a);
	//SetVelocity (Vector (b.AsNormal () * 3.4f));
	SetVelocity (Vector (b.AsNormal () * 0.4f));
	WhoFiredMe = WhoFired;
}

//------------------------------------------

void	PhaseBolt :: Setup (const Scorey& scorey, UUID& WhoFired)
{
	Vector v = scorey.GetCenter();
	Vector a, b;
	b.ConvertAngleInDegreesToXYComponents (scorey.GetAngle ().z);
	a = scorey.GetAngle ();
	
	float Length = GetProjectileLength () * GetScaleFactor ();
	// fudge factor pushes the bolt away from the nose a bit
	Vector StartPosition = v + b * Length * 1.2f; 
	SetCenter (StartPosition);
	SetAngle (a);
	//SetVelocity (Vector (b.AsNormal () * 3.4f ));
	SetVelocity (Vector (b.AsNormal () * 0.4f));
	WhoFiredMe = WhoFired;
}

//------------------------------------------

void	PhaseBolt :: SetMaterialsAndColor ()
{
	float Amb = 0.0f;
	float Specular = 0.0f;
	float Shininess = 0.0f;
	float Diffuse = 0.0f;
	
	float emission_mat []  = {Color.r, Color.g, Color.b, 1.0f};
	float ambient_color_mat [] = {Amb, Amb, Amb, 1.0f};
	float diffuse_mat [] = {Diffuse, Diffuse, Diffuse, 1.0f};
	float specular_mat [] = {Specular, Specular, Specular, 1.0f};
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_color_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_mat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_mat);
	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission_mat);
	
	glBlendColor (Color.r, Color.g, Color.b, 1.0f);
}

//------------------------------------------

void		PhaseBolt :: SetMaterialsAndColorForFuzzyOutline ()
{
	float Amb = 0.0;
	float Specular = 0.0;
	float Shininess = 0.0f;
	float Diffuse = 0.0f;
	
	float emission_mat []  = {Color.r, Color.g, Color.b, 1.0f};
	float ambient_color_mat [] = {Amb, Amb, Amb, 1.0f};
	float diffuse_mat [] = {Diffuse, Diffuse, Diffuse, 1.0f};
	float specular_mat [] = {Specular, Specular, Specular, 1.0f};
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_color_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_mat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_mat);
	glMaterialf(GL_FRONT, GL_SHININESS, Shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission_mat);
	
	glBlendColor (Color.r, Color.g, Color.b, 0.3f );
}

//------------------------------------------
//------------------------------------------


