/*
 *  GameData.cpp
 *  Station05
 */

#include "stdafx.h"
#include <windows.h>
#include "GameData.h"
#include "PlayerDatabase.h"

//-----------------------------------------------

GameData :: GameData ():TimeInMS(0), currentGameMode( GameMode_Game )
{
	PlayerDB = new PlayerDatabase ();
	StaticGameDatum = new StaticGameData ();
}

//-----------------------------------------------

GameData :: ~GameData ()
{
	delete PlayerDB;
	delete StaticGameDatum;
}

//-----------------------------------------------

#include "../DataView/PlayerShipDataView.h"
void		GameData :: Update()
{
	PlayerDatabase* playerdb = GetPlayerDatabase();
	playerdb->Update (*this);
	playerdb->PostDrawCleanup ();

	PlayerShipDataView view;
	int numRows = view.GetNumRows();
}