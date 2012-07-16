/*
 *  StellarCartography.h
 *  Station05
 *
 */

#pragma once
#include "Vector.h"
#include <list>

class PlayerStatus;
class Asteroid;
// Space is top to bottom 16536 tall.
// across is also 16536 but staggered


//-----------------------------------------------

class StellarHex
{
public:
	StellarHex ();
public:
	Vector			Position;
	PlayerStatus*	CurrentOwner;
};

//-----------------------------------------------

class StellarCartography
{
public:
	enum	Dimensions {Width = 16536, Height = 16536};
	enum	TShape {Up, UpsideDown, LeftPointing, RightPointing};
	
	StellarCartography ();
	
	void	Reinitialize ();
	void	Update ();
	
	void	Draw ();// manage all drawn items not stations or player objects
	
	
protected:
	StellarHex**	Map; // Width x Height
	std::list <Asteroid*>	Asteroids;
};

//-----------------------------------------------

