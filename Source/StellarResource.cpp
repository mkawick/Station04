/*
 *  StellarResource.cpp
 *  Station05
 */
#include "StdAfx.h"
#include "StellarResource.h"
#include "AssetLoadManager.h"

StellarResource::StellarResource() : StellarObject (),
									Resource( ResourceType_None ),
									Quantity( 0 ),
									Asset( NULL )
{
	Partitioning.collisionFlag = CollisionFlags_Resource;
	Partitioning.obj = this;
}

StellarResource::~StellarResource ()
{
}

void StellarResource::Draw()
{
	if( Asset )
	{
		Asset->Render( Center.x, Center.y, Center.z, Angle.x, Angle.y, Angle.z, 1 );
	}
}

/*void StellarResource::Update ( GameData& data);
{
	StellarObject::Update( data );
}*/
/*
StellarResource::~StellarResource()
{
}*/
