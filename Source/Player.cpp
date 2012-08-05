/*
 *  Player.cpp
 *  Station05
 *
 */

#include "stdafx.h"
#include <windows.h>
#include <assert.h>
#include "Player.h"
#include "spacestation.h"
#include "gazelle.h"
#include "../common/Math/Vector.h"
#include "GameData.h"

//-----------------------------------------------

Player :: Player () : ActiveUnit (), Ship (NULL), StationViewFocus (0)
{
}

//-----------------------------------------------

Player :: ~Player ()
{
	if (Ship)
	{
		delete Ship;
	}
	std::list<SpaceStation*>:: iterator it = Stations.begin ();
	int Count = 0;
	while (it != Stations.end ())
	{
		delete *it;
		it++;
	}
}

//-----------------------------------------------
//-----------------------------------------------

void	Player :: Update (GameData& data)
{
	if (Ship)
	{
		Ship->Update (data);
	}
	std::list<SpaceStation*>:: iterator it = Stations.begin ();
	while (it != Stations.end ())
	{
		(*it)->Update (data);
		it++;
	}
}

//-----------------------------------------------

void	Player :: Draw ()
{
	std::list<SpaceStation*>:: iterator it = Stations.begin ();
	while (it != Stations.end ())
	{
		(*it)->Draw ();
		it++;
	}
	// I'm not completely sure why but drawing the shields interferes with the other layers,
	// but the ship seems to correct that.
	if (Ship)
	{
		Ship->Draw ();
	}
	
}

//-----------------------------------------------

void	Player :: PostDrawCleanup ()
{
	std::list<SpaceStation*>:: iterator it = Stations.begin ();
	while (it != Stations.end ())
	{
		(*it)->PostDrawCleanup ();
		it++;
	}
	if (Ship)
	{
		Ship->PostDrawCleanup ();
	}
}

void	Player :: Rotate ( ShipArchetype::RotationDir dir)
{
	if( IsViewFollowingShip )
	{
		Ship->Rotate( dir );
	}
	else
	{
		SpaceStation* station = GetStation (StationViewFocus);
		station->Rotate( dir );
	}
}

void	Player :: ApplyThrust ()// simple for now.
{
	if( IsViewFollowingShip )
	{
		Ship->ApplyThrust();
	}
	else
	{
		SpaceStation* station = GetStation (StationViewFocus);
		station->ApplyThrust();
	}
}


//-----------------------------------------------
//-----------------------------------------------

// we may want to add configuration params here
//void					AddStation (Vector location);
void					Player :: AddStation (SpaceStation* station)
{
	Stations.push_back (station);
	Stations.unique ();// ensure no duplicates.
}

//-----------------------------------------------

SpaceStation*				Player :: GetStation (int index)
{
	assert (index < static_cast<int> (Stations.size ()));
	std::list<SpaceStation*>:: iterator it = Stations.begin ();
	int Count = 0;
	while (it != Stations.end ())
	{
		if (index == Count)
			return *it;
		Count++;
		it++;
	}
	assert (0);
	return NULL;
}

//-----------------------------------------------

int						Player :: GetNumStations () const
{
	return Stations.size ();
}

//-----------------------------------------------
//-----------------------------------------------

// the PlayerDB will move the ship based on input.
void					Player :: SetShip (ShipArchetype* ship)
{
	Ship = ship;
}

//-----------------------------------------------

ShipArchetype*			Player :: GetShip () const
{
	return Ship;
}

//-----------------------------------------------

Vector					Player :: GetViewFocus () const
{
	if (IsViewFollowingShip == true)
	{
		assert (Ship != NULL);
		return Ship->GetCenter ();
	}
	const SpaceStation* station = const_cast <Player*> (this)->GetStation (StationViewFocus);
	return station->GetCenter ();
}

//-----------------------------------------------

void					Player :: SetFocus (int StationIndex)
{
	assert (StationIndex < static_cast<int> (Stations.size ()));
	IsViewFollowingShip = false;
	StationViewFocus = StationIndex;
}

//-----------------------------------------------

void					Player :: SetViewFocusToShip ()
{
	IsViewFollowingShip = true;
}

//-----------------------------------------------

float					Player :: AggregrateResource ( ResourceTypes type) const
{
	float total = 0;
	std::list<SpaceStation*>:: const_iterator it = Stations.begin ();
	while (it != Stations.end ())
	{
		SpaceStation* station = *it++;
		total += station->GetResourcesStored (type);
		//total += resource.GetQuantity ();
	}
	if( Ship )
	{
		total += Ship->GetResourceStorage( type );
	}
	return total;
}

void					Player :: AddResources( ResourceTypes type, float amount )
{
	if( Ship )
	{
		Ship->AddResourceStorage( type, amount );
	}
}

//-----------------------------------------------
std::list<SpaceStation*>::iterator	Player :: FindStation (SpaceStation*s)
{
	std::list<SpaceStation*>:: iterator it = Stations.begin ();
	while (it != Stations.end ())
	{
		if (s == *it)
			return it;
		it++;
	}
	
	return Stations.end ();
}

//-----------------------------------------------
