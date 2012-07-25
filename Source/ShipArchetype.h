/*
 *  ShipArchetype.h
 *  Station05
 */


#pragma once
#include "../common/Math/Vector.h"
#include <list>
#include "GameEvent.h"
#include "StellarObject.h"

//------------------------------------------

class ShipArchetype : public StellarObject
{
public:
	enum	RotationDir {CW, CCW};
	enum    WingTypes {FrontLeft, FrontRight, RearLeft, RearRight};
	enum	EngineTypes {Thruster, Ion, BurntGas, Explosive};
	enum	EngineStates {Off, Engaging, On, ShuttingOff};
	enum	{ NumShields = 2 };
	
	ShipArchetype ();

	virtual const char* GetName() const { return ""; }
	void			SetOwner( UUID id ) { PlayerId = id; }
	UUID			GetOwner() const {return PlayerId; }
	
	int				GetShieldLevel ( int which = -1 ) const;// default sums the shields
	void			SetBodyColor (Vector color);
	void			SetCockpitColors (Vector FrameColor, Vector WindowsColor);
	void			SetNationalColors (Vector* colors, int NumColors);
	void			SetShieldColor (Vector color);
	void			SetCenter (Vector center);
	void			SetAngle (Vector angle);
	
	const Vector&	GetBodyColor () const {return BodyColor;}
	const Vector&	GetCenter () const {return Center;}
	const Vector&	GetAngle () const {return Angle;}
	
	virtual void	Rotate (RotationDir dir) = 0;
	virtual void	ApplyThrust () = 0;// simple for now.
	virtual void	Setup () = 0;
	virtual void	Draw () = 0;
	virtual void	Update (GameData&) = 0;
	virtual void	PostDrawCleanup () = 0;

	void			SetResourceStorage( ResourceTypes type, float quantity ) { ResourceStorage[type] = quantity; }
	float			GetResourceStorage( ResourceTypes type ) const {return ResourceStorage[ type ]; }
	void			AddResourceStorage( ResourceTypes type, float quantity ) { ResourceStorage[type] += quantity; }
	
protected:
	void		SetMaterialsAndColorForShipBody ();
	void		SetMaterialsAndColorForCockpitFrame ();
	void		SetMaterialsAndColorForCockpitWindows ();
	void		SetMaterialsAndColorForShields ();
	void		SetMaterialsAndColorForExhaust ();
	
	void		AddQuad (float x0, float y0, float z0, 
						 float x1, float y1, float z1,
						 float x2, float y2, float z2,
						 float x3, float y3, float z3);
	void		AddQuad (Vector* CornerPoints);// calculates nomals too
	
	void		AddTri (float x0, float y0, float z0, 
						float x1, float y1, float z1,
						float x2, float y2, float z2);
	void		AddTri (Vector* CornerPoints);// calculates nomals too
	
	Vector					BodyColor, CockpitFrameColor, CockpitWindowColor;
	Vector					ShieldColor;
	std::list <Vector>		NationalColors;
	Vector					ExhaustPosition;
	int						ShieldLevel [NumShields];//
	UUID					PlayerId;
	
	bool					IsApplyingThrust;
	Events::GameEventQueue	EventQueue;
	float					ResourceStorage[ ResourceTypes_Count ];
};