/*
 *  GameData.h
 *  SDL04_Station
 *
 */

#pragma once
//#include 

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
	StaticGameData*	GetStaticGameData () {return StaticGameDatum;}
	PlayerDatabase* GetPlayerDatabase () {return PlayerDB;}

	int				GetTimeElapsedInMS() const {return TimeInMS;}
	void			SetTimeElapsed(int _TimeInMS) {TimeInMS = _TimeInMS;}

	bool			AddPlayer( ShipArchetype* playersShip, UUID uuid );
	ShipArchetype*  GetPlayerShip( UUID uuid );

protected:
	PlayerDatabase* PlayerDB;
	StaticGameData* StaticGameDatum;
	int				TimeInMS;
	
};

