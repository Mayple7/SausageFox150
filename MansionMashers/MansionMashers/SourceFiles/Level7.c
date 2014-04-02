/*****************************************************************************/
/*!
\file				Level7.c
\author				Juli Gregg (j.gregg)	
\date				Mar 8, 2014

\brief				Functions for Level7

\par				Functions:
\li					LoadLevel7
\li					InitializeLevel7
\li					UpdateLevel7
\li					DrawLevel7
\li					FreeLevel7
\li					UnloadLevel7
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Level7.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"
#include "../HeaderFiles/EasyEdit.h"

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// define
#define PANELSIZE 1920.0f
#define MAXENEMIES 10

// ---------------------------------------------------------------------------
// globals
static int newID;					// ID number
static int levelComplete;
static int PlayerIsAlive;
static int beginningAnimation;
static float spawnTimer;			// Timer for spawning enemies
static float spawnTime;				// When to spawn more enemies

static float levelTimer;

HUD* CurrentHUD;

Sprite* BlackOverlay;

Platform* Plat;
Wall* Wall1;

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
void LoadLevel7(void)
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
void InitializeLevel7(void)
{
	Vec3 TextTint;
	int i;
	newID = 10;
	ResetObjectList();
	ResetCamera();
	PlayerIsAlive = TRUE;
	levelComplete = FALSE;
	beginningAnimation = TRUE;
	spawnTimer = 0;
	spawnTime = 2.0f;
	levelTimer = 60.0f;
	ResetEnemyPanelNumber();

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -1300, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;
	CurrentHUD = CreateHUD(&CurrentPlayer);

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	//Panel1
	CreateSprite("TextureFiles/Level7Pan0.png", 1920, 1080, 5, 1, 1, 0, 0);
	//Panel2
	CreateSprite("TextureFiles/Level7Pan1.png", 1920, 1080, 5, 1, 1, PANELSIZE, 0);

	//Bounding Boxes
	CreateBoundingBoxes();

	//Black Overlay
	Vec3Set(&TextTint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = TextTint;

	//Tree Backgrounds
	for(i = 0; i < 4; i++)
		TreeBackground1[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground1.png", 1920, 1080, 2, 1, 1, 1920.0f * i, 0);

	for(i = 0; i < 4; i++)
		TreeBackground2[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground2.png", 1920, 1080, 1, 1, 1, 1920.0f * i, 0);

	for(i = 0; i < 4; i++)
		TreeBackground3[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground3.png", 1920, 1080, 0, 1, 1, 1920.0f * i, 0);

	/////////////////////////////////
	//		Platforms			   //
	/////////////////////////////////
	//Panel1
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 1000.0f, 100.0f, newID++, -460, -225);
	Plat->PlatformSprite->Visible = FALSE;
	//Panel2
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 1000.0f, 100.0f, newID++, 2380, -245);
	Plat->PlatformSprite->Visible = FALSE;

	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	//Far Left Wall
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100, 1080.0f, newID++, -958, 0);
	Wall1->WallSprite->Visible = FALSE;
	Wall1->enemyNotCollidable = TRUE;

	//Far Right Wall
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100, 1080.0f, newID++, -958 + 2 * PANELSIZE, 0);
	Wall1->WallSprite->Visible = FALSE;
	Wall1->enemyNotCollidable = TRUE;

	/////////////////////////////////
	//			Particles		   //
	/////////////////////////////////
	//Particles fo Torches
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 75, -95, 10, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 140, -95, 10, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 1780, -95, 10, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 1845, -95, 10, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

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
void UpdateLevel7(void)
{
	Weapon* wList = weaponList;

	EventLevel();
	EasyEditPlatform(Plat, 10);

	if(levelTimer > 0)
	{
		char charTemp[32];
		levelTimer -= GetDeltaTime();

		// Update HUD text for time remaining
		sprintf(charTemp, "Seconds Remaining: %d", (int)levelTimer);
		ChangeTextString(CurrentHUD->StatusText, charTemp);
		
		// Infini-Spawner Logic
		spawnTimer += GetDeltaTime();
		if(spawnTimer >= spawnTime && EnemyPanelNumber[0] < MAXENEMIES - 2)
		{
			Enemy* CurrentEnemy;
			int EnemyVer = rand() % 5;

			// 1/5th chance to spawn a ranged enemy
			if(EnemyVer > 1)
				EnemyVer = 1;
			else if(EnemyVer == 0)
				EnemyVer = 2;

			CurrentEnemy = CreateEnemy(EnemyVer, EnemyType, newID++, GetCameraXPosition() + PANELSIZE / 2, GROUNDLEVEL, 0);
			CurrentEnemy->HomePos.x = GetCameraXPosition() - PANELSIZE / 2;
			CurrentEnemy->EnemyState = AIIdle;

			EnemyVer = rand() % 5;
			// 1/5th chance to spawn a ranged enemy
			if(EnemyVer > 1)
				EnemyVer = 1;
			else if(EnemyVer == 0)
				EnemyVer = 2;

			CurrentEnemy = CreateEnemy(EnemyVer, EnemyType, newID++, GetCameraXPosition() - PANELSIZE / 2, GROUNDLEVEL, 0);
			CurrentEnemy->HomePos.x = GetCameraXPosition() + PANELSIZE / 2;
			CurrentEnemy->EnemyState = AIIdle;

			EnemyPanelNumber[0];
			spawnTimer = 0;
		}
		else if(spawnTimer >= spawnTime)
		{
			spawnTimer = 0;
		}	
	}

	// This should be the last line in this function
	UpdateAllEnemies();
	UpdateFloatingText();
	ParticleSystemUpdate();
	BoundingBoxUpdate();
	UpdateAllProjectiles();

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
void DrawLevel7(void)
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
void FreeLevel7(void)
{
	if(levelComplete && CurrentPlayer.CurrentLevel < GS_Kevin)
	{
		CurrentPlayer.levelClearBitFlags |= 256;
		CurrentPlayer.CurrentLevel = GS_Kevin;
	}
	else if(CurrentPlayer.CurrentLevel < GS_Level7)
		CurrentPlayer.CurrentLevel = GS_Level7;

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
void UnloadLevel7(void)
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
void EventLevel(void)
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
		InitializePause(&DrawLevel7);
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

	//////////////////////////////////
	//    CAMERA POSITION SECOND    //
	//////////////////////////////////
	if(PlayerIsAlive == TRUE)
	{
		//Don't Let camera go beyond left boundary
		if(CurrentPlayer.Position.x <= 0 && GetCameraXPosition() <= 5.0f)
			SetCameraXPosition(0.0f);
		//Don't Let camera go beyond right boundary
		else if(CurrentPlayer.Position.x >= (PANELSIZE) && GetCameraXPosition() >= ((PANELSIZE - 5.0f)))
			SetCameraXPosition(PANELSIZE);
		//Free Roam Camera
		else
			SetCamera(&CurrentPlayer.Position, 250);
	}

	//////////////////////////////////
	//       EVERYTHING ELSE        //
	//////////////////////////////////
	//Level Transition
	BlackOverlay->Position.x = GetCameraXPosition();
	if(CurrentPlayer.Position.x >= (PANELSIZE * 3 + PANELSIZE / 2) && levelComplete)
	{
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
