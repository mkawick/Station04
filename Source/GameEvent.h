/*
 *  GameEvent.h
 *  Station05
 */

#pragma once
#include <queue>
#include <list>
#include "../common/uuid.h"
#include "GlobalDefinitions.h"

#include "../common/Math/Vector.h"

class GameData;
class StellarResource;
namespace Events
{
	class MessageSenderReceiver;
	//----------------------------------------------

	enum  EventMessages
	{
		NoMessage,
		DebugText,
		ApplyThrust,
		GameModeChange,
		Maneuvers,
		SwitchViewport,
		FireWeapon,
		CameraMode,
		PlayerStatus,
		PlayerChat,
		PingTime,
		CreateResource,
		CaptureResource,
		UI_ShipConfig,
		UI_MouseMove,
		UI_MouseButton,
		UI_MouseWheel,
		UI_Map,
		UI_MyStatus,
		QuitGame
	};

	//----------------------------------------------

	class GameEvent// must be for  derived class
	{
	public:
		GameEvent ();
		~GameEvent ();
		void			SetType (EventMessages& msg) {Message = msg;}
		EventMessages	GetType () const {return Message;}
		
	protected:
		EventMessages		Message;
	};
	
	//----------------------------------------------
	
	class DebugTextEvent : public GameEvent
	{
	public:
		DebugTextEvent () {Message = DebugText;}
		
		void				SetMessageText (const char* text);
		const char*			GetMessageText () const {return MessageText;}
		
	protected:
		enum		{MaxStringLength = 54};// arbitrary size but small enough
		char		MessageText[MaxStringLength];
	};
	
	//----------------------------------------------

	class ManeuverEvent : public GameEvent
	{
	public:
		enum ManeuverType {TurnLeft, TurnRight, StraffeRight, StraffeLeft, Thrust};
		ManeuverEvent () {Message = Maneuvers;}
		ManeuverEvent( const ManeuverEvent& man ): GameEvent( man ), Maneuver( man.Maneuver ) {}
		
		void			SetManeuver (ManeuverType type) {Maneuver = type;}
		ManeuverType	GetManeuver () const {return Maneuver;}
		
	protected:
		ManeuverType Maneuver;
	};

	//----------------------------------------------

	class FireWeaponEvent : public GameEvent
	{
	public:
		FireWeaponEvent () {Message = FireWeapon;}
		
		void		SetPlayerID (UUID pid) {PlayerID = pid;}
		UUID		GetPlayerID () const {return PlayerID;}
					
	protected:
		UUID		PlayerID;
	};

	//----------------------------------------------

	class CreateResourceNodeEvent : public GameEvent
	{
	public:
		CreateResourceNodeEvent () { Message = CreateResource; }
		CreateResourceNodeEvent ( const Vector& loc, ResourceTypes type, float amount ) : 
										Location( loc ), 
										Type( type ),
										Amount ( amount ),
										AsteroidId( 0 ){Message = CreateResource; }
		
		void			SetLocation (const Vector& loc) {Location = loc;}
		const Vector&	GetLocation() const {return Location;}

		void			SetType( ResourceTypes type ) {Type = type ;}
		ResourceTypes	GetType() const { return Type; }

		void			SetAmount ( float amount ) { Amount = amount; }
		float			GetAmount() const { return Amount; }

		void			SetAsteroidIdFromWhichItCame( int id ) { AsteroidId = id; }
		int				GetAsteroidId() const { return AsteroidId; }
					
	protected:
		Vector			Location;
		ResourceTypes	Type;
		float			Amount;
		int				AsteroidId;
	};

	//----------------------------------------------

	class CaptureResourceEvent : public GameEvent
	{
	public:
		CaptureResourceEvent () {Message = CaptureResource;}
		
		void				Set( StellarResource* res ) { resource = res; }
		StellarResource*	Get() const { return resource; }
		void				SetPlayerId( UUID pid ) { PlayerID = pid; }
		UUID				GetPlayerId() const { return PlayerID; }

	protected:
		StellarResource*	resource;
		UUID				PlayerID;
	};

	//----------------------------------------------

	class GameQuitEvent : public GameEvent
	{
	public:
		GameQuitEvent () {Message = QuitGame;}
		
	protected:
	};
	
	//----------------------------------------------
	
	class SwitchViewportEvent : public GameEvent
	{
	public:
		enum	ViewportSetting {ShipView, StationView, GlobalView};
		SwitchViewportEvent () : View (ShipView), ViewIndex (0) {Message = SwitchViewport;}
		
		void				SetView (ViewportSetting vp) {View = vp;}
		ViewportSetting		GetView () const {return View;}
		
		void				SetViewIndex (int index) {ViewIndex = index;}
		int					GetViewIndex () const {return ViewIndex;}
		
	protected:
		ViewportSetting		View;
		int		ViewIndex;
	};
	//----------------------------------------------

	class UIShipConfigEvent : public GameEvent
	{
	public:
		UIShipConfigEvent () {Message = UI_ShipConfig;}
		
	protected:
	};
	//----------------------------------------------

	class UIMapEvent : public GameEvent
	{
	public:
		UIMapEvent () {Message = UI_Map;}
		
	protected:
	};
	//----------------------------------------------

	class UIMyStatusEvent : public GameEvent
	{
	public:
		UIMyStatusEvent () {Message = UI_MyStatus;}
		
	protected:
	};

	//----------------------------------------------

	class UIMouseMoveEvent : public GameEvent
	{
	public:
		UIMouseMoveEvent (): diffX(0), diffY(0), x(0), y(0) {Message = UI_MouseMove;}
		
		void	SetPosition( int _x, int _y ) { x = _x, y = _y; }
		void	SetDiff( int _x, int _y ) { diffX = _y, diffY = _y; }

	protected:
		int diffX, diffY;
		int x, y;
	};
	//----------------------------------------------

	class UIMouseButtonEvent : public GameEvent
	{
	public:
		enum Button {Left, Right, Middle };
		enum State { Up, Down };

		UIMouseButtonEvent (): button(Left), state( Down ), x(0), y(0) {Message = UI_MouseButton;}
		
		void	SetButton( Button b ) { button = b; }
		void	SetState( State s ) { state = s; }
		void	SetPosition( int _x, int _y ) { x = _y, y = _y; }

	protected:
		Button		button;
		State		state;
		int			x, y;
	};
	//----------------------------------------------

	class UIMouseWheelScrollEvent : public GameEvent
	{
	public:
		enum State { Up, Down };

		UIMouseWheelScrollEvent (): state( Down ) {Message = UI_MouseWheel;}
		
		State	GetState() const { return state; }
		void	SetState( State s ) { state = s; }

	protected:
		State		state;
	};

	//----------------------------------------------
	//----------------------------------------------

	class GameEventWrapper
	{
	public: 
		enum {MAX_DATA_SIZE = 64};
		GameEventWrapper ();
		template <typename t>
		GameEventWrapper (const t& event);
		
		template <typename t>
		void	SetMessage (const t& Event);// it's copied anyway
		
		//void	SetMessage (EventMessages Message, template <typename t>& event);// it's copied anyway
		const	GameEvent*	GetEvent () const;// you must copy this
		
		operator  GameEvent () const;
		
	protected:
		unsigned char		BufferToHoldGenericData[MAX_DATA_SIZE];// max of 256 bits
	};

	//----------------------------------------------

	class GameEventQueue : public std::queue <GameEventWrapper>
	{
	public:
		enum {MAX_ITEMS = 32};
		GameEventQueue ();
		void				Clear ();
		
		template <typename gameEvent>
		bool				Enqueue (const gameEvent& event);
		const void*			Dequeue ();
		
		int					Count ();
		int					Capacity ();
	protected:
	};
	
	//----------------------------------------------
	

	
	//----------------------------------------------
	
	// this is meant to be a derived class only.
	// it has nearly no behavior itself, but allows any I/O event detected
	class	IOFiredPlugin 
	{
	public:
		IOFiredPlugin ();
		
		virtual void	FireEvent (GameData& GlobalGameData, MessageSenderReceiver& MessageSender) = 0;
	};

	//----------------------------------------------

	class MessageSenderReceiver
	{
	public:
		MessageSenderReceiver ();
		~MessageSenderReceiver ();
		
		template <typename gameEvent>
		void	SendMessages (const gameEvent& event);
		
		void	UpdateAllClients ();
		
		void	AddClient (MessageSenderReceiver* Client);
		
		virtual void ProcessMessages (GameData& data) = 0;
		
	protected:
		void	TransportToClient (MessageSenderReceiver* client, const GameEventWrapper* wraper);
		GameEventQueue SendQueue;
		GameEventQueue ReceiveQueue;
		std::list <MessageSenderReceiver*> Clients;// to who do we send messges.
	};


#include "GameEvent.inl"
//----------------------------------------------
}

