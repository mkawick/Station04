#include "StdAfx.h"
#include <boost/lexical_cast.hpp>
#include <windows.h>
#include "PlayerShipDataView.h"
#include "../Common/UUID.h"
#include "../Source/GameFramework.h"
#include "../Source/PlayerDatabase.h"
#include "../Source/Player.h"

//typedef boost::lexical_cast<std::string, float> FloatToString;

const char*		PlayerShipDataView::DataViewName = "Player.Ship";
PlayerShipDataView::PlayerShipDataView(void)
{
}

PlayerShipDataView::~PlayerShipDataView(void)
{
}

int		PlayerShipDataView :: GetNumRows() const
{
	PlayerDatabase* playerdb = GlobalGameFramework->GetGameData().GetPlayerDatabase();

	UUID playerId = playerdb->GetCurrentPlayerID();
	Player* p = GlobalGameFramework->GetGameData().GetPlayerDb()-> GetPlayer( playerId );
	
	ShipArchetype* ship = p->GetShip();
	if( ship )
		return 1;
	return 0;
}

bool	PlayerShipDataView :: GetColumnName( int column, std::string& name ) const
{
	switch( column )
	{
	case Column_ShipName:			name = "shipname"; return true;
	case Column_Type:				name = "type"; return true;
	case Column_ShipShield1:		name = "shield1"; return true;
	case Column_ShipShield2:		name = "shield2"; return true;
	case Column_ShipShield3:		name = "shield3"; return true;
	case Column_Speed:				name = "speed"; return true;
	case Column_Direction:			name = "direction"; return true;
	case Column_Health:				name = "health"; return true;
	}
	return false;
}

bool	PlayerShipDataView :: GetData( int row, int column, std::string& data ) const
{
	Player* pPlayer = GlobalGameFramework->GetGameData().GetPlayerDatabase()->GetCurrentPlayer();
	if( pPlayer == NULL )
		return false;

	ShipArchetype* pShip = pPlayer->GetShip();
	if( pShip == NULL )
		return false;

	switch( row )
	{
		case Column_ShipName:
			data = pShip->GetName();
			return true;
		case Column_Type:
			data = pShip->GetName();
			return true;	
		case Column_ShipShield1:
			data = pShip->GetShieldLevel( 0 );
			return true;
		case Column_ShipShield2:
			data = pShip->GetShieldLevel( 1 );
			return true;
		case Column_ShipShield3:
			data = pShip->GetShieldLevel( 2 );
			return true;
		case Column_Speed:
			data = boost::lexical_cast<std::string>( pShip->GetVelocity().Magnitude() );
			return true;	
		case Column_Direction:
			data = boost::lexical_cast<std::string>( pShip->GetAngle().Magnitude() );
			return true;	
		case Column_Health:
			data = pShip->GetHealth();
			return true;
	}

	return false;
}