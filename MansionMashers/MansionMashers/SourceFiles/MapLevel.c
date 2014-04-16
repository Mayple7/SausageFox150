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

static int iconPosition;

Sprite* HazeBackground;
Sprite* MapBackground;
Sprite* FadeOverlay;
Sprite* PlayerIcon;
Sprite* MapInfoText;

ParticleSystem* SystemOne;

Button* Tutorial;
Button* Level1;
Button* Level2;

Button* Shop1;
Button* Level3;
Button* Level4;

Button* ArmGuy;
Button* Shop2;

Button* HandGuy;
Button* Shop3;

Button* Level5;
Button* Level6;
Button* YeahGuy;

Button* Shop4;
Button* Level7;
Button* Kevin;

FoxSound* BackSnd;

/*
	1		GS_Tutorial,
	2		GS_Level1,
	4		GS_Level2,
	8		GS_Level3,
	16		GS_Level4,
	32		GS_Level5,
	64		GS_Level6,
	128		GS_YeahGuy,
	256		GS_Level7,
	512		GS_Kevin,
*/

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

	//Reset me captain
	ResetObjectList();
	ResetCamera();

	//Reads the level from a file if it exits, otherwise set to tutorial level
	fp = fopen(GameData, "rt");
	if(fp)
	{
		// Initialize player to get all of their data loaded into the game.
		InitializePlayer(&CurrentPlayer, Mayple, 999999, -9999999);
		fclose(fp);
	}
	else
		CurrentPlayer.CurrentLevel = GS_Tutorial;

	//Icon position is what level we just came from
	iconPosition = GetPreviousState();

	// If coming from a menu, set the player to the latest level
	if(iconPosition > GS_LevelNUM && iconPosition != GS_Level31)
		iconPosition = CurrentPlayer.CurrentLevel;
	// Set the player's icon position to the next level if applicable (sorry long if statement :( )
	else if(iconPosition == GS_Tutorial && CurrentPlayer.CurrentLevel > GS_Tutorial)
	{
		iconPosition = GS_Level1;
	}
	else if(iconPosition == GS_Level1 && CurrentPlayer.CurrentLevel > GS_Level1)
	{
		iconPosition = GS_Level2;
	}
	else if(iconPosition == GS_Level2 && CurrentPlayer.CurrentLevel > GS_Level2)
	{
		iconPosition = GS_Level3;
	}
	else if((iconPosition == GS_Level31 || iconPosition == GS_Level3) && CurrentPlayer.armUnlock)
	{
		iconPosition = GS_ArmGuy;
	}
	else if(iconPosition == GS_Level4 && CurrentPlayer.handUnlock)
	{
		iconPosition = GS_HandGuy;
	}
	else if(iconPosition == GS_ArmGuy && CurrentPlayer.armClear)
	{
		iconPosition = GS_Shop2;
	}
	else if(iconPosition == GS_HandGuy && CurrentPlayer.handClear)
	{
		iconPosition = GS_Shop3;
	}
	else if(iconPosition == GS_Level5 && CurrentPlayer.CurrentLevel >= GS_Level6)
	{
		iconPosition = GS_Level6;
	}
	else if(iconPosition == GS_Level6 && CurrentPlayer.CurrentLevel > GS_Level6)
	{
		iconPosition = GS_YeahGuy;
	}
	else if(iconPosition == GS_YeahGuy && CurrentPlayer.CurrentLevel > GS_YeahGuy)
	{
		iconPosition = GS_Level7;
	}
	else if(iconPosition == GS_Level7 && CurrentPlayer.CurrentLevel > GS_Level7)
	{
		iconPosition = GS_Kevin;
	}
	if(iconPosition == GS_Level31)
		iconPosition = GS_Level3;

	//Create the large sprites
	HazeBackground = (Sprite *)CreateSprite("TextureFiles/MapHaze.png", 4000, 1080, 1, 1, 1, 480, 0);
	MapBackground = (Sprite *)CreateSprite("TextureFiles/Map.png", 2880, 1080, 20, 1, 1, 480, 0);
	FadeOverlay = (Sprite *)CreateSprite("TextureFiles/FadeOverlay.png", 1920, 1080, 50, 1, 1, 0, 0);
	MapInfoText = (Sprite *)CreateSprite("TextureFiles/MapInfoText.png", 769, 119, 50, 1, 1, GetCameraXPosition(), 450);

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

	Tutorial = CreateButton("TextureFiles/BlankPlatform.png", -652.52f, -233.28f, 175, 75);
	Level1 = CreateButton("TextureFiles/BlankPlatform.png", -432.86f, -221.43f, 175, 75);
	Level2 = CreateButton("TextureFiles/BlankPlatform.png", -113.06f, -120.21f, 175, 75);

	Shop1 = CreateButton("TextureFiles/BlankPlatform.png", -211.05f, 97.29f, 175, 75);
	Level3 = CreateButton("TextureFiles/BlankPlatform.png", 114.14f, -273.12f, 175, 75);
	Level4 = CreateButton("TextureFiles/BlankPlatform.png", 122.75f, 236.19f, 175, 75);

	ArmGuy = CreateButton("TextureFiles/BlankPlatform.png", 388.71f, -235.43f, 175, 75);
	Shop2 = CreateButton("TextureFiles/BlankPlatform.png", 627.76f, -258.04f, 175, 75);

	HandGuy = CreateButton("TextureFiles/BlankPlatform.png", 386.56f, 184.51f, 175, 75);
	Shop3 = CreateButton("TextureFiles/BlankPlatform.png", 606.22f, 186.66f, 175, 75);

	Level5 = CreateButton("TextureFiles/BlankPlatform.png", 400.56f, 5.77f, 175, 75);
	Level6 = CreateButton("TextureFiles/BlankPlatform.png", 658.98f, -28.69f, 175, 75);
	YeahGuy = CreateButton("TextureFiles/BlankPlatform.png", 1064.93f, -86.83f, 175, 75);

	Shop4 = CreateButton("TextureFiles/BlankPlatform.png", 1106.91f, 155.44f, 175, 75);
	Level7 = CreateButton("TextureFiles/BlankPlatform.png", 1324.43f, -121.29f, 175, 75);
	Kevin = CreateButton("TextureFiles/BlankPlatform.png", 1582.85f, -139.60f, 175, 75);

	Tutorial->ButtonSprite->Visible = FALSE;
	Level1->ButtonSprite->Visible = FALSE;
	Level2->ButtonSprite->Visible = FALSE;

	Shop1->ButtonSprite->Visible = FALSE;
	Level3->ButtonSprite->Visible = FALSE;
	Level4->ButtonSprite->Visible = FALSE;

	ArmGuy->ButtonSprite->Visible = FALSE;
	Shop2->ButtonSprite->Visible = FALSE;

	HandGuy->ButtonSprite->Visible = FALSE;
	Shop3->ButtonSprite->Visible = FALSE;

	Level5->ButtonSprite->Visible = FALSE;
	Level6->ButtonSprite->Visible = FALSE;
	YeahGuy->ButtonSprite->Visible = FALSE;

	Shop4->ButtonSprite->Visible = FALSE;
	Level7->ButtonSprite->Visible = FALSE;
	Kevin->ButtonSprite->Visible = FALSE;

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
		FadeOverlay->Position.x = 400.0f;
		break;
	case GS_Shop1:
	case GS_Level3:
	case GS_Level4:
		FadeOverlay->Position.x = 600.0f;
		break;
	case GS_ArmGuy:
	case GS_HandGuy:
	case GS_Level5:
	case GS_Shop2:
	case GS_Shop3:
		if(CurrentPlayer.armUnlock || CurrentPlayer.handUnlock)
			FadeOverlay->Position.x = 1100.0f;
		else
			FadeOverlay->Position.x = 700.0f;
		break;
	case GS_Level6:
		FadeOverlay->Position.x = 1100.0f;
		break;
	case GS_YeahGuy:
		FadeOverlay->Position.x = 1500.0f;
		break;
	case GS_Level7:
	case GS_Shop4:
		FadeOverlay->Position.x = 1800.0f;
		break;
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
	SetClearFlags(&CurrentPlayer);
	MapInfoText->Position.x = GetCameraXPosition();

	BackSnd = CreateSound("Sounds/MenuTheme.mp3", LargeSnd);
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
	PlayAudio(BackSnd);

	//Camera movement based on icon movement
	if(PlayerIcon->Position.x > GetCameraXPosition() + 300 )
	{
		SetCameraXPosition(PlayerIcon->Position.x - 300);
	}
	else if(PlayerIcon->Position.x < GetCameraXPosition() - 300)
	{
		SetCameraXPosition(PlayerIcon->Position.x + 300);
	}
	MapInfoText->Position.x = GetCameraXPosition();
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
	I wasn't going to make this function...but then I did...
*/
/*************************************************************************/
int pointRectCollisionSprite(Sprite *objA, Vec2 *objB)
{
	Vec2 posA = objA->Position;

	//Collidable 1
	float leftAx   = posA.x - (objA->Width / 2);
	float rightAx  = leftAx + objA->Width;
	float topAy    = posA.y + (objA->Height / 2);
	float bottomAy = topAy - objA->Height;

	if(objB->x >= leftAx && objB->x <= rightAx && objB->y >= bottomAy && objB->y <= topAy)
		return TRUE;
	else
		return FALSE;
}

/*************************************************************************/
/*!
	\brief
	Handles all events in the level
*/
/*************************************************************************/
void EventLevel(void)
{
	int worldX, worldY;
	Vec2 MouseClick;

	FoxInput_GetWorldPosition(&worldX, &worldY);
	Vec2Set(&MouseClick, (float)worldX, (float)worldY);

	//Update the particles
	ParticleSystemUpdate();

	//Execute the pause menu
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		InitializePause(&DrawMapLevel);
		TogglePauseSound(BackSnd);
		UpdatePause();
		if(Cheats)
			FadeOverlay->Visible = FALSE;
		else
			FadeOverlay->Visible = TRUE;

		TogglePauseSound(BackSnd);
	}

	// Go into the level the icon is on
	if(FoxInput_KeyTriggered(VK_SPACE) || FoxInput_KeyTriggered('E') || 
	   FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT) && pointRectCollisionSprite(PlayerIcon, &MouseClick))
	{
		SetNextState(iconPosition);
	}

	if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		//Tutorial
		if(PointRectCollision(&Tutorial->ButtonCollider, &MouseClick) && (GS_Tutorial <= CurrentPlayer.CurrentLevel || Cheats))
		{
			GetNewIconPosition(&PlayerIcon->Position, GS_Tutorial);
			iconPosition = GS_Tutorial;
		}
		//Level1
		else if(PointRectCollision(&Level1->ButtonCollider, &MouseClick) && (GS_Level1 <= CurrentPlayer.CurrentLevel || Cheats))
		{
			GetNewIconPosition(&PlayerIcon->Position, GS_Level1);
			iconPosition = GS_Level1;
		}
		//Level2
		else if(PointRectCollision(&Level2->ButtonCollider, &MouseClick) && (GS_Level2 <= CurrentPlayer.CurrentLevel || Cheats))
		{
			GetNewIconPosition(&PlayerIcon->Position, GS_Level2);
			iconPosition = GS_Level2;
		}
		//Shop1
		else if(PointRectCollision(&Shop1->ButtonCollider, &MouseClick) && (GS_Shop1 <= CurrentPlayer.CurrentLevel || Cheats))
		{
			GetNewIconPosition(&PlayerIcon->Position, GS_Shop1);
			iconPosition = GS_Shop1;
		}
		//Level3
		else if(PointRectCollision(&Level3->ButtonCollider, &MouseClick) && (GS_Shop1 <= CurrentPlayer.CurrentLevel || Cheats))
		{
			GetNewIconPosition(&PlayerIcon->Position, GS_Level3);
			iconPosition = GS_Level3;
		}
		//Level4
		else if(PointRectCollision(&Level4->ButtonCollider, &MouseClick) && (GS_Shop1 <= CurrentPlayer.CurrentLevel || Cheats))
		{
			GetNewIconPosition(&PlayerIcon->Position, GS_Level4);
			iconPosition = GS_Level4;
		}
		//ArmGuy
		else if(PointRectCollision(&ArmGuy->ButtonCollider, &MouseClick) && (CurrentPlayer.armUnlock || Cheats))
		{
			GetNewIconPosition(&PlayerIcon->Position, GS_ArmGuy);
			iconPosition = GS_ArmGuy;
		}
		//HandGuy
		else if(PointRectCollision(&HandGuy->ButtonCollider, &MouseClick) && (CurrentPlayer.handUnlock || Cheats))
		{
			GetNewIconPosition(&PlayerIcon->Position, GS_HandGuy);
			iconPosition = GS_HandGuy;
		}
		//Level5
		else if(PointRectCollision(&Level5->ButtonCollider, &MouseClick) && (GS_ArmGuy <= CurrentPlayer.CurrentLevel || Cheats))
		{
			GetNewIconPosition(&PlayerIcon->Position, GS_Level5);
			iconPosition = GS_Level5;
		}
		//Shop2
		else if(PointRectCollision(&Shop2->ButtonCollider, &MouseClick) && (CurrentPlayer.armClear || Cheats))
		{
			GetNewIconPosition(&PlayerIcon->Position, GS_Shop2);
			iconPosition = GS_Shop2;
		}
		//Shop3
		else if(PointRectCollision(&Shop3->ButtonCollider, &MouseClick) && (CurrentPlayer.handClear || Cheats))
		{
			GetNewIconPosition(&PlayerIcon->Position, GS_Shop3);
			iconPosition = GS_Shop3;
		}
		//Level6
		else if(PointRectCollision(&Level6->ButtonCollider, &MouseClick) && (GS_Level6 <= CurrentPlayer.CurrentLevel || Cheats))
		{
			GetNewIconPosition(&PlayerIcon->Position, GS_Level6);
			iconPosition = GS_Level6;
		}
		//YeahGuy
		else if(PointRectCollision(&YeahGuy->ButtonCollider, &MouseClick) && (GS_YeahGuy <= CurrentPlayer.CurrentLevel || Cheats))
		{
			GetNewIconPosition(&PlayerIcon->Position, GS_YeahGuy);
			iconPosition = GS_YeahGuy;
		}
		//Shop4
		else if(PointRectCollision(&Shop4->ButtonCollider, &MouseClick) && (GS_Level7 <= CurrentPlayer.CurrentLevel || Cheats))
		{
			GetNewIconPosition(&PlayerIcon->Position, GS_Shop4);
			iconPosition = GS_Shop4;
		}
		//Level7
		else if(PointRectCollision(&Level7->ButtonCollider, &MouseClick) && (GS_Level7 <= CurrentPlayer.CurrentLevel || Cheats))
		{
			GetNewIconPosition(&PlayerIcon->Position, GS_Level7);
			iconPosition = GS_Level7;
		}
		//Kevin
		else if(PointRectCollision(&Kevin->ButtonCollider, &MouseClick) && (GS_Kevin <= CurrentPlayer.CurrentLevel || Cheats))
		{
			GetNewIconPosition(&PlayerIcon->Position, GS_Kevin);
			iconPosition = GS_Kevin;
		}
	}

	//Debug Buttons
	if(FoxInput_KeyTriggered('U'))
	{
		SetDebugMode();
	}
	if(FoxInput_KeyTriggered('I'))
	{
		RemoveDebugMode();
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
		Vec2Set(NewPosition, -652.52f, -228.28f);
		break;
	case GS_Level1:
		Vec2Set(NewPosition, -432.86f, -216.43f);
		break;
	case GS_Level2:
		Vec2Set(NewPosition, -113.06f, -115.21f);		
		break;
	case GS_Shop1:
		Vec2Set(NewPosition, -211.05f, 102.29f);
		break;
	case GS_Level3:
		Vec2Set(NewPosition, 114.14f, -268.12f);
		break;
	case GS_Level4:
		Vec2Set(NewPosition, 122.75f, 241.19f);
		break;
	case GS_ArmGuy:
		Vec2Set(NewPosition, 388.71f, -230.43f);
		break;
	case GS_Shop2:
		Vec2Set(NewPosition, 627.76f, -258.04f);
		break;
	case GS_HandGuy:
		Vec2Set(NewPosition, 387.63f, 195.97f);
		break;
	case GS_Shop3:
		Vec2Set(NewPosition, 606.22f, 186.66f);
		break;
	case GS_Level5:
		Vec2Set(NewPosition, 400.56f, 5.77f);
		break;
	case GS_Level6:
		Vec2Set(NewPosition, 658.98f, -28.69f);
		break;
	case GS_YeahGuy:
		Vec2Set(NewPosition, 1067.22f, -80.76f);
		break;
	case GS_Shop4:
		Vec2Set(NewPosition, 1106.91f, 155.44f);
		break;
	case GS_Level7:
		Vec2Set(NewPosition, 1324.43f, -121.29f);
		break;
	case GS_Kevin:
		Vec2Set(NewPosition, 1582.85f, -139.60f);
		break;
	default:
		Vec2Set(NewPosition, -596.4f, -41.4f);
		break;
	}
}

void SetClearFlags(Player *CurrentPlayer)
{
	// Tutorial Level
	if(CurrentPlayer->levelClearBitFlags & 1)
	{
		CreateSprite("TextureFiles/FoxFlag.png", 110, 110, 40, 1, 1, -645.52f, -179.28f);
	}
	else
	{
		CreateSprite("TextureFiles/DogFlag.png", 110, 110, 40, 1, 1, -645.52f, -179.28f);
	}


	// Level 1
	if(CurrentPlayer->levelClearBitFlags & 2)
	{
		CreateSprite("TextureFiles/FoxFlag.png", 110, 110, 40, 1, 1, -422.86f, -172.43f);
	}
	else
	{
		CreateSprite("TextureFiles/DogFlag.png", 110, 110, 40, 1, 1, -422.86f, -172.43f);
	}

	// Level 2
	if(CurrentPlayer->levelClearBitFlags & 4)
	{
		CreateSprite("TextureFiles/FoxFlag.png", 110, 110, 40, 1, 1, -103.06f, -70.21f);
	}
	else
	{
		CreateSprite("TextureFiles/DogFlag.png", 110, 110, 40, 1, 1, -103.06f, -70.21f);
	}

	// Level 3
	if(CurrentPlayer->levelClearBitFlags & 8)
	{
		CreateSprite("TextureFiles/FoxFlag.png", 110, 110, 40, 1, 1, 122.14f, -222.12f);
	}
	else
	{
		CreateSprite("TextureFiles/DogFlag.png", 110, 110, 40, 1, 1, 122.14f, -222.12f);
	}

	// Level 4
	if(CurrentPlayer->levelClearBitFlags & 16)
	{
		CreateSprite("TextureFiles/FoxFlag.png", 110, 110, 40, 1, 1, 132.75f, 281.19f);
	}
	else
	{
		CreateSprite("TextureFiles/DogFlag.png", 110, 110, 40, 1, 1, 132.75f, 281.19f);
	}

	// ArmGuy
	if(CurrentPlayer->armClear)
	{
		CreateSprite("TextureFiles/FoxFlag.png", 110, 110, 40, 1, 1, 398.71f, -185.43f);
	}
	else
	{
		CreateSprite("TextureFiles/DogFlag.png", 110, 110, 40, 1, 1, 398.71f, -185.43f);
	}

	// HandGuy
	if(CurrentPlayer->handClear)
	{
		CreateSprite("TextureFiles/FoxFlag.png", 110, 110, 40, 1, 1, 397.63f, 235.97f);
	}
	else
	{
		CreateSprite("TextureFiles/DogFlag.png", 110, 110, 40, 1, 1, 397.63f, 235.97f);
	}

	// Level 5
	if(CurrentPlayer->levelClearBitFlags & 32)
	{
		CreateSprite("TextureFiles/FoxFlag.png", 110, 110, 40, 1, 1, 410.56f, 53.77f);
	}
	else
	{
		CreateSprite("TextureFiles/DogFlag.png", 110, 110, 40, 1, 1, 410.56f, 53.77f);
	}

	// Level 6
	if(CurrentPlayer->levelClearBitFlags & 64)
	{
		CreateSprite("TextureFiles/FoxFlag.png", 110, 110, 40, 1, 1, 668.98f, 17.69f);
	}
	else
	{
		CreateSprite("TextureFiles/DogFlag.png", 110, 110, 40, 1, 1, 668.98f, 17.69f);
	}

	// YeahGuy
	if(CurrentPlayer->levelClearBitFlags & 128)
	{
		CreateSprite("TextureFiles/FoxFlag.png", 110, 110, 40, 1, 1, 1077.22f, -37.76f);
	}
	else
	{
		CreateSprite("TextureFiles/DogFlag.png", 110, 110, 40, 1, 1, 1077.22f, -37.76f);
	}

	// Level 7
	if(CurrentPlayer->levelClearBitFlags & 256)
	{
		CreateSprite("TextureFiles/FoxFlag.png", 110, 110, 40, 1, 1, 1334.43f, -71.29f);
	}
	else
	{
		CreateSprite("TextureFiles/DogFlag.png", 110, 110, 40, 1, 1, 1334.43f, -71.29f);
	}

	// Kevin
	if(CurrentPlayer->levelClearBitFlags & 512)
	{
		CreateSprite("TextureFiles/FoxFlag.png", 110, 110, 40, 1, 1, 1597.85f, -85.60f);
	}
	else
	{
		CreateSprite("TextureFiles/DogFlag.png", 110, 110, 40, 1, 1, 1597.85f, -85.60f);
	}
}
