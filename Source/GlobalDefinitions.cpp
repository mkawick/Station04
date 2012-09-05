/*
 *  GlobalDefinitions.cpp
 *  Station05
 */

#include "stdafx.h"
#include "GlobalDefinitions.h"
#include <string.h>
#include <boost/algorithm/string.hpp>
#include <vector>

GameMode LookupGameMode( const char* modeName )
{
	if( stricmp( modeName, "attract" ) == 0 )		return GameMode_Attract;
	if( stricmp( modeName, "login" ) == 0 )			return GameMode_Login;
	if( stricmp( modeName, "selectaccount" ) == 0 ) return GameMode_SelectAccount;
	if( stricmp( modeName, "game" ) == 0 )			return GameMode_Game;
	if( stricmp( modeName, "logout" ) == 0 )		return GameMode_Logout;
	if( stricmp( modeName, "highscore" ) == 0 )		return GameMode_HighScore;
	return GameMode_none;
}

bool	 Validate( GameMode mode )
{
	if( GameMode_none < mode && mode < GameMode_count )
		return true;
	return false;
}

U32 LookupKeyModifier( const char* KeyModifiers )
{
	std::vector<std::string> stringArray;
	boost::split( stringArray, KeyModifiers, boost::is_any_of("\t ,|"));

	U32 keyMods = KeyModifier_none;
	std::vector<std::string>::iterator it = stringArray.begin();
	while( it != stringArray.end() )
	{
		std::string		modifier = *it++;
		boost::algorithm::to_lower( modifier );
 
		if( modifier == "ctrl" ) keyMods |= KeyModifier_Ctrl;
		if( modifier == "alt" ) keyMods |= KeyModifier_Ctrl;
		if( modifier == "shift" ) keyMods |= KeyModifier_Ctrl;
	}

	return keyMods;
}


U32 LookupShipViewType( const char* viewName )
{
	if( stricmp( viewName, "ship" ) == 0 )			return ShipViewType_ship;
	if( stricmp( viewName, "station" ) == 0 )		return ShipViewType_station;
	if( stricmp( viewName, "ship_cockpit" ) == 0 )	return ShipViewType_ship_cockpit;

	return ShipViewType_none;
}

U32 LookupStatusBinding( const char* bindingName )
{
	if( stricmp( bindingName, "ship" ) == 0 )				return UI_StatusBinding_ship;
	if( stricmp( bindingName, "station" ) == 0 )			return UI_StatusBinding_station;
	if( stricmp( bindingName, "resource" ) == 0 )			return UI_StatusBinding_resource;
	if( stricmp( bindingName, "production" ) == 0 )		return UI_StatusBinding_production;
	if( stricmp( bindingName, "experience" ) == 0 )		return UI_StatusBinding_experience;
	if( stricmp( bindingName, "experiencelevel" ) == 0 )	return UI_StatusBinding_experience_level;
	if( stricmp( bindingName, "territorycontrol" ) == 0 )	return UI_StatusBinding_territory_control;

	return UI_StatusBinding_none;
}