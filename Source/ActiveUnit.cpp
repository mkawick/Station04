/*
 *  ActiveUnit.cpp
 *  Station05
 */
#include "stdafx.h"
#include <windows.h>
#include "ActiveUnit.h"

//-----------------------------------------------
ActiveUnit :: ActiveUnit () : Leader (NULL)
{
	UnitId = UuidGenerate ();
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