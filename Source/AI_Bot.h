/*
 *  AI_Bot.h
 *  SDL04_Station
 */

#pragma once

#include "GameData.h"
#include "activeunit.h"

//class  Station;
//------------------------------------------

class   AI_Bot : public ActiveUnit// position, velocity, angle, etc
{
public:
	enum	BotState 
	{
		None, Idle, Seeking, Attacking, Drilling, Firing, Moving, Targeting,
		ReturningHomeWithSomething
	};
	enum	BotTarget
	{
		NoTarget, Player, Station, Home, Asteriod
	};
	
	//---------------------------------------
	
public:
	AI_Bot ();
	void			Clear ();
	
	BotState		GetBotState () const {return myState;}
	BotTarget		GetBotTarget () const {return myTarget;}
	UUID			GetPlayerTarget () const {return playerTarget;}
	//---------------------------------------
	
	void	Setup ();
	void	Draw ();
	void	Update (GameData&);
	
	//---------------------------------------
	
protected:
	
	bool					CheckForTransition () const;
	void					BeginTranition ();
	bool					IsInTransition () const;
	void					ResetState ();
	
	void					UpdatePosition ();
	void					IsPossibleToInterruptCurrentState ();
	
	BotState				myState;
	BotTarget				myTarget;
	uint32_t				timeOfLastStateChange;
	BotState				transistioningState;
	UUID					playerTarget;
	Vector					destination;
	
	//BasicProjectile*		selectedWeapon;
	UUID					EnemyIdID;
};

//------------------------------------------
