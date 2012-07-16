/*
 *  PlayerDatabase.cpp
 *  Station05
 */

#include "stdafx.h"
#include <windows.h>
#include "PlayerDatabase.h"
#include "SpaceStation.h"
#include "gazelle.h"
#include "scorey.h"
#include "../common/Math/MathFunctions.h"
#include <assert.h>

//Scorey	testShip;
//-------------------------------------------
//-------------------------------------------

PlayerDatabase :: PlayerDatabase ()
{
	CurrentPlayer = PlayerList.end ();
	
}

//-------------------------------------------

void			PlayerDatabase ::SetScreenDimensions (float l, float t, float r, float b)
{
	StatusBars.SetScreenDimensions (l, t, r, b);
}

//-------------------------------------------

void			PlayerDatabase ::SetPositionForPlayerStatusBars (float l, float t, float r, float b)
{
	StatusBars.SetScreenPosition (l, t, r, b);
}

//-------------------------------------------

void			PlayerDatabase :: ClearAllPlayers ()// clear the db of all players
{
	PlayerList.clear ();
	CurrentPlayer = PlayerList.end();
}

//-------------------------------------------

void			PlayerDatabase :: Update (GameData& GlobalGameData)// call update on all players and update player stats
{
	std::list <Player*>::iterator it = PlayerList.begin ();
	while (it != PlayerList.end ())
	{
		Player* player = *it++;
		player->Update (GlobalGameData);
	}
	
	//testShip.Update (GlobalGameData);
	
	ProcessMessages (GlobalGameData);
	UpdateAllClients ();
	
	StatusBars.Update (GlobalGameData);
}

//-------------------------------------------

void			PlayerDatabase :: Draw ()// make all players draw themselves.
{
	std::list <Player*>::iterator it = PlayerList.begin ();
	while (it != PlayerList.end ())
	{
		Player* player = *it++;
		player->Draw ();
	}
	
	//testShip.Draw ();
	
	StatusBars.Draw ();
}

//-------------------------------------------

void			PlayerDatabase :: PostDrawCleanup ()
{
	std::list <Player*>::iterator it = PlayerList.begin ();
	while (it != PlayerList.end ())
	{
		Player* player = *it++;
		player->PostDrawCleanup ();
	}
	
	//testShip.PostDrawCleanup();
	
	StatusBars.PostDrawCleanup();
}

//-------------------------------------------
//-------------------------------------------

void			PlayerDatabase :: ProcessMessages (GameData& GlobalGameData)//inherited, send all messages to the appropriate players
{
	Player* player = *CurrentPlayer;
	ShipArchetype* Ship = NULL;
	if (player)
	{
		Ship = player->GetShip ();
	}

	int NumMessages = ReceiveQueue.Count ();
	for (int i=0; i<NumMessages; i++)
	{
		const Events::GameEvent* msg = reinterpret_cast <const Events::GameEvent*> (ReceiveQueue.Dequeue());
		switch (msg->GetType())
		{
			case Events :: ApplyThrust:
				Ship->ApplyThrust ();
				break;
			case Events :: GameModeChange:
				break;
			case Events :: Maneuvers:
			{
				const Events::ManeuverEvent* me = reinterpret_cast <const Events::ManeuverEvent*> (msg);
				Events::ManeuverEvent::ManeuverType maneuverType = me->GetManeuver ();
				
				if (maneuverType == Events::ManeuverEvent::TurnLeft)
				{
					Ship->Rotate (ShipArchetype::CCW);
				}
				if (maneuverType == Events::ManeuverEvent::TurnRight)
				{
					Ship->Rotate (ShipArchetype::CW);
				}
				if (maneuverType == Events::ManeuverEvent::Thrust)
				{
					Ship->ApplyThrust ();
				}
			}
				break;
			case Events :: SwitchViewport:
			{
				const Events::SwitchViewportEvent* sve = reinterpret_cast <const Events::SwitchViewportEvent*> (msg);
				Events::SwitchViewportEvent::ViewportSetting viewtype = sve->GetView ();
				int WhichStation = sve->GetViewIndex ();
				Events::DebugTextEvent debugText;
				
				switch (viewtype)
				{
					case Events::SwitchViewportEvent::ShipView:
						player->SetViewFocusToShip ();
						debugText.SetMessageText ("Switch to shipview");
						SendMessages (debugText);
						break;
					case Events::SwitchViewportEvent::StationView:
						player->SetFocus (WhichStation);
						debugText.SetMessageText ("Switch to stationview");
						SendMessages (debugText);
						break;
						
				}
			}
				break;
			case Events :: FireWeapon:
				//projectiles.AddBolt (gazelle);
				//Ship->FireWeapon ();
				break;
		}
	}
	
}

//-------------------------------------------
//-------------------------------------------

void			PlayerDatabase :: AddPlayer (Player* player)
{
	std::list <Player*>::iterator it = FindPlayer (player->GetUnitID ());
	if (it != PlayerList.end ())
	{
		return;// already here
	}
	PlayerList.push_back (player);
}

//-------------------------------------------

Player*			PlayerDatabase :: CreatePlayer ()// this is not added to any lists
{
	return NULL;
}

//-------------------------------------------

Player*			PlayerDatabase :: CreatePlayer (const char* name, Vector center, StellarCartography::TShape shape) const// this is not added to any lists
{	
	Player* player = new Player ();
	const int MaxStations = 4;
	SpaceStation*		stations[MaxStations];
	
	
	for (int i=0; i<MaxStations; i++)
	{
		stations[i] = new SpaceStation ();
	}
	stations[0]->SetColor (Vector (1, 0.3F, 0.3F));
	stations[1]->SetColor (Vector (1, 0, 1));
	stations[2]->SetColor (Vector (0, 1, 0));
	stations[3]->SetColor (Vector (0.6F, 1.0F, 0.7F));
	for (int i=0; i<MaxStations; i++)
	{
		stations[i]->SetAngleOfRotation (Vector (0, 0, 0.6F-static_cast<float> (i)/10.0F));
		stations[i]->Setup ();
	}
	
	float DistanceFromCenter = 80;
	stations[0]->SetCenter (center);
	stations[1]->SetCenter (CreateVectorDownInHex (center, DistanceFromCenter));
	stations[2]->SetCenter (CreateVectorUpRightInHex (center, DistanceFromCenter));
	stations[3]->SetCenter (CreateVectorUpLeftInHex (center, DistanceFromCenter));
	
	stations[0]->SetCurrentHitPoints(20);
	stations[1]->SetCurrentHitPoints(45);
	stations[2]->SetCurrentHitPoints(60);
	stations[3]->SetCurrentHitPoints(80);
	stations[0]->SetShieldLevel (0, 2);
	stations[0]->SetShieldLevel (1, 2);
	
	stations[1]->SetShieldLevel (0, 1);
	stations[1]->SetShieldLevel (1, 1);
	
	stations[2]->SetShieldLevel (0, 1);
	stations[2]->SetShieldLevel (1, 1);
	
	stations[3]->SetShieldLevel (0, 1);
	stations[3]->SetShieldLevel (1, 1);
	
	// based on shape, this configuration may change
	
	Gazelle*		gazelle = new Gazelle ();
	gazelle->SetBodyColor (Vector (0.2F, 0.2F, 0.25F));
	gazelle->SetCockpitColors (Vector (0, 0.5F, 0.5F), Vector (0.2F, 0.2F, 0.2F));
	gazelle->SetShieldColor (Vector (0, 0, 0.5F));
	gazelle->SetAngle (Vector (0, 0, 1));
	
	gazelle->SetCurrentHitPoints (100);
	gazelle->SetCenter (center + Vector (6, 5, 0));
	gazelle->Setup ();
	
	player->SetName (name);
	for (int i=0; i<MaxStations; i++)
	{
		player->AddStation (stations[i]);
	}
	player->SetShip (gazelle);
	
/*	testShip.SetBodyColor (Vector (0.3, 0.3, 0.25));
	testShip.SetCockpitColors (Vector (0, 0.5, 0.5), Vector (0.2, 0.2, 0.2));
	testShip.SetShieldColor (Vector (0, 0, 0.5));
	testShip.SetAngle (Vector (0, 0, 1));
	testShip.SetCenter (Vector (10, 6, 0));
	
	testShip.Setup ();*/
	return player;
}

//-------------------------------------------

void			PlayerDatabase :: RemovePlayer (Player*)
{
}

//-------------------------------------------

void			PlayerDatabase :: SetLocalPlayer (UUID PlayerID)
{
	std::list <Player*>::iterator it = FindPlayer (PlayerID);
	if (it != PlayerList.end ())
		CurrentPlayer = it;
}

//-------------------------------------------

UUID			PlayerDatabase :: GetCurrentPlayerID ()
{
	assert (CurrentPlayer != PlayerList.end ());
	Player* player = *CurrentPlayer;
	return player->GetUnitID ();
}

//-------------------------------------------

Player*			PlayerDatabase :: GetCurrentPlayer ()
{
	assert (CurrentPlayer != PlayerList.end ());
	Player* player = *CurrentPlayer;
	return player;
}

//-------------------------------------------

Player*			PlayerDatabase :: GetPlayer (UUID PlayerID)
{
	std::list <Player*>::iterator it = FindPlayer (PlayerID);
	if (it != PlayerList.end ())
		return *it;
	return NULL;
}

//-------------------------------------------

int				PlayerDatabase :: GetNumPlayers () const
{
	return PlayerList.size ();
}

//-------------------------------------------

Player*			PlayerDatabase :: GetPlayerByIndex (int index)// returns NULL for invalid indices
{
	assert (index < static_cast<int> (PlayerList.size ()));
	
	int Count = 0;
	std::list <Player*>::iterator it = PlayerList.begin ();
	while (it != PlayerList.end ())
	{
		if (Count == index)
			return *it;
		it++;
	}
	return NULL;
}

//-------------------------------------------

std::list <Player*>::iterator	PlayerDatabase :: FindPlayer (UUID PlayerID)
{
	std::list <Player*>::iterator it = PlayerList.begin ();
	while (it != PlayerList.end ())
	{
		Player* player = *it;
		if (player->GetUnitID () == PlayerID)
			return it;
		
		it++;
	}
	return PlayerList.end ();
}
//-------------------------------------------
//-------------------------------------------

