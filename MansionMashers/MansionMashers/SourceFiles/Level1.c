/*****************************************************************************/
/*!
\file				Level1.c
\author				Dan Muller (d.muller)
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


// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
int newID;					// ID number

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
}

/*************************************************************************/
/*!
	\brief
	Initializes the objects for the level
*/
/*************************************************************************/
void InitializeLevel1(void)
{
	newID = 1;
	resetObjectList();
	ResetCamera();

	// Initialize the player
	InitializePlayer(&CurrentPlayer, newID++, 0, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateLevel1(void)
{
	EventLevel1();

	// This should be the last line in this function
	UpdatePlayerPosition(&CurrentPlayer);
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
	freeObjectList();
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
}

/*************************************************************************/
/*!
	\brief
	Handles all events in the level
*/
/*************************************************************************/
void EventLevel1(void)
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
		InitializePause(&DrawLevel1);
		//TogglePauseSound(&BackgroundSnd);
		//SetNextState(GS_MainMenu);
		UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
	}
}
