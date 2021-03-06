/*
 *  GameFramework.h
 *  Station05
 */
#include "StdAfx.h"

#include "UI_Toolbox.h"
#include "PlayerDatabase.h"
#include "GameFramework.h"

// better fonts
// http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001/

using namespace UI_Toolbox;
UI_Framework* UI_Frame :: pUiManager = NULL;

//----------------------------------------------

UI_Frame :: ~UI_Frame()
{
	UiElementList::iterator it = children.begin();

	while(it != children.end() )
	{
		delete *it;
		it++;
	}
}

//----------------------------------------------

void	UI_Frame :: SetScreenPosition (int Left, int Top, int Right, int Bottom)
{
	screenPosition.Corners[0].x = static_cast<float> ( Left );
	screenPosition.Corners[0].y = static_cast<float> ( Top );
	screenPosition.Corners[1].x = static_cast<float> ( Right );
	screenPosition.Corners[1].y = static_cast<float> ( Bottom );
}

//----------------------------------------------

void	UI_Frame :: AddChild( UI_Frame* pChild )
{
	UiElementListIter it = children.begin();

	while( it != children.end() )// validate no duplicates
	{
		if( pChild == (*it) )
			return;
		it++;
	}
	children.push_back( pChild );
}

//----------------------------------------------

void	UI_Frame :: Draw ()
{
	if( frameStyle == Frame )
	{
		if( isFillColorValid )
		{
			DrawFilledRect( screenPosition, fillColor );
		}
		if( isFrameColorValid )
		{
			DrawFrame ( screenPosition, frameColor );
		}
	}
	else
	{
		DrawLine( screenPosition, frameColor );
	}

	UiElementListIter it = children.begin();

	while(it != children.end() )
	{
		(*it)->Draw();
		it++;
	}
}

//----------------------------------------------

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

//----------------------------------------------

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

//----------------------------------------------

void	UI_Frame :: SetUiManager( UI_Framework* pManager )
{
	pUiManager = pManager;
}

//----------------------------------------------

bool	UI_Frame :: LoadIniFile( json_t* root )
{
	bool success = false;
	json_t * pId = json_object_get( root, "id");
	if( json_is_string( pId ) )
	{
		id = json_string_value( pId );
	}
	json_t * pParent = json_object_get( root, "parent");
	if( json_is_string( pParent ) )
	{
		parent = json_string_value( pParent );
	}

	json_t * pFrameColor = json_object_get( root, "framecolor");
	isFrameColorValid = false;
	if( json_is_string( pFrameColor ) || json_is_array( pFrameColor ) )
	{
		isFrameColorValid = LoadColor( pFrameColor, frameColor );
	}
	json_t * pFillColor = json_object_get( root, "fillcolor");
	isFillColorValid = false;
	if( json_is_string( pFillColor ) || json_is_array( pFillColor ) )
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

bool	UI_Frame :: operator< (const UI_Frame &rhs) const
{ 
	return rhs.GetZDepth() > GetZDepth(); 
}

//----------------------------------------------

void	UI_Frame :: DrawFrame ( const ScreenRect& screenPosition, const Vector& color ) const
{
	// treating 0,0 as center.
	glLineWidth ( lineWidth );
	if( frameColor.isAlphaSet == true )
	{
		glColor4f ( frameColor.r, frameColor.g, frameColor.b, frameColor.a );
	}
	else
	{
		glColor3f ( frameColor.r, frameColor.g, frameColor.b );
	}
	
	float Left = screenPosition.Corners[0].x+1;
	float Top = screenPosition.Corners[0].y+1;
	float Right = screenPosition.Corners[1].x-1;
	float Bottom = screenPosition.Corners[1].y-1;
	
	glBegin (GL_LINES);	// two top lines
		glVertex3f (Left, Top, 0);
		glVertex3f (Right, Top, 0);	
		glVertex3f (Left, Top+20, 0);
		glVertex3f (Right, Top+20, 0);	
	glEnd ();
	
	glBegin (GL_LINE_STRIP);	
		glVertex3f (Left, Top+120, 0);
		glVertex3f (Right, Top+120, 0);
		glVertex3f (Right, Bottom/3, 0);
		glVertex3f (Right, Bottom, 0);		
	glEnd ();
}

//----------------------------------------------

void	UI_Frame :: DrawLine ( const ScreenRect& screenPosition, const Vector& color )
{
	// treating 0,0 as center.
	glLineWidth ( lineWidth );
	if( frameColor.alpha >=0 )
	{
		glColor4f ( frameColor.r, frameColor.g, frameColor.b, frameColor.a );
	}
	else
	{
		glColor3f ( frameColor.r, frameColor.g, frameColor.b );
	}
	float Left = (screenPosition.Corners[0].x+1);
	float Top = screenPosition.Corners[0].y+1;
	float Right = (screenPosition.Corners[1].x-1);
	float Bottom = screenPosition.Corners[1].y-1;
	
	glBegin (GL_LINE_STRIP);	
		glVertex3f (Left, Top, 0);
		glVertex3f (Right, Top, 0);
		glVertex3f (Right, Bottom, 0);
		glVertex3f (Left, Bottom, 0);
		glVertex3f (Left, Top, 0);
	glEnd ();
}

//----------------------------------------------

void	UI_Frame :: DrawFilledRect ( const ScreenRect& screenPosition, const Vector& color ) const
{
	float Left = screenPosition.Corners[0].x+1;
	float Top = screenPosition.Corners[0].y+1;
	float Right = screenPosition.Corners[1].x-1;
	float Bottom = screenPosition.Corners[1].y-1;

	if( fillColor.isAlphaSet == true )
	{
		glColor4f ( fillColor.r, fillColor.g, fillColor.b, fillColor.a );
	}
	else
	{
		glColor3f ( fillColor.r, fillColor.g, fillColor.b );
	}
	glBegin (GL_QUADS);
		glVertex2f (Left, Top);
		glVertex2f (Left, Bottom);
		glVertex2f (Right, Bottom);
		glVertex2f (Right, Top);
	glEnd ();
}

//----------------------------------------------
//----------------------------------------------

bool	UI_Label :: LoadIniFile( json_t* root )
{
	bool success = false;
	json_t * pTextColor = json_object_get( root, "textcolor");
	isTextColorValid = false;
	if( json_is_string( pTextColor ) || json_is_array( pTextColor ) )
	{
		isTextColorValid = LoadColor( pTextColor, textColor );
	}
	json_t * pShadowColor = json_object_get( root, "shadowcolor");
	isShadowSet = false;
	if( json_is_string( pShadowColor ) || json_is_array( pShadowColor ) )
	{
		isShadowSet = LoadColor( pShadowColor, shadowColor );
	}
	
	json_t * pText = json_object_get( root, "text");
	text = "";
	if( json_is_string( pText ) )
	{
		text = json_string_value( pText );
	}
	json_t * pLabelAlign = json_object_get( root, "labelalign");
	labelAlign = Left;
	if( json_is_string( pLabelAlign ) )
	{ 
		const char* pStyle = json_string_value( pLabelAlign );
		{
			if( stricmp( pStyle, "left" ) == 0 )
			{
				labelAlign = Left;
			}
			else if( stricmp( pStyle, "right" ) == 0 )
			{
				labelAlign = Right;
			} 
			else if (stricmp( pStyle, "centered" ) == 0 || stricmp( pStyle, "center" ) == 0 )
			{
				labelAlign = Centered;
			}
			else if (stricmp( pStyle, "justified" ) == 0 )
			{
				labelAlign = Justified;
			}
		}
	}
	json_t * pVerticalAlign = json_object_get( root, "verticalalign");
	verticalAlign = Bottom;
	if( json_is_string( pVerticalAlign ) )
	{ 
		const char* pStyle = json_string_value( pVerticalAlign );
		{
			if( stricmp( pStyle, "top" ) == 0 )
			{
				verticalAlign = Top;
			}
			else if( stricmp( pStyle, "bottom" ) == 0 )
			{
				verticalAlign = Bottom;
			} 
			else if (stricmp( pStyle, "middle" ) == 0 )
			{
				verticalAlign = Middle;
			}
		}
	}
	json_t * pFontSize = json_object_get( root, "fontsize");
	fontSize = 1;
	if( json_is_number( pFontSize ) )
	{
		fontSize = static_cast< int >( json_integer_value( pFontSize ) );
	}
	UI_Frame::LoadIniFile( root );// finish loading
	return isTextColorValid && text.size() > 0;
}

//----------------------------------------------

void	UI_Label :: Draw ()
{
	ScreenRect offsetRect;// this isn't much cost and it's pretty clear what it does.
	Draw ( offsetRect );
}

void	UI_Label :: Draw ( const ScreenRect& offsetRect )// common method for drawing.
{
	float left =	screenPosition.Corners[0].x + offsetRect.Corners[0].x + 1;
	float top =		screenPosition.Corners[0].y + offsetRect.Corners[0].y + 1;
	float right =	screenPosition.Corners[1].x + offsetRect.Corners[1].x - 1;
	float bottom =	screenPosition.Corners[1].y + offsetRect.Corners[1].y - 1;

	int fontWidth = 8;
	void * fontUsed = GLUT_STROKE_ROMAN;
	switch( fontSize )
	{
	case 1: 
		fontUsed = GLUT_BITMAP_HELVETICA_10;
		fontWidth = 10;
		break;
	case 2:
		fontUsed = GLUT_BITMAP_HELVETICA_12;
		fontWidth = 12;
		break;
	case 3: 
		fontUsed = GLUT_BITMAP_HELVETICA_18;
		fontWidth = 18;
		break;
	}

	float centerX = (left + right) * 0.5f;
	float positionY = bottom;
	float positionX = left;
	int length = text.size() * fontWidth / 2;
	switch( labelAlign ) 
	{
	case Left:
		//glRasterPos2f (left, bottom);
		break;
	case Right:
		//glRasterPos2f (right-length, bottom);
		positionX = right-length;
		break;
	case Centered:
		//glRasterPos2f (centerX-length/2, bottom);
		positionX = centerX-length/2;
		break;
	default:
		assert(0);
	}
	switch( verticalAlign )
	{
	case Bottom: break;
	case Top: 
		positionY = top + 20;
		break;
	case Middle:
		positionY = (top + bottom) *0.5f + 10;
		break;
	}


	// shadow first
	if( isShadowSet )
	{
		if( shadowColor.isAlphaSet == true )
		{
			glColor4f ( shadowColor.r, shadowColor.g, shadowColor.b, shadowColor.a );
		}
		else
		{
			glColor3f ( shadowColor.r, shadowColor.g, shadowColor.b );
		}
		glRasterPos2f( positionX+2, positionY+2 );
		const char* str = text.c_str ();
		while (*str)
		{
			glutBitmapCharacter (fontUsed, *str);
			str++;
		}
	}

	// regular text
	if( textColor.isAlphaSet == true )
	{
		glColor4f ( textColor.r, textColor.g, textColor.b, textColor.a );
	}
	else
	{
		glColor3f ( textColor.r, textColor.g, textColor.b );
	}
	glRasterPos2f( positionX, positionY );
	const char* str = text.c_str ();
	while (*str)
	{
		glutBitmapCharacter (fontUsed, *str);
		str++;
	}
}
//----------------------------------------------
//----------------------------------------------

void	UI_Image :: Load( const char* path )
{
	//texture = GlobalGameFramework->GetAssets().FindTexture( "scribble" );
}

//----------------------------------------------

void	UI_Image ::Draw()
{
	if( IsValid() == false )
	{
		texture = GlobalGameFramework->GetAssets().FindTexture( textureName.c_str() );
		return;
	}
	else
	{
		GlobalGameFramework->GetAssets().RenderTexture( texture, 
						Vector2D( screenPosition.Corners[0].x, screenPosition.Corners[0].y ), 
						Vector2D( screenPosition.Corners[1].x, screenPosition.Corners[1].y ) );
	}
}

//----------------------------------------------

bool	UI_Image :: LoadIniFile( json_t* root )
{
	json_t * pTexture = json_object_get( root, "texture");
	texture = InvalidTexture;
	textureName = "";
	if( json_is_string( pTexture ) )
	{
		const char * name = json_string_value( pTexture );
		textureName = name;
		texture = GlobalGameFramework->GetAssets().FindTexture( name );
	}
	UI_Frame::LoadIniFile( root );// finish loading
	return textureName.size() > 0;
}

//----------------------------------------------
//----------------------------------------------
// what happens when you click on this button
void	UI_EventElement :: SetClickEvent( Events::EventMessages _event, int datum1, int datum2 ) 
{ 
	event = _event; 
	eventDatum1 = datum1;
	eventDatum2 = datum2;
}

//----------------------------------------------
// key that triggers this button click
void	UI_EventElement :: SetKeyBinding( int key ) 
{ 
	keyBinding = key ;
} 

//----------------------------------------------

bool	UI_EventElement :: IsMouseOver( float x, float y ) const
{
	if( x >= screenPosition.Corners[0].x && x <= screenPosition.Corners[1].x )
	{
		if( y >= screenPosition.Corners[0].y && y <= screenPosition.Corners[1].y )
		{
			return true;
		}
	}
	return false;
}

//----------------------------------------------

bool	UI_EventElement :: MouseMove( float x, float y )
{
	isMouseOver = IsMouseOver( x, y );
	return isMouseOver;
}

//----------------------------------------------

bool	UI_EventElement :: MouseButton( float x, float y, int whichButton, bool isDown )
{
	isMouseOver = IsMouseOver( x, y );
	if( isMouseOver == false )
	{
		previousMouseButton = Events::UIMouseButtonEvent::None;
		mouseButton = Events::UIMouseButtonEvent::None;
		return false;
	}

	// we want to prevent a user from sliding a click onto a button as well. (should be corrected by the above check)
	if( isDown )
	{
		//previousMouseButton = whichButton;
		mouseButton = whichButton;
	}
	else if( mouseButton != Events::UIMouseButtonEvent::None )
	{
		previousMouseButton = mouseButton;// just 
		mouseButton = whichButton;
		Events::GameEvent* pEvent = Events::GameEventFactory( event, eventDatum1, eventDatum2 );
		if( pEvent && pUiManager )
		{
			pUiManager->SendMessages ( *pEvent );
			delete pEvent;
		}
	}

	return true;
}

//----------------------------------------------

bool	UI_EventElement :: KeyEvent( int key, bool isDown )
{
	Events::GameEvent* pEvent = Events::GameEventFactory( event, eventDatum1, eventDatum2 );
	if( pEvent && pUiManager )
	{
		pUiManager->SendMessages ( *pEvent );
		delete pEvent;
		return true;
	}
	return false;
}

//----------------------------------------------

bool	UI_EventElement :: LoadIniFile( json_t* root )
{
	// TODO
	return false;
}

//----------------------------------------------

void	UI_Button :: Draw ()
{
	UI_Frame::Draw();

	// if we add texture support, this is where it goes.

	if( text.GetText().size() > 0 )
	{
		text.Draw( screenPosition );
	}
	// we'll want to overlay if the mouse is down on the button.
}

//----------------------------------------------

bool	UI_Button :: LoadIniFile( json_t* root )
{
	// TODO
	// KeyboardEvent
	return false;
}

//----------------------------------------------

// finally, we come to it... this is the part I was hoping to build from the beginning.

void	UI_Status :: Draw ()
{
	if( binding == UI_StatusBinding_none || fieldbinding == UI_StatusFieldBinding_none )
		return;

	UI_Frame::Draw();

	if( text.GetText().size() > 0 )
	{
		text.Draw( screenPosition );
	}

	DrawStatus();
}

void	UI_Status :: DrawStatus()
{
	// need to branch an look up our values for display.
	switch( binding )
	{
	case UI_StatusBinding_ship:
		DrawShipStatus();
		break;
	case UI_StatusBinding_station:
		break;
	case UI_StatusBinding_resource:
		break;
	case UI_StatusBinding_production:
		break;
	case UI_StatusBinding_experience:
		break;
	case UI_StatusBinding_experience_level:
		break;
	case UI_StatusBinding_territory_control:
		break; 
	}
}

void	UI_Status :: DrawPlayerStatus()
{
	PlayerDatabase* playerDb = GlobalGameFramework->GetGameData().GetPlayerDatabase();
	Player*			player = playerDb->GetCurrentPlayer ();

	switch( fieldbinding )
	{
	case UI_StatusFieldBinding_name:
		{
			std::string name = player->GetName();
		}
		break;
	case UI_StatusFieldBinding_num_stations:
		{
			int numStations = player->GetNumStations();
		}
		break;
	case UI_StatusFieldBinding_score:
		break;
	case UI_StatusFieldBinding_value:
		break;
	case UI_StatusFieldBinding_rate:
		break;
	}
}

void	UI_Status :: DrawShipStatus()
{
	PlayerDatabase* playerDb = GlobalGameFramework->GetGameData().GetPlayerDatabase();
	Player*			player = playerDb->GetCurrentPlayer ();

	int NumberOfStationsBeingTracked = player->GetNumStations();
	ShipArchetype* ship = player->GetShip ();
	float PlayerShipTracking = static_cast<float>( ship->GetHealth () );
	//ship->GetHea
	

	float statusValue = -1;
	//int statusMax = 100;

	switch( fieldbinding )
	{
	case UI_StatusFieldBinding_shield:
		statusValue = static_cast<float>( ship->GetShieldLevel () );
		
		break;
	case UI_StatusFieldBinding_health:
		statusValue = static_cast<float>( ship->GetHealth () );
		break;
	case UI_StatusFieldBinding_regeneration:
		statusValue = static_cast<float>( ship->GetRegenerationRate() );
		break;
	case UI_StatusFieldBinding_armor:
		statusValue = static_cast<float>( ship->GetArmorLevel() );
		break;
	case UI_StatusFieldBinding_repair_rate:
		break;
	case UI_StatusFieldBinding_energy_level:
		statusValue = static_cast<float>( ship->GetPowerLevel() );
		break;
	case UI_StatusFieldBinding_energy_generation_rate:
		statusValue = static_cast<float>( ship->GetPowerGeneration() );
		break;
	case UI_StatusFieldBinding_speed:
		statusValue = static_cast<float>( ship->GetVelocity().Magnitude() );
		break;
	case UI_StatusFieldBinding_acceleration:
		statusValue = static_cast<float>( ship->GetAcceleration().Magnitude() );
		break;
	case UI_StatusFieldBinding_value:
		break;
	case UI_StatusFieldBinding_rate:
		break;
	}

	if( statusValue >= 0 )
	{
		ScreenRect position = screenPosition;
		position.Corners[0].x = ( position.Corners[0].x - position.Corners[1].x ) * ( statusValue * 0.01f) + position.Corners[1].x;
		DrawFilledRect ( position, CalculateColor( statusValue ) );
	}
}

void	UI_Status :: PostDrawCleanup ()
{
}

void	UI_Status :: Update (GameData& GlobalGameData)
{
}

Vector	UI_Status :: CalculateColor( float percentageOfMax )
{
	if( statusColors.size() == 0 )
		return Vector( 1, 1, 1 );

	float maxPercentage = 100;
	float minPercentage = 0;
	Vector highColor( 1, 1, 1 );
	Vector lowColor( 0, 0, 0 );

	std::vector< UI_StatusColor >:: iterator it = statusColors.begin();
	if( statusColors.size() == 1 )
	{
		int centerPercentage = (*it).percentage;
		if( centerPercentage == -1 )// exceptional case. Always return single color.
			return (*it).color;

		if( percentageOfMax >= centerPercentage )
		{
			minPercentage = static_cast<float>( centerPercentage );
			lowColor = (*it).color;
		}
		else
		{
			maxPercentage = static_cast<float>( centerPercentage ); 
			highColor = (*it).color;
		}
	}
	else
	{
		// walk until the end or until we are in-between 
		while( it != statusColors.end() )
		{
			int testPercentage = (*it).percentage;
			if( percentageOfMax >= testPercentage )
			{
				minPercentage = static_cast<float>( testPercentage );
				lowColor = (*it).color;
			}
			else if( (it+1) != statusColors.end() &&
				percentageOfMax < (*(it+1)).percentage )// test against the next
			{
				maxPercentage = static_cast<float>( (*(it+1)).percentage ); 
				highColor = (*(it+1)).color;
				break;
			}
			it++;
		}
	}

	// normalize the ranges and convert them to 0-1 range.
	float range = static_cast<float>(maxPercentage - minPercentage) * 0.01f;
	float normalizedPercentage = static_cast<float>(percentageOfMax - minPercentage) * 0.01f;
	float t = normalizedPercentage / range;// sliding scale allows us to use this as a raw percentage of the high-value color

	return highColor * t + lowColor * (1-t);
}

void	UI_Status :: InsertColorSorted( Vector color, int percentage )
{
	UI_StatusColor statusColor;
	statusColor.color = color;
	statusColor.percentage= percentage;

	if( statusColors.size() == 0 )
	{
		statusColors.push_back( statusColor );
		return;
	}

	std::vector< UI_StatusColor >:: iterator it = statusColors.begin();
	while( it != statusColors.end() )
	{
		if( percentage >= (*it).percentage && 
			(it+1) != statusColors.end() &&
			percentage < (*(it+1)).percentage)
		{
			statusColors.insert( it, statusColor );
			return;
		}
		it++;
	}

	statusColors.push_back( statusColor );
}

void	UI_Status :: NormalizeColorPercentages()// to correct for users going beyond 100 %. TODO
{
	if( statusColors.size() == 0 )
		return;
}

//-----------------------------------------
bool	UI_Status :: LoadIniFile( json_t* root )
{
	bool success = false;
	json_t * pLabel = json_object_get( root, "label");
	if( json_is_object ( pLabel ) )
	{
		text.LoadIniFile( pLabel );
	}

	UI_Frame::LoadIniFile( root );

	json_t * pStatusBinding = json_object_get( root, "statusbinding");
	binding = UI_StatusBinding_none;
	if( json_is_string( pStatusBinding ) )
	{
		const char * bindingName = json_string_value( pStatusBinding );
		binding = static_cast<UI_StatusBinding>( LookupStatusBinding( bindingName ) );
	}

	json_t * pStatusFieldBinding = json_object_get( root, "statusfield");
	fieldbinding = UI_StatusFieldBinding_none;
	if( json_is_string( pStatusFieldBinding ) )
	{
		const char * bindingName = json_string_value( pStatusFieldBinding );
		fieldbinding = static_cast<UI_StatusFieldBinding>( LookupStatusFieldBinding( bindingName ) );
	}

	NormalizeColorPercentages();
	return binding != UI_StatusBinding_none;
}