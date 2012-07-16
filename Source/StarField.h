/*
 *  StarField.h
 *  Station05
 */

#pragma once
#include <list>
#include "../common/Math/Vector.h"

struct Star
{
	Vector Position;
	Vector Color;
};

class StarField
{
public:
	StarField ();
	
	void	SetCenter (Vector v);
	void	Setup ();
	void	Update ();
	void	Draw ();
	
protected:
	enum {NumStars = 4500, Range = 10000};
	Vector Center;
	unsigned int DrawList;
	int		Top, Bottom, Left, Right;
	
	
	Star	CreateStar ();
};