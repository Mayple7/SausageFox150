/*****************************************************************************/
/*!
\file				Level2.c
\author				Juli Gregg (j.gregg)	
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


// ---------------------------------------------------------------------------
// defines
#define PANELSIZE 1920.0f

#define BACKGROUND_LENGTH 4

#define GLOW_OVERLAY_NUM 4

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
static int newID;					// ID number
static int levelComplete;
static int beginningAnimation;
static int enemiesDefeated;
static int counter;

static int enemiesRemaining;

Sprite *TxtScrollRight;
Sprite *TxtScrollMiddle;

Sprite* PlatOverlay[GLOW_OVERLAY_NUM];
static int GlowBool;

EnemySpawner* Spawners[6]; //2 Per Panel not including first (can do this because no blocker boxes)

Enemy* Pan3Enemy;

Food* Strength1;

Platform *Crate;
Wall* Wall1;
Wall* WTBot;
Wall* LeftBarrier;
Wall* RightBarrier;

HUD* CurrentHUD;

// Arrows
Sprite *Arrow1;
static int Arrow1Grow;

FoxSound* BackSnd;
FoxSound* IntelFoxStart;
TextGlyphs* IntelFoxTxtStart;

Sprite* IntelFox;

static int PlayerInSight; // Enemy in watch tower checks for player 
static int PlayerIsAlive; 

Sprite* BlackOverlay;

// Tree Background
Sprite* TreeBackground1[BACKGROUND_LENGTH];
Sprite* TreeBackground2[BACKGROUND_LENGTH];
Sprite* TreeBackground3[BACKGROUND_LENGTH];
static void TreeBackgroundUpdate(void);
static void ObjectGlowUpdate();

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
	int i;
	Vec3 TextTint;
	Vec2 SpawnerLocation;

	levelComplete = FALSE;
	PlayerInSight = FALSE;
	PlayerIsAlive = TRUE;
	enemiesDefeated = FALSE;
	beginningAnimation = TRUE;
	GlowBool = TRUE;
	counter = 2 * FRAMERATE;

	newID = 10;
	ResetObjectList();
	ResetCamera();
	ResetEnemyPanelNumber();

	//Set the camera so it currently isn't gated
	ResetGatedCamera();

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -1150, -100);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;
	
	/////////////////////////////////
	//		Text				   //
	/////////////////////////////////
	Vec3Set(&TextTint, 1, 1, 1);
	IntelFoxTxtStart = CreateText("Mash all the enemies", 0, 150, 100, TextTint, Center, Border);
	ChangeTextZIndex(IntelFoxTxtStart, 500);
	TextProgressiveInit(IntelFoxTxtStart);

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	//Panel 1
	CreateSprite("TextureFiles/OutsideMan0.png", 1920, 1080, 5, 1, 1, 0, 0);
	CreateSprite("TextureFiles/OutsideMan0Overlay.png", 1920, 1080, 200, 1, 1, 0, 0);
	PlatOverlay[0] = (Sprite*)CreateSprite("TextureFiles/Lvl2Pan1PlatOverlay.png", 1920, 1080, 201, 1, 1, 0, 0);
	PlatOverlay[0]->Alpha = .1f;
	//Panel2
	CreateSprite("TextureFiles/OutsideMan1.png", 1920, 1080, 5, 1, 1, 1920, 0);
	PlatOverlay[1] = (Sprite*)CreateSprite("TextureFiles/Lvl2Pan2PlatOverlay.png", 1920, 1080, 6, 1, 1, 1920, 0);
	PlatOverlay[1]->Alpha = .2f;
	//Panel2
	CreateSprite("TextureFiles/OutsideMan2.png", 1920, 1080, 5, 1, 1, 1920 * 2, 0);
	CreateSprite("TextureFiles/OutsideMan2Overlay.png", 1920, 1080, 200, 1, 1, 1920 * 2, 0);
	PlatOverlay[2] = (Sprite*)CreateSprite("TextureFiles/Lvl2Pan3PlatOverlay.png", 1920, 1080, 6, 1, 1, 1920 * 2, 0);
	PlatOverlay[2]->Alpha = .2f;
	PlatOverlay[3] = (Sprite*)CreateSprite("TextureFiles/Lvl2Pan3PlatOverlay.png", 1920, 1080, 201, 1, 1, 1920 * 2, 0);
	PlatOverlay[3]->Alpha = .2f;
	//Panel 3
	CreateSprite("TextureFiles/OutsideMan3.png", 1920, 1080, 5, 1, 1, 1920 * 3, 0);
	CreateSprite("TextureFiles/OutsideMan3Overlay.png", 1920, 1080, 200, 1, 1, 1920 * 3, 0);

	for(i = 0; i < BACKGROUND_LENGTH; i++)
		TreeBackground1[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground1.png", 1920, 1080, 2, 1, 1, 1920.0f * i, 0);

	for(i = 0; i < BACKGROUND_LENGTH; i++)
		TreeBackground2[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground2.png", 1920, 1080, 1, 1, 1, 1920.0f * i, 0);

	for(i = 0; i < BACKGROUND_LENGTH; i++)
		TreeBackground3[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground3.png", 1920, 1080, 0, 1, 1, 1920.0f * i, 0);

	//Black Overlay
	Vec3Set(&TextTint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = TextTint;

	/////////////////////////////////
	//		Sounds				   //
	/////////////////////////////////
	BackSnd = CreateSound("Sounds/Temp.mp3", LargeSnd);
	IntelFoxStart = CreateSound("Sounds/IntelFoxLvl2Start.mp3", SmallSnd);

	/////////////////////////////////
	//		Platforms			   //
	/////////////////////////////////
	//Panel2
	Crate = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 100.0f, newID++, 2050, -340);
	Crate->PlatformSprite->Visible = FALSE;
	Crate = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 100.0f, newID++, 2170, -350);
	Crate->PlatformSprite->Visible = FALSE;
	//Panel3
	Crate = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 40.0f, newID++, 3640, -240);
	Crate->PlatformSprite->Visible = FALSE;
	Crate = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 90.0f, 100.0f, newID++, 3765, -210);
	Crate->PlatformSprite->Visible = FALSE;


	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	//Hidden Wall for beginning animation
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 1920, 500.0f, newID++, -1920, -432);
	Wall1->WallSprite->Visible = FALSE;
	
	//Stairs
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 450.0f, 100.0f, newID++, -869, -232);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -618, -281);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -559, -336);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -488, -370);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -408, -420);
	Wall1->WallSprite->Visible = FALSE;
	
	//Left Bounding Wall
	LeftBarrier = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 1040.0f, newID++, -958, 0);
	LeftBarrier->WallSprite->Visible = FALSE;
	//Stone Door
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 540.0f, newID++, 810, 140);
	Wall1->WallSprite->Visible = FALSE;
	//Door Hang Over
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -903, 226);
	Wall1->WallSprite->Visible = FALSE;
	//Top of Watch Tower
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 350.0f, 100.0f, newID++, 4020, 370);
	Wall1->WallSprite->Visible = FALSE;
	//Bottom of Watch Tower
	WTBot = CreateWall("TextureFiles/BlankPlatform.png", 260.0f, 65.0f, newID++, 4025, 22);
	WTBot->WallSprite->Visible = FALSE;
	//Top of Forest Door
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 200, 250.0f, newID++, 6630, 2);
	Wall1->WallSprite->Visible = FALSE;
	//Right Bounding Wall
	RightBarrier = CreateWall("TextureFiles/BlankPlatform.png", 200, 1080, newID++, 6630, 0);
	RightBarrier->WallSprite->Visible = FALSE;

	/////////////////////////////////
	//		Spawners			   //
	/////////////////////////////////
	//Panel2
	Vec2Set(&SpawnerLocation, PANELSIZE + PANELSIZE / 4, 0);
	//Left
	Spawners[0] = CreateEnemySpawner(1, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 1);
	//Right
	Spawners[1] = CreateEnemySpawner(2, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 1);

	//Panel3
	Vec2Set(&SpawnerLocation, (PANELSIZE * 2), 0);
	//Left
	Spawners[2] = CreateEnemySpawner(1, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 2);
	//Right
	Spawners[3] = CreateEnemySpawner(1, BasicRanged, TRUE, 100, 1080, SpawnerLocation, &newID, 2);
	//PreSpawned
	Pan3Enemy = CreateEnemy(BasicMelee, EnemyType, newID++, WTBot->Position.x, 300, 2);

	//Panel4
	Vec2Set(&SpawnerLocation, (PANELSIZE * 3 - PANELSIZE / 4), 0);
	//Left
	Spawners[4] = NULL;
	//Right
	Spawners[5] = CreateEnemySpawner(2, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 3);

	// Set number of enemies remaining
	enemiesRemaining = 0;
	for(i = 0; i < PANELAMOUNT; ++i)
	{
		enemiesRemaining += EnemyPanelNumber[i];
	}

	/////////////////////////////////
	//			Objects			   //
	/////////////////////////////////
	// Arrows
	Arrow1 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 180, 165, 202, 1, 1, 0, 0);
	Arrow1->Visible = FALSE;
	Arrow1Grow = FALSE;

	Strength1 = CreateFood(Strength, 100, 100, PANELSIZE * 2 + 190, 250, newID++);

	CreatePaperScroll(GetCameraXPosition(), 150);
	
	// Create the HUD
	CurrentHUD = CreateHUD(&CurrentPlayer);

	// Create Bounding boxes
	CreateBoundingBoxes();

	IntelFox = (Sprite*)CreateSprite("TextureFiles/IntelFoxHead.png", 256, 256, 300, 4, 1, 740, 380);
	IntelFox->Alpha = 0.0f;

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
void UpdateLevel2(void)
{
	int i, numEnemies = 0;

	EventLevel2();
	
	BoundingBoxUpdate();
	ParticleSystemUpdate();

	// Update remaining enemies
	for(i = 0; i < PANELAMOUNT; ++i)
	{
		numEnemies += EnemyPanelNumber[i];
	}

	// Update HUD if needed
	if(numEnemies < enemiesRemaining)
	{
		char CharTemp[32];

		enemiesRemaining = numEnemies;
		sprintf(CharTemp, "Enemies Remaining: %d", enemiesRemaining);
		ChangeTextString(CurrentHUD->StatusText, CharTemp);
	}

	if(CurrentPlayer.Position.x > (PANELSIZE * 2 - PANELSIZE / 4))
	{
		if(Pan3Enemy->EnemyState == AINone)
		{
			Pan3Enemy->EnemyState = AIIdle;
			PlayerInSight = TRUE;
		}
	}
	else if(PlayerInSight == FALSE)
		Pan3Enemy->EnemyState = AINone;

	PlayAudio(BackSnd);
	//ScrollPaperScroll(1);

	// This should be the last line in this function
	UpdatePlayerPosition(&CurrentPlayer);

	UpdateHUDPosition(CurrentHUD);
	UpdateHUDItems(CurrentHUD, &CurrentPlayer);
	BoundingBoxUpdate();
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
	if(levelComplete && CurrentPlayer.CurrentLevel < GS_Level3)
	{
		CurrentPlayer.CurrentLevel = GS_Level3;
	}
	else if(CurrentPlayer.CurrentLevel < GS_Level2)
		CurrentPlayer.CurrentLevel = GS_Level2;

	//Only save stats if the level was actually completed
	if (levelComplete)
	{
		CurrentPlayer.levelClearBitFlags |= 4;
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
		if(PlayerIsAlive == TRUE)
		{
			InitializePause(&DrawLevel2);
			TogglePauseSound(BackSnd);
			//SetNextState(GS_MainMenu);
			UpdatePause();
			TogglePauseSound(BackSnd);
		}
	}
	if(FoxInput_KeyTriggered('U'))
		SetDebugMode();
	if(FoxInput_KeyTriggered('I'))
		RemoveDebugMode();

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
			ZeroVelocity(&CurrentPlayer.PlayerRigidBody);
			CurrentPlayer.Position.y = -100;
			// Threshold to give control back to the player
			if(CurrentPlayer.Position.x > -850)
				beginningAnimation = FALSE;
		}
		//DetectCollision For stairs
		DetectPlayerCollision();
		//Always animate the player otherwise the sprites get stuck in the middle
		Animation(&CurrentPlayer);
		UpdateCollisionPosition(&CurrentPlayer.PlayerWeapon->WeaponAttack, &CurrentPlayer.PlayerWeapon->WeaponAttackPosition);
		MoveObject(&CurrentPlayer.Position, CurrentPlayer.PlayerDirection, CurrentPlayer.Speed);
	}

	/*////////////////////////////////
	//    CAMERA POSITION SECOND    //
	////////////////////////////////*/

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

	/*////////////////////////////////
	//       EVERYTHING ELSE        //
	////////////////////////////////*/
	UpdateAllEnemies();
	UpdateFloatingText();
	UpdateAllProjectiles();
	TreeBackgroundUpdate();
	ObjectGlowUpdate();
	UpdateArrow(Arrow1, &Arrow1Grow);

	//Switch barrier position for beginning
	if(beginningAnimation)
		LeftBarrier->WallCollider.Position.y = -1080;
	else
		LeftBarrier->WallCollider.Position.y = 0;

	//Intel Fox Starting Narrative
	if(beginningAnimation == FALSE && !IntelFoxStart->hasPlayed)
	{
		PlayAudio(IntelFoxStart);
		IntelFoxStart->hasPlayed = TRUE;
	}

	//When sound is play show Intel Fox in da corner
	if(FoxSoundCheckIsPlaying(IntelFoxStart))
	{
		if(IntelFox->Alpha < 1)
			IntelFox->Alpha += 3 * GetDeltaTime();
	}
	else
	{
		if(IntelFox->Alpha > 0)
			IntelFox->Alpha -= 3 * GetDeltaTime();
	}

	//Always update intel foxes position you need him
	IntelFox->Position.x = GetCameraXPosition() + 740;

	if(!beginningAnimation)
		SetUpScrollWithText(IntelFoxTxtStart, &counter);

	//Check if all enemies are dead & remove right barrier
	if(EnemyPanelNumber[1] <= 0 && EnemyPanelNumber[2] <= 0 && EnemyPanelNumber[3] <= 0)
	{
		enemiesDefeated = TRUE;
		RightBarrier->Position.y = -1080;
		UpdateCollisionPosition(&RightBarrier->WallCollider, &RightBarrier->Position);
		Arrow1->Visible = TRUE;
		Arrow1->Position.x = GetCameraXPosition() + 750;
	}

	//Level Transition
	BlackOverlay->Position.x = GetCameraXPosition();
	if(CurrentPlayer.Position.x >= (PANELSIZE * 3 + PANELSIZE / 2) && enemiesDefeated)
	{
		levelComplete = TRUE;
		BlackOverlay->Alpha += 1 * GetDeltaTime();
		if(BlackOverlay->Alpha > 1)
			SetNextState(GS_MapLevel);
	}

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

	for(i = 0; i < BACKGROUND_LENGTH; i++)
		TreeBackground2[i]->Position.x = (1920.0f * i) + (GetCameraXPosition() / 30.0f);

	for(i = 0; i < BACKGROUND_LENGTH; i++)
		TreeBackground3[i]->Position.x = (1920.0f * i) + (GetCameraXPosition() / 15.0f);
}

void ObjectGlowUpdate(void)
{
	int i;

	for(i = 0; i < GLOW_OVERLAY_NUM; i++)
	{
		if(GlowBool)
		{
			PlatOverlay[i]->Alpha += .005f;
			if(PlatOverlay[GLOW_OVERLAY_NUM - 1]->Alpha > 0.3f)
				GlowBool = FALSE;
		}
		else 
		{
			PlatOverlay[i]->Alpha -= .005f;
			if(PlatOverlay[GLOW_OVERLAY_NUM -1]->Alpha < 0.05f)
				GlowBool = TRUE;
		}
	}
}
