/*****************************************************************************/
/*!
\file				Shop1.c
\author				Kaden Nugent (kaden.n)
\date				Feb 15, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					LoadLevel
\li					InitializeLevel
\li					UpdateLevel
\li					DrawLevel
\li					FreeLevel
\li					UnloadLevel
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Shop1.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"
#include "../HeaderFiles/BoundingBox.h"

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
static int newID;					// ID number
static int levelComplete;
static int beginningAnimiation;
HUD* CurrentHUD;
Sprite* BlackOverlay;

FoxSound *BackSnd;

static int PlayerIsAlive;

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadShop1(void)
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
void InitializeShop1(void)
{
	Vec3 Tint;
	int randNum, randType;

	levelComplete = FALSE;
	PlayerIsAlive = TRUE;
	beginningAnimiation = TRUE;
	newID = 10;
	ResetObjectList();
	ResetCamera();

	//Weapon/Shop
	randNum = (int)((rand() / (float)RAND_MAX) * 60);
	randType = (int)((rand() / (float)RAND_MAX) * FoxWeapon);

	// First shop plackard
	if(randNum > 98)
		CreateWeaponShop(-400, -140, newID++, randType, Sausage);
	else if(randNum > 85)
		CreateWeaponShop(-400, -140, newID++, randType, Rare);
	else if(randNum > 50)
		CreateWeaponShop(-400, -140, newID++, randType, Uncommon);
	else
		CreateWeaponShop(-400, -140, newID++, randType, Common);
	
	randNum = (int)((rand() / (float)RAND_MAX) * 60);
	randType = (int)((rand() / (float)RAND_MAX) * FoxWeapon);

	// Second shop plackard
	if(randNum > 98)
		CreateWeaponShop(600, -140, newID++, randType, Sausage);
	else if(randNum > 85)
		CreateWeaponShop(600, -140, newID++, randType, Rare);
	else if(randNum > 50)
		CreateWeaponShop(600, -140, newID++, randType, Uncommon);
	else
		CreateWeaponShop(600, -140, newID++, randType, Common);

	

	CreateSprite("TextureFiles/LevelGrassGround.png", 5760.0f, 1080.0f, 1, 1, 1, 0, 0);

	CreateSprite("TextureFiles/ShopKeeper.png", 350.0f, 350.0f, 80, 1, 1, 0, -250);

	Vec3Set(&Tint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = Tint;

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -1200, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	//Bounding Boxes
	CreateBoundingBoxes();

	BackSnd = CreateSound("Sounds/ShopTheme.wav", LargeSnd);

	CurrentHUD = CreateHUD(&CurrentPlayer);
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateShop1(void)
{
	EventLevel();

	PlayAudio(BackSnd);

	if(levelComplete)
	{
		BlackOverlay->Position.x = GetCameraXPosition();
		BlackOverlay->Alpha += 1 * GetDeltaTime();
		if(BlackOverlay->Alpha > 1)
		{
			SetNextState(GS_MapLevel);
		}
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
void DrawShop1(void)
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
void FreeShop1(void)
{
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
void UnloadShop1(void)
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
	/*////////////////////////////////
	//   INPUT & COLLISION FIRST    //
	////////////////////////////////*/
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		if(PlayerIsAlive == TRUE)
		{
			InitializePause(&DrawShop1);
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

	if(!beginningAnimiation && !levelComplete)
	{
		// Check for any collision and handle the results
		DetectPlayerCollision();
		// Handle any input for the current player
		InputPlayer(&CurrentPlayer);
		UpdateHUDItems(CurrentHUD, &CurrentPlayer);

		if(CurrentPlayer.Position.x > (1920.0f / 2) + CurrentPlayer.PlayerCollider.width || CurrentPlayer.Position.x < -(1920.0f / 2) - CurrentPlayer.PlayerCollider.width)
		{
			levelComplete = TRUE;
		}

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
			CurrentPlayer.FlipX = 1;
			CurrentPlayer.PlayerDirection = RIGHT;
			CurrentPlayer.Speed = CurrentPlayer.CurrentPlayerStats.MoveSpeed * GetDeltaTime();
			
			// Threshold to give control back to the player
			if(CurrentPlayer.Position.x > -500)
			{
				beginningAnimiation = FALSE;
			}
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
		{
			SetNextState(GS_MapLevel);
		}

	}

	UpdateFloatingText();
}
