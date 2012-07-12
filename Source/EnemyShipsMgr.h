/*
 *  EnemyShipsMgr.h
 *  SDL04_Station
 *
 */

#pragma once
#include <list>
#include "ai_bot.h"
using namespace std;

class EnemyShipsMgr
{
public:
	
	void	Clear ();
	void	PrepForStage (int Stage);
	//---------------------------------------
	
	virtual void	Setup () {}
	virtual void	Draw () {}
	virtual void	Update (GameData&);
	
	//---------------------------------------
protected:
	list <AI_Bot*> BotList;
};