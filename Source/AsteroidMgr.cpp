/*
 *  AsteroidMgr.cpp
 *  Station05
 */

#include "stdafx.h"
#include <windows.h>
#include <list>
#include <math.h>
#include "../common/Math/mathfunctions.h"
#include "AsteroidMgr.h"
#include "asteroid.h"
#include "GameData.h"
#include "PlayerDatabase.h"

//------------------------------------------

AsteroidMgr :: AsteroidMgr () : AsteroidIDs (0)
{
	//Setup ();// open gl is not ready yet, do not do this.
}

AsteroidMgr :: ~AsteroidMgr ()
{
	std::list <Asteroid*>::iterator it = AsteroidField.begin ();
	
	while (it != AsteroidField.end ())
	{
		Asteroid* asteroid = *it++;
		delete asteroid;
	}
}

//------------------------------------------

Asteroid*		AsteroidMgr :: CreateAsteroid ()
{
	const float AvoidanceBall = 60;// as defined by the ball in analysis
	const float OutsideRange = 1060;// fairly low asteroid density

	const int		Clamp = 1000;
	// all of these should fall in the radial range of OutsideRange - AvoidanceBall	
	float	Range = OutsideRange - AvoidanceBall;
	float	RangeRecip = 1.0f/static_cast <float> (Clamp) * (Range);// the reciprical makes the math easier later
	
	float Angle = static_cast <float> (rand ()%720) / 2.0F;
	float Distance = (static_cast <float> (rand ()%Clamp) * RangeRecip) + AvoidanceBall;
	
	float	x = sin (ConvertToRadians (Angle)) * Distance;
	float	y = cos (ConvertToRadians (Angle)) * Distance;
	float	z = -2; // a little behind other things

	return CreateAsteroid ( x, y, z, Asteroid::Spikey );
}

Asteroid*		AsteroidMgr :: CreateAsteroid ( float x, float y, float z, Asteroid::Shape shape )
{
	int AngularVelocity = 20;
	float AngularMultiplier = 0.0001F;
	float rx = (rand ()%AngularVelocity - (AngularVelocity/2)) * AngularMultiplier;
	float ry = (rand ()%AngularVelocity - (AngularVelocity/2)) * AngularMultiplier;
	float rz = (rand ()%AngularVelocity - (AngularVelocity/2)) * AngularMultiplier;
	Vector Color; 
	Color.r = static_cast <float> (rand () % 100)/200.0F + 0.4F;
	Color.g = static_cast <float> (rand () % 100)/200.0F + 0.4F;
	Color.b = static_cast <float> (rand () % 100)/200.0F + 0.4F;
	
	float Size = static_cast <float> (rand () % 100) * 0.019F + 0.1F;// 0.1 to 2
	
	Asteroid* asteroid = new Asteroid (Size, shape);
	
	asteroid->SetCenter (Vector (x, y, z));
	asteroid->SetColor (Color);
	asteroid->SetAngleOfRotation (Vector (rx, ry, rz));

	asteroid->Setup();
	
	return asteroid;
}

void		AsteroidMgr :: DeleteAsteroid( int id )
{
	std::list <Asteroid*>::iterator it = AsteroidField.begin ();
	
	while (it != AsteroidField.end ())
	{
		Asteroid* asteroid = *it;
		if( asteroid->GetID() == id )
		{
			AsteroidField.erase( it );
			delete asteroid;
			return;
		}
		++it;
	}
}
//------------------------------------------

// we probably want to tie this into the stellarcartography so as to create
// areas for players to startup. Then each hex could be a spawn point for new asteroids
void		AsteroidMgr :: Setup ()
{
	for (int i=0; i<NumDefaultAsteroids; i++)
	{
		Asteroid* as = CreateAsteroid();
		as->SetID (AsteroidIDs++);// just id tracking
		AsteroidField.push_back (as);
	}

	Asteroid* as = CreateAsteroid( 3, 9, -2 );
	as->SetID (AsteroidIDs++);// just id tracking
	AsteroidField.push_back (as);
}

//------------------------------------------

void		AsteroidMgr :: Update (GameData& GlobalGameData)
{
	std::list <Asteroid*>::iterator it = AsteroidField.begin ();
	
	while (it != AsteroidField.end ())
	{
		Asteroid* asteroid = *it++;
		asteroid->Update (GlobalGameData);
	}
	ProcessMessages (GlobalGameData);
	UpdateAllClients ();
}

//------------------------------------------

void		AsteroidMgr :: Draw ()
{
	std::list <Asteroid*>::iterator it = AsteroidField.begin ();
	
	while (it != AsteroidField.end ())
	{
		Asteroid* asteroid = *it++;
		asteroid->Draw ();
	}
}

//------------------------------------------
//------------------------------------------

void		AsteroidMgr :: CheckForDamage (GameData& data)
{
}

//------------------------------------------
void		AsteroidMgr :: ProcessMessages (GameData& GlobalGameData)//inherited, send all messages to the appropriate players
{	
	int NumMessages = ReceiveQueue.Count ();
	for (int i=0; i<NumMessages; i++)
	{
		const Events::GameEvent* msg = reinterpret_cast <const Events::GameEvent*> (ReceiveQueue.Dequeue());
		switch (msg->GetType())
		{
			case Events:: CreateResource:
			{
				const Events::CreateResourceNodeEvent* cr = reinterpret_cast <const Events::CreateResourceNodeEvent*>( msg );
				/*cr->
				if( cr->GetAsteroidId() )
				{
					DeleteAsteroid( cr->GetAsteroidId() );// for now
				}*/
			}
			break;
		default:
			break;
		}
	}
	
}

//-------------------------------------------
