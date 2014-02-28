/*****************************************************************************/
/*!
\file				Level1.c
\author				Dan Muller (d.muller)
					Juli Gregg (j.gregg)
\date				Feb 15, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					LoadLevel1
\li					InitializeLevel1
\li					UpdateLevel1
\li					DrawLevel1
\li					FreeLevel1
\li					UnloadLevel1
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Level1.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"
#include "../HeaderFiles/BoundingBox.h"

// ---------------------------------------------------------------------------
// defines
#define PANELSIZE 1920.0f

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
static int newID;					// ID number
static int levelComplete;
static int beginningAnimiation;
TextGlyphs* LevelName;

EnemySpawner* FirstSpawner;

Platform* Table1;

Wall* Wall1;

Sprite* BlackOverlay;
HUD* CurrentHUD;

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadLevel1(void)
{
	//Allocate space for a large texture
	CreateTextureList();
	LoadEnemy(BasicMelee);
}

/*************************************************************************/
/*!
	\brief
	Initializes the objects for the level
*/
/*************************************************************************/
void InitializeLevel1(void)
{
	Vec3 TextTint;
	Vec2 SpawnerLocation;

	beginningAnimiation = TRUE;
	levelComplete = FALSE;
	newID = 10;
	ResetObjectList();
	ResetCamera();

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -1200, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	CurrentHUD = CreateHUD(&CurrentPlayer);

	Vec3Set(&TextTint, 1, 1, 1);
	LevelName = CreateText("Level 1", 0, 500, 100, TextTint, Center);
	ChangeTextVisibility(LevelName);

	//Bounding Boxes
	CreateBoundingBoxes();

	//Background: Panel 1
	CreateSprite("TextureFiles/FoxMansion2.png", 1920, 1080, 0, 1, 1, 0, 0);
	CreateSprite("TextureFiles/OverlayDoorTorch.png", 1920, 1080, 200, 1, 1, 0, 0);
	CreateSprite("TextureFiles/FoxMansionHall1.png", 1920, 1080, 0, 1, 1, 1920, 0);
	CreateSprite("TextureFiles/OverlayDoor.png", 1920, 1080, 300, 1, 1, 1920, 0);
	CreateSprite("TextureFiles/FoxMansion1.png", 1920, 1080, 0, 1, 1, (1920 * 2), 0);
	Vec3Set(&TextTint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 400, 1, 1, 0, 0);
	BlackOverlay->Tint = TextTint;

	//Platforms
	Table1 = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 40.0f, newID++, -628, -285);
	Table1->PlatformSprite->Visible = FALSE;

	Table1 = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 40.0f, newID++, -288, -285);
	Table1->PlatformSprite->Visible = FALSE;

	Table1 = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 40.0f, newID++, 68, -285);
	Table1->PlatformSprite->Visible = FALSE;

	Table1 = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 40.0f, newID++, 450, -285);
	Table1->PlatformSprite->Visible = FALSE;

	Table1 = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 290.0f, 40.0f, newID++, 1774, -255);
	Table1->PlatformSprite->Visible = FALSE;
	
	//Walls
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 160.0f, 500.0f, newID++, 865, 130);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 1040.0f, newID++, -900, 0);
	Wall1->WallSprite->Visible = FALSE;

	//Enemy spawners
	Vec2Set(&SpawnerLocation, 0, 0);
	FirstSpawner = CreateEnemySpawner(3, BasicMelee, TRUE, 200, 1080, SpawnerLocation, &newID);

	//Particles fo Torches
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 695, -140, 10, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 806, -235, 201, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	CreatePaperScroll(200);
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateLevel1(void)
{
	EventLevel1();

	ParticleSystemUpdate();
	BoundingBoxUpdate();

	/*
	if(FoxInput_KeyTriggered('R'))
		Scroll = TRUE;
	if(FoxInput_KeyTriggered('T'))
		ReScroll = TRUE;
	if(Scroll == TRUE)
		ScrollPaperScroll(2);
	if(ReScroll == TRUE)
	{
		Scroll = FALSE;
		ReScrollPaperScroll(2);
	}
	*/
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
void DrawLevel1(void)
{
	// Draws the object list and sets the camera to the correct location
	DrawObjectList();
	//DrawHUD(&HUDList);
	DrawCollisionList();

	if(CurrentPlayer.Position.x > -(PANELSIZE / 2) * GetLoadRatio() && CurrentPlayer.Position.x < (PANELSIZE / 2) * GetLoadRatio())
		SetCameraPan(0.0f, PANELSIZE);
	else if(CurrentPlayer.Position.x > (PANELSIZE / 2) * GetLoadRatio() && CurrentPlayer.Position.x < (PANELSIZE + (PANELSIZE / 2)) * GetLoadRatio())
		SetCameraPan(PANELSIZE * GetLoadRatio(), PANELSIZE);
	else if(CurrentPlayer.Position.x > (PANELSIZE + (PANELSIZE / 2)) * GetLoadRatio() && CurrentPlayer.Position.x < ((PANELSIZE * 2) + (PANELSIZE / 2)) * GetLoadRatio())
		SetCameraPan((PANELSIZE * 2) * GetLoadRatio(), PANELSIZE);
	else if(CurrentPlayer.Position.x > (PANELSIZE / 2) * 5 * GetLoadRatio() + CurrentPlayer.PlayerCollider.width)
	{
		levelComplete = TRUE;
	}

	//else
		//SetCamera(&CurrentPlayer.Position, 250);

}

/*************************************************************************/
/*!
	\brief
	Frees all the objects in the level
*/
/*************************************************************************/
void FreeLevel1(void)
{
	if(levelComplete && CurrentPlayer.CurrentLevel < GS_Level2)
		CurrentPlayer.CurrentLevel = GS_Level2;
	else if(CurrentPlayer.CurrentLevel < GS_Level1)
		CurrentPlayer.CurrentLevel = GS_Level1;

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
void UnloadLevel1(void)
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
void EventLevel1(void)
{
	int i;

	// Runs if the beginning animation is finished
	if(!beginningAnimiation && !levelComplete)
	{
		// Check for any collision and handle the results
		DetectPlayerCollision();
		// Handle any input for the current player
		InputPlayer(&CurrentPlayer);
		UpdateHUDItems(CurrentHUD, &CurrentPlayer);
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
			CurrentPlayer.FlipX = 1;
			CurrentPlayer.PlayerDirection = RIGHT;
			CurrentPlayer.Speed = CurrentPlayer.CurrentPlayerStats.MoveSpeed * GetLoadRatio() * GetDeltaTime();
			
			// Threshold to give control back to the player
			if(CurrentPlayer.Position.x > -500)
			{
				beginningAnimiation = FALSE;
			}
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
		{
			SetNextState(GS_MapLevel);
		}

	}

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
		//InitializePause(&DrawLevel1);
		//TogglePauseSound(&BackgroundSnd);
		SetNextState(GS_MainMenu);
		//UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
	}
}
