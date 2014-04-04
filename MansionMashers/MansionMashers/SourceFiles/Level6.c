/*****************************************************************************/
/*!
\file				Level6.c
\author				Juli Gregg (j.gregg)	
\date				Mar 8, 2014

\brief				Functions for Level6

\par				Functions:
\li					LoadLevel6
\li					InitializeLevel6
\li					UpdateLevel6
\li					DrawLevel6
\li					FreeLevel6
\li					UnloadLevel6
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Level6.h"
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

#define GLOW_OVERLAY_NUM 4

// ---------------------------------------------------------------------------
// globals
static int newID;					// ID number
static int levelComplete;
static int beginningAnimation;
static int PlayerIsAlive;
static int numPanels;

Sprite *Arrow1;
Sprite *Arrow2;
Sprite *Arrow3;
Sprite *Arrow4;

static int Arrow1Grow;
static int Arrow2Grow;
static int Arrow3Grow;
static int Arrow4Grow;

EnemySpawner* Spawners[8];

Sprite* PlatOverlay[GLOW_OVERLAY_NUM];
static int GlowBool;

FoxSound* BackSnd;

Sprite* BlackOverlay;
HUD* CurrentHUD;

Platform* Plat;
Wall* Wall1;

static void ObjectGlowUpdate(void);

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadLevel6(void)
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
void InitializeLevel6(void)
{
	Vec3 TextTint;
	Vec2 SpawnerLocation;
	newID = 10;
	ResetObjectList();
	ResetCamera();
	ResetEnemyPanelNumber();
	ResetGatedCamera();
	levelComplete = FALSE;
	beginningAnimation = TRUE;
	PlayerIsAlive = TRUE;
	numPanels = 4;
	GlowBool = TRUE;

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -1300, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	CurrentHUD = CreateHUD(&CurrentPlayer);

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	//Panel1
	CreateSprite("TextureFiles/Level6Pan0.png", 1920, 1080, 5, 1, 1, 0, 0);
	CreateSprite("TextureFiles/Level6Pan0Overlay.png", 1920, 1080, 300, 1, 1, 0, 0);
	PlatOverlay[0] = (Sprite*)CreateSprite("TextureFiles/Lvl6Pan1PlatOverlay.png", 1920, 1080, 6, 1, 1, 0, 0);
	PlatOverlay[0]->Alpha = .1f;
	//Panel2
	CreateSprite("TextureFiles/Level6Pan1.png", 1920, 1080, 5, 1, 1, PANELSIZE, 0);
	CreateSprite("TextureFiles/Level6Pan1Overlay.png", 1920, 1080, 300, 1, 1, PANELSIZE, 0);
	PlatOverlay[1] = (Sprite*)CreateSprite("TextureFiles/Lvl6Pan2PlatOverlay.png", 1920, 1080, 6, 1, 1, PANELSIZE, 0);
	PlatOverlay[1]->Alpha = .1f;
	//Panel3
	CreateSprite("TextureFiles/Level6Pan2.png", 1920, 1080, 5, 1, 1, PANELSIZE * 2, 0);
	CreateSprite("TextureFiles/Level6Pan2Overlay.png", 1920, 1080, 300, 1, 1, PANELSIZE * 2, 0);
	PlatOverlay[2] = (Sprite*)CreateSprite("TextureFiles/Lvl6Pan3PlatOverlay.png", 1920, 1080, 6, 1, 1, PANELSIZE * 2, 0);
	PlatOverlay[2]->Alpha = .1f;
	//Panel4
	CreateSprite("TextureFiles/Level6Pan3.png", 1920, 1080, 5, 1, 1, PANELSIZE * 3, 0);
	CreateSprite("TextureFiles/Level6Pan3Overlay.png", 1920, 1080, 300, 1, 1, PANELSIZE * 3, 0);
	PlatOverlay[3] = (Sprite*)CreateSprite("TextureFiles/Lvl6Pan4PlatOverlay.png", 1920, 1080, 6, 1, 1, PANELSIZE * 3, 0);
	PlatOverlay[3]->Alpha = .1f;

	//Bounding Boxes
	CreateBoundingBoxes();

	//Black Overlay
	Vec3Set(&TextTint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = TextTint;

	//Blocker Boxes
	CreateBlockerBoxes(&newID);

	//Background Sound
	BackSnd = CreateSound("Sounds/Temp.mp3", LargeSnd);

	// Arrows
	Arrow1 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 250, 235, 390, 1, 1, 0, 200);
	Arrow2 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 250, 235, 390, 1, 1, PANELSIZE, 200);
	Arrow3 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 250, 235, 390, 1, 1, 2 * PANELSIZE, 200);
	Arrow4 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 250, 235, 390, 1, 1, 3 * PANELSIZE, 200);

	Arrow1->Visible = FALSE;
	Arrow2->Visible = FALSE;
	Arrow3->Visible = FALSE;
	Arrow4->Visible = FALSE;

	Arrow1Grow = FALSE;
	Arrow2Grow = TRUE;
	Arrow3Grow = FALSE;
	Arrow4Grow = TRUE;

	/////////////////////////////////
	//		Platforms			   //
	/////////////////////////////////
	//Panel1
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 150.0f, 100.0f, newID++, 160, -285);
	Plat->PlatformSprite->Visible = FALSE;

	//Panel1
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 150.0f, 100.0f, newID++, 1380, -285);
	Plat->PlatformSprite->Visible = FALSE;
	//Panel2
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 100.0f, newID++, 1790, -255);
	Plat->PlatformSprite->Visible = FALSE;
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 100.0f, newID++, 2000, -255);
	Plat->PlatformSprite->Visible = FALSE;
	//Panel3
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 100.0f, newID++, 3670, -255);
	Plat->PlatformSprite->Visible = FALSE;
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 100.0f, newID++, 3480, -285);
	Plat->PlatformSprite->Visible = FALSE;
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 130.0f, 100.0f, newID++, 4350, -285);
	Plat->PlatformSprite->Visible = FALSE;
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 130.0f, 100.0f, newID++, 4150, -285);
	Plat->PlatformSprite->Visible = FALSE;
	//Panel4
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 150.0f, 100.0f, newID++, 4960, -285);
	Plat->PlatformSprite->Visible = FALSE;


	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	//Ceiling
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 1920.0f, 100.0f, newID++, 0, 250);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 1920.0f, 100.0f, newID++, PANELSIZE, 250);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 1920.0f, 100.0f, newID++, PANELSIZE * 2, 250);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 1920.0f, 100.0f, newID++, PANELSIZE * 3, 250);
	Wall1->WallSprite->Visible = FALSE;	
	//Far Left Wall
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100, 1080.0f, newID++, -958, 0);
	Wall1->WallSprite->Visible = FALSE;

	/////////////////////////////////
	//		Spawners			   //
	/////////////////////////////////
	//Panel1
	Vec2Set(&SpawnerLocation, -(PANELSIZE / 4), 0);
	//Left
	Spawners[0] = NULL;
	//Right
	Spawners[1] = CreateEnemySpawner(3, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 0);
	
	//Panel2
	Vec2Set(&SpawnerLocation, PANELSIZE -(PANELSIZE / 4), 0);
	//Left
	Spawners[2] = CreateEnemySpawner(3, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 1);
	//Right
	Spawners[3] = CreateEnemySpawner(2, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 1);
	
	//Panel3
	Vec2Set(&SpawnerLocation, (PANELSIZE * 2) -(PANELSIZE / 4), 0);
	//Left
	Spawners[4] = CreateEnemySpawner(3, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 2);
	//Right
	Spawners[5] = CreateEnemySpawner(2, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 2);
	
	//Panel4
	Vec2Set(&SpawnerLocation, (PANELSIZE * 3), 0);
	//Left
	Spawners[6] = CreateEnemySpawner(2, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 3);
	//Right
	Spawners[7] = CreateEnemySpawner(3, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 3);


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
void UpdateLevel6(void)
{
	EventLevel();
	PlayAudio(BackSnd);
	//EasyEditPlatform(Plat, 10);
	//EasyEditWall(Wall1, 10);

	if(!EnemyPanelNumber[0])
		Arrow1->Visible = TRUE;
	if(!EnemyPanelNumber[1])
		Arrow2->Visible = TRUE;
	if(!EnemyPanelNumber[2])
		Arrow3->Visible = TRUE;
	if(!EnemyPanelNumber[3])
		Arrow4->Visible = TRUE;

	// Update all the arrows
	UpdateArrow(Arrow1, &Arrow1Grow);
	UpdateArrow(Arrow2, &Arrow2Grow);
	UpdateArrow(Arrow3, &Arrow3Grow);
	UpdateArrow(Arrow4, &Arrow4Grow);

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
void DrawLevel6(void)
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
void FreeLevel6(void)
{
	if(levelComplete && CurrentPlayer.CurrentLevel < GS_YeahGuy)
	{
		CurrentPlayer.levelClearBitFlags |= 64;
		CurrentPlayer.CurrentLevel = GS_YeahGuy;
	}
	else if(CurrentPlayer.CurrentLevel < GS_Level6)
		CurrentPlayer.CurrentLevel = GS_Level6;

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
void UnloadLevel6(void)
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
	{
		SetDebugMode();
	}
	if(FoxInput_KeyTriggered('I'))
	{
		RemoveDebugMode();
	}
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		if(PlayerIsAlive)
		{
			InitializePause(&DrawLevel6);
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

	//SetCamera(&CurrentPlayer.Position, 250);
	SetUpCameraPanAndLock(&levelComplete, PANELSIZE, Spawners, numPanels);
	UpdateBlockerBoxes(PANELSIZE);

	//////////////////////////////////
	//       EVERYTHING ELSE        //
	//////////////////////////////////
	BoundingBoxUpdate();
	ParticleSystemUpdate();
	UpdateAllEnemies();
	UpdateFloatingText();
	UpdateAllProjectiles();
	ObjectGlowUpdate();

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

void ObjectGlowUpdate(void)
{
	int i;

	for(i = 0; i < GLOW_OVERLAY_NUM; i++)
	{
		if(GlowBool)
		{
			PlatOverlay[i]->Alpha += .01f;
			if(PlatOverlay[GLOW_OVERLAY_NUM - 1]->Alpha > 0.5f)
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
