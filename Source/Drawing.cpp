/*
 *  Drawing.cpp
 *  Station05
 */
#include "stdafx.h"
#include "Drawing.h"
#include "../tools/SDL/include/SDL.h"

#include <windows.h>
#ifndef __gl_h_
#include "../tools/GL/include/glee.h"
#include "../tools/GL/include/glut.h"
#endif
#include <math.h>
#define M_PI       3.14159265358979323846f

namespace	CORE_AI
{

	float ToRad (int x)
	{
		float _x = static_cast<float> (x) / 180.F;
		_x *= M_PI;
		return _x;
	}
	
	float InvertYPosition (float y, float height)
	{
		return height - y;
	}
	
	//-------------------------------------------------------
	
SimpleDrawing :: SimpleDrawing (int width, int height) : 
									Width (width),
									Height (height)
{
	
	const SDL_VideoInfo* video;
	
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
	
    /* Quit SDL properly on exit */
    atexit(SDL_Quit);
	
    /* Get the current video information */
    video = SDL_GetVideoInfo( );
    if( video == NULL ) {
        fprintf(stderr, "Couldn't get video information: %s\n", SDL_GetError());
        exit(1);
    }
	
    /* Set the minimum requirements for the OpenGL window */
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	
    /* Note the SDL_DOUBLEBUF flag is not required to enable double 
     * buffering when setting an OpenGL video mode. 
     * Double buffering is enabled or disabled using the 
     * SDL_GL_DOUBLEBUFFER attribute.
     */
    if( SDL_SetVideoMode( Width, Height, video->vfmt->BitsPerPixel, SDL_OPENGL ) == 0 ) {
        fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
        exit(1);
    }
	
	// setup openGl
	float aspect = (float)Width / (float)Height;
	
	/* Make the viewport cover the whole window */
	glViewport(0, 0, Width, Height);
	
	/* Set the camera projection matrix:
	 * field of view: 90 degrees
	 * near clipping plane at 0.1
	 * far clipping plane at 100.0
	 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(60.0, aspect, 0.1, 100.0);
	/* We're done with the camera, now matrix operations 
	 * will affect the modelview matrix
	 * */
	glMatrixMode(GL_MODELVIEW);
	
	/* set the clear color to gray */
	glClearColor(0.1f, 0.1f ,0.1f, 0);
	
	/* We want z-buffer tests enabled*/
	glEnable(GL_DEPTH_TEST);
	
	/* Do draw back-facing polygons*/
	glDisable(GL_CULL_FACE);
}

SimpleDrawing :: ~SimpleDrawing ()
{
	SDL_Quit ();
}

//-------------------------------------------------------
	
float	SimpleDrawing :: ConvertToColor (int c)
{
	return static_cast <float> (c)/255.0f;
}
//-------------------------------------------------------

void	SimpleDrawing :: SetBackgroundColor (int red, int green, int blue)
{
	glClearColor(ConvertToColor(red), ConvertToColor(green), ConvertToColor(blue), 1.0);
}
	
//-------------------------------------------------------

void	SimpleDrawing :: SetColor (int red, int green, int blue)
{
	glColor3f (ConvertToColor(red), ConvertToColor(green), ConvertToColor(blue));	
}
	
//-------------------------------------------------------
	
void	SimpleDrawing :: Line (int x1, int y1, int x2, int y2)
{
	// correct for the upside-down nature of openGL
	y1 = static_cast<int>( InvertYPosition ( static_cast<float>(y1), static_cast<float>(Height) ) );
	y2 = static_cast<int>( InvertYPosition ( static_cast<float>(y2), static_cast<float>(Height) ) );
	
	glBegin (GL_LINES);	
	 glVertex3f ( static_cast<float>( x1 ), static_cast<float>( y1 ), 0);
	 glVertex3f ( static_cast<float>( x2 ), static_cast<float>( y2 ), 0);		
	glEnd ();
}
	
//-------------------------------------------------------

void	SimpleDrawing :: Rect (int Left, int Top, int Right, int Bottom)
{
	// correct for the upside-down nature of openGL
	Top = static_cast<int>( InvertYPosition ( static_cast<float>( Top ), static_cast<float>( Height ) ) );
	Bottom = static_cast<int>( InvertYPosition ( static_cast<float>( Bottom ), static_cast<float>( Height ) ) );
	glBegin (GL_LINE_LOOP);	
		glVertex3f ( static_cast<float>( Left ), static_cast<float>( Top ), 0);
		glVertex3f ( static_cast<float>( Left ), static_cast<float>( Bottom ), 0);
		glVertex3f ( static_cast<float>( Right ), static_cast<float>( Bottom ), 0);
		glVertex3f ( static_cast<float>( Right ), static_cast<float>( Top ), 0);	
	glEnd ();
}
	
//-------------------------------------------------------

void	SimpleDrawing :: Dot (int x, int y)
{
	y = static_cast<int>( InvertYPosition ( static_cast<float>( y ), static_cast<float>( Height ) ) );
	glPointSize (2);
	glBegin (GL_POINTS);	
		glVertex3f ( static_cast<float>( x ), static_cast<float>( y ), 0);
	glEnd ();
}
	
//-------------------------------------------------------


void	SimpleDrawing :: Circle (int _x, int _y, int _radius)
{
	glBegin (GL_LINE_LOOP);	
	bool	firstSet = false;
	float	beginx, beginy;
	float	radius = static_cast<float>( _radius ); 
	_y = static_cast<int>( InvertYPosition ( static_cast<float>( _y ), static_cast<float>( Height ) ) );
	
	for (int i=0; i<360; i+=5)
	{
		float xpos = cos (ToRad (i) ) * radius + _x;
		float ypos = sin (ToRad (i)) * radius + _y;
		glVertex3f (xpos, ypos, 0);
		if (firstSet == false)
		{
			firstSet = true;
			beginx = xpos, beginy = ypos;
		}
	}
	glVertex3f (beginx, beginy, 0);
	glEnd ();
}
	
//-------------------------------------------------------

void	SimpleDrawing :: Text (int x, int y, const char* text)
{
	void*	FontChoosen = GLUT_BITMAP_HELVETICA_12;
	y = static_cast<int>( InvertYPosition ( static_cast<float>( y ), static_cast<float>( Height ) ) );
	glRasterPos2f ((GLfloat)x, (GLfloat)y);
	while (*text)
	{
		glutBitmapCharacter (FontChoosen, *text);
		text++;
	}
}


//-------------------------------------------------------
	
void	SimpleDrawing :: Render ()
{
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
	
	glEnable (GL_LIGHTING);
    // render everything accumulated during the last draw cycle
    SDL_GL_SwapBuffers();

	//-------------------------------------
	// prep for the next round of drawing
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glDisable (GL_LIGHTING);	
	glColor3f (1.0, 1.0, 0.0);	
	//glBlendColor (1.0, 1.0, 1.0, 1.0);
	glColor3f (1.0, 1.0, 1.0);	
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	
	glLoadIdentity ();

	glOrtho (0, Width, 0, Height, -100, 500);
	
	//glLineWidth (2;);
	glColor3f (1, 0, 0);
	
}
}// namespace