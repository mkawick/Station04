/*
 *  GameFramework.h
 *  Station05
 */

#include "StdAfx.h"
#include "UI_Framework.h"
#include <windows.h>
#include "../tools/GL/include/glut.h"
#include "GameFramework.h"
#include "GameData.h"

#include <boost/functional/hash.hpp>

UI_Framework :: UI_Framework() : currentGameMode( 0 )
{
}

UI_Framework :: ~UI_Framework()
{
	for( ModeUiPairIter listOfElements = UiElements.begin(); listOfElements != UiElements.end(); listOfElements++ )
	{
		UI_Toolbox::UiElementList& list = listOfElements->second;

		UI_Toolbox::UiElementList::iterator it = list.begin();
		while( it != list.end() )
		{
			delete (*it);
			it++;
		}
	}
    UiElements.clear();
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
	ProcessMessages (GlobalGameData);

	currentGameMode = GlobalGameData.GetGameMode();
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


void	UI_Framework :: ProcessMessages (GameData& GlobalGameData)
{
	int NumMessages = ReceiveQueue.Count ();
	for (int i=0; i<NumMessages; i++)
	{
		const Events::GameEvent* msg = reinterpret_cast <const Events::GameEvent*> (ReceiveQueue.Dequeue());
		switch (msg->GetType())
		{
			case Events :: UI_MouseMove:
			{
				const Events::UIMouseMoveEvent* mbEvent = reinterpret_cast <const Events::UIMouseMoveEvent*> (msg);
			}
			break;
			case Events :: UI_MouseButton:
			{
				const Events::UIMouseButtonEvent* mbEvent = reinterpret_cast <const Events::UIMouseButtonEvent*> (msg);
			}
			break;
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

bool	UI_Framework :: LoadIniFile( json_t* root, const char* filePath )
{
	json_t * uiRoot = json_object_get( root, "ui");
	if( json_is_object( uiRoot ) )
	{
		json_t * modeNode = json_object_get( uiRoot, "mode");
		GameMode mode = GameMode_none;
		if( json_is_string( modeNode ) )
		{
			mode = LookupGameMode( json_string_value( modeNode ) );
		}
		if( Validate( mode ) == false )
			return false;

		json_t * toolbox = json_object_get( uiRoot, "toolbox");
		if( json_is_array( toolbox ) )
		{
			int numItems = json_array_size( toolbox );
			for( int i=0; i< numItems; i++ )
			{
				json_t * toolObj = json_array_get( toolbox, i );
				if( json_is_object( toolObj ) )
				{
					// determine what type of object
					json_t * toolType = json_object_get( toolObj, "tool");
					if( json_is_string( toolType ) )
					{
						UI_Toolbox::UI_Frame* pFrame = NULL;
						const char* type = json_string_value( toolType );
						if( stricmp( type, "ui_frame" ) == 0 )// turn this into a factory?
						{
							pFrame = new UI_Toolbox::UI_Frame();
						}
						else if( stricmp( type, "ui_button" ) == 0 )
						{
							pFrame = new UI_Toolbox::UI_Button();
						}
						else if( stricmp( type, "ui_label" ) == 0 )
						{
							pFrame = new UI_Toolbox::UI_Label();
						}
						else if( stricmp( type, "ui_image" ) == 0 )
						{
							pFrame = new UI_Toolbox::UI_Image();
						}
						else if( stricmp( type, "ui_status" ) == 0 )
						{
							pFrame = new UI_Toolbox::UI_Status();
						}
						bool success = pFrame->LoadIniFile( toolObj );
						if( success == false )
						{
							cout << "ERROR: player status ui ini file bad: " << filePath << endl;
							return false;
						}

						AddElement( pFrame, mode );

						AddToIdHash( pFrame );
					}
				}
			}
		}
	}
	return true;
}

void	InsertSorted( UI_Toolbox::UI_Frame* pFrame, UI_Toolbox::UiElementList& listOfElements )
{
	if( listOfElements.size() == 0 )
	{
		listOfElements.push_back( pFrame );
		return;
	}
	UI_Toolbox::UiElementList::iterator it = listOfElements.begin();
	while( it != listOfElements.end() )
	{
		UI_Toolbox::UI_Frame* pNextFrame = (*it);
		if( pFrame->GetZDepth() < pNextFrame->GetZDepth() )
		{
			listOfElements.insert( it, pFrame );
			return;
		}
		it++;
	}
	listOfElements.push_back( pFrame );
}

void	UI_Framework :: AddElement( UI_Toolbox::UI_Frame* pFrame, U32 mode )
{
	ModeUiPairIter it = UiElements.find( mode ); 
	if( it == UiElements.end() )
	{
		ModeUiPair pair( mode, UI_Toolbox::UiElementList() );
		UiElements.insert( pair );
		it = UiElements.find( mode );
	}
	InsertSorted( pFrame, it->second );
}

void	UI_Framework :: AddToIdHash( UI_Toolbox::UI_Frame* pFrame )
{
	if( pFrame && pFrame->GetId().size() )
	{
		boost::hash<std::string> string_hash;

		std::size_t hash = string_hash( pFrame->GetId() );

		UiByIdPairIter iter = UiIdElements.find( hash );
		assert( iter == UiIdElements.end() );// duplicate id found

		UiIdElements.insert( UiByIdPair( hash, pFrame ) );
	}
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