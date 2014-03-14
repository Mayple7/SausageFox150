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
static int PlayerIsAlive; 

TextGlyphs* LevelName;

EnemySpawner* FirstSpawner;
EnemySpawner* SecondSpawnerRight;
EnemySpawner* SecondSpawnerLeft;
EnemySpawner* ThirdSpawnerRight;
EnemySpawner* ThirdSpawnerLeft;

Enemy* SetEnemy1;
Enemy* SetEnemy2;

Platform* Table1;

Wall* Wall1;

Wall* BBWallLeft;
Wall* BBWallRight;

FoxSound* BackSnd;

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
	PlayerIsAlive = TRUE;
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
	LevelName = CreateText("Level 1", 0, 500, 100, TextTint, Center);
	ChangeTextVisibility(LevelName);

	//Bounding Boxes
	CreateBoundingBoxes();

	BackSnd = CreateSound("Sounds/main.theme_.mp3", LargeSnd);

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
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 160.0f, 500.0f, newID++, 2785, 130);
	Wall1->WallSprite->Visible = FALSE;
	
	
	// Bounding Box Walls
	BBWallLeft = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 1080.0f, newID++, 0, 0);
	BBWallLeft->WallSprite->Visible = FALSE;
	BBWallLeft->enemyNotCollidable = TRUE;
	BBWallRight = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 1080.0f, newID++, 0, 0);
	BBWallRight->WallSprite->Visible = FALSE;
	BBWallRight->enemyNotCollidable = TRUE;

	

	//Enemy spawners
	Vec2Set(&SpawnerLocation, -200, 0);
	FirstSpawner = CreateEnemySpawner(1, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 0);

	Vec2Set(&SpawnerLocation, PANELSIZE, 0);
	SecondSpawnerRight = CreateEnemySpawner(2, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 1);
	SecondSpawnerLeft = CreateEnemySpawner(1, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 1);

	Vec2Set(&SpawnerLocation, 1.90f * PANELSIZE, 0);
	ThirdSpawnerRight = CreateEnemySpawner(1, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 2);
	ThirdSpawnerLeft = CreateEnemySpawner(1, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 2);

	SetEnemy1 = CreateEnemy(BasicMelee, EnemyType, newID++, 2.25f * PANELSIZE / GetLoadRatio(), GROUNDLEVEL, 2);
	SetEnemy2 = CreateEnemy(BasicMelee, EnemyType, newID++, 2.25f * PANELSIZE / GetLoadRatio(), GROUNDLEVEL, 2);
	SetEnemy1->HomePos.x = 2 * PANELSIZE * GetLoadRatio();
	SetEnemy2->HomePos.x = 2 * PANELSIZE * GetLoadRatio();

	//Particles fo Torches
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 695, -140, 10, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 806, -235, 201, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	CreatePaperScroll(200);

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

	ParticleSystemUpdate();
	BoundingBoxUpdate();

	if(CurrentPlayer.Position.x > (1.5f * PANELSIZE) * GetLoadRatio())
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

	
	/*if(FoxInput_KeyTriggered('T'))
		SetNextState(GS_Restart);
	/*
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

	/*////////////////////////////////
	//   INPUT & COLLISION FIRST    //
	////////////////////////////////*/
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		if(PlayerIsAlive == TRUE)
		{
			InitializePause(&DrawLevel1);
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
			CurrentPlayer.Speed = CurrentPlayer.CurrentPlayerStats.MoveSpeed * GetLoadRatio() * GetDeltaTime();
			
			// Threshold to give control back to the player
			if(CurrentPlayer.Position.x > -500)
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

	SetCameraLockState(FALSE);
	//Panel1
	if(CurrentPlayer.Position.x > -(PANELSIZE / 2) * GetLoadRatio() && CurrentPlayer.Position.x < (PANELSIZE / 2) * GetLoadRatio())
	{
		if(EnemyPanelNumber[0] > 0 && GetCameraMovedState())
			SetCameraLockState(TRUE);
		SetCameraPan(0.0f, PANELSIZE);
	}
	//Panel2
	else if(CurrentPlayer.Position.x > (PANELSIZE / 2) * GetLoadRatio() && CurrentPlayer.Position.x < (PANELSIZE + (PANELSIZE / 2)) * GetLoadRatio())
	{
		if(EnemyPanelNumber[1] > 0 && GetCameraMovedState())
			SetCameraLockState(TRUE);
		SetCameraPan(PANELSIZE * GetLoadRatio(), PANELSIZE);
	}
	//Panel3
	else if(CurrentPlayer.Position.x > (PANELSIZE + (PANELSIZE / 2)) * GetLoadRatio() && CurrentPlayer.Position.x < ((PANELSIZE * 2) + (PANELSIZE / 2)) * GetLoadRatio())
	{
		if(EnemyPanelNumber[2] > 0 && GetCameraMovedState())
			SetCameraLockState(TRUE);
		SetCameraPan((PANELSIZE * 2) * GetLoadRatio(), PANELSIZE);
	}
	else if(CurrentPlayer.Position.x > (PANELSIZE / 2) * 5 * GetLoadRatio() + CurrentPlayer.PlayerCollider.width)
	{
			levelComplete = TRUE;
	}

	BBWallLeft->Position.y = -1080 * GetLoadRatio() + 1080 * GetLoadRatio() * GetCameraLockState();
	BBWallLeft->Position.x = GetCameraXPosition() - (PANELSIZE / 2 * GetLoadRatio());
	UpdateCollisionPosition(&BBWallLeft->WallCollider, &BBWallLeft->Position);
	BBWallRight->Position.y = -1080 * GetLoadRatio() + 1080 * GetLoadRatio() * GetCameraLockState();
	BBWallRight->Position.x = GetCameraXPosition() + (PANELSIZE / 2 * GetLoadRatio());
	UpdateCollisionPosition(&BBWallRight->WallCollider, &BBWallRight->Position);

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

	if(CurrentPlayer.CurrentPlayerStats.CurrentHealth <= 0.0f)
	{
		PlayerIsAlive = FALSE;
		BlackOverlay->Position.x = GetCameraXPosition();
		BlackOverlay->Alpha = 0.5f;

		UpdateDeathConfirmObjects();
	}

}
