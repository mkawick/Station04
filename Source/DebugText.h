/*
 *  DebugText.h
 *  Station05
 */

#pragma once
#include <string>
#include <vector>
#include "Vector.h"

//----------------------------------------------------

struct TimedText
{
	std::string		Text;
	unsigned int	TimePosted;
};

//----------------------------------------------------

struct PositionedText
{
	Vector			Position;
	std::string		Text;
};

//----------------------------------------------------

class DebugText
{
public: 
	DebugText ();
	
	void	SetScreenDiensions (int width, int height);
	void	SetScreenPosition (int x, int y);
	
	void	Draw ();
	void	Update ();
	
	void	Clear ();
	
	void	AddText (const char* text);
	void	AddText (const char* text, int x, int y);
	
protected:
	
	void	PrepToDraw ();
	void	CleanupFromDrawing ();
	int		FindTextByPosition (int x, int y) const;
	
	int							MostRecentlyAddedMessageIndex;
	int							MaxMessages;
	void*						FontChoosen;
	int							FontLeading;// the spacing between lines (top to top)
	std::vector <std::string>	ListOfMessages;
	std::vector <PositionedText>TextByPosition;
	int							x, y;
	int							ScreenWidth, ScreenHeight;
};

//----------------------------------------------------
