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
static int PlayerIsAlive; 
static int numPanels;

TextGlyphs *LevelName;

static EnemySpawner *Spawners[6]; //Two per panel, Left & Right

Enemy *SetEnemy1;
Enemy *SetEnemy2;

Platform *Table1;

Wall *Wall1;

FoxSound *BackSnd;

Sprite *Arrow1;
Sprite *Arrow2;
Sprite *Arrow3;

static int Arrow1Grow;
static int Arrow2Grow;
static int Arrow3Grow;

Sprite *BlackOverlay;
HUD *CurrentHUD;

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
	PlayerIsAlive = TRUE;
	numPanels = 3;
	newID = 10;
	ResetObjectList();
	ResetCamera();
	ResetEnemyPanelNumber();

	//Set the camera so it currently isn't gated
	ResetGatedCamera();

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -1200, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	CurrentHUD = CreateHUD(&CurrentPlayer);

	Vec3Set(&TextTint, 1, 1, 1);
	LevelName = CreateText("Level 1", 0, 500, 100, TextTint, Center, Border);
	ChangeTextVisibility(LevelName);

	//CreatePaperScroll(200);

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	//Panel 1
	CreateSprite("TextureFiles/FoxMansion2.png", 1920, 1080, 0, 1, 1, 0, 0);
	CreateSprite("TextureFiles/OverlayDoorTorch.png", 1920, 1080, 200, 1, 1, 0, 0);
	//Panel 2
	CreateSprite("TextureFiles/FoxMansionHall1.png", 1920, 1080, 0, 1, 1, 1920, 0);
	CreateSprite("TextureFiles/OverlayDoor.png", 1920, 1080, 300, 1, 1, 1920, 0);
	//Panel 3
	CreateSprite("TextureFiles/FoxMansion1.png", 1920, 1080, 0, 1, 1, (1920 * 2), 0);
	
	//Black Overlay
	Vec3Set(&TextTint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = TextTint;

	//Bounding Boxes
	CreateBoundingBoxes();

	//Background Sound
	BackSnd = CreateSound("Sounds/Temp.mp3", LargeSnd);

	// Arrows
	Arrow1 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 250, 235, 200, 1, 1, 0, 200);
	Arrow2 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 250, 235, 200, 1, 1, PANELSIZE, 200);
	Arrow3 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 250, 235, 200, 1, 1, 2 * PANELSIZE, 200);

	Arrow1Grow = FALSE;
	Arrow2Grow = TRUE;
	Arrow3Grow = FALSE;

	/////////////////////////////////
	//		Platforms			   //
	/////////////////////////////////
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
	
	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 160.0f, 500.0f, newID++, 865, 130);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 1040.0f, newID++, -900, 0);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 160.0f, 500.0f, newID++, 2785, 130);
	Wall1->WallSprite->Visible = FALSE;
	
	// Blocker Box Walls
	CreateBlockerBoxes(&newID);	

	/////////////////////////////////
	//			Particles		   //
	/////////////////////////////////
	//Particles fo Torches
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 695, -140, 10, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 806, -235, 201, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	/////////////////////////////////
	//			Spawners		   //
	/////////////////////////////////
	//Enemy spawners (always set left and then right if none set to null)
	Vec2Set(&SpawnerLocation, -(PANELSIZE / 4) - 100, 0);
	//First Spawner Left (none so set to null
	Spawners[0] = NULL;
	//First Spawner Right
	Spawners[1] = CreateEnemySpawner(1, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 0);

	Vec2Set(&SpawnerLocation, PANELSIZE, 0);
	//2nd Spawner Left
	Spawners[2] = CreateEnemySpawner(1, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 1);
	//2nd Spawner Right
	Spawners[3] = CreateEnemySpawner(2, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 1);

	Vec2Set(&SpawnerLocation, 1.90f * PANELSIZE, 0);
	//3rd Spawner Left
	Spawners[4] = CreateEnemySpawner(1, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 2);
	//3rd Spawner Right
	Spawners[5] = CreateEnemySpawner(1, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 2);

	SetEnemy1 = CreateEnemy(BasicMelee, EnemyType, newID++, 2.25f * PANELSIZE, GROUNDLEVEL, 2);
	SetEnemy2 = CreateEnemy(BasicMelee, EnemyType, newID++, 2.25f * PANELSIZE, GROUNDLEVEL, 2);
	SetEnemy1->HomePos.x = 2 * PANELSIZE;
	SetEnemy2->HomePos.x = 2 * PANELSIZE;

	/////////////////////////////////
	//		On Death			   //
	/////////////////////////////////

	///Last thing in initialize
	CreateDeathConfirmObjects(&newID);
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateLevel1(void)
{
	//Handle the special events right off the bat yo
	EventLevel1();
	PlayAudio(BackSnd);

	// Update all the arrows
	UpdateArrow(Arrow1, &Arrow1Grow);
	UpdateArrow(Arrow2, &Arrow2Grow);
	UpdateArrow(Arrow3, &Arrow3Grow);
	
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
	DrawCollisionList();
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
	{
		CurrentPlayer.levelClearBitFlags |= 2;
		CurrentPlayer.CurrentLevel = GS_Level2;
	}
	else if(CurrentPlayer.CurrentLevel < GS_Level1)
		CurrentPlayer.CurrentLevel = GS_Level1;

	//Only save stats if the level was actually completed
	if (levelComplete)
		LevelCompletion(&CurrentPlayer);

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
	//////////////////////////////////
	//   INPUT & COLLISION FIRST    //
	//////////////////////////////////
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		if(PlayerIsAlive == TRUE)
		{
			InitializePause(&DrawLevel1);
			TogglePauseSound(BackSnd);
			UpdatePause();
			TogglePauseSound(BackSnd);
		}
	}
	if(FoxInput_KeyTriggered('U'))
		SetDebugMode();
	if(FoxInput_KeyTriggered('I'))
		RemoveDebugMode();

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
			CurrentPlayer.FlipX = TRUE;
			CurrentPlayer.PlayerDirection = RIGHT;
			CurrentPlayer.Speed = CurrentPlayer.CurrentPlayerStats.MoveSpeed * GetDeltaTime();
			
			// Threshold to give control back to the player
			if(CurrentPlayer.Position.x > -800)
				beginningAnimiation = FALSE;
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

	SetUpCameraPanAndLock(&levelComplete, PANELSIZE, Spawners, numPanels);
	UpdateBlockerBoxes(PANELSIZE);

	//////////////////////////////////
	//       EVERYTHING ELSE        //
	//////////////////////////////////
	UpdateAllEnemies();
	UpdateFloatingText();
	ParticleSystemUpdate();
	BoundingBoxUpdate();

	//Setting Enemy AI Stuff
	if(CurrentPlayer.Position.x > (1.5f * PANELSIZE))
	{
		if(SetEnemy1->EnemyState == AINone)
		{
			SetEnemy1->EnemyState = AIIdle;
			SetEnemy2->EnemyState = AIIdle;
		}
	}
	else
	{
		SetEnemy1->EnemyState = AINone;
		SetEnemy2->EnemyState = AINone;
	}

	//Player Dies
	if(CurrentPlayer.CurrentPlayerStats.CurrentHealth <= 0.0f)
	{
		PlayerIsAlive = FALSE;
		BlackOverlay->Position.x = GetCameraXPosition();
		BlackOverlay->Alpha = 0.5f;

		UpdateDeathConfirmObjects();
	}
}
