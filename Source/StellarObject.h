/*
 *  StellarObject.h
 *  Station05
 *
 */

#pragma once
#ifndef __gl_h_
//#define GL_GLEXT_PROTOTYPES
#include "../tools/GL/include/glee.h"
#include "../tools/SDL/include/SDL_opengl.h"
//#import <GLUT/glut.h>
#endif
#include "../common/Math/Vector.h"
#include <list>
#include "WeaponBase.h"

#include "../tools/SpacePartitioning/PartitionObject.h"

class GameData;
//------------------------------------------

class StellarObject
{
public:
	StellarObject ();
	~StellarObject ();
	
	void			SetColor ( const Vector& color ) {Color = color;}
	void			SetCenter ( const Vector& center );
	void			SetVelocity ( const Vector& vel ) {Velocity = vel;}
	void			SetAngle ( const Vector& angle ) {Angle = angle;}
	void			SetAngleOfRotation ( const Vector& rate ) {RotationRate = rate;}
	
	const Vector&	GetCenter () const {return Center;}
	
	//---------------------------------------
	
	virtual void	Setup ();
	virtual void	Draw ();
	virtual void	Update ( GameData& );
	
	//---------------------------------------
	
	void			SetHitPointRange (float min, float max);
	void			SetCurrentHitPoints (float value);
	int				GetHealth () const;// scaled value from 0-100
	void			AddDamageResistance (const WeaponResistance& res);
	virtual void	Damage (const DamageTaken& dam);// this will need to be modified for each object
	virtual bool	IsDestroyed ();// this will need to be modified for each object
	
	//---------------------------------------
	
	virtual bool	DoesIntersect (const Vector& startPosition, const Vector& endPosition) const;
	virtual bool	DoesIntersect (const StellarObject& object) const;
	
	//virtual void	Regenerate (const );;// undefined but some objects can regenerate
	
	//---------------------------------------
	
	virtual void	CalculateMaxAABB(){ Partitioning.x1=Partitioning.x2=Center.x; Partitioning.y1=Partitioning.y2=Center.y;}
protected:
	
	//---------------------------------------
	virtual void	SetupMaterialsAndLighting ();
	//---------------------------------------
	void	AddQuad (float x0, float y0, float z0, 
					  float x1, float y1, float z1,
					  float x2, float y2, float z2,
					  float x3, float y3, float z3);
	void	AddQuad (Vector* corners);
	//---------------------------------------
	void	AddTri (float x0, float y0, float z0, 
					 float x1, float y1, float z1,
					 float x2, float y2, float z2);
	void	AddTri (Vector* corners);
	//---------------------------------------
	Vector	GetNormal (const Vector& v1, const Vector& v2, const Vector& v3);// assume right-hand
	
	std::list <WeaponResistance>::iterator GetResistanceOfType (WeaponDamageType damage);
	//---------------------------------------
	
	Vector							Color;			// r, g, b
	Vector							Center;			// x, y, z
	Vector							Velocity;		// ∆x, ∆y, ∆z
	Vector							Angle;			// theta, phi, psi
	Vector							RotationRate;	// ∆theta, ∆phi, ∆psi
	GLuint							DrawList;		// used for optimization in drawing
	
	//---------------------------------------
	
	float							HitPoints;
	float							HitPointsLow;
	float							HitPointsHigh;
	std::list <DamageTaken>			AccumulatedDamage;
	std::list <WeaponResistance>	Resistances;
	PartitionObject					Partitioning;
};

//-----------------------------------------------

