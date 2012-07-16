/*
 *  InputManager.cpp
 *  Station05
 */

#include "stdafx.h"
#include <windows.h>
#include <assert.h>
#include <SDL/SDL.h>
#include "InputManager.h"
#include "GameEvent.h"
#include "Gamedata.h"
#include "PlayerDatabase.h"
#include "GameFramework.h"

extern unsigned long  GetTimeInMilliSeconds ();
//#define EVENT_DEBUG
// ----------------------------------------------------
// ----------------------------------------------------

void	FillinBaselineKeyMapper (KeyMapper& Mapper);

// ----------------------------------------------------
// ----------------------------------------------------

InputManager :: InputManager ()
{
	FillinBaselineKeyMapper (Mapper);
	Events::SwitchViewportEvent Viewport;
	MessageSenderReceiver::SendMessages (Viewport);
}

// ----------------------------------------------------

void		InputManager :: Update (GameData& GlobalGameData)
{
	SDL_PumpEvents();
	Mapper.ProcessAllKeys (GlobalGameData, *this);
	
	ProcessMessages (GlobalGameData);
	HandleKeyboard (GlobalGameData);
	UpdateAllClients ();
	
}

// ----------------------------------------------------

bool	InputManager :: HandleKeyboard (GameData& GlobalGameData)
{
	bool done = false;
	SDL_Event event;
	SDL_Event events[6];
	PlayerDatabase* playerdb = GlobalGameData.GetPlayerDatabase();
	
	//float AngleAdd = 0.5;
	//float PositionAdd = 0.5;
	//float LightPositionAdd = 0.8;
	
	Uint32 mask = SDL_KEYUPMASK;
	int NumEventsReturned = SDL_PeepEvents(events, 6, SDL_PEEKEVENT, mask);
	if (NumEventsReturned > 0)// when they let go, remove all keys on stack
	{
		mask = SDL_KEYUPMASK | SDL_KEYDOWNMASK;
		NumEventsReturned = SDL_PeepEvents(events, 6, SDL_GETEVENT, mask);
	}
	else
	{	// do not remove event, so we can hold down the key
		mask = SDL_KEYDOWNMASK;
		NumEventsReturned = SDL_PeepEvents(events, 6, SDL_PEEKEVENT, mask);
		bool ShouldKeyBeRemoved = false;
		
		for (int i=0; i<NumEventsReturned; i++)
		{
			event = events[i];			
			
			SDLKey key = event.key.keysym.sym;
			if (key == SDLK_UP)
			{
				//CameraPosition.x += PositionAdd;
			}
			else if (key == SDLK_DOWN)
			{
				//CameraPosition.x -= PositionAdd;
			}
			else if (key == SDLK_LEFT)
			{
				//CameraPosition.y -= PositionAdd;
			}
			else if (key == SDLK_RIGHT)
			{
				//CameraPosition.y += PositionAdd;
			}
			else if (key == SDLK_LEFTBRACKET) //'['
			{
				Events::ManeuverEvent Man;
				Man.SetManeuver (Events::ManeuverEvent::TurnLeft);
				MessageSenderReceiver::SendMessages (Man);
				ShouldKeyBeRemoved = false;
			}
			else if (key == SDLK_RIGHTBRACKET) //']'
			{
				Events::ManeuverEvent Man;
				Man.SetManeuver (Events::ManeuverEvent::TurnRight);
				MessageSenderReceiver::SendMessages (Man);
				ShouldKeyBeRemoved = false;
			}
			else if (key == SDLK_PERIOD) //'.'
			{
				Events::ManeuverEvent Man;
				Man.SetManeuver (Events::ManeuverEvent::Thrust);
				MessageSenderReceiver::SendMessages (Man);
			}
			else if (key == SDLK_COMMA) //','
			{
				Events:: FireWeaponEvent Event;
				Event.SetPlayerID (playerdb->GetCurrentPlayerID ());
				MessageSenderReceiver::SendMessages (Event);
				ShouldKeyBeRemoved = false;
			}
			else if (key == SDLK_SEMICOLON) //';'
			{
				//DirectionalLightPosition.y += LightPositionAdd;
			}
			else if (key == '\'') //','
			{
				//DirectionalLightPosition.y -= LightPositionAdd;
			}
			else if (key == 'a' || key == 'A') 
			{
				//CameraAngle.yaw -= AngleAdd;
			}
			else if (key == 's' || key == 'S') 
			{
				//CameraAngle.pitch += AngleAdd;
			}
			else if (key == 'd' || key == 'D') 
			{
				//CameraAngle.yaw += AngleAdd;
			}
			else if (key == 'w' || key == 'W') 
			{
				//CameraAngle.pitch -= AngleAdd;
			}
			else if (key == SDLK_ESCAPE) 
			{
				Events:: GameQuitEvent Event;
				MessageSenderReceiver::SendMessages (Event);

				//done = true;
			}
			else if ((key == 'q' || key == 'Q') && (KMOD_ALT))
			{
				Events:: GameQuitEvent Event;
				MessageSenderReceiver::SendMessages (Event);

				//done = true;
			}
		/*	else if (key == 'b' || key == 'B')
			{
				SourceIndex ++;
				SourceIndex = SourceIndex %15;
			}
			else if (key == 'v' || key == 'V')
			{
				DstIndex ++;
				DstIndex = DstIndex %15;
			}*/
			else if (key == '1')
			{
				Events::SwitchViewportEvent Viewport;
				Viewport.SetView (Events::SwitchViewportEvent::ShipView);
				MessageSenderReceiver::SendMessages (Viewport);
				ShouldKeyBeRemoved = true;
			}
			else if (key == '2')
			{
				Events::SwitchViewportEvent Viewport;
				Viewport.SetView (Events::SwitchViewportEvent::StationView);
				Viewport.SetViewIndex (0);
				MessageSenderReceiver::SendMessages (Viewport);
				ShouldKeyBeRemoved = true;
			}
			else if (key == '3')
			{
				Events::SwitchViewportEvent Viewport;
				Viewport.SetView (Events::SwitchViewportEvent::StationView);
				Viewport.SetViewIndex (1);
				MessageSenderReceiver::SendMessages (Viewport);
				ShouldKeyBeRemoved = true;
			}
			else if (key == '4')
			{
				Events::SwitchViewportEvent Viewport;
				Viewport.SetView (Events::SwitchViewportEvent::StationView);
				Viewport.SetViewIndex (2);
				MessageSenderReceiver::SendMessages (Viewport);
				ShouldKeyBeRemoved = true;
			}
			else if (key == '5')
			{
				Events::SwitchViewportEvent Viewport;
				Viewport.SetView (Events::SwitchViewportEvent::StationView);
				Viewport.SetViewIndex (3);
				MessageSenderReceiver::SendMessages (Viewport);
				ShouldKeyBeRemoved = true;
			}
		}
		if (ShouldKeyBeRemoved)
		{
			mask = SDL_KEYDOWNMASK;
			NumEventsReturned = SDL_PeepEvents(events, 6, SDL_GETEVENT, mask);
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
// ----------------------------------------------------

class	TurnRightKeyCombo : public Events::IOFiredPlugin
{
	void	FireEvent (GameData& GlobalGameData, Events::MessageSenderReceiver& MessageSender)
	{
		Events::ManeuverEvent Man;
		Man.SetManeuver (Events::ManeuverEvent::TurnRight);
		MessageSender.SendMessages (Man);
#ifdef EVENT_DEBUG
		if (GlobalGameFramework) GlobalGameFramework->PrintText ("TurnRightKeyCombo");
#endif
	}
};

class	TurnLeftKeyCombo : public Events::IOFiredPlugin
{
	void	FireEvent (GameData& GlobalGameData, Events::MessageSenderReceiver& MessageSender)
	{
		Events::ManeuverEvent Man;
		Man.SetManeuver (Events::ManeuverEvent::TurnLeft);
		MessageSender.SendMessages (Man);
#ifdef EVENT_DEBUG
		if (GlobalGameFramework) GlobalGameFramework->PrintText ("TurnLeftKeyCombo");
#endif
	}
};

class	ExitApplicationKeyCombo : public Events::IOFiredPlugin
{
	void	FireEvent (GameData& GlobalGameData, Events::MessageSenderReceiver& MessageSender)
	{
		Events:: GameQuitEvent Event;
		MessageSender.SendMessages (Event);
#ifdef EVENT_DEBUG
		if (GlobalGameFramework) GlobalGameFramework->PrintText ("ExitApplicationKeyCombo");
#endif
	}
};

class	FireWeaponKeyCombo : public Events::IOFiredPlugin
{
	void	FireEvent (GameData& GlobalGameData, Events::MessageSenderReceiver& MessageSender)
	{
		PlayerDatabase* playerdb = GlobalGameData.GetPlayerDatabase();
		Events:: FireWeaponEvent Event;
		Event.SetPlayerID (playerdb->GetCurrentPlayerID ());
		MessageSender.SendMessages (Event);
#ifdef EVENT_DEBUG
		if (GlobalGameFramework) GlobalGameFramework->PrintText ("FireWeaponKeyCombo");
#endif
	}
};

class	EngineThrustKeyCombo : public Events::IOFiredPlugin
{
	void	FireEvent (GameData& GlobalGameData, Events::MessageSenderReceiver& MessageSender)
	{
		Events::ManeuverEvent Man;
		Man.SetManeuver (Events::ManeuverEvent::Thrust);
		MessageSender.SendMessages (Man);
#ifdef EVENT_DEBUG
		if (GlobalGameFramework) GlobalGameFramework->PrintText ("EngineThrustKeyCombo");
#endif
	}
};

class	SetStation1KeyCombo : public Events::IOFiredPlugin
{
	void	FireEvent (GameData& GlobalGameData, Events::MessageSenderReceiver& MessageSender)
	{
		Events::SwitchViewportEvent Viewport;
		Viewport.SetView (Events::SwitchViewportEvent::StationView);
		Viewport.SetViewIndex (0);
		MessageSender.SendMessages (Viewport);
#ifdef EVENT_DEBUG
		if (GlobalGameFramework) GlobalGameFramework->PrintText ("SetStation1KeyCombo");
#endif
	}
};

class	SetStation2KeyCombo : public Events::IOFiredPlugin
{
	void	FireEvent (GameData& GlobalGameData, Events::MessageSenderReceiver& MessageSender)
	{
		Events::SwitchViewportEvent Viewport;
		Viewport.SetView (Events::SwitchViewportEvent::StationView);
		Viewport.SetViewIndex (1);
		MessageSender.SendMessages (Viewport);
#ifdef EVENT_DEBUG
		if (GlobalGameFramework) GlobalGameFramework->PrintText ("SetStation2KeyCombo");
#endif
	}
};

class	SetStation3KeyCombo : public Events::IOFiredPlugin
{
	void	FireEvent (GameData& GlobalGameData, Events::MessageSenderReceiver& MessageSender)
	{
		Events::SwitchViewportEvent Viewport;
		Viewport.SetView (Events::SwitchViewportEvent::StationView);
		Viewport.SetViewIndex (2);
		MessageSender.SendMessages (Viewport);
#ifdef EVENT_DEBUG
		if (GlobalGameFramework) GlobalGameFramework->PrintText ("SetStation3KeyCombo");
#endif
	}
};

class	SetStation4KeyCombo : public Events::IOFiredPlugin
{
	void	FireEvent (GameData& GlobalGameData, Events::MessageSenderReceiver& MessageSender)
	{
		Events::SwitchViewportEvent Viewport;
		Viewport.SetView (Events::SwitchViewportEvent::StationView);
		Viewport.SetViewIndex (3);
		MessageSender.SendMessages (Viewport);
#ifdef EVENT_DEBUG
		if (GlobalGameFramework) GlobalGameFramework->PrintText ("SetStation4KeyCombo");
#endif
	}
};

class	SetShipViewKeyCombo : public Events::IOFiredPlugin
{
	void	FireEvent (GameData& GlobalGameData, Events::MessageSenderReceiver& MessageSender)
	{
		Events::SwitchViewportEvent Viewport;
		Viewport.SetView (Events::SwitchViewportEvent::ShipView);
		MessageSender.SendMessages (Viewport);
#ifdef EVENT_DEBUG
		if (GlobalGameFramework) GlobalGameFramework->PrintText ("SetShipViewKeyCombo");
#endif
	}
};


// ----------------------------------------------------
// ----------------------------------------------------

KeyMapper :: KeyMapper ()
{
}

// ----------------------------------------------------

KeyMapper :: ~KeyMapper ()
{
	std::list <KeyboardInputBehavior*>::iterator it = KeyEvents.begin ();
	while (it != KeyEvents.end ())
	{
		KeyboardInputBehavior* plugin = *it++;
		plugin->Clear ();
		delete plugin;
	}
}

// ----------------------------------------------------

bool	KeyMapper :: AddInputBehavior (KeyboardInputBehavior* input)
{
	std::list <KeyboardInputBehavior*>::iterator it = KeyEvents.begin ();
	while (it != KeyEvents.end ())
	{
		KeyboardInputBehavior* plugin = *it++;
		if (*plugin == *input)
		{			
			return false;
		}
	}

	KeyEvents.push_back (input);	
	return true;
}

// ----------------------------------------------------

void	KeyMapper :: RemoveInputBehavior (KeyboardInputBehavior* input)
{
	std::list <KeyboardInputBehavior*>::iterator it = KeyEvents.begin ();
	while (it != KeyEvents.end ())
	{
		KeyboardInputBehavior* plugin = *it;
		if (*plugin == *input)
		{
			KeyEvents.erase (it);
			return;
		}
		it++;
	}
}

// ----------------------------------------------------

void	KeyMapper :: ProcessAllKeys (GameData& data, Events:: MessageSenderReceiver& sender)
{
	const int NumEventsToPull = 10;
	SDL_Event DownEvents[NumEventsToPull];
	SDL_Event UpEvents[NumEventsToPull];
	int NumDownEvents = SDL_PeepEvents (DownEvents, NumEventsToPull, SDL_GETEVENT, SDL_KEYDOWNMASK);
	int NumUpEvents = SDL_PeepEvents (UpEvents, NumEventsToPull, SDL_GETEVENT, SDL_KEYUPMASK);
	
	if (NumDownEvents > 0)
	{	
#ifdef EVENT_DEBUG
		if (GlobalGameFramework) GlobalGameFramework->PrintText ("New down event");
#endif
	}
	if (NumUpEvents > 0)
	{	
#ifdef EVENT_DEBUG
		if (GlobalGameFramework) GlobalGameFramework->PrintText ("New up event");
#endif
	}
	std::list <KeyboardInputBehavior*>::iterator it = KeyEvents.begin ();
	while (it != KeyEvents.end ())
	{
		KeyboardInputBehavior* plugin = *it++;
		plugin->UpdateKeyDown (DownEvents, NumDownEvents, data);
		plugin->UpdateKeyUp (UpEvents, NumUpEvents, data);
		plugin->Update (data, sender);
	}
}

// ----------------------------------------------------
// ----------------------------------------------------

KeyboardInputBehavior :: KeyboardInputBehavior () : EventToFire (NULL)
{
	Clear ();
}

// ----------------------------------------------------

KeyboardInputBehavior :: ~KeyboardInputBehavior ()
{
	if (EventToFire)
		delete EventToFire;
	EventToFire = NULL;
}

// ----------------------------------------------------

void	KeyboardInputBehavior :: Clear ()
{
	NumKeys = 0;
	GapTimeBetweenFirings = 0;
	LastTimeFired = 0;
	
	for (int i=0; i<NumPossibleKeys; i++)
	{
		KeyBehavior[i] = OnDown;
		KeyCodes[i] = SDLK_UNKNOWN;
		KeyMods[i] = KMOD_NONE;
		IsKeyHeldDown[i] = false;
		FiredDown [i] = false;
		FiredUp [i] = false;
	}
}

// ----------------------------------------------------

void	KeyboardInputBehavior :: AddKeyCode (FiresBehavior behavior, SDLKey key, SDLMod mod)
{
	assert (NumKeys <= NumPossibleKeys);
	KeyBehavior[NumKeys] = behavior;
	KeyCodes[NumKeys] = key;
	KeyMods[NumKeys] = mod;
	IsKeyHeldDown[NumKeys] = false;
	FiredDown [NumKeys] = false;
	FiredUp [NumKeys] = false;
	
	NumKeys++;
}

// ----------------------------------------------------

void	KeyboardInputBehavior :: SetEventToFire (Events::IOFiredPlugin* newEvent)
{
	if (EventToFire)
		delete EventToFire;
	
	EventToFire = newEvent;
}

// ----------------------------------------------------

void	KeyboardInputBehavior :: operator = (const KeyboardInputBehavior& KeyInput)
{
	assert (0);// unimplemented.
	this->Clear ();// go to delete 
}

// ----------------------------------------------------

bool	KeyboardInputBehavior :: operator == (const KeyboardInputBehavior& RHS) const
{
	if (NumKeys != RHS.NumKeys) return false;
	
	for (int i=0; i<NumKeys; i++)// try to return at all costs.
	{
		int which = RHS.FindKeyCode (KeyCodes[i]);
		if (which == -1)
			return false;
		
		if (KeyBehavior[i] != RHS.KeyBehavior[which]) return false;
		if (KeyMods[i] != RHS.KeyMods[which]) return false;
	}
	
	if ((EventToFire == NULL && RHS.EventToFire != NULL)  ||
		(EventToFire != NULL && RHS.EventToFire == NULL)) 
		return false;
	
	return true;
}

// ----------------------------------------------------

void	KeyboardInputBehavior :: UpdateKeyUp (SDL_Event* KeyboardEvents, int NumEvents, GameData& data)
{
	if (EventToFire == NULL || NumEvents == 0)
		return;
	
	for (int i=0; i<NumEvents; i++)
	{
		SDL_KeyboardEvent KeyboardEvent = KeyboardEvents[i].key;
		SDLKey key = KeyboardEvent.keysym.sym;
		int which = FindKeyCode (key);
		if (which != -1)
		{
			SDLMod mod = KeyboardEvent.keysym.mod;

			// filter out num lock
			mod = static_cast<SDLMod> (mod & ~KMOD_NUM);

			if ((mod & KeyMods[which]) || 
				(mod == KeyMods[which]))// the correct type
			{ 
				IsKeyHeldDown [which] = false;
				FiredUp [which] = true;
			}
		}
	}
}

// ----------------------------------------------------

void	KeyboardInputBehavior :: UpdateKeyDown (SDL_Event* KeyboardEvents, int NumEvents, GameData& data)
{
	if (EventToFire == NULL || NumEvents == 0)
		return;
	
	for (int i=0; i<NumEvents; i++)
	{
		SDL_KeyboardEvent KeyboardEvent = KeyboardEvents[i].key;
		SDLKey key = KeyboardEvent.keysym.sym;
		int which = FindKeyCode (key);
		if (which != -1)
		{
#ifdef EVENT_DEBUG
			if (GlobalGameFramework) GlobalGameFramework->PrintText ("key found.. testing mod");
#endif
			SDLMod mod = KeyboardEvent.keysym.mod;
			
			// filter out num lock
			mod = static_cast<SDLMod> (mod & ~KMOD_NUM);

			if ((mod & KeyMods[which]) || 
				(mod == KeyMods[which]))// the correct type
			{
#ifdef EVENT_DEBUG
				if (GlobalGameFramework) GlobalGameFramework->PrintText ("mod found... setting flag");
#endif
				IsKeyHeldDown [which] = true;
				FiredDown [which] = true;
			}
		}
	}
}

// ----------------------------------------------------

void	KeyboardInputBehavior :: Update (GameData& data, Events:: MessageSenderReceiver& sender)
{
	for (int i=0; i<NumKeys; i++)
	{
		if (FiredUp [i] == true)
		{
			if (KeyBehavior [i] == OnUp)
			{
#ifdef EVENT_DEBUG
				if (GlobalGameFramework) GlobalGameFramework->PrintText ("fire onup");
#endif
				EventToFire->FireEvent (data, sender);
			}
			FiredUp [i] = false;
		}
		if (FiredDown [i] == true)
		{
			if (KeyBehavior [i] == OnDown)
			{
#ifdef EVENT_DEBUG
				if (GlobalGameFramework) GlobalGameFramework->PrintText ("fire ondown");
#endif
				EventToFire->FireEvent (data, sender);
			}
			FiredDown [i] = false;
		}
		//---------------------------
		if (IsKeyHeldDown [i] == true)
		{
			if (KeyBehavior [i] == WhileDown)
			{
				bool TimePassedSufficient = true;
				if (GapTimeBetweenFirings != 0)
				{
					unsigned long currentTime = GetTimeInMilliSeconds ();
					if (currentTime - LastTimeFired < GapTimeBetweenFirings)
					{
						TimePassedSufficient = false;
					}
					else 
						LastTimeFired = currentTime;
				}
				if (TimePassedSufficient == true)
				{
#ifdef EVENT_DEBUG
					if (GlobalGameFramework) GlobalGameFramework->PrintText ("fire whiledown");
#endif
					EventToFire->FireEvent (data, sender);
				}
			}
		}
		else 
		{
			if (KeyBehavior [i] == WhileUp)
			{
#ifdef EVENT_DEBUG
				if (GlobalGameFramework) GlobalGameFramework->PrintText ("fire whileup");
#endif
				EventToFire->FireEvent (data, sender);
			}
		}
	}
/*	for (int i=0; i<NumKeys; i++)
	{
		if (IsKeyHeldDown[i] == true)
		{
			switch (KeyBehavior[i])
			{
				case OnDown:
					IsKeyHeldDown[i] = false;// clear state
#ifdef EVENT_DEBUG
					if (GlobalGameFramework) GlobalGameFramework->PrintText ("fire event");
#endif
					EventToFire->FireEvent (data, sender);
					break;
				case WhileDown:
					EventToFire->FireEvent (data, sender);
					break;
			}
		}
		else
		{
			switch (KeyBehavior[i])
			{
				case OnUp:
					IsKeyHeldDown[i] = true;// clear state
#ifdef EVENT_DEBUG
					if (GlobalGameFramework) GlobalGameFramework->PrintText ("fire event");
#endif
					EventToFire->FireEvent (data, sender);
					break;
				case WhileUp:
					EventToFire->FireEvent (data, sender);
					break;
			}
		}
	}*/
}

// ----------------------------------------------------

int		KeyboardInputBehavior :: FindKeyCode (SDLKey key) const
{
	for (int i=0; i<NumKeys; i++)
	{
		if (key == KeyCodes[i])
			return i;
	}
	return -1;
}

// ----------------------------------------------------
// ----------------------------------------------------

void	FillinBaselineKeyMapper (KeyMapper& Mapper)
{
	KeyboardInputBehavior* key = new KeyboardInputBehavior ();
	key->AddKeyCode (KeyboardInputBehavior::WhileDown, SDLK_RIGHTBRACKET, KMOD_NONE);
	TurnRightKeyCombo* Righter = new TurnRightKeyCombo (); 
	key->SetEventToFire (Righter);
	Mapper.AddInputBehavior (key);
	
	key = new KeyboardInputBehavior ();
	key->AddKeyCode (KeyboardInputBehavior::WhileDown, SDLK_LEFTBRACKET, KMOD_NONE);
	TurnLeftKeyCombo* Lefter = new TurnLeftKeyCombo (); 
	key->SetEventToFire (Lefter);
	Mapper.AddInputBehavior (key);
	
	key = new KeyboardInputBehavior ();
	key->AddKeyCode (KeyboardInputBehavior::WhileDown, SDLK_PERIOD, KMOD_NONE);
	key->SetGapTimeBetweenRepeats(300);
	FireWeaponKeyCombo* Firer = new FireWeaponKeyCombo (); 
	key->SetEventToFire (Firer);
	Mapper.AddInputBehavior (key);
	
	key = new KeyboardInputBehavior ();
	key->AddKeyCode (KeyboardInputBehavior::WhileDown, SDLK_COMMA, KMOD_NONE);
	EngineThrustKeyCombo* Enginer = new EngineThrustKeyCombo (); 
	key->SetEventToFire (Enginer);
	Mapper.AddInputBehavior (key);
	
	key = new KeyboardInputBehavior ();
	key->AddKeyCode (KeyboardInputBehavior::OnDown, SDLK_ESCAPE, KMOD_NONE);
	key->AddKeyCode (KeyboardInputBehavior::OnDown, SDLK_q, static_cast <SDLMod> (KMOD_META));
	key->AddKeyCode (KeyboardInputBehavior::OnDown, SDLK_q, static_cast <SDLMod> (KMOD_SHIFT | KMOD_META));
	
	ExitApplicationKeyCombo* Exiter = new ExitApplicationKeyCombo (); 
	key->SetEventToFire (Exiter);
	Mapper.AddInputBehavior (key);
	
	key = new KeyboardInputBehavior ();
	key->AddKeyCode (KeyboardInputBehavior::OnDown, SDLK_1, KMOD_NONE);
	SetShipViewKeyCombo* ShipSelector = new SetShipViewKeyCombo (); 
	key->SetEventToFire (ShipSelector);
	Mapper.AddInputBehavior (key);
	
	key = new KeyboardInputBehavior ();
	key->AddKeyCode (KeyboardInputBehavior::OnDown, SDLK_2, KMOD_NONE);
	SetStation1KeyCombo* Station1Selector = new SetStation1KeyCombo (); 
	key->SetEventToFire (Station1Selector);
	Mapper.AddInputBehavior (key);
	
	key = new KeyboardInputBehavior ();
	key->AddKeyCode (KeyboardInputBehavior::OnDown, SDLK_3, KMOD_NONE);
	SetStation2KeyCombo* Station2Selector = new SetStation2KeyCombo (); 
	key->SetEventToFire (Station2Selector);
	Mapper.AddInputBehavior (key);
	
	key = new KeyboardInputBehavior ();
	key->AddKeyCode (KeyboardInputBehavior::OnDown, SDLK_4, KMOD_NONE);
	SetStation3KeyCombo* Station3Selector = new SetStation3KeyCombo (); 
	key->SetEventToFire (Station3Selector);
	Mapper.AddInputBehavior (key);
	
	key = new KeyboardInputBehavior ();
	key->AddKeyCode (KeyboardInputBehavior::OnDown, SDLK_5, KMOD_NONE);
	SetStation4KeyCombo* Station4Selector = new SetStation4KeyCombo (); 
	key->SetEventToFire (Station4Selector);
	Mapper.AddInputBehavior (key);
}



// ----------------------------------------------------
// ----------------------------------------------------
