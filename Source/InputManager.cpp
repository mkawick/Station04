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
InputManager :: InputManager () : currentGameMode( GameMode_Game )
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

bool	InputManager :: HandleKeyboard (GameData& GlobalGameData)
{
	bool done = false;
	SDL_Event event;
	const int numEventsToHandle = 6;
	SDL_Event events[ numEventsToHandle ];
	//

	GameModeKeySetIter iter;
	iter = keyboardSetup.find( currentGameMode );
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
