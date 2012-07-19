/*
 *  InputManager.h
 *  Station05
 *
 */

#pragma once
#include "gameevent.h"
#include <list>
#include <hash_map>

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


struct KeyMapping2
{
	KeyMapping2() : keyPress ( SDLK_UNKNOWN ), event( 0 ), selectionData( 0 ), allowHold( 0 ), maxRepeatRate( 0 )
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

typedef std::list< KeyMapping2 > KeySet;
typedef std::list< KeyMapping2 >::iterator KeySetIter;

typedef std::pair <GameMode, KeySet> GameModeKeyMappingPair;
typedef stdext::hash_map < GameMode, KeySet > GameModeKeySet;
typedef GameModeKeySet::iterator GameModeKeySetIter;

class InputManager2 : public Events:: MessageSenderReceiver
{
public:
	InputManager2 ();

	void	AddKeyMapping( GameMode mode, const char* key, const char* event, const char* typeData, bool allowHold, int maxRepeatRate, U32 modifiers, int selectionData = 0 );
	void	SetGameMode( GameMode mode );// mode controled by string
	
	void	Update (GameData& restrict); // data
	
private:
	void	ProcessMessages (GameData& data);
	bool	HandleKeyboard (GameData& data);
	
	GameMode currentGameMode;
	GameModeKeySet keyboardSetup;
};

// ----------------------------------------------------
