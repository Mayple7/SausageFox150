/*****************************************************************************/
/*!
\file				Level3.c
\author				Juli Gregg (j.gregg)
\date				Mar 17, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					LoadLevel3
\li					InitializeLevel3
\li					UpdateLevel3
\li					DrawLevel3
\li					FreeLevel3
\li					UnloadLevel3
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// Level Notes!!!!
//
//	Since this level goes into the tree tops. There will be a fade out at
//  the end of Panel one and game will switch to Level3.1 which will place
//	the player at the bottom of the screen and the game will take place 
//	as if in a high spot. Can only access Level3.1 through Level 3.
//	Level3.1 is just a continuation of Level3.
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Level3.1.h"
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
// defines
#define PANELSIZE 1920.0f

// ---------------------------------------------------------------------------
// globals
static int newID;					// ID number
static int levelComplete;
static int beginningAnimiation;
static int numPanels;
static int PlayerIsAlive;
TextGlyphs* LevelName;

Platform *Plat;
Wall *Wall1;

Sprite* SecondOverlay[3];

static EnemySpawner* Spawners[6];
static Enemy* TopDeckEnemy[4];

Sprite* BlackOverlay;

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadLevel31(void)
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
void InitializeLevel31(void)
{
	Vec3 TextTint;
	Vec2 SpawnerLocation;
	int i;
	newID = 10;
	ResetObjectList();
	ResetCamera();
	beginningAnimiation = TRUE;
	levelComplete = FALSE;
	numPanels = 3;
	PlayerIsAlive = TRUE;
	ResetEnemyPanelNumber();

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -1300, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	Vec3Set(&TextTint, 1, 1, 1);
	LevelName = CreateText("Level 3", 0, 300, 100, TextTint, Center, Border);
	ChangeTextVisibility(LevelName);

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	//Panel 1
	CreateSprite("TextureFiles/Level3Pan2.png", 1920, 1080, 1, 1, 1, 0, 0);
	CreateSprite("TextureFiles/Level3Pan2Overlay.png", 1920, 1080, 401, 1, 1, 0, 0);
	//Panel2
	CreateSprite("TextureFiles/Level3Pan3.png", 1920, 1080, 1, 1, 1, 1920, 0);
	CreateSprite("TextureFiles/Level3Pan2Overlay.png", 1920, 1080, 401, 1, 1, 1920, 0);
	//Panel3
	CreateSprite("TextureFiles/Level3Pan4.png", 1920, 1080, 1, 1, 1, (1920 * 2), 0);
	CreateSprite("TextureFiles/Level3Pan2Overlay.png", 1920, 1080, 401, 1, 1, (1920 * 2), 0);

	//Create Upper Deck Overlays
	for(i = 0; i < 3; i++)
	{
		SecondOverlay[i] = (Sprite *)CreateSprite("TextureFiles/Level3Pan2Overlay2.png", 1920, 1080, 400, 1, 1, (1920.0f * i), 0);
	}

	//Black Overlay
	Vec3Set(&TextTint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = TextTint;

	//Bounding Boxes
	CreateBoundingBoxes();

	/////////////////////////////////
	//		Platforms			   //
	/////////////////////////////////
	//Step 1
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 50.0f, 100.0f, newID++, -655, -210);
	Plat->PlatformSprite->Visible = FALSE;
	//Step 2
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 50.0f, 100.0f, newID++, -655 + 1920, -210);
	Plat->PlatformSprite->Visible = FALSE;
	//Step 3
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 50.0f, 100.0f, newID++, -655 + (1920 * 2), -210);
	Plat->PlatformSprite->Visible = FALSE;

	//Top Walkway
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 1920.0f, 100.0f, newID++, 0, -20);
	Plat->PlatformSprite->Visible = FALSE;
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 1920.0f, 100.0f, newID++, 1920, -20);
	Plat->PlatformSprite->Visible = FALSE;
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 1920.0f, 100.0f, newID++, (1920 * 2), -20);
	Plat->PlatformSprite->Visible = FALSE;

	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	//Bounding Walls
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 1920.0f, 100.0f, newID++, 0, 590);
	Wall1->WallSprite->Visible = FALSE;

	//Top Cicle Blockers
	//Panel 1
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 100.0f, newID++, -880, -20);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 115.0f, 100.0f, newID++, -490, -20);
	Wall1->WallSprite->Visible = FALSE;
	//Panel 2
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 160.0f, 100.0f, newID++, 1060, -20);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 115.0f, 100.0f, newID++, -490 + 1920, -20);
	Wall1->WallSprite->Visible = FALSE;
	//Panel 3
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 160.0f, 100.0f, newID++, 1060 + 1920, -20);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 115.0f, 100.0f, newID++, -490 + (1920 * 2), -20);
	Wall1->WallSprite->Visible = FALSE;

	//Blocker Boxes
	CreateBlockerBoxes(&newID);

	/////////////////////////////////
	//			Spawners		   //
	/////////////////////////////////
	Vec2Set(&SpawnerLocation, 0, 0);
	//1st Left
	Spawners[0] = CreateEnemySpawner(2, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 0);
	//1st Right
	Spawners[1] = CreateEnemySpawner(1, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 0);
	TopDeckEnemy[0] = CreateEnemy(BasicMelee, EnemyType, newID++, 300, 110, 0);
	
	Vec2Set(&SpawnerLocation, PANELSIZE, 0);
	//2nd Left
	Spawners[2] = CreateEnemySpawner(1, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 1);
	//2nd Right
	Spawners[3] = CreateEnemySpawner(2, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 1);
	TopDeckEnemy[1] = CreateEnemy(BasicMelee, EnemyType, newID++, 300 + PANELSIZE, 110, 1);

	Vec2Set(&SpawnerLocation, PANELSIZE * 2, 0);
	//3rd /eft
	Spawners[4] = CreateEnemySpawner(1, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 2);
	//3rd Right
	Spawners[5] = CreateEnemySpawner(1, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 2);
	TopDeckEnemy[2] = CreateEnemy(BasicMelee, EnemyType, newID++, 300 + (2 * PANELSIZE), 110, 2);
	TopDeckEnemy[3] = CreateEnemy(BasicMelee, EnemyType, newID++, (2 * PANELSIZE), 110, 2);

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
void UpdateLevel31(void)
{
	EventLevel31();

	ParticleSystemUpdate();
	BoundingBoxUpdate();

	//EasyEditPlatform(Plat, 10);
	//EasyEditWall(Wall1 ,10);


	// This should be the last line in this function
	UpdatePlayerPosition(&CurrentPlayer);
}

/*************************************************************************/
/*!
	\brief
	Draws the level
*/
/*************************************************************************/
void DrawLevel31(void)
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
void FreeLevel31(void)
{
	if(levelComplete && CurrentPlayer.CurrentLevel < GS_ArmGuy)
	{
		CurrentPlayer.CurrentLevel = GS_ArmGuy;
		CurrentPlayer.armUnlock = TRUE;
	}
	else if(CurrentPlayer.CurrentLevel < GS_Level3)
		CurrentPlayer.CurrentLevel = GS_Level3;
	SavePlayer(&CurrentPlayer);
	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadLevel31(void)
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
void EventLevel31(void)
{
	int i;

	/*////////////////////////////////
	//   INPUT & COLLISION FIRST    //
	////////////////////////////////*/
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
		InitializePause(&DrawLevel31);
		//TogglePauseSound(&BackgroundSnd);
		//SetNextState(GS_MainMenu);
		UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
	}	

	// Runs if the beginning animation is finished
	if(!beginningAnimiation && !levelComplete)
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



	/*////////////////////////////////
	//    CAMERA POSITION SECOND    //
	////////////////////////////////*/

	SetUpCameraPanAndLock(&levelComplete, PANELSIZE, Spawners, numPanels);
	UpdateBlockerBoxes(PANELSIZE);


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

	//Logic for upper deck overlays
	if(CurrentPlayer.Position.y > 105)
	{
		//Player is on top deck so don't show overlay
		int i;
		for(i = 0; i < 3; i++)
		{
			SecondOverlay[i]->Alpha = 0.0f;
		}
	}
	else 
	{
		//Don't run loop if Alpha is already 1.0f
		if(SecondOverlay[0]->Alpha == 0.0f)
		{
			//Player is on bottom deck so show overlays
			int i;
			for(i = 0; i < 3; i++)
			{
				SecondOverlay[i]->Alpha = 1.0f;
			}
		}
	}

	//If player dies
	if(CurrentPlayer.CurrentPlayerStats.CurrentHealth <= 0.0f)
	{
		PlayerIsAlive = FALSE;
		BlackOverlay->Alpha = 0.5f;

		UpdateDeathConfirmObjects();
	}

}
