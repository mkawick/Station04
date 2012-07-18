/*
 *  GlobalDefinitions.h
 *  Station05
 */
#pragma once

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
	GameMode_Attract,
	GameMode_Login,
	GameMode_SelectAccount,
	GameMode_Game,
	GameMode_Logout,
	GameMode_High_score
};

enum KeyboardEvent
{
	KeyboardEvent_RotateCW,
	KeyboardEvent_RotateCCW,
	KeyboardEvent_FireMain,
	KeyboardEvent_Thrust
};