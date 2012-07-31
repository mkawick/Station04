/*
 *  PlayerStatusBars.h
 *  Station05
 */

#pragma once
#include "gameevent.h"
#include <string>
#include "../common/Math/Vector.h"
#include "GeneralDataTypes.h"

class Player;
namespace Events
{
	//----------------------------------------------

	class  PlayerStatusChangedEvent : public Events::GameEvent
	{
	public:
		enum ChangeType {Score, LoggedIn, LoggedOut, Offline, Resource, Station, Ship, Credits, 
						GuildPlayersAdd, GuildPlayersRemove};
		PlayerStatusChangedEvent () {Message = Events::PlayerStatus;}
		
		void			SetChangeType (ChangeType type) {Change = type;}
		ChangeType		GetChangeType () const {return Change;}
		
	protected:
		ChangeType Change;
	};

	//----------------------------------------------

	class  PingEvent : public GameEvent
	{
	public:
		PingEvent () {Message = Events::PingTime;}
		
		void			SetPingTime (int time) {PingTime = time;}
		int				GetPingTime () const {return PingTime;}
		
	protected:
		int		PingTime;
	};

	//----------------------------------------------

	class  PlayerChatEvent : public GameEvent
	{
	public:
		enum ChatType {PtPlayer, PtGuild, PtAll};
		PlayerChatEvent () {Message = PlayerChat;}
		
		void			SetChatType (ChatType type) {Chat = type;}
		ChatType		GetChatType () const {return Chat;}
		
		void			SetMessage (std:: string& msg) {Message = msg;}
		const std::string& GetMessage () const {return Message;}
		
	protected:
		ChatType		Chat;
		std::string		Message;
	};

	//----------------------------------------------

	class	PlayerStatusBars //: public Events::MessageSenderReceiver
	{
	public:
		enum	eDisplayFormat {GraphLike, LeftSided, TopDown, RightSided};
		
		PlayerStatusBars ();
		
		void	SetDisplayFormat (eDisplayFormat format) {DisplayFormat = format;}
		void	SetScreenPosition (int Left, int Top, int Right, int Bottom);
		void	SetScreenDimensions (int Left, int Top, int Right, int Bottom);
		void	SetResourceDisplay (int Left, int Top, int Right, int Bottom);

		// ----------------------------------------------------------------------		
		
		void	SetTextColor (Vector color) {TextColor = color;}// score, playername, ping time 
		void	SetFrameColor (Vector color) {FrameColor = color;}
		void	SetShipColor (Vector color) {ShipColor = color;}
		void	SetStationColor (Vector color) {StationColor = color;}
		void	SetShieldColor (Vector color) {ShieldColor = color;}
		
		void	SetNumberOfStations (int Num);
		
		// ----------------------------------------------------------------------
		
		void	Draw ();
		void	PostDrawCleanup ();
		void	Update (GameData& GlobalGameData);
		
		// ----------------------------------------------------------------------
		
	private:
		
		Vector	CalculateColor (const Vector& NormalColor, const Vector& WarningColor, const Vector& DangerColor, float Health);
		void	PrepToDrawText ();
		void	CleanupFromDrawingText ();
		
		void	DrawVerticalBar (const Vector& Color, float left, float top, float right, float bottom, float value );
		void	DrawHorizontalBar (const Vector& Color, float left, float top, float right, float bottom, float value );
		
		void	DrawFrame ();
		void	DrawBackground ();
		void	DrawUserName ();
		void	DrawPlayerShipValues ();
		void	DrawPlayerStationValues ();
		void	DrawResources ();

		enum	{MaxTrackingValues = 10};
		
		ScreenRect		ScreenPosition;
		ScreenRect		ScreenDimensions;
		ScreenRect		ResourceDisplay;
		float			Width, Height;
		
		eDisplayFormat	DisplayFormat;
		int				NumberOfStationsBeingTracked;
		std::string		PlayerName;
		float			PlayerShipShieldTracking;
		float			PlayerShipTracking;
		float			StationShieldTracking [10];
		float			StationTracking [10];
		Player*			player;
		//float			ResourceQuantities[ResourceTypes_Count];
		
		Vector			TextColor, FrameColor;
		Vector			ShipColor, StationColor, ShieldColor;
		Vector			WarningColor, DangerColor;
		
	};
	//----------------------------------------------

}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

namespace UI
{
	enum	eStatusBarFormat { GraphLike, LeftSided, TopDown, RightSided };

	//-----------------------------------------------
	class UI_Element
	{
	protected:
		enum Style{ Frame, Separator, Line, Bar };

		UI_Element() : style( Frame ) {}
		void			SetColor( const Vector& color );
		const Vector&	GetColor() const { return color; }

		void			SetScreenPosition (int Left, int Top, int Right, int Bottom);

		virtual void	Draw (){}
		virtual void	PostDrawCleanup (){}
		virtual void	Update (GameData& GlobalGameData){}

	protected:
		Style		style;
		ScreenRect	screenPosition;
		Vector		color;
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
	class UI_Button : public UI_Element
	{
	public:
		enum ButtonStyle { Check, Standard };

		UI_Button () : 
			UI_Element(), buttonStyle( Standard ), text() {}

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

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
