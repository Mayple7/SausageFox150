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

Platform *Crate;
Wall* Wall1;

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
	
	CreateSprite("TextureFiles/OutsideMan0.png", 1920, 1080, 1, 1, 1, 0, 0);
	CreateSprite("TextureFiles/OutsideMan0Overlay.png", 1920, 1080, 200, 1, 1, 0, 0);
	CreateSprite("TextureFiles/OutsideMan1.png", 1920, 1080, 1, 1, 1, 1920, 0);
	CreateSprite("TextureFiles/OutsideMan2.png", 1920, 1080, 1, 1, 1, 1920 * 2, 0);
	CreateSprite("TextureFiles/OutsideMan2Overlay.png", 1920, 1080, 200, 1, 1, 1920 * 2, 0);
	CreateSprite("TextureFiles/OutsideMan3.png", 1920, 1080, 1, 1, 1, 1920 * 3, 0);

	//Taco = CreateFood(Agility, 150, 150, -800, 0, newID++);
	//Ham = CreateFood(Strength, 150, 150, -400, 0, newID++);
	//Pizza = CreateFood(Defense, 150, 150, 0, 0, newID++);
	//Cake = CreateFood(Haste, 150, 150, 400, 0, newID++);

	//Test = CreateWall("TextureFiles/BlankPlatform.png", 500, 500, newID++, 0, 0);
	//Test->WallSprite->Visible = FALSE;

	//CreatePaperScroll(200);


	//Platforms
	//Panel3
	Crate = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 40.0f, newID++, 3640, -240);
	Crate->PlatformSprite->Visible = FALSE;
	Crate = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 100.0f, newID++, 3780, -210);
	Crate->PlatformSprite->Visible = FALSE;
	//Panel2
	Crate = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 100.0f, newID++, 2050, -340);
	Crate->PlatformSprite->Visible = FALSE;
	Crate = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 100.0f, newID++, 2170, -350);
	Crate->PlatformSprite->Visible = FALSE;

	



	//Walls
	//Stairs
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 350.0f, 100.0f, newID++, -819, -232);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -618, -281);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -675, -266);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -559, -336);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -604, -321);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -488, -370);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -538, -355);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -408, -420);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -473, -404);
	Wall1->WallSprite->Visible = FALSE;
	//Left Bounding Wall
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 1040.0f, newID++, -958, 0);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 540.0f, newID++, 810, 140);
	Wall1->WallSprite->Visible = FALSE;
	//Door Hang Over
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -903, 226);
	Wall1->WallSprite->Visible = FALSE;
	
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 350.0f, 100.0f, newID++, 4020, 370);
	Wall1->WallSprite->Visible = FALSE;

	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 260.0f, 65.0f, newID++, 4025, 22);
	Wall1->WallSprite->Visible = FALSE;
	
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
	EasyEditWall(Wall1, 10);
	//EasyEditPlatform(Crate, 10);

	// This should be the last line in this function
	UpdatePlayerPosition(&CurrentPlayer);

	UpdateHUDPosition(CurrentHUD);
	UpdateHUDItems(CurrentHUD, &CurrentPlayer);
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
	/*////////////////////////////////
	//   INPUT & COLLISION FIRST    //
	////////////////////////////////*/
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		//InitializePause(&DrawLevel2);
		//TogglePauseSound(&BackgroundSnd);
		SetNextState(GS_MainMenu);
		//UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
	}
	if(FoxInput_KeyTriggered('U'))
		SetDebugMode();
	if(FoxInput_KeyTriggered('I'))
		RemoveDebugMode();

	// Check for any collision and handle the results
	DetectPlayerCollision();
	// Handle any input for the current player
	InputPlayer(&CurrentPlayer);

	/*////////////////////////////////
	//    CAMERA POSITION SECOND    //
	////////////////////////////////*/
	SetCamera(&CurrentPlayer.Position, 250);

	/*////////////////////////////////
	//       EVERYTHING ELSE        //
	////////////////////////////////*/
	UpdateFloatingText();
}
