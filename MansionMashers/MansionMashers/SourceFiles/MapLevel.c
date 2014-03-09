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
	fp = fopen(GameData, "rt");
	if(fp)
	{
		// Initialize player to get all of their data loaded into the game.
		InitializePlayer(&CurrentPlayer, Mayple, 999999, -9999999);
		fclose(fp);
	}
	else
		CurrentPlayer.CurrentLevel = GS_Tutorial;

	//If not coming from a level set to the player's latest level
	if(iconPosition > GS_LevelNUM)
		iconPosition = CurrentPlayer.CurrentLevel;

	//Create the large sprites
	HazeBackground = (Sprite *)CreateSprite("TextureFiles/MapHaze.png", 4000, 1080, 1, 1, 1, 480, 0);
	MapBackground = (Sprite *)CreateSprite("TextureFiles/Map.png", 2880, 1080, 20, 1, 1, 480, 0);
	FadeOverlay = (Sprite *)CreateSprite("TextureFiles/FadeOverlay.png", 1920, 1080, 40, 1, 1, 0, 0);
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

	Tutorial = CreateButton("TextureFiles/RedX.png", -652.52f, -233.28f, 175, 75, newID++);
	Level1 = CreateButton("TextureFiles/RedX.png", -432.86f, -221.43f, 175, 75, newID++);
	Level2 = CreateButton("TextureFiles/RedX.png", -113.06f, -120.21f, 175, 75, newID++);

	Shop1 = CreateButton("TextureFiles/RedX.png", -211.05f, 97.29f, 175, 75, newID++);
	Level3 = CreateButton("TextureFiles/RedX.png", 114.14f, -273.12f, 175, 75, newID++);
	Level4 = CreateButton("TextureFiles/RedX.png", 122.75f, 236.19f, 175, 75, newID++);

	ArmGuy = CreateButton("TextureFiles/RedX.png", 388.71f, -235.43f, 175, 75, newID++);
	Shop2 = CreateButton("TextureFiles/RedX.png", 627.76f, -258.04f, 175, 75, newID++);

	HandGuy = CreateButton("TextureFiles/RedX.png", 386.56f, 184.51f, 175, 75, newID++);
	Shop3 = CreateButton("TextureFiles/RedX.png", 606.22f, 186.66f, 175, 75, newID++);

	Level5 = CreateButton("TextureFiles/RedX.png", 400.56f, 5.77f, 175, 75, newID++);
	Level6 = CreateButton("TextureFiles/RedX.png", 658.98f, -28.69f, 175, 75, newID++);
	YeahGuy = CreateButton("TextureFiles/RedX.png", 1064.93f, -86.83f, 175, 75, newID++);

	Shop4 = CreateButton("TextureFiles/RedX.png", 1106.91f, 155.44f, 175, 75, newID++);
	Level7 = CreateButton("TextureFiles/RedX.png", 1324.43f, -121.29f, 175, 75, newID++);
	Kevin = CreateButton("TextureFiles/RedX.png", 1582.85f, -139.60f, 175, 75, newID++);

	UpdateProgression(&CurrentPlayer);

	SystemOne = CreateFoxParticleSystem("TextureFiles/MapParticle.png", 0, 0, 10, -1, 15, 0.5f, 0, 100, 20.0f, 5.0f, 4000, 1080, 50, 2.0f, 2.0f);
	SystemOne->FadeIn = TRUE;

/*	GS_Level6,
	GS_YeahGuy,
	GS_Shop4,
	GS_Level7,
	GS_Kevin,*/
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
	case GS_Shop1:
	case GS_Level3:
	case GS_Level4:
		FadeOverlay->Position.x = 500.0f;
		break;
	case GS_ArmGuy:
	case GS_HandGuy:
	case GS_Level5:
	case GS_Shop2:
	case GS_Shop3:
		if(CurrentPlayer.armUnlock || CurrentPlayer.handUnlock)
			FadeOverlay->Position.x = 900.0f;
		else
			FadeOverlay->Position.x = 700.0f;
		break;
	case GS_Level6:
		FadeOverlay->Position.x = 900.0f;
		break;
	case GS_YeahGuy:
		FadeOverlay->Position.x = 1300.0f;
		break;
	case GS_Level7:
	case GS_Shop4:
		FadeOverlay->Position.x = 1600.0f;
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
	MapInfoText->Position.x = GetCameraXPosition();
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

		UpdateProgression(&CurrentPlayer);
		//TogglePauseSound(&BackgroundSnd);
	}

	if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		int worldX, worldY;
		Vec2 MouseClick;

		FoxInput_GetWorldPosition(&worldX, &worldY);
		Vec2Set(&MouseClick, (float)worldX, (float)worldY);

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
		else if(PointRectCollision(&Shop4->ButtonCollider, &MouseClick) && (GS_Shop4 <= CurrentPlayer.CurrentLevel || Cheats))
		{
			GetNewIconPosition(&PlayerIcon->Position, GS_Shop4);
			iconPosition = GS_Shop4;
		}
		//Level7
		else if(PointRectCollision(&Level7->ButtonCollider, &MouseClick) && (GS_Shop4 <= CurrentPlayer.CurrentLevel || Cheats))
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


	// Go into the level the icon is on
	if(FoxInput_KeyTriggered(VK_SPACE) || FoxInput_KeyTriggered('E'))
	{
		SetNextState(iconPosition);
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

	//Account for a scaled window
	Vec2Scale(NewPosition, NewPosition, GetLoadRatio());
}

void UpdateProgression(Player *CurrentPlayer)
{
	Tutorial->ButtonSprite->Visible = TRUE;
	Level1->ButtonSprite->Visible = TRUE;
	Level2->ButtonSprite->Visible = TRUE;

	Shop1->ButtonSprite->Visible = TRUE;
	Level3->ButtonSprite->Visible = TRUE;
	Level4->ButtonSprite->Visible = TRUE;

	ArmGuy->ButtonSprite->Visible = TRUE;
	Shop2->ButtonSprite->Visible = TRUE;

	HandGuy->ButtonSprite->Visible = TRUE;
	Shop3->ButtonSprite->Visible = TRUE;

	Level5->ButtonSprite->Visible = TRUE;
	Level6->ButtonSprite->Visible = TRUE;
	YeahGuy->ButtonSprite->Visible = TRUE;

	Shop4->ButtonSprite->Visible = TRUE;
	Level7->ButtonSprite->Visible = TRUE;
	Kevin->ButtonSprite->Visible = TRUE;

	if(!Cheats)
	{
		switch(CurrentPlayer->CurrentLevel)
		{
		case GS_Kevin:
			Kevin->ButtonSprite->Visible = FALSE;
		case GS_Level7:
		case GS_Shop4:
			Level7->ButtonSprite->Visible = FALSE;
			Shop4->ButtonSprite->Visible = FALSE;
		case GS_YeahGuy:
			YeahGuy->ButtonSprite->Visible = FALSE;
		case GS_Level6:
			Level6->ButtonSprite->Visible = FALSE;
		case GS_Level5:
		case GS_ArmGuy:
		case GS_HandGuy:
		case GS_Shop2:
		case GS_Shop3:
			Level5->ButtonSprite->Visible = FALSE;
		case GS_Level4:
		case GS_Level3:
		case GS_Shop1:
			Level3->ButtonSprite->Visible = FALSE;
			Level4->ButtonSprite->Visible = FALSE;
			Shop1->ButtonSprite->Visible = FALSE;;
		case GS_Level2:
			Level2->ButtonSprite->Visible = FALSE;
		case GS_Level1:
			Level1->ButtonSprite->Visible = FALSE;
		case GS_Tutorial:
			Tutorial->ButtonSprite->Visible = FALSE;
		}
		if(CurrentPlayer->armUnlock)
		{
			ArmGuy->ButtonSprite->Visible = FALSE;
			
			if(CurrentPlayer->armClear)
			{
				Shop2->ButtonSprite->Visible = FALSE;
			}
		}
		if(CurrentPlayer->handUnlock)
		{
			HandGuy->ButtonSprite->Visible = FALSE;
			if(CurrentPlayer->handClear)
			{
				Shop3->ButtonSprite->Visible = FALSE;
			}
		}
	}
	else
	{
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
	}

}

