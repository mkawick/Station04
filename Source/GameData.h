/*
 *  GameData.h
 *  Station05
 *
 */

#pragma once
#include "GlobalDefinitions.h"

class PlayerDatabase;
class ShipArchetype;

class StaticGameData
{
};

/*
class PlayerDatabase
{
public:
	enum ConnectedStatus {Startup, Awaiting, Listening, Connecting, Connected, Disconnecting, };
};*/

class GameData
{
public:
	GameData ();
	~GameData ();
	StaticGameData*	GetStaticGameData () const {return StaticGameDatum;}
	PlayerDatabase* GetPlayerDatabase () const {return PlayerDB;}

	int				GetTimeElapsedInMS() const {return TimeInMS;}
	void			SetTimeElapsed(int _TimeInMS) {TimeInMS = _TimeInMS;}

	bool			AddPlayer( ShipArchetype* playersShip, UUID uuid );
	ShipArchetype*  GetPlayerShip( UUID uuid );

	PlayerDatabase* GetPlayerDb() { return PlayerDB;}

	GameMode		GetGameMode() const { return currentGameMode; }
	void			SetGameMode( GameMode _gameMode ) { currentGameMode = _gameMode; }
	void			Update();

protected:
	PlayerDatabase* PlayerDB;
	StaticGameData* StaticGameDatum;
	GameMode		currentGameMode;
	int				TimeInMS;
	
};

