/*
 *  ResourceManager.h
 *  Station05
 */
#pragma once
#include "GameEvent.h"
//#include 

class StellarResource;
struct AssetObject;

class ResourceManager : public Events:: MessageSenderReceiver
{
public:
	ResourceManager();
	~ResourceManager();
    void    Shutdown();

	void	AddResource( ResourceTypes type, float amount, const Vector& location );

	void	Draw();
	void	Update (GameData& GlobalGameData);
	void	PostDrawCleanup ();
	void	ProcessMessages (GameData& data);

	Vector	GetResourceColor( ResourceTypes type );

protected:
	void	GetVelocityOfNewResource( Vector& velocity, Vector& deceleration ) const;
	std::list <StellarResource*>	Resources;
};
