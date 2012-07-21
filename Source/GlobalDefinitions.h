/*
 *  GlobalDefinitions.h
 *  Station05
 */
#pragma once
#include "../Common/struct.h"

const U32 InvalidDrawList = 0xFFFFFFFF;

enum ResourceTypes
{
	ResourceType_None,
	ResourceType_Iron,
	ResourceType_DropiumCrystal, // used for energy bonding
	ResourceType_Hydrogen, // used for straight energy
	ResourceType_Aluminium,
	ResourceType_Copper,
	ResourceTypes_Count
};

enum TechTree
{
};

enum GameMode
{
	GameMode_none,
	GameMode_Attract,
	GameMode_Login,
	GameMode_SelectAccount,
	GameMode_Game,
	GameMode_Logout,
	GameMode_HighScore,
	GameMode_count
};

GameMode LookupGameMode( const char* modeName );

enum KeyboardEvent
{
	KeyboardEvent_RotateCW,
	KeyboardEvent_RotateCCW,
	KeyboardEvent_FireMain,
	KeyboardEvent_Thrust
};

enum KeyModifier
{
	KeyModifier_none = 0,
	KeyModifier_Ctrl = 1 << 0,
	KeyModifier_Alt  = 1 << 1,
	KeyModifier_Shift= 1 << 2
};

U32 LookupKeyModifier( const char* KeyModifiers ); // separated by spaces, commas, ors
