#include "StdAfx.h"
#include "PositionalCircles.h"
#include "Drawing.h"
#include <windows.h>
#include <mmsystem.h>

using namespace CORE_AI;
#pragma warning (disable:4996)

PositionalCircles::PositionalCircles(int x, int y, const char* _name): cx(x), cy(y)
{
	strcpy (name, _name);
}

//--------------------------------------------------------------------

void	PositionalCircles :: Update ()
{
	list <Circle> :: iterator it = circleList.begin ();
	while (it != circleList.end ())
	{
		Circle& c = *it;
		if (c.IsStillAlive () == false)
		{
			circleList.erase (it++);// don't advance the iterator
			continue;
		}
		it++;
		c.Update ();
	}

	if (rand () % 40 == 0 || // one chance in Forty
		circleList.size () == 0)// or an empty list
	{
		int r = rand()%192+64;
		int g = rand()%192+64;
		int b = rand()%192+64;
		Circle c (r, g, b);
		circleList.push_back (c);
	}
}

//--------------------------------------------------------------------

void	PositionalCircles :: Draw (CORE_AI::SimpleDrawing* Drawing)
{
	list <Circle> :: iterator it = circleList.begin ();
	while (it != circleList.end ())
	{
		Circle& c = *it;
		it++;
		c.Draw (cx, cy, Drawing);
	}

	int len = strlen (name);// assume 8 pixels per char
	int posx = len * 6 /2;// half of the width

	Drawing->SetColor (255, 255, 255);
	Drawing->Text (cx - posx, cy + 32, name);
}

//--------------------------------------------------------------------

void	PositionalCircles :: GetPosition (int& x, int& y) const
{
	x = cx, y = cy;
}

//--------------------------------------------------------------------

PositionalCircles :: Circle :: Circle (int _r, int _g, int _b): r(_r), g(_g), b(_b)
{
	radius = rand () % 18 + 5;// between 5 and 23
	state = Fadein;
	startTime = timeGetTime ();
	unsigned long seconds = rand () % 5 + 2;
	lifetime = seconds * 1000;// cnvert to milliseconds
	percentageLifeElapsed = 0;
}

//--------------------------------------------------------------------

void	PositionalCircles :: Circle :: Update ()
{
	if (state == Dead)
		return;

	// for the first 1/3 of the lifetime, we are fading in
	unsigned long currentTime = timeGetTime ();
	unsigned long elapsedTime = currentTime - startTime;
	if (elapsedTime > lifetime)// terminal case
	{
		state = Dead;
		return;
	}

	percentageLifeElapsed = static_cast <float> (elapsedTime);
	percentageLifeElapsed /= static_cast <float> (lifetime);

	if (percentageLifeElapsed < 0.33)
	{
		state = Fadein;
	}
	else if (percentageLifeElapsed < 0.67)
	{
		state = Normal;
	}
	else 
	{
		state = Fadeout;
	}
}

//--------------------------------------------------------------------

void	PositionalCircles :: Circle :: Draw (int x, int y, CORE_AI::SimpleDrawing* Drawing)
{
	if (state == Dead)
		return;

	float percentageColor = 1;
	if (state == Fadein)
	{
		percentageColor = percentageLifeElapsed*3;
	}
	else if (state == Normal)
	{
		percentageColor = 1;
	}
	else
	{
		percentageColor = 1.0F + (0.67F - percentageLifeElapsed)*3.0F;// working toward 0
	}

	int finalr = (int)(static_cast <float> (r)*percentageColor);
	int finalg = (int)(static_cast <float> (g)*percentageColor);
	int finalb = (int)(static_cast <float> (b)*percentageColor);

	Drawing->SetColor (finalr, finalg, finalb);
	Drawing->Circle (x, y, radius);
}
//--------------------------------------------------------------------