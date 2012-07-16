/*
 *  GameObject.cpp
 *  Station05
 */

#include "stdafx.h"
#include "GameObject.h"
#include "Drawing.h"
#include <math.h>
#include "EnemyShip.h"
#define M_PI       3.14159265358979323846F

namespace CORE_AI
{

int		CreateRandomColor ()
{
	return rand () % 192 + 64;
}

//---------------------------------------------------

GameObject :: GameObject () : ExitTime (false)
{
	Drawing = new SimpleDrawing ();
	enemy = new EnemyShip (300, 200, 60, 0.1F, 0);
	
	circles [0] = new PositionalCircles (100, 200, "Start point");
	circles [1] = new PositionalCircles (440, 200, "End point");

	enemy->SetHeadingAt (440, 200);
}
	
//---------------------------------------------------
		
GameObject :: ~GameObject ()
{
	delete Drawing;
	delete enemy;
	delete circles[0];
	delete circles[1];
}
	
//---------------------------------------------------

void	GameObject :: Update ()
{
	enemy->Update ();
	circles[0]->Update ();
	circles[1]->Update ();
}

//---------------------------------------------------

void	GameObject :: Draw ()
{
	circles[0]->Draw (Drawing);
	circles[1]->Draw (Drawing);

	enemy->Draw (Drawing);

	Drawing->SetColor (255, 255, 255);
	int x = 10;
	int y = 380;
	int textHeight = 15;
	Drawing->Text (x, y, "Instructions:");
	x+= 10; y+= textHeight;
	Drawing->Text (x, y, "* the enemy ship must start at one point and move to the other");
	y+= textHeight;
	Drawing->Text (x, y, "* once it arrives at the destination, it must linger for a few seconds");
	y+= textHeight;
	Drawing->Text (x, y, "* then it must return to the start point. The ship needs to know the start and end positions.");
	y+= textHeight;
	Drawing->Text (x, y, "* the main files you need to modify are EnemyShip.h and EnemyShip.cpp and a little in GameObject");
	y+= textHeight;
	Drawing->Text (x, y, "* any form of movement is acceptable other than teleport");
	y+= textHeight;
	Drawing->Text (x, y, "* you can play with the player heading simply by setting it\'s destination point");

	Drawing->Render ();
}

//---------------------------------------------------

void	GameObject :: SetExit ()
{
	ExitTime = true;
}

//---------------------------------------------------
	
}
