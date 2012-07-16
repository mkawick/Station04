//  GameObject.h
//	Station05
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "PositionalCircles.h"

namespace	CORE_AI
{
class	SimpleDrawing;
class	EnemyShip;
//---------------------------------------------------

class GameObject
{
public:
	GameObject ();
	~GameObject ();
	
	void	Update ();
	void	Draw ();
	
	void	SetExit ();
	bool	TimeToExit () const {return ExitTime;}

protected:
	bool	ExitTime;
	SimpleDrawing* Drawing;
	EnemyShip* enemy;

	PositionalCircles* circles[2];
};

//---------------------------------------------------
}