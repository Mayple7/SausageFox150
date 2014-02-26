/*****************************************************************************/
/*!
\file				MapLevel.c
\author				Dan Muller (d.muller)
\date				Jan 31, 2014

\brief				Functions for the tutorial level

\par				Functions:
\li					LoadMapLevel
\li					InitializeMapLevel
\li					UpdateMapLevel
\li					DrawMapLevel
\li					FreeMapLevel
\li					UnloadMapLevel
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../AEEngine.h"
#include "../HeaderFiles/MapLevel.h"
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

Sprite* GrassBackground;
Sprite* MapBackground;

void LoadMapLevel(void)
{
	//Allocate space for a large texture
	CreateTextureList();
}

void InitializeMapLevel(void)
{
	Vec3 Tint;
	Vec3Set(&Tint, 0.0f, 0.5f, 0.0f);
	newID = 10;
	ResetObjectList();
	ResetCamera();

	GrassBackground = (Sprite *)CreateSprite("TextureFiles/BlankPlatform.png", 4000, 1080, 1, 1, 1, 480, 0);
	GrassBackground->Tint = Tint;
	MapBackground = (Sprite *)CreateSprite("TextureFiles/Map.png", 2880, 1080, 2, 1, 1, 480, 0);

	CreateBoundingBoxes();
}

void UpdateMapLevel(void)
{
	EventLevel();
	BoundingBoxUpdate();
}

void DrawMapLevel(void)
{
	// Draws the object list and sets the camera to the correct location
	DrawObjectList();
	//DrawHUD(&HUDList);
	DrawCollisionList();
}

void FreeMapLevel(void)
{
	FreeAllLists();
}

void UnloadMapLevel(void)
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
void EventLevel(void)
{
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
		//InitializePause(&DrawLevel);
		//TogglePauseSound(&BackgroundSnd);
		SetNextState(GS_MainMenu);
		//UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
	}
	if(FoxInput_KeyDown('D'))
	{
		SetCameraXPosition(GetCameraXPosition() + 400 * GetDeltaTime() * GetLoadRatio());
	}
	if(FoxInput_KeyDown('A'))
	{
		SetCameraXPosition(GetCameraXPosition() - 400 * GetDeltaTime() * GetLoadRatio());
	}
}
