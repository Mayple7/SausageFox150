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
Sprite *HandGauy;

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
	int i;

	newID = 1;
	ResetObjectList();
	ResetCamera();

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, newID++, 0, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;
	CurrentPlayer.PlayerCollider.collisionDebug = FALSE;

	HandGauy = (Sprite *) CreateSprite("TextureFiles/HandGauy.png", 400, 400, 10, 1, 1, -700, -250);

	for (i = -50; i < 50; i++)
	{
		CreateSprite("TextureFiles/HandGauy.png", (float)(i + 60) * 4, (float)(i + 60) * 4, (i + 60) / 10, 1, 1, (float)i * 10, (float)-230);
	}
	for (i = -25; i < 25; i++)
	{
		CreateSprite("TextureFiles/HandGauy.png", (float)(i + 60) * 4, (float)(i + 60) * 4, (i + 60) / 10, 1, 1, (float)i * -20 - 100, (float)i * 20 - 50);
	}
	for (i = -50; i < 50; i++)
	{
		CreateSprite("TextureFiles/HandGauy.png", (float)(i + 60) * 4, (float)(i + 60) * 4, (i + 60) / 10, 1, 1, (float)i * 10, (float)sin((float)i) * 200 + i * 10);
	}

	CreateSprite("TextureFiles/MansionHandGauy.png", 1920, 1080, 0, 1, 1, 0, 0);
	CreateSprite("TextureFiles/MansionHandGauyDoor.png", 1920, 1080, 200, 1, 1, 0, 0);

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
	FreeObjectList();
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
