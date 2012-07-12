/*
 *  Input.cpp
 *  SDL04_Station
 */
#include "stdafx.h"
#include "Input.h"
#include "GameObject.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

namespace CORE_AI
{
//--------------------------------------------

Input::Input () : ExitKeyHit (false), LastKeyPressed (0)
{
}

//--------------------------------------------

void	Input :: Update (GameObject& game)
{
	SDL_Event event;
	while( SDL_PollEvent ( &event ) ) 
	{
		switch( event.type ) 
		{
			case SDL_KEYDOWN:
				switch ( event.key.keysym.sym ) 
				{
					case SDLK_ESCAPE:
						ExitKeyHit = true;
						game.SetExit ();
						break;
				}
				LastKeyPressed = event.key.keysym.sym;
				break;

			case SDL_KEYUP:
				LastKeyPressed = 0;
				break;
				
			case SDL_MOUSEMOTION:
//				pitch += event.motion.yrel;
//				if (pitch < -70) pitch = -70;
//				if (pitch > 70) pitch = 70;
				break;
				
			case SDL_QUIT:
				ExitKeyHit = true;
				break;
		}
	}
}
//--------------------------------------------
}