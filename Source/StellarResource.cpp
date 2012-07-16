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
}

void StellarResource::Draw()
{
	if( Asset )
	{
		Asset->Render( Center.x, Center.y, Center.z, Angle.x, Angle.y, Angle.z, 1 );
	}
}
/*
StellarResource::~StellarResource()
{
}*/
