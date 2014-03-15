/*****************************************************************************/
/*!
\file				Level3.c
\author				Dan Muller (d.muller)
\date				Feb 15, 2034

\brief				Functions for the showcase level

\par				Functions:
\li					LoadLevel3
\li					InitializeLevel3
\li					UpdateLevel3
\li					DrawLevel3
\li					FreeLevel3
\li					UnloadLevel3
  
\par 
<b> Copyright (C) 2034 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// Level Notes!!!!
//
//	Since this level goes into the tree tops. There will be a fade out at
//  the end of Panel one and game will switch to Level3.1 which will place
//	the player at the bottom of the screen and the game will take place 
//	as if in a high spot. Can only access Level3.1 through Level 3.
//	Level3.1 is just a continuation of Level3.
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Level3.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"
#include "../HeaderFiles/EasyEdit.h"


// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// defines

#define PANELSIZE 1920.0f

// ---------------------------------------------------------------------------
// globals
static int newID;					// ID number
static int levelComplete = FALSE;
TextGlyphs* LevelName;

Platform *Plat;
Wall *Wall1;

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadLevel3(void)
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
void InitializeLevel3(void)
{
	Vec3 TextTint;
	newID = 10;
	ResetObjectList();
	ResetCamera();

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, 0, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	Vec3Set(&TextTint, 1, 1, 1);
	LevelName = CreateText("Level 3", 0, 300, 100, TextTint, Center, Border);
	ChangeTextVisibility(LevelName);

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	CreateSprite("TextureFiles/Level3Pan1.png", 1920, 1080, 1, 1, 1, 0, 0);

	/////////////////////////////////
	//		Platforms			   //
	/////////////////////////////////
	//Panel1
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 80.0f, 100.0f, newID++, 190, -255);
	Plat->PlatformSprite->Visible = FALSE;
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 80.0f, 100.0f, newID++, 580, -35);
	Plat->PlatformSprite->Visible = FALSE;


	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	//Ceiling
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 350.0f, 100.0f, newID++, 930, 590);
	Wall1->WallSprite->Visible = FALSE;
	//Bounding Walls
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 1040.0f, newID++, -958, 0);
	Wall1->WallSprite->Visible = FALSE;
	//Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 750.0f, newID++, 958, -170);
	Wall1->WallSprite->Visible = FALSE;
	//Final Platform (needs to have wall properties
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 780.0f, newID++, 940, -100);
	Wall1->WallSprite->Visible = FALSE;

	//Temporary Bounding Wall until rock pile is finalized
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 780.0f, newID++, 800, -340);
	Wall1->WallSprite->Visible = FALSE;
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateLevel3(void)
{
	EventLevel3();

	EasyEditWall(Wall1, 10);


	// This should be the last line in this function
	UpdatePlayerPosition(&CurrentPlayer);
}

/*************************************************************************/
/*!
	\brief
	Draws the level
*/
/*************************************************************************/
void DrawLevel3(void)
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
void FreeLevel3(void)
{
	//Second Part of Level
	if(levelComplete)
		CurrentPlayer.CurrentLevel = GS_Level31;
	else if(CurrentPlayer.CurrentLevel < GS_Level3)
		CurrentPlayer.CurrentLevel = GS_Level3;
	SavePlayer(&CurrentPlayer);
	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadLevel3(void)
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
void EventLevel3(void)
{
	// Check for any collision and handle the results
	DetectPlayerCollision();
	// Handle any input for the current player
	InputPlayer(&CurrentPlayer);

	if(CurrentPlayer.Position.x > (960 * GetLoadRatio()))
	{
		levelComplete = TRUE;
		SetNextState(GS_Level31);
	}

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
		//InitializePause(&DrawLevel3);
		//TogglePauseSound(&BackgroundSnd);
		SetNextState(GS_MainMenu);
		//UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
	}
}
