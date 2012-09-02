/*
 *  GameFramework.h
 *  Station05
 */

#pragma once
#include "../common/struct.h"
#include <list>
#include <hash_map>
#include "UI_Toolbox.h"
#include "GeneralDataTypes.h"
#include "../tools/json/jansson.h"

class GameData;

typedef std::pair < U32, UI_Toolbox::UiElementList > ModeUiPair; 
typedef stdext::hash_map< U32, UI_Toolbox::UiElementList > UiByGameMode;
typedef UiByGameMode::iterator ModeUiPairIter;

//class UI_Frame;
typedef std::pair < U32, UI_Toolbox::UI_Frame* > UiByIdPair;
typedef stdext::hash_map< U32, UI_Toolbox::UI_Frame* > UiByGameIdHash;
typedef UiByGameIdHash::iterator UiByIdPairIter;


class UI_Framework : public Events:: MessageSenderReceiver
{
public:
	UI_Framework();
	~UI_Framework();

	//void	SetScreenPosition (int Left, int Top, int Right, int Bottom);
	//void	SetScreenDimensions (int Left, int Top, int Right, int Bottom);

	void	SetGameMode( GameMode );
	void	Update( GameData& GlobalGameData );


	void	Draw();
	
	U32		AddElement( int GameMode, UI_Toolbox::UI_Frame* pElement, U32 parentId );

	bool	LoadIniFile( json_t* root, const char* filePath );

	void	DrawOld();

protected:

	void	PrepOrthoDrawing();
	void	PostDrawCleanup();
	void	ProcessMessages (GameData& GlobalGameData);
	
	void	AddElement( UI_Toolbox::UI_Frame* pFrame, U32 mode );
	void	AddToIdHash( UI_Toolbox::UI_Frame* pFrame );

	UiByGameMode	UiElements;
	UiByGameIdHash	UiIdElements;

	ScreenRect		ScreenPosition;
	ScreenRect		ScreenDimensions;
	ScreenRect		ResourceDisplay;
	float			Width, Height;
	int				currentGameMode;
};
