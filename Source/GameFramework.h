/*
 *  GameFramework.h
 *  Station05
 */

#pragma once

#include <SDL/SDL.h>
#include "vector.h"
#include "SpaceStation.h"
#include <math.h>
#include <SDL/SDL_keysym.h>
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
#include "../tools/SpacePartitioning/Quadtree.h"

//----------------------------------------------

class GameFramework : public Events::MessageSenderReceiver
{
public:
	GameFramework (int ScreenWidth, int ScreenHeight);
	
	//---------------------------------------
	
	void	MainLoop ();// once entered, there is no return, except to end
	void	PrintText (const char* text);
	
	//---------------------------------------
	
	Quadtree& GetSpacePartition() { return SpacePartition; }
	AssetLoadManager& GetAssets() { return AssetLoader; }
	UUID	GetLocalPlayer();

protected:
	void	Init ();
	void	SetupGraphics ();
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
	float				ScreenWidth, ScreenHeight;
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
	ResourceManager		SpaceResources;

	AssetLoadManager	AssetLoader;
	Quadtree			SpacePartition;
};

//----------------------------------------------

extern GameFramework* GlobalGameFramework;