/*****************************************************************************/
/*!
\file				Level6.c
\author				Dan Muller (d.muller)
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

// ---------------------------------------------------------------------------
// globals
static int newID;					// ID number
static int levelComplete;
static int beginningAnimation;
static int PlayerIsAlive;
static int numPanels;
TextGlyphs* LevelName;

Sprite* BlackOverlay;
HUD* CurrentHUD;

Platform* Plat;
Wall* Wall1;

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

	newID = 10;
	ResetObjectList();
	ResetCamera();
	ResetEnemyPanelNumber();
	ResetGatedCamera();
	levelComplete = FALSE;
	beginningAnimation = TRUE;
	PlayerIsAlive = TRUE;
	numPanels = 4;

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -1300, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	CurrentHUD = CreateHUD(&CurrentPlayer);

	Vec3Set(&TextTint, 1, 1, 1);
	LevelName = CreateText("Level 5", 0, 300, 100, TextTint, Center, Border);
	ChangeTextVisibility(LevelName);

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	//Panel1
	CreateSprite("TextureFiles/Level6Pan0.png", 1920, 1080, 5, 1, 1, 0, 0);
	CreateSprite("TextureFiles/Level6Pan0Overlay.png", 1920, 1080, 300, 1, 1, 0, 0);
	//Panel2
	CreateSprite("TextureFiles/Level6Pan1.png", 1920, 1080, 5, 1, 1, PANELSIZE, 0);
	CreateSprite("TextureFiles/Level6Pan1Overlay.png", 1920, 1080, 300, 1, 1, PANELSIZE, 0);
	//Panel3
	CreateSprite("TextureFiles/Level6Pan2.png", 1920, 1080, 5, 1, 1, PANELSIZE * 2, 0);
	CreateSprite("TextureFiles/Level6Pan2Overlay.png", 1920, 1080, 300, 1, 1, PANELSIZE * 2, 0);

	//Bounding Boxes
	CreateBoundingBoxes();

	//Black Overlay
	Vec3Set(&TextTint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = TextTint;

	//Blocker Boxes
	CreateBlockerBoxes(&newID);

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

	EasyEditPlatform(Plat, 10);
	//EasyEditWall(Wall1, 10);

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
		CurrentPlayer.CurrentLevel = GS_YeahGuy;
	else if(CurrentPlayer.CurrentLevel < GS_Level6)
		CurrentPlayer.CurrentLevel = GS_Level6;
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
		InitializePause(&DrawLevel6);
		//TogglePauseSound(&BackgroundSnd);
		UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
	}


	// Runs if the beginning animation is finished
	if(!beginningAnimation)
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
	SetUpCameraPanAndLockNoSpawner(&levelComplete, PANELSIZE, numPanels);
	UpdateBlockerBoxes(PANELSIZE);

	//////////////////////////////////
	//       EVERYTHING ELSE        //
	//////////////////////////////////
	BoundingBoxUpdate();
}
