/*****************************************************************************/
/*!
\file				Level4.c
\author				Dan Muller (d.muller)
\date				Feb 15, 2044

\brief				Functions for the showcase level

\par				Functions:
\li					LoadLevel4
\li					InitializeLevel4
\li					UpdateLevel4
\li					DrawLevel4
\li					FreeLevel4
\li					UnloadLevel4
  
\par 
<b> Copyright (C) 2044 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Level4.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"


// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// defines
#define PANELSIZE 1920.0f

// ---------------------------------------------------------------------------
// globals
static int newID;					// ID number
static int levelComplete;
static int beginningAnimation;
static int PlayerIsAlive;
TextGlyphs *LevelName;

Sprite* BlackOverlay;

// Tree Background
Sprite* TreeBackground1[4];
Sprite* TreeBackground2[4];
Sprite* TreeBackground3[4];
static void TreeBackgroundUpdate(void);

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadLevel4(void)
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
void InitializeLevel4(void)
{
	Vec3 TextTint;
	int i;
	newID = 10;
	ResetObjectList();
	ResetCamera();
	ResetEnemyPanelNumber();
	ResetGatedCamera();
	levelComplete = FALSE;
	beginningAnimation = TRUE;
	PlayerIsAlive = TRUE;

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -1300, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	Vec3Set(&TextTint, 1, 1, 1);
	LevelName = CreateText("Level 4", 0, 300, 100, TextTint, Center, Border);
	ChangeTextVisibility(LevelName);

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	//Panel1
	CreateSprite("TextureFiles/Level4Pan0.png", 1920, 1080, 5, 1, 1, 0, 0);
	CreateSprite("TextureFiles/Level4Pan1.png", 1920, 1080, 5, 1, 1, PANELSIZE, 0);
	CreateSprite("TextureFiles/Level4Pan2.png", 1920, 1080, 5, 1, 1, PANELSIZE * 2, 0);

	//Tree Backgrounds
	for(i = 0; i < 4; i++)
		TreeBackground1[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground1.png", 1920, 1080, 2, 1, 1, 1920.0f * i, 0);

	for(i = 0; i < 4; i++)
		TreeBackground2[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground2.png", 1920, 1080, 1, 1, 1, 1920.0f * i, 0);

	for(i = 0; i < 4; i++)
		TreeBackground3[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground3.png", 1920, 1080, 0, 1, 1, 1920.0f * i, 0);

	//Bounding Boxes
	CreateBoundingBoxes();

	//Black Overlay
	Vec3Set(&TextTint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = TextTint;

	/////////////////////////////////
	//		Platforms			   //
	/////////////////////////////////

	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////

	/////////////////////////////////
	//		Spawners			   //
	/////////////////////////////////

	/////////////////////////////////
	//		On Death			   //
	/////////////////////////////////
	CreateDeathConfirmObjects(&newID);
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateLevel4(void)
{
	EventLevel4();

	// This should be the last line in this function
	UpdatePlayerPosition(&CurrentPlayer);
}

/*************************************************************************/
/*!
	\brief
	Draws the level
*/
/*************************************************************************/
void DrawLevel4(void)
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
void FreeLevel4(void)
{
	if(levelComplete && CurrentPlayer.CurrentLevel < GS_Kevin)
		CurrentPlayer.CurrentLevel = GS_Kevin;
	else if(CurrentPlayer.CurrentLevel < GS_Level4)
		CurrentPlayer.CurrentLevel = GS_Level4;

	SavePlayer(&CurrentPlayer);
	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadLevel4(void)
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
void EventLevel4(void)
{
	//////////////////////////////////
	//   INPUT & COLLISION FIRST    //
	//////////////////////////////////

	if(FoxInput_KeyTriggered('U'))
		SetDebugMode();
	
	if(FoxInput_KeyTriggered('I'))
		RemoveDebugMode();

	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		InitializePause(&DrawLevel4);
		//TogglePauseSound(BackSnd);
		UpdatePause();
		//TogglePauseSound(BackSnd);
	}

	// Runs if the beginning animation is finished
	if(!beginningAnimation && !levelComplete)
	{
		// Check for any collision and handle the results
		DetectPlayerCollision();
		// Handle any input for the current player
		InputPlayer(&CurrentPlayer);
		//UpdateHUDItems(CurrentHUD, &CurrentPlayer);
	}
	else if(!levelComplete)
	{
		// Fade in the level
		if(BlackOverlay->Alpha > 0)
		{
			BlackOverlay->Alpha -= 1 * GetDeltaTime();
		}
		// Makes the player walk into view
		else
		{
			BlackOverlay->Alpha = 0.0f;
			CurrentPlayer.FlipX = TRUE;
			CurrentPlayer.PlayerDirection = RIGHT;
			CurrentPlayer.Speed = CurrentPlayer.CurrentPlayerStats.MoveSpeed * GetDeltaTime();
			
			// Threshold to give control back to the player
			if(CurrentPlayer.Position.x > -800)
				beginningAnimation = FALSE;
		}
		//Always animate the player otherwise the sprites get stuck in the middle
		Animation(&CurrentPlayer);
		UpdateCollisionPosition(&CurrentPlayer.PlayerWeapon->WeaponAttack, &CurrentPlayer.PlayerWeapon->WeaponAttackPosition);
		MoveObject(&CurrentPlayer.Position, CurrentPlayer.PlayerDirection, CurrentPlayer.Speed);
	}
	else
	{
		BlackOverlay->Position.x = GetCameraXPosition();
		BlackOverlay->Alpha += 1 * GetDeltaTime();
		if(BlackOverlay->Alpha > 1)
			SetNextState(GS_MapLevel);
	}

	//////////////////////////////////
	//    CAMERA POSITION SECOND    //
	//////////////////////////////////
	if(PlayerIsAlive == TRUE)
	{
		//Don't Let camera go beyond left boundary
		if(CurrentPlayer.Position.x <= 0 && GetCameraXPosition() <= 5.0f)
			SetCameraXPosition(0.0f);
		//Don't Let camera go beyond right boundary
		else if(CurrentPlayer.Position.x >= (PANELSIZE * 3) && GetCameraXPosition() >= ((PANELSIZE * 3 - 5.0f)))
			SetCameraXPosition(PANELSIZE * 3);
		//Free Roam Camera
		else
			SetCamera(&CurrentPlayer.Position, 250);
	}

	//////////////////////////////////
	//       EVERYTHING ELSE        //
	//////////////////////////////////
	BoundingBoxUpdate();
	ParticleSystemUpdate();
	TreeBackgroundUpdate();
	UpdateAllEnemies();
	UpdateFloatingText();
	
	//If player dies
	if(CurrentPlayer.CurrentPlayerStats.CurrentHealth <= 0.0f)
	{
		PlayerIsAlive = FALSE;
		BlackOverlay->Alpha = 0.5f;

		UpdateDeathConfirmObjects();
	}
}

void TreeBackgroundUpdate(void)
{
	int i;

	for(i = 0; i < 4; i++)
		TreeBackground2[i]->Position.x = (1920.0f * i) + (GetCameraXPosition() / 30.0f);

	for(i = 0; i < 4; i++)
		TreeBackground3[i]->Position.x = (1920.0f * i) + (GetCameraXPosition() / 15.0f);
}
