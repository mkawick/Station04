/*
 *  DebugText.cpp
 *  Station05
 */

#include "stdafx.h"
#include <windows.h>
#include "DebugText.h"
#ifndef __gl_h_
#include "../tools/GL/include/glee.h"
#include "../tools/SDL/include/SDL_opengl.h"
#include "../tools/GL/include/glut.h"
#endif
#include <assert.h>

//----------------------------------------------------

DebugText :: DebugText () : 
						MaxMessages (16), 
						MostRecentlyAddedMessageIndex (-1), 
						FontChoosen (GLUT_BITMAP_HELVETICA_10),
						FontLeading (12),
						x (16), y (16), 
						ScreenWidth (640), ScreenHeight(480)
{
}

//----------------------------------------------------

void	DebugText :: SetScreenDiensions (int width, int height)
{
	ScreenWidth = width, ScreenHeight = height;
}

//----------------------------------------------------

void	DebugText :: SetScreenPosition (int _x, int _y)
{
	assert (_y > 0 && _y < ScreenHeight);
	assert (_x > 0 && _x < ScreenWidth - 50);
	
	x = _x, y = _y;
	
	int SpaceBetweenLines = 2 * MaxMessages;
	int PixelsAvailablePerlineOfText = ((y-SpaceBetweenLines)/MaxMessages);
	
	FontChoosen = GLUT_BITMAP_HELVETICA_10;
	FontLeading = 10;

	/*if (PixelsAvailablePerlineOfText< 12)	
	{
		FontChoosen = GLUT_BITMAP_HELVETICA_10;
		FontLeading = 10;
	}
	else if (PixelsAvailablePerlineOfText< 15)
	{
		FontChoosen = GLUT_BITMAP_HELVETICA_12;
		FontLeading = 12;
	}
	else
	{
		FontChoosen = GLUT_BITMAP_HELVETICA_18;
		FontLeading = 18;
	}*/
}

//----------------------------------------------------


void	DebugText :: Draw ()
{
	if (MostRecentlyAddedMessageIndex == -1)
		return;
	
	PrepToDraw ();	
	
	// draw debug output
	const int NumMessages = ListOfMessages.size ();
	int yPosition = y;
	// the positions work from 0 at the bottom to 480 at the top.
	for (int i=0, WhichMsg = MostRecentlyAddedMessageIndex; 
		 i<NumMessages; 
		 i++, WhichMsg--)
	{
		if (WhichMsg < 0)
			WhichMsg = NumMessages-1;
		const char* text = ListOfMessages [WhichMsg].c_str ();
		glRasterPos2f (static_cast< float > ( x ), static_cast< float > ( yPosition ) );
		yPosition += FontLeading;
		while (*text)
		{
			glutBitmapCharacter (FontChoosen, *text);
			text++;
		}
	}
	
	// draw positional text
	const int NumTextByPosition = TextByPosition.size ();
	for (int i=0; i<NumTextByPosition; i++)
	{
		const char* text = TextByPosition [i].Text.c_str ();
		Vector position = TextByPosition [i].Position;
		glRasterPos2f (position.x, position.y);
		while (*text)
		{
			glutBitmapCharacter (FontChoosen, *text);
			text++;
		}
	}
	
	CleanupFromDrawing ();
}

//----------------------------------------------------

void	DebugText :: PrepToDraw ()
{
	/*glDisable (GL_LIGHTING);	
	glColor3f (1.0, 1.0, 0.0);	
	glBlendColor (1.0, 1.0, 1.0, 1.0);
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	
	glLoadIdentity ();
	glOrtho (0, ScreenWidth, 0, ScreenHeight, -100, 500);*/

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
	//glColor3f (1,1,1);
	glDisable (GL_LIGHTING);
	glDisable (GL_BLEND);

	glColor3f (1.0, 1.0, 0.0);	
	glBlendColor (1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------

void	DebugText :: CleanupFromDrawing ()
{
	glEnable (GL_BLEND);	
	glEnable (GL_LIGHTING);
	glDepthFunc (GL_LESS);
	glPopMatrix ();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix ();
	/*glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
	
	glEnable (GL_LIGHTING);*/
}

//----------------------------------------------------

void	DebugText :: Update ()
{
}

//----------------------------------------------------

void	DebugText :: Clear ()
{
	MostRecentlyAddedMessageIndex = -1;
	ListOfMessages.clear ();
	TextByPosition.clear ();
}

//----------------------------------------------------

void	DebugText :: AddText (const char* text)
{
	const int NumMessages = ListOfMessages.size ();
	if (NumMessages < MaxMessages)
	{
		ListOfMessages.push_back (text);
		MostRecentlyAddedMessageIndex = NumMessages;// size - 1
	}
	else
	{
		MostRecentlyAddedMessageIndex++;
		if (MostRecentlyAddedMessageIndex >= MaxMessages)
		{
			MostRecentlyAddedMessageIndex = 0;
		}
		ListOfMessages[MostRecentlyAddedMessageIndex] = text;
	}
}

//----------------------------------------------------

void	DebugText :: AddText (const char* text, int x, int y)
{
	int position = FindTextByPosition (x, y);

	float textX = static_cast< float > ( x );
	float textY = static_cast< float > ( y );
	
	if (position == -1)
	{
		PositionedText newPositionedText;
		newPositionedText.Position.Set (textX, textY, 0);
		newPositionedText.Text = text;
		TextByPosition.push_back (newPositionedText);
	}
	else
	{
		TextByPosition[position].Position.Set (textX, textY, 0);
		TextByPosition[position].Text = text;
	}
}

//----------------------------------------------------


int		DebugText :: FindTextByPosition (int x, int y) const
{
	int NumItems = TextByPosition.size ();
	for (int i=0; i<NumItems; i++)
	{
		if (TextByPosition[i].Position.x == x && 
			TextByPosition[i].Position.y == y)
			return i;
	}
	return -1;
}

//----------------------------------------------------




