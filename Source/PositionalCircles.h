#pragma once
#include <list>
using namespace std;


namespace	CORE_AI
{
class	SimpleDrawing;

class PositionalCircles
{
public:
	PositionalCircles(int x, int y, const char* name);

	void	Update ();
	void	Draw (CORE_AI::SimpleDrawing* Drawing);
	void	GetPosition (int& x, int& y) const;

protected:
	// define another structure that no one else should see
	//-------------------------------------------
	struct Circle
	{
	public:
		enum State {Fadein, Normal, Fadeout, Dead};

		Circle (int r, int g, int b);
		void	Update ();
		void	Draw (int x, int y, CORE_AI::SimpleDrawing* Drawing);

		bool	IsStillAlive () {if (state == Dead) return false; return true;}
	protected:
		int		r, g, b;
		int		radius;
		State	state;
		unsigned long	startTime;
		unsigned long	lifetime;
		float	percentageLifeElapsed;
	};
	//-------------------------------------------

protected:

	int cx, cy;
	char name [256];
	list <Circle> circleList;
};
}
