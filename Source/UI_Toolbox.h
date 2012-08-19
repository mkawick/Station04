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

		virtual void	Draw ();
		virtual void	PostDrawCleanup (){}
		virtual void	Update (GameData& GlobalGameData){}

		//-----------------------------------------
		virtual bool	LoadIniFile( json_t* root );

	protected:

		void			DrawFrame ();
		void			DrawLine ();
		void			DrawFilledRect (const Vector& Color, float left, float top, float right, float bottom );

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
		enum LabelStyle { Normal, Centered, Right_Aligned, Justified };

		UI_Label() : 
			UI_Frame(), labelStyle( Normal ), text( "" ), isTextColorValid( false ){}
		UI_Label( LabelStyle _labelStyle, const char* _text ) : 
			UI_Frame(), labelStyle( _labelStyle ), text( _text ), isTextColorValid( false ){}

		void			SetText( const char* _text ) { text = _text; }
		void			Draw ();

		//-----------------------------------------
		bool			LoadIniFile( json_t* root );

	protected:
		LabelStyle	labelStyle;
		std::string text;
		ColorVector	textColor;
		bool		isTextColorValid;
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
		GLuint texture;
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