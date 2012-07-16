/*
 *  Projectile.h
 *  Station05
 *
 */

#pragma once
#include "../common/Math/Vector.h"
#include "../common/uuid.h"

#include "StellarObject.h"

class ShipArchetype;
class Scorey;

//------------------------------------------
// all projectiles assume that we are in the correct draw mode before they start drawing
//------------------------------------------

class BasicProjectile : public StellarObject
{
public:
	BasicProjectile ();
	void			SetColor (Vector color);
	void			SetCenter (Vector center);
	void			SetAngle (Vector angle);
	void			SetVelocity (Vector v);
	void			SetAcceleration (Vector a);
	void			SetLifeSpan (float life);
	
	const Vector&	GetCenter () const {return Center;}
	const Vector&	GetAngle () const {return Angle;}
	const float		GetProjectileLength () const {return ProjectileLength;}
	const float		GetScaleFactor () const {return ScaleFactor;}
	int				GetTimeRemaining () const {return LifeSpan;}
	
	virtual void	Setup () = 0;
	virtual void	Draw () = 0;
	virtual void	Update () = 0;
	virtual void	PostDrawCleanup () = 0;
	
	//-------------------------------------
	
protected:
	virtual void		SetMaterialsAndColor () = 0;
	
	Vector				Color;
	Vector				Center;
	Vector				Angle;
	Vector				Velocity;
	Vector				Acceleration;
	int					LifeSpan;
	float				ProjectileLength;
	float				ScaleFactor;
	UUID				WhoFiredMe;
	//unsigned int DrawList;// probably not needed


	void	CalculateMaxAABB();
	void	CalculateMaxAABB( float& _x1, float& _y1, float& _x2, float& _y2 );
};


//------------------------------------------

class PhaseBolt : public BasicProjectile
{
public:
	PhaseBolt ();
	
	void	Setup ();
	void	Draw ();
	void	Update ();
	void	PostDrawCleanup ();
	
	void	Setup (const ShipArchetype& g, UUID& WhoFired);
	void	Setup (const Scorey& g, UUID& WhoFired);
	
protected:
	void		SetMaterialsAndColor ();
	void		SetMaterialsAndColorForFuzzyOutline ();
};
		
//------------------------------------------
