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

namespace UI_Toolbox
{
	enum	eStatusBarFormat { GraphLike, LeftSided, TopDown, RightSided };

	//-----------------------------------------------
	//-----------------------------------------------

	class UI_Frame;
	typedef std::list< UI_Frame* > UiElementList;
	//-----------------------------------------------
	class UI_Frame
	{
	protected:
		enum FrameStyle{ Invalid, Frame, Separator, Line, Bar };

	public:
		UI_Frame() : frameStyle( Invalid ), zDepth( 5.0), isFrameColorValid( false ), isFillColorValid( false ), lineWidth( 1.0 ) {}
		~UI_Frame();
		void				SetFrameColor( const ColorVector& color ) { frameColor = color; }
		const ColorVector&	GetFrameColor() const { return frameColor; }
		void				SetFillColor( const ColorVector& color ) { fillColor = color; }
		const ColorVector&	GetFillColor() const { return fillColor; }

		void			SetScreenPosition (int Left, int Top, int Right, int Bottom);

		void			SetLineWidth( float width ) { lineWidth = width; }
		float			GetZDepth() const { return zDepth; }

		virtual void	Draw ();
		virtual void	PostDrawCleanup (){}
		virtual void	Update (GameData& GlobalGameData){}

		//-----------------------------------------
		virtual bool	LoadIniFile( json_t* root );
		bool operator< (const UI_Frame &rhs) const; 


	protected:

		void			DrawFrame () const;
		void			DrawLine ();
		void			DrawFilledRect () const;

	protected:
		UiElementList	children;
		FrameStyle		frameStyle;
		float			lineWidth;
		float			zDepth;
		ScreenRect		screenPosition;
		bool			isFrameColorValid;
		bool			isFillColorValid;
		ColorVector		frameColor;
		ColorVector		fillColor;
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

		void			SetText( const char* _text ) { text = _text; }
		void			Draw ();

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
		  UI_Frame(), event( 0 ), keyEvent( 0 ){}
		void			SetOnClickEvent( int _event ) { event = _event; }
		bool			MouseClick( float x, float y, bool isDown = true );
		bool			KeyEvent( int key, bool isDown = true );

	protected:
		int			event;
		int			keyEvent;
	};

	//-----------------------------------------------

	class UI_Button : public UI_EventElement
	{
	public:
		enum ButtonStyle { Check, Standard };

		UI_Button () : 
			UI_EventElement(), buttonStyle( Standard ), text() {}

		void			SetStyle( ButtonStyle _buttonStyle ) { buttonStyle = _buttonStyle; }
		void			SetText( const char* _text ) { text.SetText( _text ); }
		void			Draw ();
		//-----------------------------------------
		bool			LoadIniFile( json_t* root );

	private:
		ButtonStyle buttonStyle;
		UI_Label	text;
		
	};

	//-----------------------------------------------
	class UI_Status : public UI_Frame
	{
	public:
		UI_Status() : UI_Frame() {}

		void			Draw ();
		void			PostDrawCleanup ();
		void			Update (GameData& GlobalGameData);
		//-----------------------------------------
		bool			LoadIniFile( json_t* root );
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