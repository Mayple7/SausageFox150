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
#include "../HeaderFiles/KevinBoss.h"

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// define
#define PANELSIZE 1920.0f
#define MAXENEMIES 10
#define GLOW_OVERLAY_NUM 2

// ---------------------------------------------------------------------------
// globals
static int levelComplete;
static int PlayerIsAlive;
static int beginningAnimation;
static float spawnTimer;			// Timer for spawning enemies
static float spawnTime;				// When to spawn more enemies

static float levelTimer;

KevinBoss *Boss;
HUD* CurrentHUD;

//Boss HP Bar
Sprite* BossHPBar;
Sprite* BossHPBarBack;

Sprite* BlackOverlay;

Platform* Plat;
Wall* Wall1;

Sprite* PlatOverlay[GLOW_OVERLAY_NUM];
static int GlowBool;

// Tree Background
Sprite* TreeBackground1[4];
Sprite* TreeBackground2[4];
Sprite* TreeBackground3[4];
static void TreeBackgroundUpdate(void);
static void ObjectGlowUpdate(void);
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
	LoadEnemy(BasicMelee);
	LoadEnemy(BasicRanged);
}

/*************************************************************************/
/*!
	\brief
	Initializes the objects for the level
*/
/*************************************************************************/
void InitializeLevel7(void)
{
	Enemy* CurrentEnemy;
	Vec3 TextTint;
	int i;

	ResetObjectList();
	ResetCamera();
	PlayerIsAlive = TRUE;
	levelComplete = FALSE;
	beginningAnimation = TRUE;
	spawnTimer = 0;
	spawnTime = 8.0f;
	levelTimer = 60.0f;
	ResetEnemyPanelNumber();
	GlowBool = TRUE;

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -1300, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;
	CurrentHUD = CreateHUD(&CurrentPlayer);

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	//Panel1
	CreateSprite("TextureFiles/Level7Pan0.png", 1920, 1080, 5, 1, 1, 0, 0);
	PlatOverlay[0] = (Sprite*)CreateSprite("TextureFiles/Lvl7Pan1PlatOverlay.png", 1920, 1080, 6, 1, 1, 0, 0);
	PlatOverlay[0]->Alpha = .1f;
	//Panel2
	CreateSprite("TextureFiles/Level7Pan1.png", 1920, 1080, 5, 1, 1, PANELSIZE, 0);
	PlatOverlay[1] = (Sprite*)CreateSprite("TextureFiles/Lvl7Pan2PlatOverlay.png", 1920, 1080, 6, 1, 1, PANELSIZE, 0);
	PlatOverlay[1]->Alpha = .1f;

	//Bounding Boxes
	CreateBoundingBoxes();

	//Black Overlay
	Vec3Set(&TextTint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = TextTint;

	// Boss HP Bar
	BossHPBar = (Sprite *)CreateSprite("TextureFiles/BossHealthBarMid.png", 1, 44, 399, 1, 1, -200, 450);
	BossHPBarBack = (Sprite *)CreateSprite("TextureFiles/BossHealthBarBack.png", 820, 64, 398, 1, 1, 0, 450);

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
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 1000.0f, 100.0f, -460, -225);
	Plat->PlatformSprite->Visible = FALSE;
	//Panel2
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 1000.0f, 100.0f, 2380, -245);
	Plat->PlatformSprite->Visible = FALSE;

	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	//Far Left Wall
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100, 1080.0f, -958, 0);
	Wall1->WallSprite->Visible = FALSE;
	Wall1->enemyNotCollidable = TRUE;

	//Far Right Wall
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100, 1080.0f, -958 + 2 * PANELSIZE, 0);
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
	Boss = CreateKevinBoss(0, 0);

	// Creates all the enemies
	while(EnemyPanelNumber[0] < 8)
	{
		CurrentEnemy = CreateEnemy(1, EnemyType, 0, GROUNDLEVEL - 5000, 0);
		CurrentEnemy->EnemyState = AINone;
	}
	CurrentEnemy = CreateEnemy(2, EnemyType, 0, GROUNDLEVEL - 5000, 0);
	CurrentEnemy->EnemyState = AINone;
	CurrentEnemy = CreateEnemy(2, EnemyType, 0, GROUNDLEVEL - 5000, 0);
	CurrentEnemy->EnemyState = AINone;

	/////////////////////////////////
	//		On Death			   //
	/////////////////////////////////
	CreateDeathConfirmObjects();

	CreateUpgradeScreenObjects();
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

	// This should be the last line in this function
	if(!levelComplete)
	{
		UpdateKevinBoss(Boss);

		// Infini-Spawner Logic
		spawnTimer += GetDeltaTime();
		if(spawnTimer >= spawnTime)
		{
			Enemy* CurrentEnemy = enemyList;
			int newSpawns = 0;

			// Loop through all the enemies for new spawns
			while(CurrentEnemy->objID != -1)
			{
				if(CurrentEnemy->objID > 0 && (newSpawns < 2 || CurrentEnemy->EnemyType == BasicRanged) && CurrentEnemy->EnemyState == AINone)
				{
					CurrentEnemy->EnemyState = AIIdle;
					if(newSpawns == 1 || newSpawns == 3)
						CurrentEnemy->Position.x = GetCameraXPosition() - PANELSIZE / 2 - 300.0f;
					else
						CurrentEnemy->Position.x = GetCameraXPosition() + PANELSIZE / 2 + 300.0f;
					CurrentEnemy->Position.y = GROUNDLEVEL;
					CurrentEnemy->HomePos.x = CurrentPlayer.Position.x;
					CurrentEnemy->HomePos.y = 0;
					CurrentEnemy->CurrentEnemyStats.CurrentHealth = CurrentEnemy->CurrentEnemyStats.MaxHealth;
					++newSpawns;
				}
				else if(CurrentEnemy->objID > 0)
				{
					UpdateEnemy(CurrentEnemy);
				}
				++CurrentEnemy;
			}
			spawnTimer = 0;
		}
		else
		{
			Enemy* CurrentEnemy = enemyList;

			// Loop through all the enemies for new spawns
			while(CurrentEnemy->objID != -1)
			{
				if(CurrentEnemy->objID > 0)
				{
					UpdateEnemy(CurrentEnemy);

					if(CurrentEnemy->CurrentEnemyStats.CurrentHealth <= 0)
					{
						CurrentEnemy->EnemyState = AINone;
						CurrentEnemy->Position.y = -5000;

					}
				}
				++CurrentEnemy;
			}
		}
	}

	// When the boss dies
	if(!levelComplete && Boss->CurrentHealth <= 0)
	{
		levelComplete = TRUE;
		FreeKevinBoss(Boss);
	}

	// What to do when the boss is dead
	if(levelComplete)
	{
		BossHPBar->Visible = FALSE;

		if(BossHPBar->Alpha > 0.0f)
		{
			BossHPBar->Alpha -= GetDeltaTime() / 2.0f;
		}
		else
			BossHPBar->Alpha = 0;

	}
	// Boss health bar logic
	else
	{
		BossHPBar->ScaleX = 800.0f * (Boss->CurrentHealth / (float)Boss->MaxHealth);
		BossHPBar->Position.x = -400.0f * (1 - (Boss->CurrentHealth / (float)Boss->MaxHealth)) + GetCameraXPosition();
		BossHPBarBack->Position.x = GetCameraXPosition();
	}

	// This should be the last line in this function
	//UpdateAllEnemies();
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
	if(!levelComplete && !beginningAnimation)
	{
		DetectPlayerCollision();
		DetectKevinBossCollision(Boss);
	// Handle any input for the current player
		InputPlayer(&CurrentPlayer);
		
	}
	else if(!levelComplete)
	{
		// Make sure the boss stays put during the start
		Boss->cooldownTimer = 0;

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
			{
				beginningAnimation = FALSE;
			}
		}
		//Always animate the player otherwise the sprites get stuck in the middle
		Animation(&CurrentPlayer);
		UpdateCollisionPosition(&CurrentPlayer.PlayerWeapon->WeaponAttack, &CurrentPlayer.PlayerWeapon->WeaponAttackPosition);
		MoveObject(&CurrentPlayer.Position, CurrentPlayer.PlayerDirection, CurrentPlayer.Speed);
	}
	else
		GameCompletion();

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


#if defined _DEBUG
	if(FoxInput_KeyTriggered('U'))
	{
		SetDebugMode();
	}
	if(FoxInput_KeyTriggered('I'))
	{
		RemoveDebugMode();
	}
#endif
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		if(PlayerIsAlive)
		{
			//TogglePauseSound(BackSnd);
			InitializePause(&DrawLevel7);
			UpdatePause();
			//TogglePauseSound(BackSnd);
		}
	}

	//////////////////////////////////
	//    CAMERA POSITION SECOND    //
	//////////////////////////////////

	//////////////////////////////////
	//       EVERYTHING ELSE        //
	//////////////////////////////////
	TreeBackgroundUpdate();
	ObjectGlowUpdate();

	//Player Dies
	if(CurrentPlayer.CurrentPlayerStats.CurrentHealth <= 0.0f)
	{
		//freeSound(BackSnd);
		PlayerIsAlive = FALSE;
		BlackOverlay->Position.x = GetCameraXPosition();
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

void ObjectGlowUpdate(void)
{
	int i;

	for(i = 0; i < GLOW_OVERLAY_NUM; i++)
	{
		if(GlowBool)
		{
			PlatOverlay[i]->Alpha += .01f;
			if(PlatOverlay[GLOW_OVERLAY_NUM - 1]->Alpha > 0.4f)
				GlowBool = FALSE;
		}
		else 
		{
			PlatOverlay[i]->Alpha -= .01f;
			if(PlatOverlay[GLOW_OVERLAY_NUM -1]->Alpha < 0.1f)
				GlowBool = TRUE;
		}
	}
}
