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

namespace UI_Toolbox
{
	enum	eStatusBarFormat { GraphLike, LeftSided, TopDown, RightSided };

	class UI_Element;
	typedef std::list< UI_Element* > UiElementList;
	//-----------------------------------------------
	class UI_Element
	{
	protected:
		enum FrameStyle{ Frame, Separator, Line, Bar };

	public:
		UI_Element() : style( Frame ) {}
		~UI_Element();
		void			SetFrameColor( const Vector& color ) { frameColor = color; }
		const Vector&	GetFrameColor() const { return frameColor; }
		void			SetFillColor( const Vector& color ) { fillColor = color; }
		const Vector&	GetFillColor() const { return fillColor; }

		void			SetScreenPosition (int Left, int Top, int Right, int Bottom);

		void			SetLineWidth( float width ) { lineWidth = width; }

		virtual void	Draw (){}
		virtual void	PostDrawCleanup (){}
		virtual void	Update (GameData& GlobalGameData){}

	protected:

		void			DrawFrame ();
		void			DrawLine ();
		void			DrawFilledRect (const Vector& Color, float left, float top, float right, float bottom );

	protected:
		UiElementList	children;
		FrameStyle		style;
		float			lineWidth;
		ScreenRect		screenPosition;
		Vector			frameColor;
		Vector			fillColor;
	};

	//-----------------------------------------------
	class UI_Label : public UI_Element
	{
	public:
		enum LabelStyle { Normal, Centered, Right_Aligned, Justified };

		UI_Label() : 
			UI_Element(), labelStyle( Normal ), text( "" ){}
		UI_Label( LabelStyle _labelStyle, const char* _text ) : 
			UI_Element(), labelStyle( _labelStyle ), text( _text ){}

		void	SetText( const char* _text ) { text = _text; }
		void	Draw ();

	protected:
		LabelStyle	labelStyle;
		std::string text;
	};

	//-----------------------------------------------
	// http://www.gamedev.net/topic/568478-opengl-texture-mapping-help/
	class UI_Image : public UI_Element
	{
	public:
		enum { InvalidTexture = -1 };
		UI_Image() : 
		  UI_Element(), texture( InvalidTexture ){}

		void	Load( const char* path );
		void	Draw ();
		bool	IsValid() const { return texture != InvalidTexture; }

	protected:
		GLuint texture;
	};

	//-----------------------------------------------

	class UI_EventElement : public UI_Element
	{
	public:
		UI_EventElement();
		void	SetOnClickEvent( int _event ) { event = _event; }
		bool	MouseClick( float x, float y, bool isDown = true );
		bool	KeyEvent( int key, bool isDown = true );
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

		void	SetStyle( ButtonStyle _buttonStyle ) { buttonStyle = _buttonStyle; }
		void	SetText( const char* _text ) { text.SetText( _text ); }
		void	Draw ();

	private:
		ButtonStyle buttonStyle;
		UI_Label	text;
		
	};

	//-----------------------------------------------
	class UI_Status : public UI_Element
	{
	public:
		UI_Status() : UI_Element() {}

		void			Draw ();
		void			PostDrawCleanup ();
		void			Update (GameData& GlobalGameData);
	
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

	class UI_Manager : public UI_Element, public Events:: MessageSenderReceiver
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