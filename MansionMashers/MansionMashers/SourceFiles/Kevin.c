/*****************************************************************************/
/*!
\file				Kevin.c
\author				Dan Muller (d.muller)
\date				Feb 15, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					LoadKevin
\li					InitializeKevin
\li					UpdateKevin
\li					DrawKevin
\li					FreeKevin
\li					UnloadKevin
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Kevin.h"
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

Platform *Shelf;
Platform *Shelf2;
Platform *Shelf3;
Platform *Shelf4;

HUD *CurrentHUD;

EnemySpawner *FirstSpawner;

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadKevin(void)
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
void InitializeKevin(void)
{
	Vec2 SpawnerLocation;

	newID = 10;
	ResetObjectList();
	ResetCamera();

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -400, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	CurrentHUD = CreateHUD(&CurrentPlayer);

	Shelf = CreatePlatform("TextureFiles/ShortShelf.png", PlatformType, 184.5f, 198.75f, newID++, 280, -280);
	Shelf->PlatformCollider.Offset.y = 5 * Shelf->PlatformSprite->Height / 16;
	UpdateCollider(&Shelf->PlatformCollider, Shelf->PlatformCollider.width, Shelf->PlatformCollider.height * 0.2f); 

	Shelf2 = CreatePlatform("TextureFiles/ShortShelf.png", PlatformType, 184.5f, 198.75f, newID++, 100, -100);
	Shelf2->PlatformCollider.Offset.y = 5 * Shelf2->PlatformSprite->Height / 16;
	UpdateCollider(&Shelf2->PlatformCollider, Shelf2->PlatformCollider.width, Shelf2->PlatformCollider.height * 0.2f); 

	Shelf3 = CreatePlatform("TextureFiles/ShortShelf.png", PlatformType, 184.5f, 198.75f, newID++, -80, -100);
	Shelf3->PlatformCollider.Offset.y = 5 * Shelf3->PlatformSprite->Height / 16;
	UpdateCollider(&Shelf3->PlatformCollider, Shelf3->PlatformCollider.width, Shelf3->PlatformCollider.height * 0.2f); 

	Shelf4 = CreatePlatform("TextureFiles/ShortShelf.png", PlatformType, 184.5f, 198.75f, newID++, -260, -280);
	Shelf4->PlatformCollider.Offset.y = 5 * Shelf4->PlatformSprite->Height / 16;
	UpdateCollider(&Shelf4->PlatformCollider, Shelf4->PlatformCollider.width, Shelf4->PlatformCollider.height * 0.2f); 


	Vec2Set(&SpawnerLocation, -200, 0);
	FirstSpawner = CreateEnemySpawner(20, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 0);
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateKevin(void)
{
	EventKevin();

	// This should be the last line in this function
	UpdatePlayerPosition(&CurrentPlayer);

	UpdateAllProjectiles();
	ParticleSystemUpdate();

	UpdateHUDPosition(CurrentHUD);
	UpdateHUDItems(CurrentHUD, &CurrentPlayer);
}

/*************************************************************************/
/*!
	\brief
	Draws the level
*/
/*************************************************************************/
void DrawKevin(void)
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
void FreeKevin(void)
{
	if(CurrentPlayer.CurrentLevel < GS_Kevin)
		CurrentPlayer.CurrentLevel = GS_Kevin;

	//Only save stats if the level was actually completed
	if (levelComplete)
	{
		CurrentPlayer.levelClearBitFlags |= 512;
		SavePlayer(&CurrentPlayer);
	}

	FreeAllLists();
	FreeHUD(CurrentHUD);
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadKevin(void)
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
void EventKevin(void)
{
	int i;
	// Check for any collision and handle the results
	DetectPlayerCollision();
	// Handle any input for the current player
	InputPlayer(&CurrentPlayer);
	
	ParticleSystemUpdate();

	//Update the enemies
	for(i = 0; i < COLLIDEAMOUNT; i++)
	{
		if (enemyList[i].objID == -1)
			break;
		if (enemyList[i].objID == 0)
			continue;

		UpdateEnemy(&enemyList[i]);
	}

	UpdateFloatingText();

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
		//InitializePause(&DrawKevin);
		//TogglePauseSound(&BackgroundSnd);
		SetNextState(GS_MainMenu);
		//UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
	}
}
