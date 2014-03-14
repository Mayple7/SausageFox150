/*****************************************************************************/
/*!
\file				Level2.c
\author				Dan Muller (d.muller)
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
static int levelComplete = FALSE;
TextGlyphs* LevelName;
Sprite *TxtScrollRight;
Sprite *TxtScrollMiddle;

EnemySpawner* Pan2SpawnerRight;
EnemySpawner* Pan2SpawnerLeft;
EnemySpawner* Pan3SpawnerRight;
EnemySpawner* Pan3SpawnerLeft;
EnemySpawner* Pan4SpawnerRight;

Enemy* Pan3Enemy;

Food* Ham;
Food* Taco;
Food* Pizza;
Food* Cake;

Platform *Crate;
Wall* Wall1;
Wall* WTBot;
Wall* RightBarrier;

HUD* CurrentHUD;

FoxSound *BackSnd;

static int PlayerInSight; // Enemy in watch tower checks for player 
static int PlayerIsAlive; 

Sprite* BlackOverlay;

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
	Vec3 TextTint;
	Vec2 SpawnerLocation;

	levelComplete = FALSE;
	PlayerInSight = FALSE;
	PlayerIsAlive = TRUE;

	newID = 10;
	ResetObjectList();
	ResetCamera();
	ResetEnemyPanelNumber();

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, 0, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	CurrentHUD = CreateHUD(&CurrentPlayer);

	Vec3Set(&TextTint, 1, 1, 1);
	LevelName = CreateText("Level 2", 0, 300, 100, TextTint, Center);
	ChangeTextVisibility(LevelName);
	
	//Bounding Boxes
	CreateBoundingBoxes();

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	CreateSprite("TextureFiles/OutsideMan0.png", 1920, 1080, 1, 1, 1, 0, 0);
	CreateSprite("TextureFiles/OutsideMan0Overlay.png", 1920, 1080, 200, 1, 1, 0, 0);
	CreateSprite("TextureFiles/OutsideMan1.png", 1920, 1080, 1, 1, 1, 1920, 0);
	CreateSprite("TextureFiles/OutsideMan2.png", 1920, 1080, 1, 1, 1, 1920 * 2, 0);
	CreateSprite("TextureFiles/OutsideMan2Overlay.png", 1920, 1080, 200, 1, 1, 1920 * 2, 0);
	CreateSprite("TextureFiles/OutsideMan3.png", 1920, 1080, 1, 1, 1, 1920 * 3, 0);
	CreateSprite("TextureFiles/OutsideMan3Overlay.png", 1920, 1080, 200, 1, 1, 1920 * 3, 0);

	//Black fade part
	Vec3Set(&TextTint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = TextTint;


	//Taco = CreateFood(Agility, 150, 150, -800, 0, newID++);
	//Ham = CreateFood(Strength, 150, 150, -400, 0, newID++);
	//Pizza = CreateFood(Defense, 150, 150, 0, 0, newID++);
	//Cake = CreateFood(Haste, 150, 150, 400, 0, newID++);

	//Test = CreateWall("TextureFiles/BlankPlatform.png", 500, 500, newID++, 0, 0);
	//Test->WallSprite->Visible = FALSE;

	//CreatePaperScroll(200);
	
	BackSnd = CreateSound("Sounds/Temp.mp3", LargeSnd);

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
	Crate = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 100.0f, newID++, 3780, -210);
	Crate->PlatformSprite->Visible = FALSE;


	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	//Stairs
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 350.0f, 100.0f, newID++, -819, -232);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -618, -281);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -675, -266);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -559, -336);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -604, -321);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -488, -370);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -538, -355);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -408, -420);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 100.0f, newID++, -473, -404);
	Wall1->WallSprite->Visible = FALSE;
	//Left Bounding Wall
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 1040.0f, newID++, -958, 0);
	Wall1->WallSprite->Visible = FALSE;
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
	Pan2SpawnerRight = CreateEnemySpawner(2, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 1);
	Pan2SpawnerLeft = CreateEnemySpawner(1, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 1);
	//Panel3
	Vec2Set(&SpawnerLocation, (PANELSIZE * 2), 0);
	Pan3SpawnerRight = CreateEnemySpawner(1, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 2);
	Pan3SpawnerLeft = CreateEnemySpawner(1, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 2);
	Pan3Enemy = CreateEnemy(BasicMelee, EnemyType, newID++, WTBot->Position.x / GetLoadRatio(), 300, 2);
	//Panel4
	Vec2Set(&SpawnerLocation, (PANELSIZE * 3 - PANELSIZE / 4), 0);
	Pan3SpawnerRight = CreateEnemySpawner(2, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 3);


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
	EventLevel2();
	
	BoundingBoxUpdate();
	ParticleSystemUpdate();

	if(CurrentPlayer.Position.x > (PANELSIZE * 2 - PANELSIZE / 4) * GetLoadRatio())
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
		CurrentPlayer.CurrentLevel = GS_Level3;
	else if(CurrentPlayer.CurrentLevel < GS_Level2)
		CurrentPlayer.CurrentLevel = GS_Level2;
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
	int i;

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

	// Check for any collision and handle the results
	DetectPlayerCollision();
	// Handle any input for the current player
	InputPlayer(&CurrentPlayer);

	/*////////////////////////////////
	//    CAMERA POSITION SECOND    //
	////////////////////////////////*/

	if(PlayerIsAlive == TRUE)
	{
		//Don't Let camera go beyond left boundary
		if(CurrentPlayer.Position.x <= 0 && GetCameraXPosition() <= 5.0f)
			SetCameraXPosition(0.0f);
		//Don't Let camera go beyond right boundary
		else if(CurrentPlayer.Position.x >= (PANELSIZE * 3) * GetLoadRatio() && GetCameraXPosition() >= ((PANELSIZE * 3 - 5.0f) * GetLoadRatio()))
			SetCameraXPosition(PANELSIZE * 3 * GetLoadRatio());
		//Free Roam Camera
		else
			SetCamera(&CurrentPlayer.Position, 250);
	}

	/*////////////////////////////////
	//       EVERYTHING ELSE        //
	////////////////////////////////*/
	for(i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Update the created enemies
		if (enemyList[i].objID == -1)
			break;
		if (enemyList[i].objID == 0)
			continue;

		UpdateEnemy(&enemyList[i]);
	}

	UpdateFloatingText();


	//Check if all enemies are dead & remove right barrier
	if(EnemyPanelNumber[1] <= 0 && EnemyPanelNumber[2] <= 0 && EnemyPanelNumber[3] <= 0)
	{
		levelComplete = TRUE;
		RightBarrier->Position.y = -1080 * GetLoadRatio();
		UpdateCollisionPosition(&RightBarrier->WallCollider, &RightBarrier->Position);
	}


	//At level start
	if(!levelComplete)
	{
		// Fade in the level
		if(BlackOverlay->Alpha > 0)
		{
			BlackOverlay->Alpha -= 1 * GetDeltaTime();
		}
	}

	//Level Transition
	if(CurrentPlayer.Position.x >= (PANELSIZE * 3 + PANELSIZE / 2) * GetLoadRatio() && levelComplete)
	{
		BlackOverlay->Position.x = GetCameraXPosition();
		BlackOverlay->Alpha += 1 * GetDeltaTime();
		if(BlackOverlay->Alpha > 1)
			SetNextState(GS_MapLevel);
	}

	//If player dies
	if(CurrentPlayer.CurrentPlayerStats.CurrentHealth <= 0.0f)
	{
		PlayerIsAlive = FALSE;
		BlackOverlay->Position.x = GetCameraXPosition();
		BlackOverlay->Alpha = 0.5f;

		UpdateDeathConfirmObjects();
	}
}
