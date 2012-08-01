/*
 *  SpaceMap.h
 *  Station05
 *
 *  Created by Mickey Kawick on 25/11/08.
 *  Copyright 2008-2012 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#ifndef __gl_h_
#include "../tools/SDL/include/SDL_opengl.h"
#include "../tools/GL/include/glut.h"
#endif
#include "../common/Math/vector.h"
#include "GeneralDataTypes.h"

class GameData;
//----------------------------------------------------

struct HudHex
{
	enum	CORNER 
	{
		LeftCorner, 
		LeftTopCorner, 
		RightTopCorner, 
		RightCorner, 
		RightBottomCorner, 
		LeftBottomCorner
	};
	Vector2D Corners [6];
	Vector2D Center;
	Vector2D VisualCenter;
	
	void	Setup (float x, float y, float DistanceToPoints);
	void	SetVisualCenter (float offsetx, float offsety);
	void	Draw (float offsetx, float offsety);
};

struct SDL_Surface;

//----------------------------------------------------

class SpaceMap
{
public:
	SpaceMap ();
	~SpaceMap ();
    void    Shutdown();

	void	SetColor (Vector color);
	void	SetFocusColor (Vector color);
	void	SetHighlightedColor (Vector color);
	void	SetConnectorColor (Vector color);
	
	void	SetScreenPosition (float Left, float Top, float Right, float Bottom);
	void	SetScreenDimensions (int Left, int Top, int Right, int Bottom, int StationSpacing);
	void	SetCameraPosition (Vector camera);
	void	SetConnectionPosition (Vector position);

	void	SetHighlighted (int region = 0);// range 0-NumHexesInMap
	
	// ----------------------------------------------------------------------
	
	void	Setup ();
	void	Draw ();
	void	Update (GameData& Data);
	
protected:
	
	enum	{NumHexesInMap = 4};
	void	DrawHex (int which, float offx, float offy);
	void	DrawFocusRectangle (float x, float y);
	void	DrawOutline ();
	void	DrawConnector ();
	
	Vector	Color;
	Vector	FocusColor;
	Vector  HighlightedColor;
	Vector  ConnectorColor;
	int		SelectedRegion;
	Vector	CameraPosition;
	Vector  ConnectonPosition;
	ScreenRect	ScreenPosition;
	ScreenRect	ScreenDimensions;
	float	Width, Height;
	float	StationSpacing;
	
	HudHex	Hexes[4];
	SDL_Surface* GL_Surface;
};

//----------------------------------------------------
