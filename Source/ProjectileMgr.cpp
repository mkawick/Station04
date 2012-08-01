/*
 *  ProjectileMgr.cpp
 *  Station05
 */

#include "stdafx.h"
#include <windows.h>
#include "../common/Math/vector.h"
#include "gamedata.h"
#include "ProjectileMgr.h"


#include "player.h"
#include "PlayerDatabase.h"
#include "shiparchetype.h"

//---------------------------------------------------------

ProjectileMgr :: ProjectileMgr ()
{
	//ListOfBolts.push_back (PhaseBolt);
}

ProjectileMgr :: ~ProjectileMgr ()
{
	
}
void    ProjectileMgr :: Shutdown()
{
    /*std::list <PhaseBolt> :: iterator it = ListOfBolts.begin ();
	while (it != ListOfBolts.end ())
	{
	}*/

    ListOfBolts.clear();
}

//---------------------------------------------------------

void	ProjectileMgr :: Setup ()
{
	for (int i=0; i<NumBolts; i++)
	{
		PhaseBolt bolt;
		bolt.Setup ();
		ListOfBolts.push_back (bolt);
	}
}

//---------------------------------------------------------

void	ProjectileMgr :: Draw ()
{
	std::list <PhaseBolt> :: iterator it = ListOfBolts.begin ();
	while (it != ListOfBolts.end ())
	{
		(*it).Draw ();
		++it;
	}
}

//---------------------------------------------------------

void	ProjectileMgr :: Update (GameData& GlobalGameData)
{
	int num = ListOfBolts.size();
	std::list <PhaseBolt> :: iterator it = ListOfBolts.begin ();
	while (it != ListOfBolts.end ())
	{
		(*it).Update ();
		++it;
	}
	ProcessMessages (GlobalGameData);
	UpdateAllClients ();
}

//---------------------------------------------------------

void	ProjectileMgr :: PostDrawCleanup ()
{
	std::list <PhaseBolt> :: iterator it = ListOfBolts.begin ();
	while (it != ListOfBolts.end ())
	{
		(*it).PostDrawCleanup ();
		++it;
	}
}

//---------------------------------------------------------

PhaseBolt&	ProjectileMgr :: AddBolt (const ShipArchetype& ship, UUID& WhoFired)
{
	std::list <PhaseBolt> :: iterator it = ListOfBolts.begin ();
	while (it != ListOfBolts.end ())
	{
		PhaseBolt& bolt = *it++;
		if (bolt.GetTimeRemaining () == 0)
		{
			bolt.SetLifeSpan(200);
			bolt.Setup (ship, WhoFired);
			
			return bolt;
		}
	}
	
	return * ListOfBolts.begin ();
}

//---------------------------------------------------------
void			ProjectileMgr :: ProcessMessages (GameData& GlobalGameData)//inherited, send all messages to the appropriate players
{
	PlayerDatabase* playerdb = GlobalGameData.GetPlayerDatabase();
	
	int NumMessages = ReceiveQueue.Count ();
	for (int i=0; i<NumMessages; i++)
	{
		const Events::GameEvent* msg = reinterpret_cast <const Events::GameEvent*> (ReceiveQueue.Dequeue());
		switch (msg->GetType())
		{
			case Events :: FireWeapon:
			{
				const Events::FireWeaponEvent* fw = reinterpret_cast <const Events::FireWeaponEvent*> (msg);
				UUID PlayerID = fw->GetPlayerID ();
				Player* p = playerdb->GetPlayer(PlayerID);
				if (p)
				{
					ShipArchetype* ship = p->GetShip();
					if (ship)
					{
						AddBolt (*ship, PlayerID);
					}
				}
			}
			break;
		}
	}
	
}

//---------------------------------------------------------
//---------------------------------------------------------