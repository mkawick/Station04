#include "StdAfx.h"

#include "UI_Toolbox.h"
#include "PlayerDatabase.h"


using namespace UI_Toolbox;

UI_Element :: ~UI_Element()
{
	UiElementList::iterator it = children.begin();

	while(it != children.end() )
	{
		delete *it;
		it++;
	}
}

void	UI_Element :: SetScreenPosition (int Left, int Top, int Right, int Bottom)
{
	screenPosition.Corners[0].x = static_cast<float> ( Left );
	screenPosition.Corners[0].y = static_cast<float> ( Top );
	screenPosition.Corners[1].x = static_cast<float> ( Right );
	screenPosition.Corners[1].y = static_cast<float> ( Bottom );
}


//----------------------------------------------

void	UI_Element :: DrawFrame ()
{
	// treating 0,0 as center.
	glLineWidth ( lineWidth );
	glColor3f (frameColor.r, frameColor.g, frameColor.b);
	float WidthMultiplier = 7.0/8.0;
	
	float Left = screenPosition.Corners[0].x+1;
	float Top = screenPosition.Corners[0].y+1;
	float Right = screenPosition.Corners[1].x-1;
	float Bottom = screenPosition.Corners[1].y-1;
	
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

void	UI_Element :: DrawLine ()
{
	// treating 0,0 as center.
	glLineWidth ( lineWidth );
	glColor3f (frameColor.r, frameColor.g, frameColor.b);
	float WidthMultiplier = 7.0/8.0;
	
	float Left = screenPosition.Corners[0].x+1;
	float Top = screenPosition.Corners[0].y+1;
	float Right = screenPosition.Corners[1].x-1;
	float Bottom = screenPosition.Corners[1].y-1;
	
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

void	UI_Element :: DrawFilledRect (const Vector& Color, float left, float top, float right, float bottom )
{
	glColor3f (fillColor.r, fillColor.g, fillColor.b);
	glBegin (GL_QUADS);
		glVertex2f (left, top);
		glVertex2f (left, bottom);
		glVertex2f (right, bottom);
		glVertex2f (right, top);
	glEnd ();
}

//----------------------------------------------

void	UI_Label :: Draw ()
{
	float Left = screenPosition.Corners[0].x+1;
	float Right = screenPosition.Corners[1].x-1;
	float WidthMultiplier = 7.0/8.0;
	float x = (Left + Right*WidthMultiplier) / 2;
	float y = 95;
	
	int Length = text.size () *12 /2;
	
	glColor3f (frameColor.r, frameColor.g, frameColor.b);
	glRasterPos2f (x-Length, y);
	const char* str = text.c_str ();
	while (*str)
	{
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, *str);
		str++;
	}
	
	glColor3f (fillColor.r, fillColor.g, fillColor.b);
	glRasterPos2f (x-Length+4, y+8);
	str = text.c_str ();
	while (*str)
	{
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, *str);
		str++;
	}
}

//----------------------------------------------
//----------------------------------------------

void	UI_Image :: Load( const char* path )
{
	texture = GlobalGameFramework->GetAssets().FindTexture( "scribble" );
}

//----------------------------------------------

void	UI_Image ::Draw()
{
	if( IsValid() == false )
	{
		texture = GlobalGameFramework->GetAssets().FindTexture( "scribble" );
		return;
	}
	else
	{
		GlobalGameFramework->GetAssets().RenderTexture( texture, Vector2D( 0, 0 ), Vector2D( 10, 10 ) );
	}
}

//----------------------------------------------

