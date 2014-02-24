/*****************************************************************************/
/*!
\file				HandGuy.c
\author				Dan Muller (d.muller)
\date				Feb 15, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					LoadHandGuy
\li					InitializeHandGuy
\li					UpdateHandGuy
\li					DrawHandGuy
\li					FreeHandGuy
\li					UnloadHandGuy
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/HandGuy.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"


// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
int newID;					// ID number
TextGlyphs *LevelName;
TextGlyphs *ShopText1;
TextGlyphs *ShopText2;
Sprite *HandGauy;
Platform *Pedestal;
Weapon* StarterSword;

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadHandGuy(void)
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
void InitializeHandGuy(void)
{
	Vec3 TextTint;

	newID = 1;
	ResetObjectList();
	ResetCamera();

	//Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, newID++, 0, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;
	CurrentPlayer.PlayerCollider.collisionDebug = FALSE;

	HandGauy = (Sprite *) CreateSprite("TextureFiles/HandGauy.png", 400, 400, 10, 1, 1, -800, -250);

	//Background
	CreateSprite("TextureFiles/MansionHandGauy.png", 1920, 1080, 0, 1, 1, 0, 0);
	CreateSprite("TextureFiles/MansionHandGauyDoor.png", 1920, 1080, 200, 1, 1, 0, 0);

	//Weapon/Shop
	CreateSprite("TextureFiles/WoodSign.png", 512, 512, 5, 1, 1, -400, -140);
	Vec3Set(&TextTint, 0, 0, 0);
	ShopText1 = CreateText("SWORD", -380, -20, 80, TextTint, Center);
	ChangeTextVisibility(ShopText1);
	Vec3Set(&TextTint, 0.2f, 0.2f, 0.2f);
	ShopText2 = CreateText("5 coin", -390, -80, 60, TextTint, Center);
	ChangeTextVisibility(ShopText2);
	StarterSword = CreateDroppedWeapon(Sword, Common, 250, 250, newID++, -350, -140);
	StarterSword->WeaponSprite->Rotation = (float)FOX_PI /2;

	//Platform
	Pedestal = CreatePlatform("TextureFiles/MarblePedestal.png", PlatformType, 1920, 1080, newID++, 0, 0);
	Pedestal->PlatformCollider.Offset.y = -3.6f * Pedestal->PlatformSprite->Height / 16;
	UpdateCollider(&Pedestal->PlatformCollider, Pedestal->PlatformCollider.width * 0.19f, Pedestal->PlatformCollider.height * 0.05f); 

	//Title text
	Vec3Set(&TextTint, 1, 1, 1);
	LevelName = CreateText("HandGauy Level", 0, 300, 100, TextTint, Center);
	ChangeTextVisibility(LevelName);
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateHandGuy(void)
{
	EventHandGuy();

	// This should be the last line in this function
	UpdatePlayerPosition(&CurrentPlayer);
}

/*************************************************************************/
/*!
	\brief
	Draws the level
*/
/*************************************************************************/
void DrawHandGuy(void)
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
void FreeHandGuy(void)
{
	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadHandGuy(void)
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
void EventHandGuy(void)
{
	// Check for any collision and handle the results
	DetectPlayerCollision();
	// Handle any input for the current player
	InputPlayer(&CurrentPlayer);

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
		//InitializePause(&DrawHandGuy);
		//TogglePauseSound(&BackgroundSnd);
		SetNextState(GS_MainMenu);
		//UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
	}
}
