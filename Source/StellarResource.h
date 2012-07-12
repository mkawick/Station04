/*
 *  StellarResource.h
 *  SDL04_Station
 */
#pragma once
#include "GlobalDefinitions.h"
#include "StellarObject.h"

struct AssetObject;

class StellarResource : public StellarObject
{
public:
	StellarResource ();
	
	//void	Update () {}// these are currently undefined
	void	Draw ();
	
	void			SetResource (ResourceTypes type) {Resource = type;}
	ResourceTypes	GetResource () const {return Resource;}
	
	void			SetQuantity (float q) {Quantity = q;}
	float			GetQuantity () const {return Quantity;}

	void			SetAsset( AssetObject* asset ) { Asset = asset; }
	
protected:
	ResourceTypes	Resource;
	float			Quantity;
	AssetObject*	Asset;
};

//------------------------------------------