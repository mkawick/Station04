/*
 *  InputManager.h
 *  SDL04_Station
 *
 */

#pragma once
#include "gameevent.h"

class InputManager;
// ----------------------------------------------------

class KeyboardInputBehavior
{
public:
	enum	FiresBehavior 
	{
		OnDown, 
		WhileDown, 
		OnUp,
		WhileUp
	};
	enum	{NumPossibleKeys = 4}; 
	
	// -----------------------
	
	KeyboardInputBehavior ();
	KeyboardInputBehavior (const KeyboardInputBehavior& KeyInput);
	~KeyboardInputBehavior ();
	
	void	Clear ();// does not clear the EventToFire
	void	AddKeyCode (FiresBehavior, SDLKey, SDLMod);
	void	SetEventToFire (Events::IOFiredPlugin*);// we will own and delete it
	void	SetGapTimeBetweenRepeats (unsigned long TimeInMilliseconds) {GapTimeBetweenFirings = TimeInMilliseconds, LastTimeFired = 0;}
	
	// -----------------------
	
	void	operator = (const KeyboardInputBehavior& KeyInput);
	bool	operator == (const KeyboardInputBehavior& KeyInput) const;
	
	// -----------------------
	
	void	UpdateKeyUp (SDL_Event* KeyboardEvents, int NumEvents, GameData& data);
	void	UpdateKeyDown (SDL_Event* KeyboardEvents, int NumEvents, GameData& data);
	
	void	Update (GameData& data, Events:: MessageSenderReceiver& sender);// for no-event changes

	// -----------------------
private:
	int		FindKeyCode (SDLKey) const;
	// -----------------------
private:
	
	// packed according to size
	Events::IOFiredPlugin*			EventToFire;
	FiresBehavior	KeyBehavior		[NumPossibleKeys];
	SDLKey			KeyCodes		[NumPossibleKeys];
	SDLMod			KeyMods			[NumPossibleKeys];
	unsigned long	GapTimeBetweenFirings;	// only applies to a repeating held key
	unsigned long	LastTimeFired;			// ditto
	
	unsigned char	NumKeys;
	bool			IsKeyHeldDown	[NumPossibleKeys];// only used for those that fire continuously when depressed (or possibly not pressed)
	bool			FiredDown		[NumPossibleKeys];// tracking state	
	bool			FiredUp			[NumPossibleKeys];
};

// ----------------------------------------------------

class	KeyMapper
{
public:
	KeyMapper ();
	~KeyMapper ();
	
	void	Clear ();
	
	bool	AddInputBehavior (KeyboardInputBehavior* input);
	void	RemoveInputBehavior (KeyboardInputBehavior* input);
	
	void	ProcessAllKeys (GameData& data, 
							Events:: MessageSenderReceiver& sender);
public:
	std::list <KeyboardInputBehavior*> KeyEvents;
};

// ----------------------------------------------------

class InputManager : public Events:: MessageSenderReceiver
{
public:
	InputManager ();
	
	void	Update (GameData& restrict); // data
	
private:
	void	ProcessMessages (GameData& data);
	bool	HandleKeyboard (GameData& data);
	
	KeyMapper	Mapper;
};

// ----------------------------------------------------
