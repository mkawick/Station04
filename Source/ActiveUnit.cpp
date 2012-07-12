/*
 *  ActiveUnit.cpp
 *  SDL04_Station
 */
#include "stdafx.h"
#include <windows.h>
#include "ActiveUnit.h"

//-----------------------------------------------
ActiveUnit :: ActiveUnit () : Leader (NULL)
{
	UnitID = UuidGenerate ();
}

//-----------------------------------------------

void			ActiveUnit :: SetLeader (ActiveUnit* leader)
{
	Leader = leader;
}

//-----------------------------------------------

ActiveUnit*		ActiveUnit :: GetLeader () const
{
	return Leader;
}

//-----------------------------------------------

void			ActiveUnit :: SetName (std::string name)
{
	UnitName = name;
}

//-----------------------------------------------