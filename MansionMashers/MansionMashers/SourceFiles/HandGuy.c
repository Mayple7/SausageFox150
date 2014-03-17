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
static int levelComplete = FALSE;
TextGlyphs *LevelName;
Sprite *HandGauy;
Platform *Pedestal;

Enemy* SetEnemy1;

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

	//Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, 0, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;
	CurrentPlayer.PlayerCollider.collisionDebug = FALSE;

	HandGauy = (Sprite *) CreateSprite("TextureFiles/HandGauy.png", 400, 400, 10, 1, 1, -800, -250);

	//Background
	CreateSprite("TextureFiles/MansionHandGauy.png", 1920, 1080, 0, 1, 1, 0, 0);
	CreateSprite("TextureFiles/MansionHandGauyDoor.png", 1920, 1080, 200, 1, 1, 0, 0);

	//Weapon/Shop
	CreateWeaponShop(-400, -140, newID++, Sword, Common);

	//Platform
	Pedestal = CreatePlatform("TextureFiles/MarblePedestal.png", PlatformType, 1920, 1080, newID++, 0, 0);
	Pedestal->PlatformCollider.Offset.y = -3.6f * Pedestal->PlatformSprite->Height / 16;
	UpdateCollider(&Pedestal->PlatformCollider, Pedestal->PlatformCollider.width * 0.19f, Pedestal->PlatformCollider.height * 0.05f); 

	//Title text
	Vec3Set(&TextTint, 1, 1, 1);
	LevelName = CreateText("HandGauy Level", 0, 300, 100, TextTint, Center, Border);
	ChangeTextVisibility(LevelName);

	//BALLISTA
	CreateEnemy(BasicRanged, EnemyType, newID++, 400, GROUNDLEVEL, 0);
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateHandGuy(void)
{
	EventHandGuy();

	ParticleSystemUpdate();

	UpdateFloatingText();

	UpdateAllProjectiles();

	UpdateAllEnemies();

	// This should be the last line in this function
	UpdatePlayerPosition(&CurrentPlayer);
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
	if(levelComplete && CurrentPlayer.CurrentLevel < GS_Level3)
		CurrentPlayer.CurrentLevel = GS_Level3;
	else if(CurrentPlayer.CurrentLevel < GS_HandGuy)
		CurrentPlayer.CurrentLevel = GS_HandGuy;

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
	// Handle any input for the current player
	InputPlayer(&CurrentPlayer);

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
		//InitializePause(&DrawHandGuy);
		//TogglePauseSound(&BackgroundSnd);
		SetNextState(GS_MainMenu);
		//UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
	}

	if(FoxInput_KeyTriggered('Y'))
	{
		CreateProjectile("TextureFiles/HandGauy.png", 100, 100, 780, -300, Arrow, WeaponEnemy, newID++, 10, -400);
	}
}
