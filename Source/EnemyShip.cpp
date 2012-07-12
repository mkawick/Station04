#include "StdAfx.h"
#include "EnemyShip.h"
#include <math.h>
#include "drawing.h"
#define M_PI       3.14159265358979323846F

float ABS (float value)
{
	if (value < 0)
		return -value;
	return value;
}

namespace CORE_AI
{

EnemyShip :: EnemyShip (int _x, int _y, float radius, float spd, float angle) : 
											x (_x),
											y (_y),
											headingAngle (0),
											speed (spd)
{
}

void	EnemyShip :: SetHeadingAt (int px, int py)
{
	float dx = static_cast <float> (px-x);
	float dy = static_cast <float> (py-y);

	headingAngle = atan2 (dy, dx);
}

bool	EnemyShip :: DoesHit (float r, float theta, float testradius)
{
	return true;
}

void	EnemyShip :: Update ()
{

	
}

void	EnemyShip :: Draw (CORE_AI::SimpleDrawing* Drawing)
{
	float positionX = static_cast <float> (x);
	float positionY = static_cast <float> (y);

	float size = 8;
	// tip 0
	float angle = 0.0F;
	float tipX = size * cos (angle + headingAngle) + positionX;
	float tipY = size * sin (angle + headingAngle) + positionY;

	// back 0
	float backX = static_cast <float> (positionX);
	float backY = static_cast <float> (positionY);

	// wing1 120
	angle = 2*M_PI / 3;
	float wing1X = size * cos (angle + headingAngle) + positionX;
	float wing1Y = size * sin (angle + headingAngle) + positionY;

	// wing2
	angle = -2*M_PI / 3;
	float wing2X = size * cos (angle + headingAngle) + positionX;
	float wing2Y = size * sin (angle + headingAngle) + positionY;

	Drawing->SetColor (255, 192, 192);
	Drawing->Line ((int)tipX, (int)tipY, (int)wing1X, (int)wing1Y);	
	Drawing->Line ((int)wing1X, (int)wing1Y, (int)backX, (int)backY);
	Drawing->Line ((int)backX, (int)backY, (int)wing2X, (int)wing2Y);
	Drawing->Line ((int)tipX, (int)tipY, (int)wing2X, (int)wing2Y);
}
}