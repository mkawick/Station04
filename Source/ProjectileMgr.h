/*
 *  ProjectileMgr.h
 *  Station05
 */

#pragma once
#include <list>
#include "projectile.h"
#include "Gameevent.h"

class ShipArchetype;

//------------------------------------------

class ProjectileMgr : public Events:: MessageSenderReceiver
{
public:
	ProjectileMgr ();
	
	void	Setup ();
	void	Draw ();
	void	Update (GameData& data);
	void	PostDrawCleanup ();
	
	PhaseBolt&	AddBolt (const ShipArchetype& g, UUID& WhoFired);
	
protected:
	void	ProcessMessages (GameData& data);
	
	enum {NumBolts = 200};
	std::list <PhaseBolt> ListOfBolts;
};

//------------------------------------------
