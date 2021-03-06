/*
 *  Gazelle.h
 *  Station05
 *
 */

#pragma once
#include "../common/Math/Vector.h"
#include <list>
#include "GameEvent.h"
#include "ShipArchetype.h"

//------------------------------------------
//------------------------------------------

class Gazelle : public ShipArchetype
{
public:
	Gazelle ();
	
	const char*		GetName() const { return "Gazelle"; }
	//---------------------------------------
	
	void			Rotate (RotationDir dir);
	void			SetAngularVelocity (Vector angle);	
	
	void			ApplyThrust ();
	void			Setup ();
	void			Draw ();
	void			Update (GameData&);
	void			PostDrawCleanup ();

	void			CheckForResourceCapture();
	
	//---------------------------------------
	
private:
	void			DrawBody (float Left, float Right, float Top, float Bottom, float WideBody, 
							  float Bevel, float MidBottom, float NearDepth, float FarDepth, float BevelDepth);
	void			DrawWing (WingTypes, float Left, float Right, float Top, float Bottom, 
							  float NearDepth, float FarDepth, float Bevel);
	void			DrawCockpit (float Left, float Right, float Top, float Bottom, 
								float Bevel, float NearDepth);
	void			DrawEngine (float Left, float Right, float Top, float Bottom);
	void			DrawThrust (float MaxLength);
	
	float			BodyWidth;
	float			BodyLength;
	Vector			RotationRate;
	bool			HasWing[4];
	int				ShieldLevel[2];
};

//------------------------------------------