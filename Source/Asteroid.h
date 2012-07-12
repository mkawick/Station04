/*
 *  Asteroid.h
 *  SDL04_Station
 */

#pragma once
#include <vector>
#include <list>
#include "StellarObject.h"
#include "GlobalDefinitions.h"
#include "StellarResource.h"

//-----------------------------------------------

class Asteroid : public StellarObject
{
public:
	enum	Shape {Smoothe, Spikey, HalfAndHalf, MostlySmoothe, MostlySpikey};
	
	Asteroid ();
	Asteroid (float size, Shape shape);
	
	void	Reset (float size, Shape shape);
	
	//-------------------------------------------------
	void	SetID (int ID) {id = ID;}
	int		GetID () const {return id;}
	
	void	BreakIntoSmallerAsteroids (std::list <Asteroid>& BabyAsteroids, // just drifting in space
									   std::list <StellarResource>& FreeFloatingResources);	

	bool	MineResource( ResourceTypes attemptedType );// reduces content and generates small quantities of a resource based on availability.

	void	CalculateMaxAABB();
protected:
	void	RandomizeContent ();
	
protected:
	std::vector <Vector>	Points;
	int						id;
	Vector					SizeModifiers;// just stretching along a given axis
	float					ResourceQuantities[ ResourceTypes_Count ];
	float					size;
	Shape					shape;
};

//-----------------------------------------------
