/*
 *  PlayerStatusBars.cpp
 *  Station05
 */

#include "stdafx.h"
#include <windows.h>
#include "../tools/GL/include/glut.h"
#include "PlayerStatusBars.h"

#include "GameFramework.h"
#include "GameData.h"
#include "PlayerDatabase.h"
#include "SpaceStation.h"

using namespace Events;
UI::UI_Image image;
//----------------------------------------------

PlayerStatusBars :: PlayerStatusBars (): PlayerShipTracking (0),
										PlayerShipShieldTracking (0),
										NumberOfStationsBeingTracked (0),
										player( NULL )
{
	SetFrameColor (Vector (0.48F, 0.38F, 0));
	SetTextColor (Vector (0.8F, 0.8F, 0));
	
	SetShipColor (Vector (0, 0.3F, 0.5F));
	SetStationColor (Vector (0.7F, 0.8F, 0.2F));// kind of a yellow
	SetShieldColor (Vector (0, 0.5F, 0.7F));
	
	WarningColor.Set (0.8F, 0.8F, 0);
	DangerColor.Set (0.8F, 0.1F, 0.1F);
	
	for (int i=0; i<10; i++)
	{
		StationShieldTracking [i] = 0;
		StationTracking [i] = 0;
	}
	if( image.IsValid() == false )
	{
		image.Load( "D:/Develop/VisualStudio/SDL/Station05/Data/Asteroids/Asteriod01_Color.bmp" );
	}
}

//----------------------------------------------------------------

void	PlayerStatusBars :: SetScreenDimensions (int Left, int Top, int Right, int Bottom)
{
	ScreenDimensions.Corners[0].x = static_cast< float> ( Left );
	ScreenDimensions.Corners[0].y = static_cast< float> ( Top );
	ScreenDimensions.Corners[1].x = static_cast< float> ( Right );
	ScreenDimensions.Corners[1].y = static_cast< float> ( Bottom );
}

//----------------------------------------------------------------

void	PlayerStatusBars :: SetResourceDisplay (int Left, int Top, int Right, int Bottom)
{
	ResourceDisplay.Corners[0].x = static_cast< float> ( Left );
	ResourceDisplay.Corners[0].y = static_cast< float> ( Top );
	ResourceDisplay.Corners[1].x = static_cast< float> ( Right );
	ResourceDisplay.Corners[1].y = static_cast< float> ( Bottom );
}

//----------------------------------------------

void	PlayerStatusBars :: SetScreenPosition (int l, int t, int r, int b)
{
	assert (ScreenDimensions.Corners[1].x != 0);// must set the dimensions first
	
	ScreenPosition.Corners[0].x = static_cast< float> ( l ), ScreenPosition.Corners[0].y = static_cast< float> ( t ), 
	ScreenPosition.Corners[1].x = static_cast< float> ( r ), ScreenPosition.Corners[1].y = static_cast< float> ( b );
	
	Height = ScreenDimensions.Corners[1].y - ScreenDimensions.Corners[0].x;
	//float CenterVertical = Height / 2;
	Width = ScreenDimensions.Corners[1].x - ScreenDimensions.Corners[0].x;
	//float CenterHorizontal = Width / 2;	
}

//----------------------------------------------

void	PlayerStatusBars :: Draw ()
{
	image.Draw();
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

	DrawFrame ();
	DrawUserName ();
	DrawPlayerShipValues ();
	DrawPlayerStationValues ();
	DrawBackground ();

	DrawResources ();
	
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

//----------------------------------------------

void	PlayerStatusBars :: PostDrawCleanup ()
{
}

//----------------------------------------------

void	PlayerStatusBars :: Update (GameData& GlobalGameData)
{
	PlayerDatabase* playerDb = GlobalGameData.GetPlayerDatabase();
	player = playerDb->GetCurrentPlayer ();
	
	if (player == NULL)
	{
		PlayerName.clear();
		NumberOfStationsBeingTracked = 0;
	}
	else
	{
		PlayerName = player->GetName();
		NumberOfStationsBeingTracked = player->GetNumStations();
		ShipArchetype* ship = player->GetShip ();
		PlayerShipTracking = static_cast<float>( ship->GetHealth () );
		PlayerShipShieldTracking = static_cast<float>( ship->GetShieldLevel () );
		
		for (int i=0; i<NumberOfStationsBeingTracked; i++)
		{
			SpaceStation* station = player->GetStation(i);
			StationTracking[i] = static_cast<float>( station->GetHealth () );
			StationShieldTracking[i] = static_cast<float>( station->GetShieldLevel () );
		}
	}
}

//----------------------------------------------

void	PlayerStatusBars :: DrawFrame ()
{
	// treating 0,0 as center.
	glLineWidth (1);
	glColor3f (FrameColor.r, FrameColor.g, FrameColor.b);
	float WidthMultiplier = 7.0/8.0;
	
	float Left = ScreenDimensions.Corners[0].x+1;
	float Top = ScreenDimensions.Corners[0].y+1;
	float Right = ScreenDimensions.Corners[1].x-1;
	float Bottom = ScreenDimensions.Corners[1].y-1;
	
	glBegin (GL_LINES);	// two top lines
		glVertex3f (Left, Top, 0);
		glVertex3f (Right*WidthMultiplier, Top, 0);	
		glVertex3f (Left, Top+20, 0);
		glVertex3f (Right*WidthMultiplier, Top+20, 0);	
	glEnd ();
	
	glBegin (GL_LINE_STRIP);	
		glVertex3f (Left, Top+120, 0);
		glVertex3f (Right*WidthMultiplier, Top+120, 0);
		glVertex3f (Right, Bottom/3, 0);
		glVertex3f (Right, Bottom, 0);		
	glEnd ();
}

//----------------------------------------------

void	PlayerStatusBars :: DrawUserName ()
{
	float Left = ScreenDimensions.Corners[0].x+1;
	float Right = ScreenDimensions.Corners[1].x-1;
	float WidthMultiplier = 7.0/8.0;
	float x = (Left + Right*WidthMultiplier) / 2;
	float y = 95;
	
	int Length = PlayerName.size () *12 /2;
	
	glColor3f (TextColor.r, TextColor.g, TextColor.b);
	glRasterPos2f (x-Length, y);
	const char* text = PlayerName.c_str ();
	while (*text)
	{
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, *text);
		text++;
	}
	
	glColor3f (0.2f, 0.2f, 0);
	glRasterPos2f (x-Length+4, y+8);
	text = PlayerName.c_str ();
	while (*text)
	{
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, *text);
		text++;
	}
}

//----------------------------------------------

void	PlayerStatusBars :: DrawPlayerShipValues ()
{
	float Left = ScreenDimensions.Corners[0].x+1;
	float Top = ScreenDimensions.Corners[0].y+1;
	float Right = ScreenDimensions.Corners[1].x-1;
	float Bottom = ScreenDimensions.Corners[1].y-1;
	
	float ScreenWidth = Right-Left;
	float ScreenHeight = Bottom-Top;
	
	float MeterWidth = ScreenWidth / 8;// includes deadspace around meter
	float MeterHeight = ScreenHeight - ScreenHeight*11/16;
	
	float MeterLeft = MeterWidth/2+Left ;
	float MeterRight = MeterLeft + MeterWidth;
	
	Vector DisplayColor = CalculateColor (ShipColor, WarningColor, DangerColor, PlayerShipTracking);
	DrawVerticalBar (DisplayColor, MeterLeft-10, MeterHeight+10, MeterRight-10, Bottom,  PlayerShipTracking*0.01f);
	
	DrawVerticalBar (ShieldColor, MeterLeft, MeterHeight, MeterRight, Bottom,  PlayerShipShieldTracking*0.01f);
}

//----------------------------------------------

void	PlayerStatusBars :: DrawVerticalBar (const Vector& Color, float left, float top, float right, float bottom, float value )
{
	float Height = (bottom-top)* (1-value) + top;
	glColor3f (Color.r, Color.g, Color.b);
	glBegin (GL_QUADS);// the shield will need to be slightly higher and wider
		glVertex2f (left, Height);
		glVertex2f (left, bottom);
		glVertex2f (right, bottom);
		glVertex2f (right, Height);
	glEnd ();
}
//----------------------------------------------

void	PlayerStatusBars :: DrawHorizontalBar (const Vector& Color, float left, float top, float right, float bottom, float value )
{
	float Width = (right-left)* (1-value) + top;
	glColor3f (Color.r, Color.g, Color.b);
	glBegin (GL_QUADS);// the shield will need to be slightly higher and wider
		glVertex2f (left, top);
		glVertex2f (left, bottom);
		glVertex2f (Width, bottom);
		glVertex2f (Width, top);
	glEnd ();
}

//----------------------------------------------

void		PlayerStatusBars :: DrawPlayerStationValues ()
{
	float Left = ScreenDimensions.Corners[0].x+1;
	float Top = ScreenDimensions.Corners[0].y+1;
	float Right = ScreenDimensions.Corners[1].x-1;
	float Bottom = ScreenDimensions.Corners[1].y-1;
	
	float ScreenWidth = Right-Left;
	float ScreenHeight = Bottom-Top;
	
	float MeterWidth = ScreenWidth / 8;// includes deadspace around meter
	float MeterHeight = ScreenHeight - ScreenHeight*11/16;
	
	float MeterLeft = MeterWidth/2+Left + 50;
	float MeterRight = MeterLeft + MeterWidth;
	
	for (int i=0; i<NumberOfStationsBeingTracked; i++)
	{
		MeterLeft += MeterWidth + 30;
		MeterRight += MeterWidth + 30;
		
		Vector DisplayColor = CalculateColor (StationColor, WarningColor, DangerColor, StationTracking[i]);
		DrawVerticalBar (DisplayColor, MeterLeft-10, MeterHeight+10, MeterRight-10, Bottom,  StationTracking[i]*0.01f);
		
		DrawVerticalBar (ShieldColor, MeterLeft, MeterHeight, MeterRight, Bottom,  StationShieldTracking[i]*0.01f);
	}
}

//----------------------------------------------

void	PlayerStatusBars :: DrawBackground ()
{/*
	glBlendFunc (GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA); 
	glBlendEquation (GL_FUNC_ADD);
	glBlendColor (0.6, 0.0, 0.0, 1.0);
	glColor3f (0.6, 0, 0);
	
	float left = ScreenDimensions.Corners[0].x+1;
	float top = ScreenDimensions.Corners[0].y+1;
	float right = ScreenDimensions.Corners[1].x-1;
	float bottom = ScreenDimensions.Corners[1].y-1;
	
	glBegin (GL_QUADS);// the shield will need to be slightly higher and wider
		glVertex2f (left, top);
		glVertex2f (left, bottom);
		glVertex2f (right, bottom);
		glVertex2f (right, top);
	glEnd ();*/
}

void	PlayerStatusBars :: DrawResources ()
{
	if( player == NULL )
		return;
	//PlayerDatabase* playerDb = GlobalGameFramework-> GetGameData().GetPlayerDatabase();
	//Player* player = playerDb->GetCurrentPlayer ();

	float Left = ResourceDisplay.Corners[0].x+1;
	float Top = ResourceDisplay.Corners[0].y+1;
	float Right = ResourceDisplay.Corners[1].x-1;
	float Bottom = ResourceDisplay.Corners[1].y-1;
	
	float ScreenWidth = Right-Left;
	float ScreenHeight = Bottom-Top;
	
	float MeterWidth = ScreenWidth / 8;// includes deadspace around meter
	float MeterHeight = ScreenHeight - ScreenHeight*11/16;
	
	float MeterLeft = MeterWidth/2+Left + 50;
	float MeterRight = MeterLeft + MeterWidth;

	for(int i=0; i< ResourceTypes_Count; i++ )
	{
		ResourceTypes type = (ResourceTypes)i;
		float quantity = player->AggregrateResource( type );
		Vector	resourceColor = GlobalGameFramework->GetResourceManager().GetResourceColor( type );
		DrawHorizontalBar (resourceColor, MeterLeft, MeterHeight, MeterRight, Bottom,  quantity*0.01f);
	}
}

//----------------------------------------------

void		PlayerStatusBars :: PrepToDrawText ()
{
	/*glDisable (GL_LIGHTING);	
	glColor3f (1.0, 1.0, 0.0);	
	glBlendColor (1.0, 1.0, 1.0, 1.0);
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	
	glLoadIdentity ();
	glOrtho (0, ScreenWidth, 0, ScreenHeight, -100, 100);*/
}

//----------------------------------------------

void		PlayerStatusBars :: CleanupFromDrawingText ()
{
/*	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
	
	glEnable (GL_LIGHTING);*/
}

//----------------------------------------------

Vector		PlayerStatusBars :: CalculateColor (const Vector& NormalColor, const Vector& WarningColor, const Vector& DangerColor, float Health)
{
	Vector returnColor;
	if (Health > 75)
	{
		returnColor = NormalColor;
	}
	else if (Health > 50)
	{
		float	HealthFactor = (Health-50) * 4.0f/100.0f;// 0-1
		returnColor = NormalColor * HealthFactor + WarningColor * (1-HealthFactor);
	}
	else if (Health >25)
	{
		float	HealthFactor = (Health-25) * 4.0f/100.0f;// 0-1
		returnColor = WarningColor * HealthFactor + DangerColor * (1-HealthFactor);
	}
	else
	{
		returnColor = DangerColor;
	}
	return returnColor;
}

//----------------------------------------------
//----------------------------------------------

void	PlayerStatusBars :: SetNumberOfStations (int Num)
{
}

//----------------------------------------------
//----------------------------------------------


void	UI::UI_Image :: Load( const char* path )
{
	SDL_Surface *bmp;
	bmp = SDL_LoadBMP( path );

	if(bmp == NULL)
	{
		return;
	}

	SDL_LockSurface( bmp );
	int bpp = bmp->format->BitsPerPixel;
	int height = bmp->h;
	int width = bmp->w;
	void* ptrToPixels = bmp->pixels;
	SDL_UnlockSurface( bmp );

	GLenum errorCode;
	if( height != 0 && width != 0 && ptrToPixels != NULL )
	{
		glGenTextures( 1, &texture );
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptrToPixels );

	}
	SDL_FreeSurface( bmp );

	errorCode = glGetError();
}

void	UI::UI_Image ::Draw()
{
	if( IsValid() == false )
		return;

	glBindTexture (GL_TEXTURE_2D, texture);

    glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);

	glTexCoord2f (0.0, 0.0);
	glVertex3f (-10.0, 0.0, 0.0);
	glTexCoord2f (1.0, 0.0);
	glVertex3f (10.0, 0.0, 0.0);
	glTexCoord2f (1.0, 1.0);
	glVertex3f (10.0, 10.0, 0.0);
	glTexCoord2f (0.0, 1.0);
	glVertex3f (-10.0, 10.0, 0.0);
	
	glEnd();

	glDisable(GL_TEXTURE_2D);
}
