/*
 *  GameFramework.h
 *  Station05
 */
#include "StdAfx.h"

#include "UI_Toolbox.h"
#include "PlayerDatabase.h"
#include "GameFramework.h"

using namespace UI_Toolbox;

UI_Frame :: ~UI_Frame()
{
	UiElementList::iterator it = children.begin();

	while(it != children.end() )
	{
		delete *it;
		it++;
	}
}

void	UI_Frame :: SetScreenPosition (int Left, int Top, int Right, int Bottom)
{
	screenPosition.Corners[0].x = static_cast<float> ( Left );
	screenPosition.Corners[0].y = static_cast<float> ( Top );
	screenPosition.Corners[1].x = static_cast<float> ( Right );
	screenPosition.Corners[1].y = static_cast<float> ( Bottom );
}

void	UI_Frame :: Draw ()
{

}

bool LoadPosition( json_t * pPosition, ScreenRect& position )
{
	if( json_is_array( pPosition ) )
	{
		int numItems = json_array_size( pPosition );
		assert( numItems == 4 );
		for( int i=0; i< numItems; i++ )
		{
			json_t * pValue = json_array_get( pPosition, i );

			float ltrb = static_cast< float >( json_number_value( pValue ) );
			position[i] = ltrb;
		}
		return true;
	}
	return false;
}

bool LoadColor( json_t * pColor, ColorVector& color )
{
	if( json_is_string( pColor ) )
	{
		const char* name = json_string_value( pColor );
		if( stricmp( name, "none" ) == 0 )
		{
			color.isAlphaSet = false;
			return false;
		}
		// other colors here.
		return false;
	}
	else if( json_is_array( pColor ) )
	{
		int numItems = json_array_size( pColor );
		if( numItems == 1 )
		{
			json_t * commonColor = json_array_get( pColor, 0 );
			if( json_is_number( commonColor ) )
			{
				float value = static_cast< float >( json_number_value( commonColor ) );
				color.x = color.y = color.b = value;
				color.alpha = 1;
				if( value < 0 )
				{
					color.isAlphaSet = false;
				}
				else
				{
					color.isAlphaSet = true;
				}
				return true;
			}
		}
		else
		{
			for( int i=0; i< numItems; i++ )
			{
				json_t * pColorValue = json_array_get( pColor, i );

				float rgba = static_cast< float >( json_number_value( pColorValue ) );
				color[i] = rgba;
			}
			if( numItems == 4 )
				color.isAlphaSet = true;
			else
				color.isAlphaSet = false;
			return true;
		}
	}

	return false;
}
bool	UI_Frame :: LoadIniFile( json_t* root )
{
	bool success = false;
	json_t * pFrameColor = json_object_get( root, "framecolor");
	isFrameColorValid = false;
	if( json_is_object( pFrameColor ) || json_is_array( pFrameColor ) )
	{
		isFrameColorValid = LoadColor( pFrameColor, frameColor );
	}
	json_t * pFillColor = json_object_get( root, "fillcolor");
	isFillColorValid = false;
	if( json_is_object( pFillColor ) || json_is_array( pFillColor ) )
	{
		isFillColorValid = LoadColor( pFillColor, fillColor );
	}
	json_t * pZDepth = json_object_get( root, "zdepth");
	zDepth = 5.0;//default
	if( json_is_number( pZDepth ) )
	{
		zDepth = static_cast< float >( json_number_value( pZDepth ) );
	}
	json_t * pFrameStyle = json_object_get( root, "framestyle");
	frameStyle = Invalid;
	if( json_is_string( pFrameStyle ) )
	{ 
		const char* pStyle = json_string_value( pFrameStyle );
		{
			if( stricmp( pStyle, "frame" ) == 0 )
			{
				frameStyle = Frame;
			}
			else if( stricmp( pStyle, "Separator" ) == 0 )
			{
				frameStyle = Separator;
			} 
			else if (stricmp( pStyle, "Line" ) == 0 )
			{
				frameStyle = Line;
			}
			else if (stricmp( pStyle, "Bar" ) == 0 )
			{
				frameStyle = Bar;
			}
		}
	}
	json_t * pPosition = json_object_get( root, "position");
	if( json_is_array( pPosition ) )
	{
		LoadPosition( pPosition, screenPosition );
	}
	json_t * pLineWidth = json_object_get( root, "linewidth");
	lineWidth = 1.0f;
	if( json_is_number( pLineWidth ) )
	{
		lineWidth = static_cast< float >( json_number_value( pLineWidth ) );
	}
	return isFrameColorValid | isFillColorValid | ( frameStyle != Invalid );
}
//----------------------------------------------

void	UI_Frame :: DrawFrame ()
{
	// treating 0,0 as center.
	glLineWidth ( lineWidth );
	//glColor4f
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

void	UI_Frame :: DrawLine ()
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

void	UI_Frame :: DrawFilledRect (const Vector& Color, float left, float top, float right, float bottom )
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

bool	UI_Label :: LoadIniFile( json_t* root )
{
	return false;
}

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

bool	UI_Image :: LoadIniFile( json_t* root )
{
	return false;
}

//----------------------------------------------

void	UI_Button :: Draw ()
{
}

bool	UI_Button :: LoadIniFile( json_t* root )
{
	return false;
}

//----------------------------------------------

void	UI_Status :: Draw ()
{
}
void	UI_Status :: PostDrawCleanup ()
{
}

void	UI_Status :: Update (GameData& GlobalGameData)
{
}

//-----------------------------------------
bool	UI_Status :: LoadIniFile( json_t* root )
{
	return false;
}