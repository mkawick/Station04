/*
 *  GameFramework.h
 *  Station05
 */

#pragma once

#include <math.h>

#include "../tools/SDL/include/SDL.h"
//#include "../tools/SDL/include/SDL_keysym.h"

#include "../common/Math/vector.h"
#include "../tools/SpacePartitioning/Quadtree.h"
#include "SpaceStation.h"

//#include "room.h"
#include "starfield.h"
#include "viewport.h"
#include "spacemap.h"
#include "gazelle.h"
#include "projectilemgr.h"
#include "audiomanager.h"
#include "networkmanager.h"
#include "AsteroidMgr.h"
#include "InputManager.h"
#include "DebugText.h"
#include "GameData.h"
#include "AssetLoadManager.h"
#include "ResourceManager.h"
#include "UI_Framework.h"

//----------------------------------------------

class GameFramework : public Events::MessageSenderReceiver
{
public:
	GameFramework (int ScreenWidth, int ScreenHeight);
	~GameFramework();

	//---------------------------------------
	
	void	MainLoop ();// once entered, there is no return, except to end
	void	PrintText (const char* text);
	
	//---------------------------------------

	UUID				GetLocalPlayer() const;
	
	Quadtree&			GetSpacePartition() { return SpacePartition; }
	AssetLoadManager&	GetAssets() { return assetLoader; }	
	GameData&			GetGameData() { return GlobalGameData; }
	InputManager&		GetInput() { return Input; }
	ResourceManager&	GetResourceManager() { return spaceResources; }

protected:
	void	Init ();
	void	SetupGraphics ();
	void	Shutdown();

	void	Update ();
	void	KeyDetect ();
	void	Draw ();
	void	ProcessMessages (GameData& data);
	void	SetupMessagingTopology ();
	
	void	DrawString (const char* str);
	void	SetupToDrawStrings ();
	void	CleanUpFromStrings ();


	//---------------------------------------
	
protected:
	void	DrawAllObjects ();
	void	RenderInstructions();
protected:
	SDL_Surface *		DrawingContext;
	int					ScreenWidth, ScreenHeight;
	bool				AwaitingExit;
	float				DistanceFromStations;
	UUID				CurrentPlayer;
	
	Vector				CameraPosition;
	Vector				CameraAngle;
	Vector				DirectionalLightPosition;
	
	GameData			GlobalGameData;
	StarField			starfield;
	Viewport			viewport;
	SpaceMap			spacemap;
	ProjectileMgr		projectiles;
	AudioManager		audio;
	NetworkManager  	network;
	AsteroidMgr			asteroids;
	InputManager		Input;
	DebugText			TextOut;
	ResourceManager		spaceResources;

	//-------------- testing only ---------------------
	UI_Framework		ui;
	//-------------- testing only ---------------------

	AssetLoadManager	assetLoader;
	Quadtree			SpacePartition;
};

//----------------------------------------------

extern GameFramework* GlobalGameFramework;