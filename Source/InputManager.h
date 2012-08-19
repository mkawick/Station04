/*
 *  InputManager.h
 *  Station05
 *
 */

#pragma once
#include "gameevent.h"
#include <list>
#include <hash_map>

#include "../tools/json/jansson.h"
//struct json_t;

// ----------------------------------------------------

struct KeyMapping
{
	KeyMapping() : keyPress ( SDLK_UNKNOWN ), event( 0 ), selectionData( 0 ), allowHold( 0 ), maxRepeatRate( 0 )
	{
		memset( type, 0, sizeof( type ) );
	}
	SDLKey keyPress;
	int event;
	char type[ 32 ];
	int selectionData;
	bool allowHold;
	int maxRepeatRate;
	U32 modifiers;
};

typedef std::list< KeyMapping > KeySet;
typedef std::list< KeyMapping >::iterator KeySetIter;

typedef std::pair <GameMode, KeySet> GameModeKeyMappingPair;
typedef stdext::hash_map < GameMode, KeySet > GameModeKeySet;
typedef GameModeKeySet::iterator GameModeKeySetIter;

class InputManager : public Events:: MessageSenderReceiver
{
public:
	InputManager ();

	void	AddKeyMapping( GameMode mode, const char* key, const char* event, const char* typeData, bool allowHold, int maxRepeatRate, U32 modifiers, int selectionData = 0 );
	void	SetGameMode( GameMode mode );// mode controled by string
	
	void	Update (GameData& restrict); // data

	bool	LoadIniFile( json_t* keySetObj, const char* filePath );
	
private:
	bool	CreateGameEvent( GameData& GlobalGameData, const KeyMapping& mapping );
	void	ProcessMessages (GameData& data);
	bool	HandleKeyboard (GameData& data);
	bool	HandleMouse (GameData& data);

	GameModeKeySet keyboardSetup;
};

// ----------------------------------------------------
