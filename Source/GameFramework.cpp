/*
 *  GameFramework.cpp
 *  Station05
 */

//#include <sys/time.h>

#include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>
#include "../tools/GL/include/glut.h"

#include <sstream>
#include <string>
#include <iostream>


#include "GameEvent.h"
#include "GameFramework.h"
#include "playerdatabase.h"
#include "Scorey.h"


GameFramework* GlobalGameFramework;

//----------------------------------------------------

double GetTimeInSeconds ()// return the time elapsed
{
#ifdef WIN32
	return timeGetTime() / 1000;
#else
	struct timeval tk_time;
	struct timezone tz;
	
	gettimeofday(&tk_time, &tz);
	
	//return 4294.967296 * tk_time.tv_sec + 0.000001 * tk_time.tv_usec;
	return  tk_time.tv_sec + 0.000001 * tk_time.tv_usec;
#endif
}

unsigned long  GetTimeInMilliSeconds ()// return the time elapsed
{
#ifdef WIN32
	return timeGetTime();
#else
	struct timeval tk_time;
	struct timezone tz;
	
	gettimeofday(&tk_time, &tz);

	return  tk_time.tv_usec / 1000;
#endif
}

double	LastTime = 0;
//----------------------------------------------------

GameFramework :: GameFramework (int Width, int Height) : 
                    AwaitingExit( false ),
                    ScreenWidth( Width ), 
                    ScreenHeight( Height )
{
	DistanceFromStations = 15.0f;
	//CameraPosition.Set (0, 0, DistanceFromStations);
	viewport.SetCameraPosition (Vector (0, 0, DistanceFromStations));
	viewport.SetNormalZDepth (DistanceFromStations);
	CameraAngle.Set (0, -1, 0);
	DirectionalLightPosition.Set (30, 500, 200);

	GlobalGameFramework = this;// used throughout the init process

	assetLoader.LoadFromManifest( "../Data/IniFiles/IniFiles.json" );

	Init ();

	assetLoader.LoadFromManifest("../Data/IniFiles/FileManifest.json" );
	
	//AssetLoader.LoadFromManifest("../Data/IniFiles/UI.json");
}

GameFramework :: ~GameFramework()
{
    network.Shutdown();
    spaceResources.Shutdown();
    assetLoader.Shutdown();
    asteroids.Shutdown();
    PlayerDatabase* playerdb = GlobalGameData.GetPlayerDatabase();
    playerdb->ClearAllPlayers();
    starfield.Shutdown();
    spacemap.Shutdown();
    projectiles.Shutdown();
    audio.Shutdown();

	//glDeleteLists (DrawList, 1);
}

//----------------------------------------------------

void	GameFramework :: MainLoop ()
{
	while ( !AwaitingExit ) 
	{
		//HandleKeyboard();
		ProcessMessages (GlobalGameData);
		
		Draw ();
		
		Update ();
		
		//SDL_Delay (10);
	}
	
	// Clean up the SDL library 
	Shutdown();
}

//----------------------------------------------------

void	GameFramework :: Update ()
{
	PlayerDatabase* playerdb = GlobalGameData.GetPlayerDatabase();
	GlobalGameData.Update();
	
	starfield.Update ();
	viewport.SetCameraAngle (CameraAngle);
	spacemap.SetCameraPosition (viewport.GetCameraPosition ());
	spacemap.SetConnectionPosition (playerdb->GetCurrentPlayer ()->GetViewFocus ());
	
	
	viewport.Update (GlobalGameData);
	audio.Update (GlobalGameData);
	network.Update (GlobalGameData);
	projectiles.Update (GlobalGameData);
	asteroids.Update (GlobalGameData);
	
	//gazelle.PostDrawCleanup();
	TextOut.Update ();
	ui.Update( GlobalGameData );
	double CurrentTime = GetTimeInMilliSeconds ();	
	
	// set up the FPS counter
	double TimeDiff = CurrentTime - LastTime;
	std::string str;
	std::ostringstream formatter;
	formatter.width (3);	

	if(TimeDiff < 15)
	{
		double SleepTime = 15 - TimeDiff;
		//Sleep(SleepTime);
		TimeDiff += SleepTime;
	}
	double FPS = 1000 / TimeDiff;
	formatter << FPS;
	formatter << " fps";
	str = formatter.str ();
	
	if (FPS > 60)
	{
		int Delay = 1;//10-TimeDiff;// shooting for 30 FPS
		SDL_Delay (Delay);
	}

	GlobalGameData.SetTimeElapsed ( static_cast<int>( TimeDiff ));

	TextOut.AddText (str.c_str (), 690, 582);
	LastTime = CurrentTime;
	Input.Update (GlobalGameData);

	spaceResources.Update( GlobalGameData );
	
	// positional
	/*Vector camera = viewport.GetCameraPosition() ;
	std::string cameraString;
	formatter.str("");
	formatter << "camera position (";
	formatter << camera.x << ",  " << camera.y << ")";
	cameraString = formatter.str ();
	TextOut.AddText (cameraString.c_str (), 620, 24);*/

	RenderInstructions();
}

void	GameFramework :: RenderInstructions()
{
	int x = 610;
	int y = 520;
	TextOut.AddText (". = fire", x, y+42);
	TextOut.AddText (", = thrust", x, y+28);
	TextOut.AddText ("[ = turn left", x, y+14);
	TextOut.AddText ("] = turn right", x, y);
}

//----------------------------------------------------

void	GameFramework :: ProcessMessages (GameData& GlobalGameData)
{	
	int NumMessages = ReceiveQueue.Count ();
	for (int i=0; i<NumMessages; i++)
	{
		//const Events::GameEventWrapper EventWrapper = ReceiveQueue.Dequeue()
		const Events::GameEvent* msg = reinterpret_cast <const Events::GameEvent*> (ReceiveQueue.Dequeue());
		switch (msg->GetType())
		{
			case Events:: DebugText:
			{
				const Events::DebugTextEvent* dt = reinterpret_cast <const Events::DebugTextEvent*> (msg);
				std::string message = dt->GetMessageText ();
				TextOut.AddText(message.c_str ());
			}
				break;
			case Events :: GameModeChange:
				break;
			case Events :: SwitchViewport:
			{
				const Events::SwitchViewportEvent* sve = reinterpret_cast <const Events::SwitchViewportEvent*> (msg);
				Events::SwitchViewportEvent::ViewportSetting viewtype = sve->GetView ();
				int WhichStation = sve->GetViewIndex ();
				
				switch (viewtype)
				{
					case Events::SwitchViewportEvent::ShipView:
						//IsCameraFollowingShip = true;
						//playerdb->SetViewToPlayerShip ();
						//todo
						//spacemap.SetConnectionPosition (gazelle.GetCenter ());
						//TextOut.AddText("Switch to shipview");
						break;
					case Events::SwitchViewportEvent::StationView:
						//IsCameraFollowingShip = false;						
						//Vector v = stations[WhichStation].GetCenter ();
						//viewport.SetCameraPosition (Vector (v.x, v.y, DistanceFromStations));
						//playerdb->SwitchPlayerView (WhichStation);
						//todo
						//spacemap.SetConnectionPosition (gazelle.GetCenter ());
						spacemap.SetHighlighted (WhichStation);
						//TextOut.AddText("Switch to stationview");
						break;
					
				}
			}
				break;
			case Events :: QuitGame:
				AwaitingExit = true;
				break;
		}
	}
	this->UpdateAllClients ();
}

//----------------------------------------------------
void print(int x, int y, const char *string);

void	GameFramework :: Init ()
{
	SetupGraphics ();
	SetupMessagingTopology ();
	int DistanceFromCenter = 80;
	
	
	SpacePartition.Init( -3000, -3000, 3000, 3000, 0, 8 );
	starfield.Setup ();
	projectiles.Setup ();
	asteroids.Setup ();
	
	spacemap.SetScreenDimensions (0, 0, ScreenWidth, ScreenHeight, DistanceFromCenter);
	spacemap.SetScreenPosition (10, 10, 120, 90);
	
	PlayerDatabase* playerdb = GlobalGameData.GetPlayerDatabase();
	playerdb->SetScreenDimensions ( 0, 0, ScreenWidth, ScreenHeight );
	//playerdb->Set (0, 0, ScreenWidth, ScreenHeight);
	playerdb->SetPositionForPlayerStatusBars (10, 105, 120, 90);
	
	spacemap.SetScreenDimensions (0, 0, ScreenWidth, ScreenHeight, DistanceFromCenter);
	spacemap.SetScreenPosition (10, 10, 120, 90);
	
	TextOut.SetScreenDiensions (ScreenWidth, ScreenHeight);
	TextOut.SetScreenPosition (16, 520);
	
	spacemap.SetColor (Vector (0.48f, 0.38f, 0));
	spacemap.SetFocusColor (Vector (0, 0.88f, 0));
	spacemap.SetHighlightedColor (Vector (1.0f, 0.68f, 0));
	spacemap.SetConnectorColor (Vector (0.38f, 0.58f, 0.0));

	////////////////////////////////////////////////////////////
	// Now setup the objects in the player universe
	
	Player* player = playerdb->CreatePlayer ("Joe", Vector (0, 0, 0), StellarCartography::Up);
	playerdb->AddPlayer(player);
	playerdb->SetLocalPlayer( player->GetUnitId () );
	//Scorey* playerShip = new Scorey();
	//player->SetShip( playerShip );
}

void	GameFramework :: Shutdown()
{
	if( DrawingContext )
	{
		SDL_Delay (100);

		//SDL_FreeSurface( DrawingContext );
		//SDL_GL_DeleteContext( DrawingContext );
		//SDL_DestroyWindow(mainwindow);

	}
	SDL_Quit();	// invokes SDL_FreeSurface
	SDL_Delay (100);
}
//----------------------------------------------------

void	GameFramework :: SetupGraphics ()
{
	Uint8  BitsPerPixel = 32;
	Uint32 videoflags = SDL_OPENGL | SDL_DOUBLEBUF;// | SDL_FULLSCREEN;SDL_SWSURFACE;//
	int error = SDL_Init( SDL_INIT_EVERYTHING );
	DrawingContext = SDL_SetVideoMode( ScreenWidth, ScreenHeight, BitsPerPixel, videoflags );
	
	SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );
	
	if (DrawingContext == NULL) 
	{
		fprintf(stderr, "Couldn't set 1024x768x%d video mode: %s\n",
				BitsPerPixel, SDL_GetError());
		SDL_Quit();
		exit(2);
	}
	
	
	/*	GLuint shaderIndex = glCreateShader (GL_FRAGMENT_SHADER); 
	 glShaderSource (shaderIndex, int numOfStrings, const char **strings, int *lenOfStrings); 
	 glCompileShader (shaderIndex);*/
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	//If you’re writing a 3D game, you must set the size of the depth buffer. The following code requests a 16-bit depth buffer:
	SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
	
	//On Mac OS X you don’t need to set the size of the frame buffer’s red, green, blue, and alpha components. If you create a draw context with 32 bit color, Mac OS X uses 8 bits for each of the four components. On other operating systems, you may need to set the size of the frame buffer’s components. 
	//The following code sets 8 bits of red, green, blue, and alpha:
	/*SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	 SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	 SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	 SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);*/
	
	// Configure the view
	glShadeModel(GL_SMOOTH);
	
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);// turn on alpha
	
	// Initialize the light
	//GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
	float Amb = 0.22F;
	float Diffuse = 1.0F;
	float Specular = 0.1F;
	//float Position = 
	GLfloat LightAmbient[] = { Amb, Amb, Amb, 1.0F }; 
	GLfloat LightDiffuse[] = { Diffuse, Diffuse, Diffuse, 1.0 }; 
	GLfloat LightSpecular[] = { Specular, Specular, Specular, 1.0 }; 
	GLfloat LightPosition[] = { 0.0f, 30.0f, 0.0f, 0.0f };
	//float[] ambient = {0.0f, 0.0f, 0.0f, 1.0f};
    //float[] diffuse = {1.0f, 1.0f, 1.0f, 1.0f};
    //float[] specular = {1.0f, 1.0f, 1.0f, 1.0f};
    //float[] position = {1.0f, 1.0f, 0.3f, 0.0f};
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular); 
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition); 
	
	// add an ambient light too
	float model_ambient [] = {0.4f, 0.4f, 0.4f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);     //small white ambient light
	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, viewpoint);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

//----------------------------------------------------

void	GameFramework :: SetupMessagingTopology ()
{
	// topology setup
	PlayerDatabase* playerdb = GlobalGameData.GetPlayerDatabase();
	Input.AddClient (this);
	Input.AddClient (playerdb);
	Input.AddClient (&projectiles);
	Input.AddClient (&asteroids);
	Input.AddClient (&ui);
	
	playerdb->AddClient (this);
	playerdb->AddClient (&asteroids);

	AddClient( &spaceResources );
	AddClient( &asteroids );
}

//----------------------------------------------------

void	GameFramework :: PrintText (const char* text)
{
	TextOut.AddText (text);
}

UUID	GameFramework :: GetLocalPlayer() const 
{ 
	return GlobalGameData.GetPlayerDatabase()->GetCurrentPlayerID(); 
}
//----------------------------------------------------

void	GameFramework :: SetupToDrawStrings ()
{
	glDisable (GL_LIGHTING);	
	glColor3f (1.0, 1.0, 0.0);	
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	
	glLoadIdentity ();
	glOrtho (0, ScreenWidth, 0, ScreenHeight, -100, 500);
	
	glRasterPos2f (16, 16);
}

//----------------------------------------------------

void	GameFramework :: DrawString (const char *text)
{
	if (text == NULL) 
	{
		return;
	}
	while (*text)
	{
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, *text);
		text++;
	}
}

//----------------------------------------------------

void	GameFramework :: CleanUpFromStrings ()
{
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
	
	glEnable (GL_LIGHTING);
}

//----------------------------------------------------

void	GameFramework :: Draw ()
{	
	glClearColor (0.0, 0.0, 0.0, 0.0);
    glClear (GL_COLOR_BUFFER_BIT |
			 GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode (GL_MODELVIEW);
	glViewport (0, 0, ScreenWidth, ScreenHeight);
    glLoadIdentity ();
	
	// Put the light in place
	Vector	Pos = DirectionalLightPosition.AsNormal ();
	GLfloat		lightPosition[] = {	Pos.x,
								Pos.y, 
								Pos.z, 0.0};

	int viewpoint = 0; //0=infiniteViewpoint, 1=localViewpoint
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, viewpoint);
    glLightfv (GL_LIGHT0, GL_POSITION, lightPosition);
	
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
//	glLightfv (GL_LIGHT1, GL_POSITION, lightPosition);
	
	/////////////////////////////////////////////////
	
	DrawAllObjects ();

	//print( 200, 200, "Damn sexy");
	//AssetLoader.FindObject("cubes")->Render( 0,0,0, 0,45,0, 1.0 );

	//AssetLoader.FindObject("cone")->Render( -5,5,0, 0,0,0, 1.0 );
	//AssetLoader.Draw();
	
	ui.Draw();

	TextOut.Draw ();
	
	
	////////////////////////////////////////////////
	
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	SDL_GL_SwapBuffers();
	//glutSwapBuffers ();
}

//----------------------------------------------------

void	GameFramework :: DrawAllObjects ()
{
	starfield.Draw ();	
	
	PlayerDatabase* playerdb = GlobalGameData.GetPlayerDatabase();
	playerdb->Draw ();
	spacemap.Draw ();
	projectiles.Draw ();
	asteroids.Draw ();
	spaceResources.Draw();
	
	// --------------------------------------
	
	// the following code is for Alpha blend testing
	
	/*int ArrayOfBlendFunctions [] = 
	{
		GL_ZERO, GL_ONE, 
	 GL_SRC_COLOR, 
	 GL_ONE_MINUS_SRC_COLOR, 
	 GL_DST_COLOR, 
	 GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA, 
	 GL_ONE_MINUS_SRC_ALPHA, 
	 GL_DST_ALPHA, 
	 GL_ONE_MINUS_DST_ALPHA, 
		GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA,
		GL_SRC_ALPHA_SATURATE
	};*/
	//glBlendFunc (GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA); 
	//glBlendFunc (ArrayOfBlendFunctions[SourceIndex], ArrayOfBlendFunctions [DstIndex]);
	//glBlendEquation (GL_FUNC_ADD);
	/*
	float Amb = 0.4;
	float Diffuse = 1.0;
	float Scale = 5.0;
	float Specular = 0.1;
	float low_shininess = 5.0f;
	
	float no_mat []  = {0.0f, 0.0f, 0.0f, 1.0f};
	float mat_ambient_color [] = {Amb, Amb, 0.8, 1.0f};
	float mat_diffuse [] = {0.1f, 0.5f, 0.8f, 1.0f};
	float mat_specular [] = {Specular, Specular, Specular, 1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, low_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	//glColor3f (0, 0.8, 0.9);
	glBlendColor (0, 0.8, 0.9, 0.5);
	// ---------------------------------
	float Left = 10, Right = -10, Top = -10, Bottom = 10;
	float z = 4;
	glBegin (GL_QUADS);	// front
		glVertex3f (Left, Top, z);
		glVertex3f (Left, Bottom, z);
		glVertex3f (Right, Bottom, z);
		glVertex3f (Right, Top, z);
	glEnd ();	*/
}

//----------------------------------------------------
//----------------------------------------------------

//----------------------------------------------------

