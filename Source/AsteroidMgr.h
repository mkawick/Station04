/*
 *  AsteroidMgr.h
 *  SDL04_Station
 */
#include <list>
#include "asteroid.h"
#include "GameEvent.h"

class GameData;

//------------------------------------------

class AsteroidMgr : public Events:: MessageSenderReceiver
{
public:
	AsteroidMgr ();
	~AsteroidMgr ();
	
	//---------------------------------------
	
	void			Setup ();
	void			Update (GameData& data);
	void			Draw ();
	void			PostDrawCleanup ();
	
	//---------------------------------------
	
protected:
	enum			{NumDefaultAsteroids = 800}; 
	
	void			ProcessMessages (GameData& GlobalGameData);//inherited, send all messages to the appropriate players
	void			CheckForDamage (GameData& data);
	void			ProcessHit (int ID);
	
	Asteroid*		CreateAsteroid ();
	Asteroid*		CreateAsteroid ( float x, float y, float z, Asteroid::Shape shape = Asteroid::Spikey );// utility function
	void			DeleteAsteroid( int id );

	int						AsteroidIDs;
	std::list <Asteroid*>	AsteroidField;
};

//------------------------------------------