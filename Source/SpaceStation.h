/*
 *  SpaceStation.h
 *  Station05
 *
 */

#pragma once

#include "GlobalDefinitions.h"
#include "StellarObject.h"

//------------------------------------------

class SpaceStation : public StellarObject
{
public:
	SpaceStation ();
	
	// note that draw uses a drawlist so we don't need to do any drawing here. See baseclass.
	void			Setup ();
	int				GetShieldLevel () const;// overall 0-100, not individually
	void			SetShieldLevel (int WhichSphere, int level);
	
	//---------------------------------------
	
	float	GetResourcesStored (ResourceTypes type) {return ResourcesStored[static_cast <int> (type)];}
	
protected:
	
	void			CreateSpikeyBall (GLUquadric* quadric, float radius);
	void			CreateCylinder (GLUquadric* quadric, float Radius, float Length);
	void			CreateCentralHumanWindowsLongitudinally (float Radius, float Length);
	
	void			SetMaterialsAndColorForStation ();
	void			SetMaterialsAndColorForShieldGenerators ();
	void			SetMaterialsAndColorForWindows ();
	void			SetMaterialsForShields ();
	void			SetColorForShields (int Level);
	
	void			DrawWindowStrip (float LeftRightPos, float Top, float Bottom, float z); 
	
	void			DrawShields (GLUquadric* quadric, float Radius, float Spread);
	
	//---------------------------------------

	Vector				ShieldColor;
	int					ShieldLevel[2];
	static const float	CylinderMultiplier;
	float				ResourcesStored [ResourceTypes_Count];
	float				ResourceCapacity [ResourceTypes_Count];
};

//------------------------------------------
