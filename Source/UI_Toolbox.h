/*
 *  GameFramework.h
 *  Station05
 */
#pragma once
#include <list>
#include <windows.h>
#include "../common/math/vector.h"
#include "../tools/GL/include/glut.h"
#include "GameData.h"
#include "GeneralDataTypes.h"
#include "GameEvent.h"
#include "../tools/json/jansson.h"

class UI_Framework;

namespace UI_Toolbox
{
	enum	eStatusBarFormat { GraphLike, LeftSided, TopDown, RightSided };

	//-----------------------------------------------
	//-----------------------------------------------

	class UI_Frame;
	

	typedef std::list< UI_Frame* > UiElementList;
	typedef UiElementList::iterator UiElementListIter;
	//-----------------------------------------------
	class UI_Frame
	{
	protected:
		enum FrameStyle{ Invalid, Frame, Separator, Line, Bar };

	public:
		UI_Frame() : frameStyle( Invalid ), zDepth( 5.0), isFrameColorValid( false ), isFillColorValid( false ), lineWidth( 1.0 ) {}
		~UI_Frame();
		void					SetFrameColor( const ColorVector& color ) { frameColor = color; }
		const ColorVector&		GetFrameColor() const { return frameColor; }
		void					SetFillColor( const ColorVector& color ) { fillColor = color; }
		const ColorVector&		GetFillColor() const { return fillColor; }

		void					SetScreenPosition (int Left, int Top, int Right, int Bottom);

		void					SetLineWidth( float width ) { lineWidth = width; }
		float					GetZDepth() const { return zDepth; }
		const std::string&		GetId() const { return id; }
		void					AddChild( UI_Frame* pChild );

		virtual void			Draw ();
		virtual void			PostDrawCleanup (){}
		virtual void			Update (GameData& GlobalGameData){}

		//-----------------------------------------
		virtual bool	LoadIniFile( json_t* root );
		bool operator< (const UI_Frame &rhs) const;

		static void				SetUiManager( UI_Framework* pUiManager );

	protected:

		void			DrawFrame () const;
		void			DrawLine ();
		void			DrawFilledRect () const;

	protected:
		std::string		id;
		std::string		parent;
		UiElementList	children;
		FrameStyle		frameStyle;
		float			lineWidth;
		float			zDepth;
		ScreenRect		screenPosition;
		bool			isFrameColorValid;
		bool			isFillColorValid;
		ColorVector		frameColor;
		ColorVector		fillColor;
		static UI_Framework* pUiManager;
	};

	//-----------------------------------------------
	class UI_Label : public UI_Frame
	{
	public:
		enum LabelAlign { Left, Centered, Right, Justified };
		enum VerticalAlign { Bottom, Top, Middle };

		UI_Label() : 
			UI_Frame(), labelAlign( Left ), verticalAlign( Bottom ), text( "" ), isTextColorValid( false ), isShadowSet( false ), fontSize(0){}
		UI_Label( LabelAlign _labelStyle, const char* _text ) : 
			UI_Frame(), labelAlign( _labelStyle ), verticalAlign( Bottom ), text( _text ), isTextColorValid( false ), isShadowSet( false ), fontSize(0){}

		void				SetText( const char* _text ) { text = _text; }
		const std::string&	GetText() const { return text; }

		void				Draw ();
		void				Draw ( const ScreenRect& offsetRect );

		//-----------------------------------------
		bool			LoadIniFile( json_t* root );

	protected:
		VerticalAlign	verticalAlign;
		LabelAlign		labelAlign;
		int				fontSize;
		std::string 	text;
		ColorVector		shadowColor;
		ColorVector		textColor;
		bool			isTextColorValid;
		bool			isShadowSet;
	};

	//-----------------------------------------------
	// http://www.gamedev.net/topic/568478-opengl-texture-mapping-help/
	class UI_Image : public UI_Frame
	{
	public:
		enum { InvalidTexture = -1 };
		UI_Image() : 
		  UI_Frame(), texture( InvalidTexture ){}

		void			Load( const char* path );
		void			Draw ();
		bool			IsValid() const { return texture != InvalidTexture; }

		//-----------------------------------------
		bool			LoadIniFile( json_t* root );

	protected:
		GLuint			texture;
		std::string		textureName;
	};

	//-----------------------------------------------

	class UI_EventElement : public UI_Frame
	{
	public:
		UI_EventElement():
			UI_Frame(), 
			event( Events::NoMessage ), 
			eventDatum1( 0 ), 
			eventDatum2( 0 ),
			keyBinding( 0 ),
			isMouseOver( false ), 
			previousMouseButton( Events::UIMouseButtonEvent::None ), 
			mouseButton( Events::UIMouseButtonEvent::None ){}

		void			SetClickEvent( Events::EventMessages _event, int datum1 = 0, int datum2 = 0 );
		void			SetKeyBinding( int key );

		bool			MouseMove( float x, float y );
		bool			MouseButton( float x, float y, int whichButton, bool isDown = true );// will not fire if release but mouse is release outside of button rect.
		bool			KeyEvent( int key, bool isDown = true );

		//-----------------------------------------
		bool			LoadIniFile( json_t* root );

	protected:
		bool			IsMouseOver( float x, float y ) const;

		Events::EventMessages			event;
		int			eventDatum1, eventDatum2;
		int			keyBinding;
		bool		isMouseOver;// used for highlighting, etc
		int 		previousMouseButton;// state tracking so that when you release, we can fire the event
		int			mouseButton;
	};

	//-----------------------------------------------

	class UI_Button : public UI_EventElement
	{
	public:
		enum ButtonStyle { Standard, Check };

		UI_Button () : 
			UI_EventElement(), buttonStyle( Standard ), text() {}

		void			SetStyle( ButtonStyle _buttonStyle ) { buttonStyle = _buttonStyle; }
		void			SetText( const char* _text ) { text.SetText( _text ); }
		void			Draw ();
		//-----------------------------------------
		bool			LoadIniFile( json_t* root );

	private:
		ButtonStyle		buttonStyle;
		UI_Label		text;
		
	};

	//-----------------------------------------------

	// used for showing varying colors in the UI based on health, shield status, level gain, etc.
	// for a health value of 33 and two colors of green for 100 health and red for level 1,
	// we'll do a smoothe blend between green and red giving more red (think parametric linear eqn).
	// for three colors, we'll expect the colors to be ordered by percentage and then we'll choose the 
	// two in-between which our health or shield value falls and lerp.
	// If the user omits a 0 percentage or a 100 percentage, then 0 will be assumed to be black and 100 will be white.
	// The exception to this will be if a percentage of -1 is provided singly and this special case will
	// keep the color constant.

	struct UI_StatusColor
	{
		UI_StatusColor() : percentage( 50 ), color( 0.5f, 0.5f, 0.5f ) {}
		int		percentage;
		Vector	color;
	};

	class UI_Status : public UI_Frame
	{
	public:
		UI_Status() : 
			UI_Frame(), binding( UI_StatusBinding_none ), fieldbinding( UI_StatusFieldBinding_none ) {}

		void			Draw ();
		void			PostDrawCleanup ();
		void			Update (GameData& GlobalGameData);
		//-----------------------------------------
		bool			LoadIniFile( json_t* root );

	protected:
		UI_Label				text;
		UI_StatusBinding		binding;
		UI_StatusFieldBinding	fieldbinding;
		void			InsertColorSorted( Vector color, int percentage );
		void			NormalizeColorPercentages();
		Vector			CalculateColor( float percentageOfMax );

		std::vector< UI_StatusColor > statusColors;
	};

	//-----------------------------------------------

	class StellarObjectStatus
	{
	public:
		void		SetBinding( const char* stat );
		const char* GetBinding() const { return binding; }

		void		SetObjectId( UUID uuid );
		UUID		GetObjectId() const { return objectId; }

		void		SetCurrentValue( float value );
		float		GetCurrentValue() const { return currentValue; }

		void		SetRange( float min, float max );
		float		GetMin() const { return maxValue; }
		float		GetMax() const { return minValue; }

		

	protected:
		const char* binding;
		UUID		objectId;
		float		currentValue;
		float		maxValue;
		float		minValue;
		
	};

	class UI_Manager : public UI_Frame, public Events:: MessageSenderReceiver
	{
	public:
		UI_Manager();
	private:
		void			ProcessMessages (GameData& GlobalGameData);//inherited, send all messages to the appropriate places
	};
class PlayerStatusBar
{
	
};
class PlayerStatusBarManager
{
	std::list<PlayerStatusBar* > statusBars;
};
}