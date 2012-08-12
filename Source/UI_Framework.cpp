#include "StdAfx.h"
#include "UI_Framework.h"
#include <windows.h>
#include "../tools/GL/include/glut.h"
#include "GameFramework.h"
#include "GameData.h"

UI_Framework :: UI_Framework() : currentGameMode( 0 )
{
}

UI_Framework :: ~UI_Framework()
{
}

void print(int x, int y, const char *string)
{
    //Assume we are in MODEL_VIEW already
	glPushMatrix ();
	glLoadIdentity ();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity();

	GLint viewport [4];
	glGetIntegerv (GL_VIEWPORT, viewport);
	gluOrtho2D (0,viewport[2], viewport[3], 0);
	
	glDepthFunc (GL_ALWAYS);
	glColor3f (1,1,1);
	glDisable (GL_LIGHTING);
	glDisable (GL_BLEND);

	glRasterPos2f(static_cast<float>(x), static_cast<float>(y));
	for (int i = 0; string[i]!= 0; ++i)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);

	glEnable (GL_BLEND);	
	glEnable (GL_LIGHTING);
	glDepthFunc (GL_LESS);
	glPopMatrix ();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix ();
}

void	UI_Framework :: Update( GameData& GlobalGameData )
{
	UiByGameMode::iterator listOfElements = UiElements.find( currentGameMode );
	if( listOfElements != UiElements.end() )
	{
		UI_Toolbox::UiElementList& list = listOfElements->second;

		UI_Toolbox::UiElementList::iterator it = list.begin();
		while( it != list.end() )
		{
			(*it)->Update( GlobalGameData );
			it++;
		}
	}
}

void	UI_Framework :: Draw()
{
	PrepOrthoDrawing();

	UiByGameMode::iterator listOfElements = UiElements.find( currentGameMode );
	if( listOfElements != UiElements.end() )
	{
		UI_Toolbox::UiElementList& list = listOfElements->second;

		UI_Toolbox::UiElementList::iterator it = list.begin();
		while( it != list.end() )
		{
			(*it)->Draw();
			it++;
		}
	}

	PostDrawCleanup();
}

void	UI_Framework :: PrepOrthoDrawing()
{
	//Assume we are in MODEL_VIEW already
	glPushMatrix ();
	glLoadIdentity ();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity();

	GLint viewport [4];
	glGetIntegerv (GL_VIEWPORT, viewport);
	gluOrtho2D (0,viewport[2], viewport[3], 0);
	
	glDepthFunc (GL_ALWAYS);
	glColor3f (1,1,1);
	glDisable (GL_LIGHTING);
	glDisable (GL_BLEND);
}

void	UI_Framework :: PostDrawCleanup()
{
	glEnable (GL_BLEND);	
	glEnable (GL_LIGHTING);
	glDepthFunc (GL_LESS);
	glPopMatrix ();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix ();
}

void	UI_Framework :: DrawOld()
{
	GLint ViewportParams [4];
	glGetIntegerv(GL_VIEWPORT, ViewportParams);// store until we restore later
	
	// from the height, subtract the height and top position arriving at a value
	// less than the height of the screen
	int top = static_cast<int>( ViewportParams[3] - (ScreenPosition.Corners[1].y + ScreenPosition.Corners[0].y) );
    int left = static_cast<int>( ScreenPosition.Corners[0].x );
    int width = static_cast<int>( ScreenPosition.Corners[1].x );
    int height = static_cast<int>( ScreenPosition.Corners[1].y );
	glViewport( left, top, width, height );// place the render screen on the bigger screen
	
	// preparation for drawing
	glDisable (GL_LIGHTING);
	glDisable (GL_BLEND);
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	
	glLoadIdentity ();
	
	glOrtho (ScreenDimensions.Corners[0].x, ScreenDimensions.Corners[1].x, 
			 ScreenDimensions.Corners[1].y, ScreenDimensions.Corners[0].y, 
			 -50, 200);

/*	DrawFrame ();
	DrawUserName ();
	DrawPlayerShipValues ();
	DrawPlayerStationValues ();
	DrawBackground ();

	DrawResources ();*/
	
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