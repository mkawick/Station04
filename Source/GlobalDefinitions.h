/*
 *  GlobalDefinitions.h
 *  Station05
 */
#pragma once
#include "../Common/struct.h"

const U32 InvalidDrawList = 0xFFFFFFFF;

enum ResourceTypes
{
	ResourceType_None = -1,
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
bool	 Validate( GameMode );

enum KeyboardEvent
{
	KeyboardEvent_none,
	KeyboardEvent_RotateCW,
	KeyboardEvent_RotateCCW,
	KeyboardEvent_FireMain,
	KeyboardEvent_Thrust,
	KeyboardEvent_count
};

enum KeyModifier
{
	KeyModifier_none = 0,
	KeyModifier_Ctrl = 1 << 0,
	KeyModifier_Alt  = 1 << 1,
	KeyModifier_Shift= 1 << 2,
	KeyModifier_count = 4
};

U32 LookupKeyModifier( const char* KeyModifiers ); // separated by spaces, commas, ors

enum ShipViewType
{
	ShipViewType_none,
	ShipViewType_ship,
	ShipViewType_station,
	ShipViewType_ship_cockpit,
	ShipViewType_count
};

U32 LookupShipViewType( const char* viewName );

enum	UI_StatusBinding
{
	UI_StatusBinding_none,
	UI_StatusBinding_ship,
	UI_StatusBinding_station,
	UI_StatusBinding_resource,
	UI_StatusBinding_production,
	UI_StatusBinding_experience,
	UI_StatusBinding_experience_level,
	UI_StatusBinding_territory_control,
	UI_StatusBinding_count
};

U32 LookupStatusBinding( const char* bindingName );

enum	UI_StatusFieldBinding
{
	UI_StatusFieldBinding_none,
	UI_StatusFieldBinding_shield,
	UI_StatusFieldBinding_health,
	UI_StatusFieldBinding_regeneration,
	UI_StatusFieldBinding_armor,
	UI_StatusFieldBinding_repair_rate,
	//UI_StatusFieldBinding_production_level,
	UI_StatusFieldBinding_value, // applies to experience, production, resource, etc
	UI_StatusFieldBinding_rate, // applies to production, healing, etc.
	UI_StatusFieldBinding_count
};

U32 LookupStatusFieldBinding( const char* bindingName );
