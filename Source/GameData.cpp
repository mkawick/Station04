/*
 *  GameData.cpp
 *  Station05
 */

#include "stdafx.h"
#include <windows.h>
#include "GameData.h"
#include "PlayerDatabase.h"

//-----------------------------------------------

GameData :: GameData ():TimeInMS(0)
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