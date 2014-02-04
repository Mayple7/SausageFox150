/*****************************************************************************/
/*!
\file				TutorialLevel.c
\author				Dan Muller (d.muller)
\date				Jan 31, 2014

\brief				Functions for the tutorial level

\par				Functions:
\li					LoadTutorial
\li					InitializeTutorial
\li					UpdateTutorial
\li					DrawTutorial
\li					FreeTutorial
\li					UnloadTutorial
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../AEEngine.h"
#include "../HeaderFiles/TutorialLevel.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

Sprite* TutorialBackground;
Sprite* OverlayGrid;
Platform* Shelf;

Player CurrentPlayer;
int newID;

void LoadTutorial(void)
{
	CreateTextureList();
}

void InitializeTutorial(void)
{
	newID = 1;
	resetObjectList();

	InitializePlayer(&CurrentPlayer, newID++);

	TutorialBackground = CreateSprite("TextureFiles/TutorialBackground.png", 1920, 1080, 0, 1, 1);
	OverlayGrid = CreateSprite("TextureFiles/OverlayGrid.png", 2000, 1080, 100, 1, 1);

	// Create the shelf sprite and initialize to be collidable
	Shelf = CreatePlatform("TextureFiles/Shelf.png", PlatformType, 184.5f, 367.5, newID++);
	UpdatePlatformPosition(Shelf, 475, -170);
	Shelf->PlatformCollider.Offset.y = Shelf->PlatformSprite->Height / 2 - 30;
	Shelf->PlatformCollider.height = 60;

	ResetCamera();
}

void UpdateTutorial(void)
{
	// Handle any events such as collision
	EventTutorial();

	// Update the player position
	UpdatePlayerPosition(&CurrentPlayer);

	// Return to main menu with RSHIFT
	// Pause with ESCAPE
	if(AEInputCheckTriggered(VK_RSHIFT))
	{
		SetNextState(GS_MainMenu);
	}
	if(AEInputCheckTriggered(VK_ESCAPE))
	{
		InitializePause(&DrawTutorial);
		UpdatePause();
	}
}

void DrawTutorial(void)
{
	drawObjectList();
	UpdatePlayerCollider(&CurrentPlayer.PlayerCollider);
}

void FreeTutorial(void)
{
	freeObjectList();
}

void UnloadTutorial(void)
{
	DestroyTextureList();
}

void EventTutorial(void)
{
	// Check for any collision and handle the results
	DetectPlayerCollision();
	// Handle any input for the current player
	InputPlayer(&CurrentPlayer);

	// check if forcing the application to quit
	if (0 == AESysDoesWindowExist())
		SetNextState(GS_Quit);
}
