/*****************************************************************************/
/*!
\file				YeahGuy.c
\author				Dan Muller (d.muller)
\date				Feb 15, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					LoadYeahGuy
\li					InitializeYeahGuy
\li					UpdateYeahGuy
\li					DrawYeahGuy
\li					FreeYeahGuy
\li					UnloadYeahGuy
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/YeahGuy.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"


// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
static int newID;					// ID number
static int levelComplete = TRUE;
TextGlyphs* LevelName;

Sprite* DebugCircle;

YeahGuyBoss *Boss;

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadYeahGuy(void)
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
void InitializeYeahGuy(void)
{
	Vec3 TextTint;
	newID = 10;
	ResetObjectList();
	ResetCamera();

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, 0, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	//Panel1
	CreateSprite("TextureFiles/ArmGuyBackground.png", 1920, 1080, 1, 1, 1, 0, 0);

	//Background
	CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 300, 50, newID++, -550, -170);
	CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 300, 50, newID++, 550, -170);
	CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 300, 50, newID++, 0, 100);

	//Create bounding walls
	CreateWall("TextureFiles/BlankPlatform.png", 400.0f, 1040.0f, newID++, -1160, 0);
	CreateWall("TextureFiles/BlankPlatform.png", 400.0f, 1040.0f, newID++, 1160, 0);

	Boss = CreateYeahGuyBoss(0, 0, &newID);
	Boss->BossCollider.collisionDebug = TRUE;

	DebugCircle = (Sprite *)CreateSprite("TextureFiles/DebugCircle.png", Boss->YeahAOERadius * 2, Boss->YeahAOERadius * 2, 300, 1, 1, Boss->Position.x, Boss->Position.y);
	DebugCircle->Visible = FALSE;

	Vec3Set(&TextTint, 1, 1, 1);
	LevelName = CreateText("YeahGuy Level", 0, 300, 100, TextTint, Center, Border);

	CreateBoundingBoxes();
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateYeahGuy(void)
{
	EventYeahGuy();
	// This should be the last line in this function
	UpdateYeahGuyBoss(Boss);
	UpdatePlayerPosition(&CurrentPlayer);

	UpdateFloatingText();
	BoundingBoxUpdate();

	UpdateAllProjectiles();

	if(Boss->CurrentState == 1 && Boss->InnerState != 2)
	{
		DebugCircle->Visible = TRUE;
		DebugCircle->Position = Boss->Position;
	}
	else
		DebugCircle->Visible = FALSE;

}

/*************************************************************************/
/*!
	\brief
	Draws the level
*/
/*************************************************************************/
void DrawYeahGuy(void)
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
void FreeYeahGuy(void)
{
	if(levelComplete)
		CurrentPlayer.CurrentLevel = GS_Level7;
	else
		CurrentPlayer.CurrentLevel = GS_YeahGuy;
	SavePlayer(&CurrentPlayer);
	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadYeahGuy(void)
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
void EventYeahGuy(void)
{
	// Check for any collision and handle the results
	DetectPlayerCollision();
	DetectYeahGuyBossCollision(Boss);
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
		InitializePause(&DrawYeahGuy);
		//TogglePauseSound(&BackgroundSnd);
		UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
	}
}
