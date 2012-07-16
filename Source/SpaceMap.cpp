/*
 *  SpaceMap.cpp
 *  Station05
 *
 *  Created by Mickey Kawick on 25/11/08.
 *  Copyright 2008-2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include <windows.h>
#include "SpaceMap.h"
#include "../tools/SDL/include/SDL_Video.h"
#include <math.h>
#include <assert.h>
#include "GameData.h"
#include "player.h"
#include "playerdatabase.h"
#include "ShipArchetype.h"
#include "../common/Math/MathFunctions.h"

//----------------------------------------------------------------

void	HudHex:: Setup (float x, float y, float DistanceToPoints)
{
	Center.Set (x, y);
	float LeftMostX = x - DistanceToPoints;
	float LeftMostY = y;
	float RightMostX = x + DistanceToPoints;
	float RightMostY = y;
	
	float SixtyDegrees = M_PI / 3;// 60∘
	float HeightOver2 = sin (SixtyDegrees) * DistanceToPoints;
	float WidthOver2 = cos (SixtyDegrees) * DistanceToPoints;
	
	Corners[LeftCorner].x = LeftMostX;
	Corners[LeftCorner].y = LeftMostY;
	Corners[LeftTopCorner].x = x - WidthOver2;
	Corners[LeftTopCorner].y = y - HeightOver2;
	Corners[RightTopCorner].x = x + WidthOver2;
	Corners[RightTopCorner].y = y - HeightOver2;
	
	Corners[RightCorner].x = RightMostX;
	Corners[RightCorner].y = RightMostY;
	Corners[RightBottomCorner].x = x + WidthOver2;
	Corners[RightBottomCorner].y = y + HeightOver2;
	Corners[LeftBottomCorner].x = x - WidthOver2;
	Corners[LeftBottomCorner].y = y + HeightOver2;
}

//----------------------------------------------------------------

void	HudHex :: Draw (float x, float y)
{
	x = -x;
	y = y;
	glBegin (GL_LINE_LOOP);	
		glVertex3f (Corners[LeftCorner].x+x, Corners[LeftCorner].y+y, 0);
		glVertex3f (Corners[LeftTopCorner].x+x, Corners[LeftTopCorner].y+y, 0);
		glVertex3f (Corners[RightTopCorner].x+x, Corners[RightTopCorner].y+y, 0);
		glVertex3f (Corners[RightCorner].x+x, Corners[RightCorner].y+y, 0);
		glVertex3f (Corners[RightBottomCorner].x+x, Corners[RightBottomCorner].y+y, 0);
		glVertex3f (Corners[LeftBottomCorner].x+x, Corners[LeftBottomCorner].y+y, 0);		
	glEnd ();
}

//----------------------------------------------------------------

void	HudHex :: SetVisualCenter (float x, float y)
{
	VisualCenter.Set (Center.x-x, Center.y+y);// note negative sign
}

//----------------------------------------------------------------

SpaceMap :: SpaceMap () : SelectedRegion (0), GL_Surface(0L)
{
}

//----------------------------------------------------------------

SpaceMap :: ~SpaceMap ()
{
	if (GL_Surface != 0L)
	{
		SDL_FreeSurface (GL_Surface);
	}
}

//----------------------------------------------------------------

void	SpaceMap :: SetColor (Vector color)
{
	Color = color;
}

//----------------------------------------------------------------

void	SpaceMap :: SetHighlightedColor (Vector color)
{
	HighlightedColor = color;
}

//----------------------------------------------------------------

void	SpaceMap :: SetConnectorColor (Vector color)
{
	ConnectorColor = color;
}

//----------------------------------------------------------------

void	SpaceMap :: SetFocusColor (Vector color)
{
	FocusColor = color;
}

//----------------------------------------------------------------

void	SpaceMap :: SetScreenDimensions (float Left, float Top, float Right, float Bottom, float ss)
{
	ScreenDimensions.Corners[0].x = Left;
	ScreenDimensions.Corners[0].y = Top;
	ScreenDimensions.Corners[1].x = Right;
	ScreenDimensions.Corners[1].y = Bottom;
	StationSpacing = ss;
}

//----------------------------------------------------------------

void	SpaceMap :: SetScreenPosition (float l, float t, float r, float b)
{
	assert (ScreenDimensions.Corners[1].x != 0);// must set the dimensions first
	
	ScreenPosition.Corners[0].x = l, ScreenPosition.Corners[0].y = t, 
	ScreenPosition.Corners[1].x = r, ScreenPosition.Corners[1].y = b;

	Height = ScreenDimensions.Corners[1].y - ScreenDimensions.Corners[0].x;
	float CenterVertical = Height / 2;
	Width = ScreenDimensions.Corners[1].x - ScreenDimensions.Corners[0].x;
	float CenterHorizontal = Width / 2;	

	float ThirtyDegrees = M_PI / 6;// 30∘
	float HeightBetweenCenters = sin (ThirtyDegrees) * StationSpacing;// should divide by 2, optimized
	float WidthBetweenCenters = cos (ThirtyDegrees) * StationSpacing;
	
	const float Sqrt3 = sqrt (3.0);// think of a 30-60-90 triangle
	float DistanceFromCenterToCornerPoints = (StationSpacing / Sqrt3);
	
	Hexes [0].Setup (CenterHorizontal, CenterVertical, DistanceFromCenterToCornerPoints);
	Hexes [1].Setup (CenterHorizontal, CenterVertical+ StationSpacing, DistanceFromCenterToCornerPoints);	
	Hexes [2].Setup (CenterHorizontal+WidthBetweenCenters, CenterVertical- HeightBetweenCenters, DistanceFromCenterToCornerPoints);
	Hexes [3].Setup (CenterHorizontal-WidthBetweenCenters, CenterVertical- HeightBetweenCenters, DistanceFromCenterToCornerPoints);
}

//----------------------------------------------------------------

void	SpaceMap :: SetHighlighted (int region)// range 0-4
{
	SelectedRegion = region;
	if (SelectedRegion<0) 
	{
		SelectedRegion = 0;
	}
	if (SelectedRegion >= NumHexesInMap) 
	{
		SelectedRegion = NumHexesInMap;
	}
}

//----------------------------------------------------------------

void	SpaceMap :: SetCameraPosition (Vector cam)
{
	CameraPosition = cam;
}

//----------------------------------------------------------------

void	SpaceMap :: SetConnectionPosition (Vector position)
{
	//ConnectonPosition.x = Width/2 + position.x;
	//ConnectonPosition.y = Height/2 - position.y;
	ConnectonPosition = position;
	
	//float StartX = Width/2 + ConnectonPosition.x;
	//float StartY = Height/2 + ConnectonPosition.y;
}

//----------------------------------------------------------------

void	SpaceMap :: Setup ()
{
}

//----------------------------------------------------------------

void	SpaceMap :: Draw ()
{
	GLint ViewportParams [4];
	glGetIntegerv(GL_VIEWPORT, ViewportParams);// store until we restore later
	
	glViewport(ScreenPosition.Corners[0].x,
			   ViewportParams[3] - ScreenPosition.Corners[1].y - ScreenPosition.Corners[0].y,
			   ScreenPosition.Corners[1].x,
			   ScreenPosition.Corners[1].y);

	// preparation for drawing
	glDisable (GL_LIGHTING);
	glDisable (GL_BLEND);
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	
	glLoadIdentity ();
	
	glOrtho (ScreenDimensions.Corners[0].x, ScreenDimensions.Corners[1].x, 
			 ScreenDimensions.Corners[1].y, ScreenDimensions.Corners[0].y, 
			 -50, 200);
	
	//float CenterX = (ScreenPosition.Corners[0].x + ScreenPosition.Corners[1].x) / 2;
	//float CenterY = (ScreenPosition.Corners[0].y + ScreenPosition.Corners[1].y) / 2;
	DrawConnector();// draw first and thus on the bottom of all other objects
	
	for (int i=0; i<NumHexesInMap; i++)
	{
		if (i== SelectedRegion)
			continue;
		DrawHex (i, CameraPosition.x, CameraPosition.y);
	}
	DrawHex (SelectedRegion, CameraPosition.x, CameraPosition.y);// always draw this one last to put it on top of the others
	
	DrawFocusRectangle (Width/2, Height/2);
	
	DrawOutline ();
		
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
	
	glEnable (GL_BLEND);	
	glEnable (GL_LIGHTING);
	
	// restore the viewport
	glViewport(ViewportParams[0],
			   ViewportParams[1],
			   ViewportParams[2],
			   ViewportParams[3]);
}

//----------------------------------------------------------------

void	SpaceMap :: DrawHex (int which, float offx, float offy)
{
	if (SelectedRegion == which)
	{
		glLineWidth (2);
		glColor3f (HighlightedColor.r, HighlightedColor.g, HighlightedColor.b);
	}
	else
	{
		glLineWidth (1.2);
		glColor3f (Color.r, Color.g, Color.b);
	}
	
	Hexes[which].SetVisualCenter (offx, offy);
	Hexes[which].Draw (offx, offy);
}

//----------------------------------------------------------------

void	SpaceMap :: DrawFocusRectangle (float x, float y)
{
	// treating 0,0 as center.
	glLineWidth (1);
	glColor3f (FocusColor.r, FocusColor.g, FocusColor.b);
	float RectSizeW = 40;
	float RectSizeH = 30;
	
	float Left = x-RectSizeW;
	float Top = y+RectSizeH;
	float Right = x+RectSizeW;
	float Bottom = y-RectSizeH;
	
	glBegin (GL_LINE_LOOP);	
		glVertex3f (Left, Top, 0);
		glVertex3f (Left, Bottom, 0);
		glVertex3f (Right, Bottom, 0);
		glVertex3f (Right, Top, 0);	
	glEnd ();
}

//----------------------------------------------------------------

void	SpaceMap :: DrawOutline ()
{
	glLineWidth (2);
	glColor3f (Color.r, Color.g, Color.b);
	//float RectSize = 4;

	float Left = ScreenDimensions.Corners[0].x+1;
	float Top = ScreenDimensions.Corners[0].y+1;
	float Right = ScreenDimensions.Corners[1].x-1;
	float Bottom = ScreenDimensions.Corners[1].y-1;
//	float Offset = 3;
	
	/*glBegin (GL_LINE_LOOP);	
		glVertex3f (Left+Offset, Top+Offset, 0);
		glVertex3f (Left+Offset, Bottom+Offset, 0);
		glVertex3f (Right+Offset, Bottom+Offset, 0);
		glVertex3f (Right+Offset, Top+Offset, 0);	
	glEnd ();
	
	glColor3f (FocusColor.r-0.1, FocusColor.g-0.1, FocusColor.b-0.1);*/
	glBegin (GL_LINE_LOOP);	
		glVertex3f (Left, Top, 0);
		glVertex3f (Left, Bottom, 0);
		glVertex3f (Right, Bottom, 0);
		glVertex3f (Right, Top, 0);	
	glEnd ();
}

//----------------------------------------------------------------


void	SpaceMap :: DrawConnector ()
{
	glLineWidth (1);
	glColor3f (ConnectorColor.r, ConnectorColor.g, ConnectorColor.b);
	
	// note the use of +/- symbols here is non-intuitive
	float StartX = Width/2 + ConnectonPosition.x - CameraPosition.x;
	float StartY = Height/2 - ConnectonPosition.y + CameraPosition.y;
	float DestX = Hexes[SelectedRegion].VisualCenter.x;
	float DestY = Hexes[SelectedRegion].VisualCenter.y;
	
	glBegin (GL_LINES);	
		glVertex3f (StartX, StartY, 0);
		glVertex3f (DestX, DestY, 0);
	glEnd ();
	
	// designator 
	float RectSize = 10;
	float Left = StartX-RectSize;
	float Right = StartX+RectSize;
	float Top = StartY+RectSize;
	float Bottom = StartY-RectSize;
	
	glBegin (GL_LINE_LOOP);	
		glVertex3f (Left, Top, 0);
		glVertex3f (Left, Bottom, 0);
		glVertex3f (Right, Bottom, 0);
		glVertex3f (Right, Top, 0);	
	glEnd ();
}

//----------------------------------------------------------------

void	SpaceMap :: Update (GameData& data)
{
	PlayerDatabase* playerdb = data.GetPlayerDatabase ();
	Player*			player = playerdb->GetCurrentPlayer ();
	ShipArchetype* ship = player->GetShip ();
	if (ship == NULL)
		return;
	SetConnectionPosition (ship->GetCenter ());
}

//----------------------------------------------------------------
//----------------------------------------------------------------
