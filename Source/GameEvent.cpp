/*
 *  GameEvent.cpp
 *  Station05
 */

#include "stdafx.h"
#include <windows.h>
#include "GameEvent.h"
#include <assert.h>
#pragma warning( disable : 4996 )

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


