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
		void	SetScreenPosition (float Left, float Top, float Right, float Bottom);
		void	SetScreenDimensions (float Left, float Top, float Right, float Bottom);

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
		
		void	DrawBar (const Vector& Color, float left, float top, float right, float bottom, float Health);
		
		void	DrawFrame ();
		void	DrawBackground ();
		void	DrawUserName ();
		void	DrawPlayerShipValues ();
		void	DrawPlayerStationValues ();
		enum	{MaxTrackingValues = 10};
		
		ScreenRect		ScreenPosition;
		ScreenRect		ScreenDimensions;
		float			Width, Height;
		
		eDisplayFormat	DisplayFormat;
		int				NumberOfStationsBeingTracked;
		std::string		PlayerName;
		float			PlayerShipShieldTracking;
		float			PlayerShipTracking;
		float			StationShieldTracking [10];
		float			StationTracking [10];
		
		Vector			TextColor, FrameColor;
		Vector			ShipColor, StationColor, ShieldColor;
		Vector			WarningColor, DangerColor;
		
	};
	//----------------------------------------------

}