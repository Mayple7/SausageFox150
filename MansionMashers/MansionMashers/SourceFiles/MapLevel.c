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

// FoxMansion, Shop1, Lvl2, Arm, Hand, Shop2, Lvl3, Yeah, Shop3, Lvl4, Kevin
// 0 through 10
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
	Vec2 PlayerIconPosition;
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
	if(Cheats)
		FadeOverlay->Visible = FALSE;

	CreateBoundingBoxes();

	//Sets the position of the icon based on the previous level
	switch(GetPreviousState())
	{
	case GS_Tutorial:
	case GS_Level1:
		Vec2Set(&PlayerIconPosition, -596.5f, -41.5f);
		break;
	case GS_Shop1:
		Vec2Set(&PlayerIconPosition, -280.8f, -12.25f);
		break;
	case GS_Level2:
		Vec2Set(&PlayerIconPosition, -41.8f, -10.7f);
		break;
	case GS_ArmGuy:
		Vec2Set(&PlayerIconPosition, 169.6f, -242.0f);
		break;
	case GS_HandGuy:
		Vec2Set(&PlayerIconPosition, 172.6f, 274.2f);
		break;
	case GS_Shop2:
		Vec2Set(&PlayerIconPosition, 393.2f, 13.8f);
		break;
	case GS_Level3:
		Vec2Set(&PlayerIconPosition, 682.6f, -1.5f);
		break;
	case GS_YeahGuy:
		Vec2Set(&PlayerIconPosition, 1113.1f, -9.2f);
		break;
	case GS_Shop3:
		Vec2Set(&PlayerIconPosition, 1256.5f, 252.8f);
		break;
	case GS_Level4:
		Vec2Set(&PlayerIconPosition, 1356.0f, -12.25f);
		break;
	case GS_Kevin:
		Vec2Set(&PlayerIconPosition, 1630.3f, -13.8f);
		break;
	default:
		Vec2Set(&PlayerIconPosition, -596.4f, -41.4f);
		break;
	}
	Vec2Scale(&PlayerIconPosition, &PlayerIconPosition, GetLoadRatio());
	PlayerIcon->Position = PlayerIconPosition;
}

void UpdateMapLevel(void)
{
	EventLevel();
	BoundingBoxUpdate();
	if(PlayerIcon->Position.x > GetCameraXPosition() + 300 * GetLoadRatio() )
	{
		SetCameraXPosition(PlayerIcon->Position.x - 300 * GetLoadRatio());
	}
	else if(PlayerIcon->Position.x < GetCameraXPosition() - 300 * GetLoadRatio())
	{
		SetCameraXPosition(PlayerIcon->Position.x + 300 * GetLoadRatio());
	}
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
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		InitializePause(&DrawMapLevel);
		//TogglePauseSound(&BackgroundSnd);
		UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
	}
	if(FoxInput_KeyDown('D'))
	{
		PlayerIcon->Position.x += 600 * GetDeltaTime() * GetLoadRatio();
	}
	if(FoxInput_KeyDown('A'))
	{
		PlayerIcon->Position.x -= 600 * GetDeltaTime() * GetLoadRatio();
	}
}
