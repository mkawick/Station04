/*
 *  PlayerDatabase.h
 *  Station05
 */

#pragma once
#include <list>
#include "Player.h"
#include "GameEvent.h"
#include "StellarCartography.h"
#include "ShipArchetype.h"
#include "PlayerStatusBars.h"

//-----------------------------------------------
typedef std::list <Player*> ListOfPlayers;
typedef ListOfPlayers::iterator ListOfPlayersIter;
// functions as a player manager
class PlayerDatabase : public Events:: MessageSenderReceiver
{
public:
	//-------------------------------------------
	PlayerDatabase ();
	
	void			ClearAllPlayers ();// clear the db of all players

	void			SetScreenDimensions (int Left, int Top, int Right, int Bottom);
	void			SetPositionForPlayerStatusBars (float l, float t, float r, float b);

	//-------------------------------------------
	
	void			AddPlayer (Player* player);
	Player*			CreatePlayer ();// this is not added to any lists
	Player*			CreatePlayer (const char* name, Vector startingLocation, StellarCartography::TShape) const;// this is not added to any lists
	void			RemovePlayer (Player*);
	
	void			SetLocalPlayer (UUID PlayerID);
	UUID			GetCurrentPlayerID ();
	
	Player*			GetCurrentPlayer ();
	Player*			GetPlayer (UUID PlayerID);// returns NULL for players not found
	int				GetNumPlayers () const;
	Player*			GetPlayerByIndex (int index);// returns NULL for invalid indices
	
	//-------------------------------------------
	
	void			SetViewToPlayerShip ();
	void			SwitchPlayerView (int WhichStation);
	void			ApplyThrust ();
	void			Rotate (ShipArchetype::RotationDir dir);
	void			FireWeapon ();
	
	//-------------------------------------------
	
	void			Update (GameData& GlobalGameData);// call update on all players and update player stats
	void			Draw ();// make all players draw themselves.
	void			PostDrawCleanup ();
	
	
protected:
	//-------------------------------------------
	
	void			ProcessMessages (GameData& GlobalGameData);//inherited, send all messages to the appropriate players
	
	Events::PlayerStatusBars	StatusBars;
	ListOfPlayersIter			FindPlayer (UUID PlayerID);
	ListOfPlayersIter			CurrentPlayer;
	ListOfPlayers PlayerList;
};

//-----------------------------------------------

