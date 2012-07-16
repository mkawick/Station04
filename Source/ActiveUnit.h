/*
 *  ActiveUnit.h
 *  Station05
 *
 */

#pragma once
#include "GameData.h"
#include "../common/uuid.h"
#include "StellarObject.h"
#include "ShipArchetype.h"

//------------------------------------------

class ActiveUnit
{
public:
	enum	Controlled {User, Enemy, AI};
	
	ActiveUnit ();
	
	void			SetLeader (ActiveUnit* leader);
	ActiveUnit*		GetLeader () const;
	
	virtual void	Setup () {}
	virtual void	Draw () {}
	virtual void	Update (GameData&) {}
	
	//-------------------------------------------
	void					SetController (Controlled c);
	Controlled				GetControlled () const;
	
	UUID					GetUnitID () const {return UnitID;}
	void					SetName (std::string name);
	const std::string&		GetName () const {return UnitName;}
	
protected:
	Controlled				ControlledBy;
	UUID					UnitID;
	std::string				UnitName;
	
	ShipArchetype*			Ship;
	
	ActiveUnit*				Leader;
};

//------------------------------------------