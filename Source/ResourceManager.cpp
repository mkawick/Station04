/*
 *  ResourceManager.cpp
 *  Station05
 */

#include "StdAfx.h"
#include <windows.h>
#include "uuid.h"
#include "ResourceManager.h"
#include "vector.h"
#include "gamedata.h"
#include "StellarResource.h"
#include "AssetLoadManager.h"
#include "GameFramework.h"

//#include "player.h"
//#include "PlayerDatabase.h"
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
void	ResourceManager :: AddResource( ResourceTypes type, float amount, const Vector& location )
{
	StellarResource* resource = new StellarResource();
	resource->SetCenter( location );
	resource->SetResource( type );
	resource->SetQuantity( amount );
	resource->SetAngle( Vector( 0, 0, 180 ) );// loaded assets come in upside-down

	AssetObject* asset = GlobalGameFramework->GetAssets().FindObject( "cone" );
	resource->SetAsset( asset );

	Resources.push_back( resource );
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
				Resources.remove( cr->Get() );

			}
			break;
		}
	}
}

//---------------------------------------------------------
//---------------------------------------------------------