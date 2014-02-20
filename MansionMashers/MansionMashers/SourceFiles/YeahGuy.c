/*****************************************************************************/
/*!
\file				YeahGuy.c
\author				Dan Muller (d.muller)
\date				Feb 15, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					LoadYeahGuy
\li					InitializeYeahGuy
\li					UpdateYeahGuy
\li					DrawYeahGuy
\li					FreeYeahGuy
\li					UnloadYeahGuy
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/YeahGuy.h"
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
TextGlyphs* LevelName;

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadYeahGuy(void)
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
void InitializeYeahGuy(void)
{
	Vec3 TextTint;

	newID = 1;
	ResetObjectList();
	ResetCamera();

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, newID++, 0, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	Vec3Set(&TextTint, 1, 1, 1);
	LevelName = CreateText("YeahGuy Level", 0, 300, 100, TextTint, Center);
	ChangeTextVisibility(LevelName);
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateYeahGuy(void)
{
	EventYeahGuy();

	// This should be the last line in this function
	UpdatePlayerPosition(&CurrentPlayer);
}

/*************************************************************************/
/*!
	\brief
	Draws the level
*/
/*************************************************************************/
void DrawYeahGuy(void)
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
void FreeYeahGuy(void)
{
	FreeObjectList();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadYeahGuy(void)
{
	//Destroy the textures
	DestroyTextureList();
}

/*************************************************************************/
/*!
	\brief
	Handles all events in the level
*/
/*************************************************************************/
void EventYeahGuy(void)
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
		//InitializePause(&DrawYeahGuy);
		//TogglePauseSound(&BackgroundSnd);
		SetNextState(GS_MainMenu);
		//UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
	}
}
