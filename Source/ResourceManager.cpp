/*
 *  ResourceManager.cpp
 *  Station05
 */

#include "StdAfx.h"
#include <windows.h>
#include "../common/uuid.h"
#include "ResourceManager.h"
#include "../common/Math/vector.h"
#include "gamedata.h"
#include "StellarResource.h"
#include "AssetLoadManager.h"
#include "GameFramework.h"
#include "../Common/Math/MathFunctions.h"

#include "player.h"
#include "PlayerDatabase.h"
//#include "shiparchetype.h"

//---------------------------------------------------------

ResourceManager :: ResourceManager ()
{
	//Resources.push_back (PhaseBolt);
}
ResourceManager :: ~ResourceManager ()
{
	std::list <StellarResource*>::iterator it = Resources.begin();
	while( it != Resources.end() )
	{
		StellarResource* obj = *it++;
		delete obj;
	}
}

//---------------------------------------------------------

void	ResourceManager :: Draw ()
{
	std::list <StellarResource*> :: iterator it = Resources.begin ();
	while (it != Resources.end ())
	{
		(*it)->Draw ();
		++it;
	}
}

//---------------------------------------------------------

void	ResourceManager :: Update (GameData& GlobalGameData)
{
	GameData emptyData;
	int num = Resources.size();
	std::list <StellarResource*> :: iterator it = Resources.begin ();
	while (it != Resources.end ())
	{
		(*it)->Update ( emptyData );
		++it;
	}
	ProcessMessages (GlobalGameData);
	UpdateAllClients ();
}

//---------------------------------------------------------

void	ResourceManager :: PostDrawCleanup ()
{
	std::list <StellarResource*> :: iterator it = Resources.begin ();
	while (it != Resources.end ())
	{
		//(*it)->PostDrawCleanup ();
		++it;
	}
}

//---------------------------------------------------------
void	ResourceManager :: AddResource( ResourceTypes resourceType, float amount, const Vector& location )
{
	AssetObject* asset = NULL;

	switch( resourceType )
	{
	case ResourceType_Iron:
		asset = GlobalGameFramework->GetAssets().FindObject( "iron" );
		break;
	case ResourceType_DropiumCrystal:
		asset = GlobalGameFramework->GetAssets().FindObject( "dropsium" );
		break;
	case ResourceType_Hydrogen:
		asset = GlobalGameFramework->GetAssets().FindObject( "hydrogen" );
		break;
	case ResourceType_Aluminium:
		asset = GlobalGameFramework->GetAssets().FindObject( "aluminium" );
		break;
	case ResourceType_Copper:
		asset = GlobalGameFramework->GetAssets().FindObject( "copper" );
		break;
	}
	if( asset == NULL )
		return;

	StellarResource* resource = new StellarResource();
	resource->SetCenter( location );
	resource->SetResource( resourceType );
	resource->SetQuantity( amount );
	resource->SetAngle( Vector( 0, 0, 180 ) );// loaded assets come in upside-down

	Vector velocity, deceleration;
	GetVelocityOfNewResource( velocity, deceleration );
	resource->SetVelocity( velocity );
	resource->SetAcceleration( deceleration );

	float rotationSpeed = static_cast<float> (rand() % 12 ) * 0.01f + 0.05f;
	Random2dVector( velocity, rotationSpeed );
	resource->SetAngleOfRotation( velocity );

	resource->SetAsset( asset );

	Resources.push_back( resource );
}

void	ResourceManager :: GetVelocityOfNewResource( Vector& velocity, Vector& deceleration ) const
{
	float speed = static_cast<float> (rand() % 12 ) * 0.01f + 0.05f;
	Random2dVector( velocity, speed );
	deceleration = velocity * -0.01f;// 1% speed loss
}

//---------------------------------------------------------
void			ResourceManager :: ProcessMessages (GameData& GlobalGameData)//inherited, send all messages to the appropriate players
{	
	int NumMessages = ReceiveQueue.Count ();
	for (int i=0; i<NumMessages; i++)
	{
		const Events::GameEvent* msg = reinterpret_cast <const Events::GameEvent*> (ReceiveQueue.Dequeue());
		switch (msg->GetType())
		{
			case Events :: CreateResource:
			{
				const Events::CreateResourceNodeEvent* cr = reinterpret_cast <const Events::CreateResourceNodeEvent*> (msg);
				AddResource( cr->GetType(), cr->GetAmount(), cr->GetLocation() );
			}
			break;

			case Events :: CaptureResource:
			{
				const Events::CaptureResourceEvent* cr = reinterpret_cast <const Events::CaptureResourceEvent*> (msg);
				Player*	player = GlobalGameData.GetPlayerDb()->GetPlayer( cr->GetPlayerId() );
				if( player )
				{
					ResourceTypes type = cr->Get()->GetResource();
					float quantity = cr->Get()->GetQuantity();
					player->AddResources( type, quantity );
				}
				Resources.remove( cr->Get() );

			}
			break;
		}
	}
}

//---------------------------------------------------------
//---------------------------------------------------------