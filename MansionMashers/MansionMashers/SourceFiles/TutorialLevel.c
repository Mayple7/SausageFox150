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
Platform* ShortShelf;

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
	CurrentPlayer.Position.y = GROUNDLEVEL + 1;

	TutorialBackground = CreateSprite("TextureFiles/TutorialBackground.png", 1920, 1080, 0, 1, 1);
	OverlayGrid = CreateSprite("TextureFiles/OverlayGrid.png", 2000, 1080, 100, 1, 1);

	// Create the shelf sprite and initialize to be collidable
	Shelf = CreatePlatform("TextureFiles/Shelf.png", PlatformType, 184.5f, 367.5, newID++);
	UpdatePlatformPosition(Shelf, 475, -170);
	UpdateCollider(&Shelf->PlatformCollider, Shelf->PlatformCollider.width, Shelf->PlatformCollider.height * 0.16f); 
	Shelf->PlatformCollider.Offset.y = 3 * Shelf->PlatformSprite->Height / 8;
	Shelf->PlatformCollider.collisionDebug = TRUE;

	ShortShelf = CreatePlatform("TextureFiles/ShortShelf.png", PlatformType, 184.5f, 198.75f, newID++);
	UpdatePlatformPosition(ShortShelf, 280, -280);
	ShortShelf->PlatformCollider.Offset.y = 5 * ShortShelf->PlatformSprite->Height / 16;
	UpdateCollider(&ShortShelf->PlatformCollider, ShortShelf->PlatformCollider.width, ShortShelf->PlatformCollider.height * 0.2f); 
	ShortShelf->PlatformCollider.collisionDebug = TRUE;

	ResetCamera();
}

void UpdateTutorial(void)
{
	// Handle any events such as collision
	EventTutorial();

	// Update the player position
	UpdatePlayerPosition(&CurrentPlayer);

	if(CurrentPlayer.PlayerCollider.Position.x - CurrentPlayer.PlayerCollider.width / 2 < -7 * TutorialBackground->Width / 16)
	{
		CurrentPlayer.Position.x = (-7 * TutorialBackground->Width / 16) + (CurrentPlayer.PlayerCollider.width / 2) + 1;
	}
	else if(CurrentPlayer.PlayerCollider.Position.x + CurrentPlayer.PlayerCollider.width / 2 > 7 * TutorialBackground->Width / 16)
	{
		CurrentPlayer.Position.x = (7 * TutorialBackground->Width / 16) - (CurrentPlayer.PlayerCollider.width / 2) - 1;
	}

	if(AEInputCheckTriggered('U'))
	{
		ShortShelf->PlatformCollider.collisionDebug = FALSE;
		Shelf->PlatformCollider.collisionDebug = FALSE;
		CurrentPlayer.PlayerCollider.collisionDebug = FALSE;
		OverlayGrid->Visible = FALSE;
	}
	if(AEInputCheckTriggered('I'))
	{
		ShortShelf->PlatformCollider.collisionDebug = TRUE;
		Shelf->PlatformCollider.collisionDebug = TRUE;
		CurrentPlayer.PlayerCollider.collisionDebug = TRUE;
		OverlayGrid->Visible = TRUE;
	}


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
	DrawObjectList();

	DrawCollisionList();
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
}
