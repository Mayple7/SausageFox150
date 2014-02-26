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
	FILE *fp;
	Vec3Set(&Tint, 0.0f, 0.5f, 0.0f);
	newID = 10;
	iconPosition = GetPreviousState();
	if(iconPosition > GS_LevelNUM)
		iconPosition = GS_Level1;
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

	GetNewIconPosition(&PlayerIcon->Position, GetPreviousState());
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

	if(FoxInput_KeyTriggered(VK_SPACE) || FoxInput_KeyTriggered('E'))
	{
		SetNextState(iconPosition);
	}

	if(FoxInput_KeyTriggered('D'))
	{
		int nextPosition = -1;

		switch(iconPosition)
		{
		case GS_Level1:
			nextPosition = GS_Shop1;
			break;
		case GS_Shop1:
			nextPosition = GS_Level2;
			break;
		case GS_ArmGuy:
			nextPosition = GS_Shop2;
			break;
		case GS_HandGuy:
			nextPosition = GS_Shop2;
			break;
		case GS_Shop2:
			nextPosition = GS_Level3;
			break;
		case GS_Level3:
			nextPosition = GS_YeahGuy;
			break;
		case GS_YeahGuy:
			nextPosition = GS_Level4;
			break;
		case GS_Shop3:
			nextPosition = GS_Level4;
			break;
		case GS_Level4:
			nextPosition = GS_Kevin;
			break;
		default:
			nextPosition = iconPosition;
		}

		if((nextPosition != iconPosition && nextPosition <= CurrentPlayer.CurrentLevel) || Cheats)
		{
			iconPosition = nextPosition;
			GetNewIconPosition(&PlayerIcon->Position, iconPosition);
		}
	}
	if(FoxInput_KeyTriggered('A'))
	{
		int nextPosition = -1;

		switch(iconPosition)
		{
		case GS_Shop1:
			nextPosition = GS_Level1;
			break;
		case GS_Level2:
			nextPosition = GS_Shop1;
			break;
		case GS_ArmGuy:
			nextPosition = GS_Level2;
			break;
		case GS_HandGuy:
			nextPosition = GS_Level2;
			break;
		case GS_Level3:
			nextPosition = GS_Shop2;
			break;
		case GS_YeahGuy:
			nextPosition = GS_Level3;
			break;
		case GS_Shop3:
			nextPosition = GS_YeahGuy;
			break;
		case GS_Level4:
			nextPosition = GS_YeahGuy;
			break;
		case GS_Kevin:
			nextPosition = GS_Level4;
			break;
		default:
			nextPosition = iconPosition;
		}

		if((nextPosition != iconPosition && nextPosition <= CurrentPlayer.CurrentLevel) || Cheats)
		{
			iconPosition = nextPosition;
			GetNewIconPosition(&PlayerIcon->Position, iconPosition);
		}
	}
	if(FoxInput_KeyTriggered('W'))
	{
		int nextPosition = -1;

		switch(iconPosition)
		{
		case GS_Level2:
			nextPosition = GS_HandGuy;
			break;
		case GS_Shop2:
			nextPosition = GS_HandGuy;
			break;
		case GS_YeahGuy:
			nextPosition = GS_Shop3;
			break;
		case GS_Level4:
			nextPosition = GS_Shop3;
			break;
		default:
			nextPosition = iconPosition;
		}

		if((nextPosition != iconPosition && nextPosition <= CurrentPlayer.CurrentLevel) || Cheats)
		{
			iconPosition = nextPosition;
			GetNewIconPosition(&PlayerIcon->Position, iconPosition);
		}
	}
	if(FoxInput_KeyTriggered('S'))
	{
		int nextPosition = -1;

		switch(iconPosition)
		{
		case GS_Level2:
			nextPosition = GS_ArmGuy;
			break;
		case GS_Shop2:
			nextPosition = GS_ArmGuy;
			break;
		case GS_Shop3:
			nextPosition = GS_YeahGuy;
			break;
		default:
			nextPosition = iconPosition;
		}

		if((nextPosition != iconPosition && nextPosition <= CurrentPlayer.CurrentLevel) || Cheats)
		{
			iconPosition = nextPosition;
			GetNewIconPosition(&PlayerIcon->Position, iconPosition);
		}
	}
}

void GetNewIconPosition(Vec2 *NewPosition, int newLocation)
{
	//Sets the position of the icon based on the previous level
	switch(newLocation)
	{
	case GS_Tutorial:
	case GS_Level1:
		Vec2Set(NewPosition, -596.5f, -41.5f);
		break;
	case GS_Shop1:
		Vec2Set(NewPosition, -280.8f, -12.25f);
		break;
	case GS_Level2:
		Vec2Set(NewPosition, -41.8f, -10.7f);
		break;
	case GS_ArmGuy:
		Vec2Set(NewPosition, 169.6f, -242.0f);
		break;
	case GS_HandGuy:
		Vec2Set(NewPosition, 172.6f, 274.2f);
		break;
	case GS_Shop2:
		Vec2Set(NewPosition, 393.2f, 13.8f);
		break;
	case GS_Level3:
		Vec2Set(NewPosition, 682.6f, -1.5f);
		break;
	case GS_YeahGuy:
		Vec2Set(NewPosition, 1113.1f, -9.2f);
		break;
	case GS_Shop3:
		Vec2Set(NewPosition, 1256.5f, 252.8f);
		break;
	case GS_Level4:
		Vec2Set(NewPosition, 1356.0f, -12.25f);
		break;
	case GS_Kevin:
		Vec2Set(NewPosition, 1630.3f, -13.8f);
		break;
	default:
		Vec2Set(NewPosition, -596.4f, -41.4f);
		break;
	}

	Vec2Scale(NewPosition, NewPosition, GetLoadRatio());
}
