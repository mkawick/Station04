/*
 *  ResourceManager.h
 *  SDL04_Station
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

	void	AddResource( ResourceTypes type, float amount, const Vector& location );

	void	Draw();
	void	Update (GameData& GlobalGameData);
	void	PostDrawCleanup ();
	void	ProcessMessages (GameData& data);

protected:
	std::list <StellarResource*>	Resources;
};
