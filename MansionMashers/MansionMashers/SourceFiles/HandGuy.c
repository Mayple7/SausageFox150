/*****************************************************************************/
/*!
\file				HandGuy.c
\author				Dan Muller (d.muller)
\date				Feb 15, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					LoadHandGuy
\li					InitializeHandGuy
\li					UpdateHandGuy
\li					DrawHandGuy
\li					FreeHandGuy
\li					UnloadHandGuy
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/HandGuy.h"
#include "../HeaderFiles/HandGuyBoss.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"
#include "../HeaderFiles/BoundingBox.h"


// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
static int newID;					// ID number
static int levelComplete = FALSE;
TextGlyphs* LevelName;

Sprite* DebugCircle;

Platform* Plat;

HandGuyBoss *Boss;

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadHandGuy(void)
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
void InitializeHandGuy(void)
{
	Vec3 TextTint;
	newID = 10;
	ResetObjectList();
	ResetCamera();

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, 0, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	//Create background
	CreateSprite("TextureFiles/HandGuyBG.png", 1920, 1080, 1, 1, 1, 0, 0);

	//Background
	//CreateSprite("TextureFiles/MansionHandGauy.png", 1920, 1080, 2, 1, 1, 140, 0);
	//CreateSprite("TextureFiles/MansionHandGauyDoor.png", 1920, 1080, 200, 1, 1, 140, 0);

	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 400, 50, newID++, 0, -180);
	Plat->PlatformSprite->Visible = FALSE;

	//Create bounding walls
	CreateWall("TextureFiles/BlankPlatform.png", 400.0f, 1040.0f, newID++, -1160, 0);
	CreateWall("TextureFiles/BlankPlatform.png", 400.0f, 1040.0f, newID++, 1160, 0);

	Boss = CreateHandGuyBoss(0, 0, &newID);

	DebugCircle = (Sprite *)CreateSprite("TextureFiles/DebugCircle.png", Boss->ShoutRadius * 2, Boss->ShoutRadius * 2, 300, 1, 1, Boss->Position.x, Boss->Position.y);
	DebugCircle->Visible = FALSE;

	Vec3Set(&TextTint, 1, 1, 1);
	LevelName = CreateText("HandGuy Level", 0, 300, 100, TextTint, Center, Border);
	TextDisappearInit(LevelName);

	CreateBoundingBoxes();
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateHandGuy(void)
{
	TextProgressiveDisappear(LevelName, 30);
	EventHandGuy();
	// This should be the last line in this function
	UpdateHandGuyBoss(Boss);
	UpdatePlayerPosition(&CurrentPlayer);

	UpdateFloatingText();
	BoundingBoxUpdate();

	UpdateAllProjectiles();
	ParticleSystemUpdate();

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
void DrawHandGuy(void)
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
void FreeHandGuy(void)
{
	if(levelComplete && CurrentPlayer.CurrentLevel < GS_Level5)
	{
		CurrentPlayer.CurrentLevel = GS_Level5;
		CurrentPlayer.handClear = TRUE;
	}
	else if(CurrentPlayer.CurrentLevel < GS_HandGuy)
		CurrentPlayer.CurrentLevel = GS_HandGuy;

	//Only save stats if the level was actually completed
	if (levelComplete)
		SavePlayer(&CurrentPlayer);

	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadHandGuy(void)
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
void EventHandGuy(void)
{
	// Check for any collision and handle the results
	DetectPlayerCollision();
	DetectHandGuyBossCollision(Boss);
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
		InitializePause(&DrawHandGuy);
		UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
		//TogglePauseSound(&BackgroundSnd);
	}
	if(FoxInput_KeyTriggered('K'))
	{
		TextDisappearEnd(LevelName);
	}

	if(FoxInput_KeyTriggered('Y'))
	{
		CreateProjectile("TextureFiles/HandGauy.png", 100, 100, 780, -300, Arrow, WeaponEnemy, newID++, 10, -400, 0);
	}
}
