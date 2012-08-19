/*
 *  InputManager.cpp
 *  Station05
 */

#include "stdafx.h"
#include <windows.h>
#include <assert.h>
#include "../tools/SDL/include/SDL.h"
#include "InputManager.h"
#include "GameEvent.h"
#include "Gamedata.h"
#include "PlayerDatabase.h"
#include "GameFramework.h"
#include "../tools/json/jansson.h"

extern unsigned long  GetTimeInMilliSeconds ();

// ----------------------------------------------------
// ----------------------------------------------------

SDLKey LookupSDLkeys( const char * key )
{
	struct KeyMap 
	{ 
		const char* key; 
		SDLKey sdlKey; 
	};

	KeyMap keyMap[] = 
	{
		{ "bksp", SDLK_BACKSPACE },
		{ "tab", SDLK_TAB },
		{ "return", SDLK_RETURN },
		{ "esc", SDLK_ESCAPE },
		{ "[", SDLK_LEFTBRACKET },
		{ "]", SDLK_RIGHTBRACKET },
		{ ",", SDLK_COMMA },
		{ ".", SDLK_PERIOD },
		{ "1", SDLK_1 },
		{ "2", SDLK_2 },
		{ "3", SDLK_3 },
		{ "4", SDLK_4 },
		{ "5", SDLK_5 },
		{ "6", SDLK_6 },
		{ "7", SDLK_q },
		{ "8", SDLK_q },
		{ "9", SDLK_q },
		{ "0", SDLK_q },
		{ "space", SDLK_SPACE },

		{ "a", SDLK_a },
		{ "b", SDLK_b },
		{ "c", SDLK_c },
		{ "d", SDLK_d },
		{ "e", SDLK_e },
		{ "f", SDLK_f },
		{ "g", SDLK_g },
		{ "h", SDLK_h },
		{ "i", SDLK_i },
		{ "j", SDLK_j },
		{ "k", SDLK_k },
		{ "l", SDLK_l },
		{ "m", SDLK_m },
		{ "n", SDLK_n },
		{ "o", SDLK_o },
		{ "p", SDLK_p },
		{ "q", SDLK_q },
		{ "r", SDLK_r },
		{ "s", SDLK_s },
		{ "t", SDLK_t },
		{ "u", SDLK_u },
        { "u", SDLK_v },
        { "u", SDLK_w },
        { "u", SDLK_x },
        { "u", SDLK_y },
        { "u", SDLK_z },

        { "f1", SDLK_F1 },
        { "f2", SDLK_F2 },
        { "f3", SDLK_F3 },
        { "f4", SDLK_F4 },
        { "f5", SDLK_F5 },
        { "f6", SDLK_F6 },
        { "f7", SDLK_F7 },
        { "f8", SDLK_F8 },
        { "f9", SDLK_F9 },
        { "f10", SDLK_F10 },
	};

	int num = sizeof( keyMap ) / sizeof( KeyMap );
	for( int i=0; i< num; i++ )
	{
		if( stricmp( key, keyMap[i].key ) == 0 )
			return keyMap[i].sdlKey;
	}
	return SDLK_UNKNOWN;
}

Events::EventMessages LookupEvent( const char* eventName )
{
	struct EventMap 
	{ 
		const char* name; 
		Events::EventMessages event; 
	};
	EventMap eventMap[] = 
	{
		{ "", Events::NoMessage },
		{ "debug", Events::DebugText },
		{ "thrust", Events::ApplyThrust },
		{ "mode", Events::GameModeChange },
		{ "maneuver", Events::Maneuvers },
		{ "view", Events::SwitchViewport },
		{ "fire", Events::FireWeapon },
		{ "camera", Events::CameraMode },
		{ "player", Events::PlayerStatus },
		{ "chat", Events::PlayerChat },
		{ "ping", Events::PingTime },
		{ "resource", Events::CreateResource },
		{ "capture_resource", Events::CaptureResource },
		{ "quit", Events::QuitGame },
		{ "uishipconfig", Events::UI_ShipConfig },
		{ "uimap", Events::UI_Map },
		{ "uimystatus", Events::UI_MyStatus }
	};

	int num = sizeof( eventMap ) / sizeof( EventMap );
	for( int i=0; i< num; i++ )
	{
		if( stricmp( eventName, eventMap[i].name ) == 0 )
			return eventMap[i].event;
	}
	return Events::NoMessage;
}


void InputManager::AddKeyMapping( GameMode mode, 
								  const char* key, 
								  const char* event, 
								  const char* typeData, 
								  bool allowHold, 
								  int maxRepeatRate,
								  U32 modifiers, 
								  int selectionData)
{
	SDLKey sdlKey = LookupSDLkeys( key );
	if( sdlKey != SDLK_UNKNOWN )
	{
		GameModeKeySetIter iter;
		iter = keyboardSetup.find( mode );
		if( iter == keyboardSetup.end() )
		{
			GameModeKeyMappingPair pair (mode, KeySet() );
			keyboardSetup.insert( pair );
			iter = keyboardSetup.find( mode );
		}
		KeySet&  setOfKeys = iter->second;

		//verify that no duplicates
		KeyMapping mapping;
		mapping.keyPress = sdlKey;
		mapping.allowHold = allowHold;
		mapping.event = LookupEvent( event );
		mapping.maxRepeatRate = maxRepeatRate;
		if( typeData )
			strcpy( mapping.type, typeData );
		mapping.selectionData = selectionData;
		mapping.modifiers = 0;
		if( modifiers & KeyModifier_Alt )
			mapping.modifiers |= KMOD_ALT;
		if( modifiers & KeyModifier_Ctrl )
			mapping.modifiers |= KMOD_CTRL;
		if( modifiers & KeyModifier_Shift )
			mapping.modifiers |= KMOD_SHIFT;

		setOfKeys.push_back( mapping );
	}
}
InputManager :: InputManager () : Events:: MessageSenderReceiver()
{
	//FillinBaselineKeyMapper (Mapper);
	Events::SwitchViewportEvent Viewport;
	MessageSenderReceiver::SendMessages (Viewport);
}

// ----------------------------------------------------

void		InputManager :: Update (GameData& GlobalGameData)
{
	SDL_PumpEvents();
	
	ProcessMessages (GlobalGameData);
	HandleKeyboard (GlobalGameData);
	HandleMouse( GlobalGameData );
	UpdateAllClients ();
}

bool InputManager :: CreateGameEvent( GameData& GlobalGameData, const KeyMapping& mapping )
{
	switch( mapping.event )
	{
	case Events::QuitGame:
		MessageSenderReceiver::SendMessages ( Events::GameQuitEvent() );
	case Events::SwitchViewport:
		{
			Events::SwitchViewportEvent Viewport;
			if( stricmp( mapping.type, "ship" ) == 0 )
			{
				Viewport.SetView (Events::SwitchViewportEvent::ShipView);
			}
			else
			{
				Viewport.SetView (Events::SwitchViewportEvent::StationView);
			}
			if( mapping.selectionData )
			{
				Viewport.SetViewIndex (mapping.selectionData);
			}
			MessageSenderReceiver::SendMessages ( Viewport );
			return true;
		}
	case Events::ApplyThrust:
		{
			Events::ManeuverEvent Man;
			Man.SetManeuver (Events::ManeuverEvent::Thrust);
			MessageSenderReceiver::SendMessages ( Man );
			return true;
		}
	case Events::Maneuvers:
		{
			Events::ManeuverEvent Man;
			if( stricmp( mapping.type, "left" ) == 0 )
			{
				Man.SetManeuver (Events::ManeuverEvent::TurnLeft);
			}
			else
			{
				Man.SetManeuver (Events::ManeuverEvent::TurnRight);
			}
			MessageSenderReceiver::SendMessages ( Man );
			return true;
		}
	case Events::FireWeapon:
		{
			Events:: FireWeaponEvent Event;
			//PlayerDatabase* playerdb = GlobalGameData.GetPlayerDatabase();
			PlayerDatabase* playerdb = GlobalGameData.GetPlayerDatabase();
			Event.SetPlayerID (playerdb->GetCurrentPlayerID ());
			MessageSenderReceiver::SendMessages ( Event );
			return true;
		}
	case Events::UI_ShipConfig:
		{
			Events::UIShipConfigEvent Event;
			MessageSenderReceiver::SendMessages ( Event );
			return true;
		}
	case Events::UI_Map:
		{
			Events::UIMapEvent Event;
			MessageSenderReceiver::SendMessages ( Event );
			return true;
		}
	case Events::UI_MyStatus:
		{
			Events::UIMyStatusEvent Event;
			MessageSenderReceiver::SendMessages ( Event );
			return true;
		}
	}

	return false;
}
// ----------------------------------------------------
bool	InputManager :: HandleMouse (GameData& data)
{
	SDL_Event event;
	const int numEventsToHandle = 6;
	SDL_Event events[ numEventsToHandle ];

	Uint32 mask = SDL_MOUSEEVENTMASK;
	int NumEventsReturned = SDL_PeepEvents( events, numEventsToHandle, SDL_GETEVENT, mask );

	for( int i=0; i<NumEventsReturned; i++ )
	{
		event = events[i];
		switch( event.type )
		{
			case SDL_MOUSEMOTION:
				{
					Events::UIMouseMoveEvent mmEvent;
					mmEvent.SetPosition( event.motion.x, event.motion.y );
					mmEvent.SetDiff( event.motion.xrel, event.motion.yrel );
					MessageSenderReceiver::SendMessages ( mmEvent );
				}
				break;
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
				{
					Events::UIMouseButtonEvent mbEvent;
					switch( event.button.button )
					{
					case 1:
						mbEvent.SetButton( Events::UIMouseButtonEvent::Left );
						break;
					case 2:
						mbEvent.SetButton( Events::UIMouseButtonEvent::Middle );
						break;
					case 3:
						mbEvent.SetButton( Events::UIMouseButtonEvent::Right );
						break;
					}
					if( event.type == SDL_MOUSEBUTTONUP )
						mbEvent.SetState( Events::UIMouseButtonEvent::Up );
					else
						mbEvent.SetState( Events::UIMouseButtonEvent::Down );

					mbEvent.SetPosition( event.button.x, event.button.y );
					MessageSenderReceiver::SendMessages ( mbEvent );
				}
				break;
		}
	}
	return true;

}

bool	InputManager :: HandleKeyboard (GameData& GlobalGameData)
{
	bool done = false;
	SDL_Event event;
	const int numEventsToHandle = 6;
	SDL_Event events[ numEventsToHandle ];
	//

	GameModeKeySetIter iter;
	iter = keyboardSetup.find( GlobalGameData.GetGameMode() );
	if( iter == keyboardSetup.end() )
	{
		Uint32 mask = SDL_KEYUPMASK | SDL_KEYDOWNMASK;
		int NumEventsReturned = SDL_PeepEvents(events, numEventsToHandle, SDL_GETEVENT, mask);
		return true;
	}

	// when there are keys to handled
	KeySet& setOfKeys = iter->second;
	
	Uint32 mask = SDL_KEYUPMASK;
	int NumEventsReturned = SDL_PeepEvents(events, numEventsToHandle, SDL_PEEKEVENT, mask);
	if (NumEventsReturned > 0)// when they let go, remove all keys on stack
	{
		mask = SDL_KEYUPMASK | SDL_KEYDOWNMASK;
		NumEventsReturned = SDL_PeepEvents(events, numEventsToHandle, SDL_GETEVENT, mask);
	}
	else
	{	// do not remove event, so we can hold down the key
		mask = SDL_KEYDOWNMASK;
		NumEventsReturned = SDL_PeepEvents(events, numEventsToHandle, SDL_PEEKEVENT, mask);
		bool clearRemainingEventQueue = false;
		
		for (int i=0; i<NumEventsReturned; i++)
		{
			event = events[i];			
			
			SDLKey key = event.key.keysym.sym;
			U32 modifiers = event.key.keysym.mod;  //SDLMod
			modifiers &= 0xffff - (KMOD_NUM | KMOD_CAPS);

			KeySetIter it = setOfKeys.begin();// look for any matchers
			while (it != setOfKeys.end() )
			{
				const KeyMapping& mappedKey = *it++;
				if( mappedKey.keyPress == key )
				{
					if( ( modifiers & mappedKey.modifiers ) || 
						( modifiers == 0 && mappedKey.modifiers == 0 ) )
					{
						bool wasSent = CreateGameEvent( GlobalGameData, mappedKey );
						if( wasSent )
						{
							if( mappedKey.allowHold == false )
								clearRemainingEventQueue = true;
						}
					}
				}
			}
		}
		if (clearRemainingEventQueue)
		{
			mask = SDL_KEYDOWNMASK;
			NumEventsReturned = SDL_PeepEvents(events, numEventsToHandle, SDL_GETEVENT, mask);
		}
	}
	return done;	
}

// ----------------------------------------------------

void	InputManager :: ProcessMessages (GameData& GlobalGameData)
{
	this->UpdateAllClients ();
}

// ----------------------------------------------------

bool	InputManager :: LoadIniFile( json_t* root, const char* filePath )
{
	json_t * keySetObj = json_object_get( root, "key_set");
	if( json_is_array( keySetObj ) )
	{
		int numItems = json_array_size( keySetObj );
		for( int i=0; i< numItems; i++ )
		{
			json_t * keysetForMode = json_array_get( keySetObj, i );
			if( json_is_object( keysetForMode ) )
			{
				const char* modeLookup = NULL;
				json_t * modeObj = json_object_get( keysetForMode, "mode");
				if( json_is_string( modeObj ) )
					modeLookup = json_string_value( modeObj );
				json_t * setObj = json_object_get( keysetForMode, "set");

				GameMode mode = LookupGameMode( modeLookup );
				if( mode == GameMode_none )
				{
					cout << "ERROR: keyboard ini file has bad mode: " << filePath <<", mode=" << modeLookup << endl;
					return false;
				}
				if( json_is_array( setObj ) )
				{
					int numKeys = json_array_size( setObj );
					for( int i=0; i< numKeys; i++ )
					{
						json_t * keyDefnObj = json_array_get( setObj, i );
						if( json_is_object( keyDefnObj ) )
						{
							U32 modifiers = KeyModifier_none;
							json_t * modifierObj = json_object_get( keyDefnObj, "modifier");
							if( modifierObj )
							{
								const char* modifierString = json_string_value( modifierObj );
								modifiers = LookupKeyModifier( modifierString );
							}
							json_t * keyboardObj = json_object_get( keyDefnObj, "key");
							if( keyboardObj == NULL )
							{
								cout << "ERROR: keyboard ini file has bad key: " << filePath <<", mode=" << modeLookup << endl;
								return false;
							}
							const char* keyString = json_string_value( keyboardObj );
							if( keyString == NULL )
							{
								cout << "ERROR: keyboard ini file has bad key: " << filePath <<", mode=" << modeLookup << endl;
								return false;
							}
							json_t * eventObj = json_object_get( keyDefnObj, "event");
							const char* eventString = json_string_value( eventObj );
							if( eventString == NULL )
							{
								cout << "ERROR: keyboard ini file has bad event: " << filePath <<", key=" << keyString << endl;
								return false;
							}
							json_t * typeObj = json_object_get( keyDefnObj, "type");
							const char* typeString = json_string_value( typeObj );

							json_t * selectionObj = json_object_get( keyDefnObj, "selection");
							int selection = static_cast<int> ( json_integer_value( selectionObj ) );

							json_t * repeatObj = json_object_get( keyDefnObj, "repeat");
							int repeat = static_cast<int> ( json_integer_value( repeatObj ) );

							json_t * holdObj = json_object_get( keyDefnObj, "hold");
							bool allowHold = json_integer_value( holdObj ) ? true : false;

							GlobalGameFramework->GetInput().AddKeyMapping( mode, keyString, eventString, typeString, allowHold, repeat, modifiers, selection );
						}
						else
						{
							cout << "ERROR: keyboard ini file has bad key: " << filePath <<", mode=" << modeLookup << endl;
							return false;
						}
					}
				}
				else
				{
					cout << "ERROR: keyboard ini file has bad set: " << filePath <<", mode=" << modeLookup << endl;
					return false;
				}
			}
		}
	}
	return true;
}
//--------------------------------------------------------------------
