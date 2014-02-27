/*****************************************************************************/
/*!
\file				Level2.c
\author				Dan Muller (d.muller)
\date				Feb 15, 2024

\brief				Functions for the showcase level

\par				Functions:
\li					LoadLevel2
\li					InitializeLevel2
\li					UpdateLevel2
\li					DrawLevel2
\li					FreeLevel2
\li					UnloadLevel2
  
\par 
<b> Copyright (C) 2024 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Level2.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"
#include "../HeaderFiles/EasyEdit.h"



// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
static int newID;					// ID number
static int levelComplete = FALSE;
TextGlyphs* LevelName;
Sprite *TxtScrollRight;
Sprite *TxtScrollMiddle;

Food* Ham;
Food* Taco;
Food* Pizza;
Food* Cake;

Wall *Test;

HUD* CurrentHUD;

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadLevel2(void)
{
	//Allocate space for a large texture
	CreateTextureList();
}

/*************************************************************************/
/*!
	\brief
	Initializes the objects for the level
*/
/*************************************************************************/
void InitializeLevel2(void)
{
	Vec3 TextTint;

	newID = 10;
	ResetObjectList();
	ResetCamera();

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, 0, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	CurrentHUD = CreateHUD(&CurrentPlayer);

	Vec3Set(&TextTint, 1, 1, 1);
	LevelName = CreateText("Level 2", 0, 300, 100, TextTint, Center);
	ChangeTextVisibility(LevelName);

	CreateSprite("TextureFiles/OutsideMan1.png", 1920, 1080, 1, 1, 1, 0, 0);

	Taco = CreateFood(Agility, 150, 150, -800, 0, newID++);
	Ham = CreateFood(Strength, 150, 150, -400, 0, newID++);
	Pizza = CreateFood(Defense, 150, 150, 0, 0, newID++);
	Cake = CreateFood(Haste, 150, 150, 400, 0, newID++);

	//Test = CreateWall("TextureFiles/BlankPlatform.png", 500, 500, newID++, 0, 0);
	//Test->WallSprite->Visible = FALSE;

	//CreatePaperScroll(200);
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateLevel2(void)
{
	EventLevel2();

	//ScrollPaperScroll(1);

	//EasyEditWall(Test, 10);

	// This should be the last line in this function
	UpdatePlayerPosition(&CurrentPlayer);

	UpdateHUDPosition(CurrentHUD);
}

/*************************************************************************/
/*!
	\brief
	Draws the level
*/
/*************************************************************************/
void DrawLevel2(void)
{
	// Draws the object list and sets the camera to the correct location
	DrawObjectList();
	//DrawHUD(&HUDList);
	DrawCollisionList();
}

/*************************************************************************/
/*!
	\brief
	Frees all the objects in the level
*/
/*************************************************************************/
void FreeLevel2(void)
{
	if(levelComplete && CurrentPlayer.CurrentLevel < GS_HandGuy)
		CurrentPlayer.CurrentLevel = GS_HandGuy;
	else if(CurrentPlayer.CurrentLevel < GS_Level2)
		CurrentPlayer.CurrentLevel = GS_Level2;
	SavePlayer(&CurrentPlayer);
	FreeAllLists();
	FreeHUD(CurrentHUD);
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadLevel2(void)
{
	//Destroy the textures
	DestroyTextureList();
	DestroyPlayer(&CurrentPlayer);
}

/*************************************************************************/
/*!
	\brief
	Handles all events in the level
*/
/*************************************************************************/
void EventLevel2(void)
{
	// Check for any collision and handle the results
	DetectPlayerCollision();
	// Handle any input for the current player
	InputPlayer(&CurrentPlayer);
	UpdateHUDItems(CurrentHUD, &CurrentPlayer);

	if(FoxInput_KeyTriggered('U'))
	{
		SetDebugMode();
		//OverlayGrid->Visible = TRUE;
	}
	if(FoxInput_KeyTriggered('I'))
	{
		RemoveDebugMode();
		//OverlayGrid->Visible = FALSE;
	}
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		//InitializePause(&DrawLevel2);
		//TogglePauseSound(&BackgroundSnd);
		SetNextState(GS_MainMenu);
		//UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
	}
}
