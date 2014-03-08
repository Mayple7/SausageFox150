/*****************************************************************************/
/*!
\file				ArmGuy.c
\author				Dan Muller (d.muller)
\date				Feb 15, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					LoadArmGuy
\li					InitializeArmGuy
\li					UpdateArmGuy
\li					DrawArmGuy
\li					FreeArmGuy
\li					UnloadArmGuy
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/ArmGuy.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"
#include "../HeaderFiles/UIButton.h"
#include "../HeaderFiles/BoundingBox.h"


// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
static int newID;					// ID number
static int levelComplete = FALSE;
static Sprite* BossBody;
TextGlyphs* LevelName;

ArmGuyBoss *Boss;

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadArmGuy(void)
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
void InitializeArmGuy(void)
{
	Vec3 TextTint;
	newID = 10;
	ResetObjectList();
	ResetCamera();

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, 0, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	//Create background
	CreateSprite("TextureFiles/ArmGuyBackground.png", 1920, 1080, 1, 1, 1, 0, 0);

	CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 300, 50, newID++, -400, -170);
	CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 300, 50, newID++, 400, -170);

	//Create bounding walls
	CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 1040.0f, newID++, -1010, 0);
	CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 1040.0f, newID++, 1010, 0);

	Boss = CreateArmGuyBoss(0, 0, &newID);
	Boss->BossCollider.collisionDebug = TRUE;
	Boss->SpinAttack.collisionDebug = TRUE;
	Boss->ArmAttack.collisionDebug = TRUE;

	Vec3Set(&TextTint, 1, 1, 1);
	LevelName = CreateText("ArmGuy Level", 0, 300, 100, TextTint, Center);
	TextProgressiveInit(LevelName);

	CreateBoundingBoxes();
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateArmGuy(void)
{
	TextProgressiveVisible(LevelName, 30);
	EventArmGuy();
	// This should be the last line in this function
	UpdateArmGuyBoss(Boss);
	UpdatePlayerPosition(&CurrentPlayer);

	UpdateFloatingText();
	BoundingBoxUpdate();
}

/*************************************************************************/
/*!
	\brief
	Draws the level
*/
/*************************************************************************/
void DrawArmGuy(void)
{
	// Draws the object list and sets the camera to the correct location
	DrawObjectList();
	//DrawHUD(&HUDList);
	DrawCollisionList();
	displayCollisionDebug(&Boss->BossCollider);
	displayCollisionDebug(&Boss->ArmAttack);
	displayCollisionDebug(&Boss->SpinAttack);
}

/*************************************************************************/
/*!
	\brief
	Frees all the objects in the level
*/
/*************************************************************************/
void FreeArmGuy(void)
{
	if(levelComplete && CurrentPlayer.CurrentLevel < GS_Level3)
		CurrentPlayer.CurrentLevel = GS_Level3;
	else if(CurrentPlayer.CurrentLevel < GS_ArmGuy)
		CurrentPlayer.CurrentLevel = GS_ArmGuy;
	SavePlayer(&CurrentPlayer);
	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadArmGuy(void)
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
void EventArmGuy(void)
{
	// Check for any collision and handle the results
	DetectPlayerCollision();
	DetectBossCollision(Boss);
	// Handle any input for the current player
	InputPlayer(&CurrentPlayer);

	if(FoxInput_KeyTriggered('U'))
	{
		SetDebugMode();
	}
	if(FoxInput_KeyTriggered('I'))
	{
		RemoveDebugMode();
	}
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		//InitializePause(&DrawArmGuy);
		//UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
		SetNextState(GS_MainMenu);
		//TogglePauseSound(&BackgroundSnd);
	}
	if(FoxInput_KeyTriggered('K'))
	{
		TextProgressiveEnd(LevelName);
	}
}
