/*
 *  GameEvent.cpp
 *  Station05
 */

#include "stdafx.h"
#include <windows.h>
#include "GameEvent.h"
#include <assert.h>
#pragma warning( disable : 4996 )
#include "../Source/PlayerDatabase.h"
#include "GameFramework.h"

using namespace Events;
//----------------------------------------------
//----------------------------------------------

GameEvent :: GameEvent () : Message (NoMessage)
{
}

GameEvent :: ~GameEvent ()
{
}

//----------------------------------------------

void				DebugTextEvent :: SetMessageText (const char* text) 
{
	int length = strlen (text);
	assert (length < MaxStringLength);
	strcpy (MessageText, text);
}

//----------------------------------------------

//----------------------------------------------
//----------------------------------------------

GameEventWrapper :: GameEventWrapper () 
{
	memset (BufferToHoldGenericData, 0, MAX_DATA_SIZE);
}

//----------------------------------------------

const  GameEvent*		GameEventWrapper :: GetEvent () const
{
	return reinterpret_cast <const GameEvent*> (BufferToHoldGenericData);
}

//----------------------------------------------

GameEventWrapper :: operator  GameEvent () const
{
	return *reinterpret_cast <const GameEvent*> (BufferToHoldGenericData);
}
//----------------------------------------------
//----------------------------------------------

GameEventQueue :: GameEventQueue () : std::queue <GameEventWrapper> ()
{
}

//----------------------------------------------

const void*	GameEventQueue :: Dequeue ()
{
	assert (size () > 0);
	const GameEventWrapper& wrapper = front ();
	pop ();
	
	return wrapper.GetEvent ();
}

//----------------------------------------------

int					GameEventQueue :: Count ()
{
	return size ();
}

//----------------------------------------------

int					GameEventQueue :: Capacity ()
{
	return MAX_ITEMS;
}

//----------------------------------------------

void				GameEventQueue :: Clear ()
{	
	while (this->empty () == false)
	{
		this->pop ();
	}
}

//----------------------------------------------

// this is meant to be a derived class only.
// it has nearly no behavior itself, but allows any I/O event detected
IOFiredPlugin :: IOFiredPlugin () {}

//----------------------------------------------
//----------------------------------------------

MessageSenderReceiver :: MessageSenderReceiver ()
{
}

//----------------------------------------------

MessageSenderReceiver :: ~MessageSenderReceiver ()
{
}

//----------------------------------------------

void	MessageSenderReceiver :: UpdateAllClients ()
{
	int NumMessages = SendQueue.Count ();
	for (int i=0; i<NumMessages; i++)
	{
		const GameEventWrapper* msg = reinterpret_cast <const GameEventWrapper*> (SendQueue.Dequeue ());
		std::list <MessageSenderReceiver*>::iterator it = Clients.begin ();
		while (it != Clients.end ())
		{
			MessageSenderReceiver* Client = *it++;
			TransportToClient (Client, msg);
		}
	}
	SendQueue.Clear ();
}

//----------------------------------------------

void	MessageSenderReceiver :: TransportToClient (MessageSenderReceiver* client,
													const GameEventWrapper* wrapper)
{
	client->ReceiveQueue.Enqueue (*wrapper);
}

//----------------------------------------------

void	MessageSenderReceiver :: AddClient (MessageSenderReceiver* Client)
{
	// first, check for duplicates
	bool FoundClient = false;
	std::list <MessageSenderReceiver*>::iterator it = Clients.begin ();
	while (it != Clients.end ())
	{
		if (*it ++ == Client)
		{
			FoundClient = true;
			break;
		}
	}
	if (FoundClient)
		return;
	
	Clients.push_back (Client);
}

//----------------------------------------------
//----------------------------------------------

/*
		DebugText,
		ApplyThrust,
		Maneuvers,
		GameModeChange,
		SwitchViewport,
		FireWeapon,
		CameraMode,
		PlayerStatus,
		PlayerChat,
		PingTime,
		CreateResource,
		CaptureResource,
		UI_ShipConfig,
		UI_MouseMove,
		UI_MouseButton,
		UI_MouseWheel,
		UI_Map,
		UI_MyStatus,
		QuitGame
*/
Events::GameEvent*  Events::GameEventFactory( EventMessages type, int datum1, int datum2, const char* text )
{
	switch( type )
	{
	case Events::DebugText:
		{
			Events::DebugTextEvent* dte = NULL;
			if( text )
			{
				dte = new Events::DebugTextEvent();
				dte->SetMessageText( text );
			}
			return dte;
		}
	case Events::ApplyThrust:
		{
			Events::ManeuverEvent* Man = new Events::ManeuverEvent();
			Man->SetManeuver (Events::ManeuverEvent::Thrust);
			return Man;
		}
	case Events::Maneuvers:
		{
			Events::ManeuverEvent* Man = new Events::ManeuverEvent();
			if( datum1 == KeyboardEvent_RotateCW )
			{
				Man->SetManeuver (Events::ManeuverEvent::TurnLeft);
			}
			else
			{
				Man->SetManeuver (Events::ManeuverEvent::TurnRight);
			}
			return Man;
		}
	case Events::GameModeChange:
		{
			Events::GameModeChangeEvent* gmc = NULL;
			if( Validate( (GameMode ) datum1 ) == true )
			{
				gmc = new Events::GameModeChangeEvent();
				gmc->SetMode( (GameMode) datum1 );
			}
			return gmc;
		}
	case Events::SwitchViewport:
		{
			Events::SwitchViewportEvent* Viewport = new Events::SwitchViewportEvent();
			if( datum1 == ShipViewType_ship )
			{
				Viewport->SetView( Events::SwitchViewportEvent::ShipView );
			}
			else
			{
				Viewport->SetView( Events::SwitchViewportEvent::StationView );
			}
			Viewport->SetViewIndex ( datum2 );
			return Viewport;
		}
	case Events::FireWeapon:
		{
			Events:: FireWeaponEvent* Event = new Events:: FireWeaponEvent();
			PlayerDatabase* playerdb = GlobalGameFramework->GetGameData().GetPlayerDatabase();
			Event->SetPlayerID (playerdb->GetCurrentPlayerID ());
			return Event;
		}
	case Events::UI_ShipConfig:
		{
			Events::UIShipConfigEvent* Event = new Events::UIShipConfigEvent();
			return Event;
		}
	case Events::UI_Map:
		{
			Events::UIMapEvent* Event = new Events::UIMapEvent();
			return Event;
		}
	case Events::UI_MyStatus:
		{
			Events::UIMyStatusEvent* Event = new Events::UIMyStatusEvent();
			return Event;
		}
	case Events::QuitGame:
		return new Events::GameQuitEvent();
	}

	return NULL;
}

//----------------------------------------------
//----------------------------------------------