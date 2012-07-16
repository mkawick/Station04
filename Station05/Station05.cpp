// Station05.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#define SDL_USE_NIB_FILE 0

#ifndef __gl_h_
#include <SDL/SDL_opengl.h>
#endif
#include <SDL/SDL.h>
#include <SDL/SDL_keysym.h>
#include "../Source/GameFramework.h"

#include <fstream>
using namespace std;


void	Test()
{
}


// things to do here.
// config file for basics and network conection
// config file for user preferences
// config file for keybindings
int _tmain(int argc, _TCHAR* argv[])
{
	//Test();
	GameFramework Game (1200, 800);
	
	Game.MainLoop ();
	exit(0);
	return 0;
}
