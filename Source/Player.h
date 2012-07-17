/*
 *  PlayerStatus.h
 *  Station05
 *
 */

#pragma once
#include <string>
#include <list>
#include "../common/Math/Vector.h"
#include "../common/uuid.h"
#include "ActiveUnit.h"

class ShipArchetype;
class SpaceStation;
class GameData;

//-----------------------------------------------

class PlayerHistory
{
	std::list <UUID>	Kills; // there will be dups for the same player killed twice
	int					NumberOfKills;
	int					ResourcesMined;// probably needs to be tracked by type
};

//-----------------------------------------------

// the Player is a conglomeration of data and stats.
// remember to create a non-player.
class Player : public ActiveUnit
{
public:
	Player ();
	~Player ();
	
	void	Update (GameData& data);
	void	Draw ();
	void	PostDrawCleanup ();
	
	//-------------------------------------------
	
	void			Rotate (ShipArchetype :: RotationDir dir);
	void			ApplyThrust ();// simple for now.
	
	
	// we may want to add configuration params here
	void					AddStation (SpaceStation* station);
	SpaceStation*			GetStation (int index);
	int						GetNumStations () const;
	
	// the PlayerDB will move the ship based on input.
	void					SetShip (ShipArchetype* ship);
	ShipArchetype*			GetShip () const;
	//void					
	
	Vector					GetViewFocus () const;
	void					SetFocus (int StationIndex);
	void					SetViewFocusToShip ();
	float					AggregrateResource (ResourceTypes type) const;// look at all stations and pull the total of the accumulated resources
	
	//-------------------------------------------
	
protected:
	std::list<SpaceStation*>::iterator	FindStation (SpaceStation*);
	
	//-------------------------------------------
protected:
	
	std::list<SpaceStation*>Stations;
	Vector					CenterBase;
	ShipArchetype*			Ship;
	
	bool					IsViewFollowingShip;
	int						StationViewFocus;// which station is the most recently selected
};

//-----------------------------------------------