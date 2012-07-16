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
		QuitGame
	};

	//----------------------------------------------

	class GameEvent// must be for  derived class
	{
	public:
		~GameEvent ();
		void			SetType (EventMessages& msg) {Message = msg;}
		EventMessages	GetType () const {return Message;}
		
	protected:
		EventMessages		Message;
		GameEvent ();
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
	protected:
		StellarResource* resource;
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

