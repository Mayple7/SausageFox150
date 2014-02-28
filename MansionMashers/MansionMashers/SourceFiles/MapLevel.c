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
\li					EventLevel
  
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

ParticleSystem* SystemOne;

/*************************************************************************/
/*!
	\brief
	Loads the map level textures
*/
/*************************************************************************/
void LoadMapLevel(void)
{
	//Allocate space for a large texture
	CreateTextureList();
}

/*************************************************************************/
/*!
	\brief
	Initializes the map level
*/
/*************************************************************************/
void InitializeMapLevel(void)
{
	Vec3 Tint;
	FILE *fp;

	//Set the grass color for the background
	Vec3Set(&Tint, 0.0f, 0.5f, 0.0f);
	newID = 10;

	//Icon position is what level we just came from
	iconPosition = GetPreviousState();

	//Reset me captain
	ResetObjectList();
	ResetCamera();

	//Reads the level from a file if it exits, otherwise set to tutorial level
	fp = fopen("../GameData.cfg", "rt");
	if(fp)
	{
		int num = 0;
		num = fscanf(fp, "%*s %d", &(CurrentPlayer.CurrentLevel));

		fclose(fp);
	}
	else
		CurrentPlayer.CurrentLevel = GS_Level1;

	//If not coming from a level set to the player's latest level
	if(iconPosition > GS_LevelNUM)
		iconPosition = CurrentPlayer.CurrentLevel;

	//Create the large sprites
	GrassBackground = (Sprite *)CreateSprite("TextureFiles/MapHaze.png", 4000, 1080, 1, 1, 1, 480, 0);
	MapBackground = (Sprite *)CreateSprite("TextureFiles/MapTemp.png", 2880, 1080, 20, 1, 1, 480, 0);
	FadeOverlay = (Sprite *)CreateSprite("TextureFiles/FadeOverlay.png", 1920, 1080, 30, 1, 1, 0, 0);

	// Foxy icon ^_^
	switch(CurrentPlayer.Princess)
	{
	case Mayple:
		PlayerIcon = (Sprite *)CreateSprite("TextureFiles/MaypleHead.png", 102.75f, 119.25f, 50, 1, 1, 0, 0);
		break;
	case Ginko:
		PlayerIcon = (Sprite *)CreateSprite("TextureFiles/GinkoHead.png", 129.75f, 101.25f, 50, 1, 1, 0, 0);
		break;
	case Kaya:
		PlayerIcon = (Sprite *)CreateSprite("TextureFiles/KayaHead.png", 104.8f, 108, 50, 1, 1, 0, 0);
		break;
	case Holly:
		PlayerIcon = (Sprite *)CreateSprite("TextureFiles/HollyHead.png", 140.25f, 108.75f, 50, 1, 1, 0, 0);
		break;
	}

	

	SystemOne = CreateFoxParticleSystem("TextureFiles/MapParticle.png", 0, 0, 10, -1, 15, 0.5f, 0, 100, 20.0f, 5.0f, 4000, 1080, 50, 2.0f, 2.0f);
	SystemOne->FadeIn = TRUE;

	// Adjusts the fade overlay based on the farthest progressed level
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
	// CHEATER!!!!
	if(Cheats)
		FadeOverlay->Visible = FALSE;

	CreateBoundingBoxes();

	//Sets icon to the correct position
	GetNewIconPosition(&PlayerIcon->Position, iconPosition);
}

/*************************************************************************/
/*!
	\brief
	Runs the update for the map level
*/
/*************************************************************************/
void UpdateMapLevel(void)
{
	EventLevel();

	//Camera movement based on icon movement
	if(PlayerIcon->Position.x > GetCameraXPosition() + 300 * GetLoadRatio() )
	{
		SetCameraXPosition(PlayerIcon->Position.x - 300 * GetLoadRatio());
	}
	else if(PlayerIcon->Position.x < GetCameraXPosition() - 300 * GetLoadRatio())
	{
		SetCameraXPosition(PlayerIcon->Position.x + 300 * GetLoadRatio());
	}

	BoundingBoxUpdate();
}

/*************************************************************************/
/*!
	\brief
	Draws the map level
*/
/*************************************************************************/
void DrawMapLevel(void)
{
	// Draws the object list and sets the camera to the correct location
	DrawObjectList();
	//DrawHUD(&HUDList);
	DrawCollisionList();
}

/*************************************************************************/
/*!
	\brief
	Frees all allocated memory from the map level
*/
/*************************************************************************/
void FreeMapLevel(void)
{
	// Memory Moses returns!
	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads the textures from the map level
*/
/*************************************************************************/
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
	//Update the particles
	ParticleSystemUpdate();
	//Execute the pause menu
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		InitializePause(&DrawMapLevel);
		//TogglePauseSound(&BackgroundSnd);
		UpdatePause();
		if(Cheats)
			FadeOverlay->Visible = FALSE;
		else
			FadeOverlay->Visible = TRUE;
		//TogglePauseSound(&BackgroundSnd);
	}

	// Go into the level the icon is on
	if(FoxInput_KeyTriggered(VK_SPACE) || FoxInput_KeyTriggered('E'))
	{
		//if(iconPosition == GS_Tutorial)
			//iconPosition = GS_Level1;
		SetNextState(iconPosition);
	}

	//Movement for the icon
	if(FoxInput_KeyTriggered('D'))
	{
		int nextPosition = -1;

		//Hard coded oh noes! Checks if able to move a certain direction based on location
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

		//If it is a valid location and the player can access the level OR IF YOU ARE A CHEATER
		if((nextPosition != iconPosition && nextPosition <= CurrentPlayer.CurrentLevel) || Cheats)
		{
			//Moves the icon
			iconPosition = nextPosition;
			GetNewIconPosition(&PlayerIcon->Position, iconPosition);
		}
	}
	//Movement for the icon
	if(FoxInput_KeyTriggered('A'))
	{
		int nextPosition = -1;

		//Hard coded oh noes! Checks if able to move a certain direction based on location
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

		//If it is a valid location and the player can access the level OR IF YOU ARE A CHEATER
		if((nextPosition != iconPosition && nextPosition <= CurrentPlayer.CurrentLevel) || Cheats)
		{
			//Moves the icon
			iconPosition = nextPosition;
			GetNewIconPosition(&PlayerIcon->Position, iconPosition);
		}
	}
	//Movement for the icon
	if(FoxInput_KeyTriggered('W'))
	{
		int nextPosition = -1;

		//Hard coded oh noes! Checks if able to move a certain direction based on location
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

		//If it is a valid location and the player can access the level OR IF YOU ARE A CHEATER
		if((nextPosition != iconPosition && nextPosition <= CurrentPlayer.CurrentLevel) || Cheats)
		{
			//Moves the icon
			iconPosition = nextPosition;
			GetNewIconPosition(&PlayerIcon->Position, iconPosition);
		}
	}
	//Movement for the icon
	if(FoxInput_KeyTriggered('S'))
	{
		int nextPosition = -1;

		//Hard coded oh noes! Checks if able to move a certain direction based on location
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

		//If it is a valid location and the player can access the level OR IF YOU ARE A CHEATER
		if((nextPosition != iconPosition && nextPosition <= CurrentPlayer.CurrentLevel) || Cheats)
		{
			//Moves the icon
			iconPosition = nextPosition;
			GetNewIconPosition(&PlayerIcon->Position, iconPosition);
		}
	}
}

/*************************************************************************/
/*!
	\brief
	Sets the vector to the new position for the icon

	\param NewPosition
	The new icon position

	\param newLocation
	The level to set the icon to
*/
/*************************************************************************/
void GetNewIconPosition(Vec2 *NewPosition, int newLocation)
{
	//Sets the position of the icon based on the previous level
	// OSHI HARD CODED!!!
	switch(newLocation)
	{
	case GS_Tutorial:
	case GS_Level1:
		Vec2Set(NewPosition, -598.16f, -73.53f);
		break;
	case GS_Shop1:
		Vec2Set(NewPosition, -290.37f, 176.17f);
		break;
	case GS_Level2:
		Vec2Set(NewPosition, -59.05f, 1.53f);
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

	//Account for a scaled window
	Vec2Scale(NewPosition, NewPosition, GetLoadRatio());
}
