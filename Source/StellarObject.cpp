/*
 *  StellarObject.cpp
 *  Station05
 */
#include "stdafx.h"
#include <windows.h>
#include <assert.h>
#include "StellarObject.h"
#include "GameData.h"
//#include <octree.h>

#include "../Source/GameFramework.h"
//
//----------------------------------------------
//----------------------------------------------

StellarObject :: StellarObject () : Color (0.5, 0.5, 0.5),
									Center (0, 0, 0),
									Velocity (0, 0, 0),
									Angle (0, 0, 0),
									RotationRate (0, 0, 0),
									DrawList (0xFFFFFFFF),
									HitPointsLow (0),
									HitPointsHigh (100),
									HitPoints (HitPointsHigh)
{
}

StellarObject :: ~StellarObject ()
{
	GlobalGameFramework->GetSpacePartition().Remove( &Partitioning );
}

//----------------------------------------------


void	StellarObject :: Setup ()
{
	Partitioning.SetObject( this );
	CalculateMaxAABB();
	GlobalGameFramework->GetSpacePartition().AddPartitionObject( &Partitioning );
}

void	StellarObject :: SetCenter ( const Vector& center) 
{
	Center = center;

	GlobalGameFramework->GetSpacePartition().UpdateObject( &Partitioning );
}

//----------------------------------------------

void	StellarObject :: Draw ()
{
	if (DrawList != 0xFFFFFFFF)
	{
		glPushMatrix();
			//glScalef (Scale, Scale, Scale);	// not currently needed
		
			glTranslatef (Center.x, Center.y, Center.z);
			
			glRotatef (Angle.x, 1, 0, 0);
			glRotatef (Angle.y, 0, 1, 0);
			glRotatef (Angle.z, 0, 0, 1);		
			
			glCallList (DrawList);
		
		glPopMatrix();
	}
}

//----------------------------------------------

void	StellarObject :: Draw ( float positionx, float positiony, float positionz )
{
	if (DrawList != 0xFFFFFFFF)
	{
		glPushMatrix();
			//glScalef (Scale, Scale, Scale);	// not currently needed
		
			glTranslatef ( positionx, positiony, positionz);
			
			glRotatef (Angle.x, 1, 0, 0);
			glRotatef (Angle.y, 0, 1, 0);
			glRotatef (Angle.z, 0, 0, 1);
			
			glCallList (DrawList);
		
		glPopMatrix();
	}
}
//----------------------------------------------

void	StellarObject :: Update (GameData& data)
{
	if( Velocity.SquareMagnitude() > 0 )
	{
		GlobalGameFramework->GetSpacePartition().Remove( &Partitioning );
		Center += Velocity;

		CalculateMaxAABB();
		GlobalGameFramework->GetSpacePartition().AddPartitionObject( &Partitioning );
	}
	Angle += RotationRate;
	
	// this might be better handled by the weapon class (weaponbase.h)
	
	// let's apply all damage taken.
	std::list <DamageTaken>::iterator it = AccumulatedDamage.begin ();
	while (it != AccumulatedDamage.end ())
	{
		//Resistances[1];
		// for now, simple application of all damage.
		std::list <WeaponResistance>::iterator resistance = GetResistanceOfType ((*it).Damage);
		float	DamageToTake = ((*it).Points);
		
		if (resistance == Resistances.end ())// no resistances
		{
			
		}
		else// apply resistances
		{
			switch (resistance->Resistance)
			{
				case BlocksPercentage:
					DamageToTake *= 1.0f - resistance->Value; // 0-100%
					break;
				case BlocksFirstAmount:
					DamageToTake -= resistance->Value;
					break;
				case BlocksLastAmount:
					assert (0);// not implemented
					break;
			}
			
		}
		
		HitPoints -= DamageToTake;
		it ++;
	}
	if (HitPoints < HitPointsLow)// clamp it assuming that we are always going negative.
	{
		HitPoints = HitPointsLow;
	}
	AccumulatedDamage.clear ();
}

//----------------------------------------------

void	StellarObject :: SetHitPointRange (float min, float max)
{
	HitPointsLow = min;
	HitPointsHigh = max;
	if (HitPoints < HitPointsLow)
		HitPoints = HitPointsLow;
	if (HitPoints > HitPointsHigh)
		HitPoints = HitPointsHigh;
}

//----------------------------------------------

void	StellarObject :: SetCurrentHitPoints (float value)
{
	HitPoints = value;
}

//----------------------------------------------

int		StellarObject :: GetHealth () const// scaled value from 0-100
{
	float Range = HitPointsHigh-HitPointsLow;
	float Percentage = HitPoints/Range;
	
	return static_cast <int> (Percentage*100.0);
}

//----------------------------------------------

void	StellarObject :: AddDamageResistance (const WeaponResistance& newResistance)
{
	WeaponDamageType ResistanceType = newResistance.Damage;
	std::list <WeaponResistance>::iterator resistance = GetResistanceOfType (ResistanceType);
	if (resistance == Resistances.end ())// doesn't already exist
	{
		Resistances.push_back (newResistance);
	}
	
	else
	{
		assert (newResistance.Damage == (*resistance).Damage);// they must be the same type
		(*resistance).Value = newResistance.Value;// just update to the new value
	}
}

//----------------------------------------------

void	StellarObject :: Damage (const DamageTaken& dam)// this will need to be modified for each object
{
	AccumulatedDamage.push_back (dam);
}

//----------------------------------------------

bool	StellarObject :: IsDestroyed ()// this will need to be modified for each object
{
	return false;
}

//----------------------------------------------

bool	StellarObject :: DoesIntersect (const Vector& startPosition, const Vector& endPosition) const
{
	return false;
}

//----------------------------------------------

bool	StellarObject :: DoesIntersect (const StellarObject& object) const
{
	return false;
}

//----------------------------------------------
//virtual void	Regenerate (const 

void	StellarObject :: SetupMaterialsAndLighting ()
{
	float Amb = 0.4f;
	float Specular = 0.6f;
	float shininess = 5.0f;
	
	float no_mat []  = {0.0f, 0.0f, 0.0f, 1.0f};
	float mat_ambient_color [] = {Amb, Amb, Amb, 1.0f};
	float mat_diffuse [] = {Color.r, Color.g, Color.b, 1.0f};
	float mat_specular [] = {Specular, Specular, Specular, 1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	
	glBlendColor (Color.r, Color.g, Color.b, 1.0);
}

//----------------------------------------------
//----------------------------------------------

void	StellarObject :: AddQuad (float x0, float y0, float z0, 
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

void	StellarObject :: AddQuad (Vector* corners)// calculates normals too
{
	Vector Normal = GetNormal (corners[0], corners[1], corners[2]);
	glBegin (GL_QUADS);		
		glNormal3fv ((float*)&Normal);
		glVertex3f (corners[0].x, corners[0].y, corners[0].z);
		glVertex3f (corners[1].x, corners[1].y, corners[1].z);
		glVertex3f (corners[2].x, corners[2].y, corners[2].z);
		glVertex3f (corners[3].x, corners[3].y, corners[3].z);
	glEnd ();
}

//------------------------------------------

void	StellarObject :: AddTri (float x0, float y0, float z0, 
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

void	StellarObject ::AddTri (Vector* corners)
{
	Vector Normal = GetNormal (corners[0], corners[1], corners[2]);
	glBegin (GL_TRIANGLES);		
		glNormal3fv ((float*)&Normal);
		glVertex3f (corners[0].x, corners[0].y, corners[0].z);
		glVertex3f (corners[1].x, corners[1].y, corners[1].z);
		glVertex3f (corners[2].x, corners[2].y, corners[2].z);
	glEnd ();
}

//----------------------------------------------
//----------------------------------------------

Vector	StellarObject :: GetNormal (const Vector& v1, const Vector& v2, const Vector& v3)// assume right-hand
{
	// use the cross product of the two vectors formed by the first three vertices
	Vector vector1 = v1 - v2;
	Vector vector2 = v2 - v3;
	return Vector (-vector2.NormalizedCross (vector1));
}

//----------------------------------------------

std::list <WeaponResistance>::iterator	StellarObject :: GetResistanceOfType (WeaponDamageType damage)
{
	std::list <WeaponResistance>::iterator it = Resistances.begin ();
	while (it != Resistances.end ())
	{
		if ((*it).Damage == damage)
		{
			return it;
		}
		it ++;
	}
	return Resistances.end ();
}

//----------------------------------------------
//----------------------------------------------

