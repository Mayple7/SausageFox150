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
static int iconPosition;

Sprite* GrassBackground;
Sprite* MapBackground;
Sprite* FadeOverlay;
Sprite* PlayerIcon;

void LoadMapLevel(void)
{
	//Allocate space for a large texture
	CreateTextureList();
}

void InitializeMapLevel(void)
{
	Vec3 Tint;
	FILE *fp;
	Vec3Set(&Tint, 0.0f, 0.5f, 0.0f);
	newID = 10;
	ResetObjectList();
	ResetCamera();

	fp = fopen("../GameData.cfg", "rt");
	if(fp)
	{
		int num = 0;
		num = fscanf(fp, "%*s %d", &(CurrentPlayer.CurrentLevel));

		fclose(fp);
	}
	else
		CurrentPlayer.CurrentLevel = GS_Tutorial;

	GrassBackground = (Sprite *)CreateSprite("TextureFiles/BlankPlatform.png", 4000, 1080, 1, 1, 1, 480, 0);
	GrassBackground->Tint = Tint;
	MapBackground = (Sprite *)CreateSprite("TextureFiles/Map.png", 2880, 1080, 2, 1, 1, 480, 0);
	FadeOverlay = (Sprite *)CreateSprite("TextureFiles/FadeOverlay.png", 1920, 1080, 3, 1, 1, 0, 0);

	PlayerIcon = (Sprite *)CreateSprite("TextureFiles/fox_head.png", 76, 74, 5, 1, 1, 0, 0);

	switch(CurrentPlayer.CurrentLevel)
	{
	case GS_Tutorial:
	case GS_Level1:
		FadeOverlay->Position.x = 0.0f;
		SetCameraXPosition(-300.0f);
		break;
	case GS_Level2:
		FadeOverlay->Position.x = 300.0f;
		break;
	case GS_ArmGuy:
	case GS_HandGuy:
	case GS_Level3:
		FadeOverlay->Position.x = 650.0f;
		break;
	case GS_YeahGuy:
		FadeOverlay->Position.x = 950.0f;
		break;
	case GS_Level4:
	case GS_Kevin:
		FadeOverlay->Visible = FALSE;
		break;
	default:
		FadeOverlay->Position.x = 0.0f;
	}


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
