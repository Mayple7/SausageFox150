/*****************************************************************************/
/*!
\file				Level4.c
\author				Juli Gregg (j.gregg)
\date				Mar 18, 2014

\brief				Functions for level 4

\par				Functions:
\li					LoadLevel4
\li					InitializeLevel4
\li					UpdateLevel4
\li					DrawLevel4
\li					FreeLevel4
\li					UnloadLevel4
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
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
static int counter;
static int enemiesDefeated;

static int enemiesRemaining;

Sprite* BlackOverlay;
HUD* CurrentHUD;

// Arrows
Sprite *Arrow1;
static int Arrow1Grow;

Wall* Wall1;
Wall* RightBarrier;

EnemySpawner* Spawners[8];
Enemy* PreSpawned;

Food* Heal1;
Food* Defense1;

TextGlyphs *IntelFoxTxtStart;

Sprite* IntelFoxBack;
Sprite* IntelFox;
static float IntelFoxValue;

FoxSound* BackSnd;
FoxSound* IntelFoxStart;
FoxSound* IntelFoxEnd;

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
	Vec2 SpawnerLocation;
	int i;
	newID = 10;
	ResetObjectList();
	ResetCamera();
	ResetEnemyPanelNumber();
	ResetGatedCamera();
	levelComplete = FALSE;
	beginningAnimation = TRUE;
	PlayerIsAlive = TRUE;
	counter = 2 * FRAMERATE;
	enemiesDefeated = FALSE;
	

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -1100, -220);
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
	//Panel1
	CreateSprite("TextureFiles/Level4Pan0.png", 1920, 1080, 5, 1, 1, 0, 0);
	CreateSprite("TextureFiles/Level4Pan1.png", 1920, 1080, 5, 1, 1, PANELSIZE, 0);
	CreateSprite("TextureFiles/Level4Pan2.png", 1920, 1080, 5, 1, 1, PANELSIZE * 2, 0);
	CreateSprite("TextureFiles/Level4Pan3.png", 1920, 1080, 5, 1, 1, PANELSIZE * 3, 0);

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
	//		Sounds				   //
	/////////////////////////////////
	//Background Sound
	BackSnd = CreateSound("Sounds/Temp.mp3", LargeSnd);

	IntelFoxStart = CreateSound("Sounds/IntelFoxLvl4Start.mp3", SmallSnd);
	IntelFoxEnd = CreateSound("Sounds/IntelFoxLvl4End.mp3", SmallSnd);

	/////////////////////////////////
	//		Platforms			   //
	/////////////////////////////////

	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	//Right Bounding Wall
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 200, 1080, newID++, -(PANELSIZE / 2), 0);
	Wall1->WallSprite->Visible = FALSE;
	//Right Bounding Wall
	RightBarrier = CreateWall("TextureFiles/BlankPlatform.png", 200, 1080, newID++, 6630, 0);
	RightBarrier->WallSprite->Visible = FALSE;

	/////////////////////////////////
	//		Spawners			   //
	/////////////////////////////////
	//Panel1
	Vec2Set(&SpawnerLocation, -(PANELSIZE / 4), 0);
	//Left
	Spawners[0] = CreateEnemySpawner(1, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 0);
	//Right
	Spawners[1] = CreateEnemySpawner(2, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 0);
	//Panel2
	PreSpawned = CreateEnemy(BasicRanged, EnemyType, newID++, PANELSIZE, 0, 1);
	Vec2Set(&SpawnerLocation, PANELSIZE, 0);
	//Left
	Spawners[2] = CreateEnemySpawner(2, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 1);
	//Right
	Spawners[3] = CreateEnemySpawner(2, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 1);
	//Panel3
	Vec2Set(&SpawnerLocation, (PANELSIZE * 2), 0);
	//Left
	Spawners[4] = CreateEnemySpawner(2, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 2);
	//Right
	Spawners[5] = CreateEnemySpawner(3, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 2);
	//Panel4
	PreSpawned = CreateEnemy(BasicRanged, EnemyType, newID++, PANELSIZE * 3, 0, 3);
	Vec2Set(&SpawnerLocation, (PANELSIZE * 3), 0);
	//Left
	Spawners[6] = CreateEnemySpawner(3, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 3);
	//Right
	Spawners[7] = CreateEnemySpawner(2, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 3);

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
	Arrow1 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 180, 165, 90, 1, 1, 0, 0);
	Arrow1->Visible = FALSE;
	Arrow1Grow = FALSE;

	Heal1 = CreateFood(Heal, 100, 100, PANELSIZE * 2, -100, newID++);
	Defense1 = CreateFood(Defense, 100, 100, PANELSIZE - 500, -100, newID++);

	CreatePaperScroll(GetCameraXPosition(), 150);
	
	IntelFoxBack	= (Sprite*)CreateSprite("TextureFiles/IntelFoxHeadBack.png", 256, 256, 300, 1, 1, 740, 380);
	IntelFox		= (Sprite*)CreateSprite("TextureFiles/IntelFoxHead.png", 256, 256, 300, 1, 1, 740, 380);
	IntelFox->Alpha = 0.0f;
	IntelFoxValue	= 0.0f;

	// Create the HUD
	CurrentHUD = CreateHUD(&CurrentPlayer);

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
	int i, numEnemies = 0;

	EventLevel4();
	PlayAudio(BackSnd);

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
	// Level complete and not reached level 5 zone
	if(levelComplete && CurrentPlayer.CurrentLevel < GS_Level5)
	{
		CurrentPlayer.levelClearBitFlags |= 16;
		CurrentPlayer.CurrentLevel = GS_Level5;
		CurrentPlayer.handUnlock = TRUE;
	}
	// Level complete and has reached level 5 zone
	else if(levelComplete)
	{
		CurrentPlayer.levelClearBitFlags |= 16;
		CurrentPlayer.handUnlock = TRUE;
	}
	// Level NOT complete
	else if(CurrentPlayer.CurrentLevel < GS_Level4)
		CurrentPlayer.CurrentLevel = GS_Level4;

	//Only save stats if the level was actually completed
	if (levelComplete)
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
		if(PlayerIsAlive)
		{
			InitializePause(&DrawLevel4);
			TogglePauseSound(BackSnd);
			UpdatePause();
			TogglePauseSound(BackSnd);
		}
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
	UpdateAllProjectiles();
	UpdateArrow(Arrow1, &Arrow1Grow);

	//Intel Fox Starting Narrative
	if(beginningAnimation == FALSE && !IntelFoxStart->hasPlayed)
	{
		PlayAudio(IntelFoxStart);
		IntelFoxStart->hasPlayed = TRUE;
	}

	//When sound is play show Intel Fox in da corner
	if(FoxSoundCheckIsPlaying(IntelFoxStart) || FoxSoundCheckIsPlaying(IntelFoxEnd))
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

	IntelFoxValue += GetDeltaTime() * 8.0f;
	IntelFox->Rotation = sinf(IntelFoxValue) / 4.0f;

	IntelFoxBack->Position = IntelFox->Position;
	IntelFoxBack->Alpha = IntelFox->Alpha;

	SetUpScrollWithText(IntelFoxTxtStart, &counter);

	//Check if all enemies are dead & remove right barrier & play sounds & show arrow
	if(EnemyPanelNumber[1] <= 0 && EnemyPanelNumber[2] <= 0 && EnemyPanelNumber[3] <= 0)
	{
		enemiesDefeated = TRUE;
		RightBarrier->Position.y = -1080;
		UpdateCollisionPosition(&RightBarrier->WallCollider, &RightBarrier->Position);
		if(!IntelFoxEnd->hasPlayed && PlayerIsAlive)
		{
			PlayAudio(IntelFoxEnd);
			IntelFoxEnd->hasPlayed = TRUE;
		}
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

	for(i = 0; i < 4; i++)
		TreeBackground2[i]->Position.x = (1920.0f * i) + (GetCameraXPosition() / 30.0f);

	for(i = 0; i < 4; i++)
		TreeBackground3[i]->Position.x = (1920.0f * i) + (GetCameraXPosition() / 15.0f);
}
