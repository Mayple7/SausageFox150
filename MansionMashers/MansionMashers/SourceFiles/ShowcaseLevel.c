/*****************************************************************************/
/*!
\file				ShowcaseLevel.c
\author				Dan Muller (d.muller)
\date				Jan 22, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					LoadShowcase
\li					InitializeShowcase
\li					UpdateShowcase
\li					DrawShowcase
\li					FreeShowcase
\li					UnloadShowcase
\li					EventShowcase
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/LevelShowcase.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"


// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals

Sprite *HUD;
Sprite *HUDitem;
Sprite *Background;
Sprite *Background2;
HUDLayer HUDList;
Platform *BouncePad;
Platform *Shelf;
Platform *Crate;

Player CurrentPlayer;
Enemy CurrentEnemy;

Sprite *Hammy;

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadShowcase(void)
{
	//Placeholder
}

/*************************************************************************/
/*!
	\brief
	Initializes the objects for the showcase level
*/
/*************************************************************************/
void InitializeShowcase(void)
{
	Vec2 startingCamera;
	int hudLoop;
	resetObjectList();

	for (hudLoop = 0; hudLoop < 20; hudLoop++)
		HUDList.HudItem[hudLoop] = 0;

	// Create single player HUD sprite
	HUD = CreateSprite("HUD", "TextureFiles/MaypleHUD.png", 320.0f, 137.0f, 200, 1, 1, HudType);
	HUD->CanCollide = FALSE;

	// Create single player HUD item sprite
	HUDitem = CreateSprite("HUDitem", "TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1, HudType);
	HUDitem->CanCollide = FALSE;
	HUDitem->ItemType = 0;

	// Create the background sprite
	Background = CreateSprite("Background", "TextureFiles/LevelBackground.png", 3840.0f, 720.0f, 0, 1, 1, BackgroundType);
	Background->CanCollide = FALSE;
	
	// Create the offset background sprite
	Background2 = CreateSprite("Background", "TextureFiles/LevelBackground.png", 3840.0f, 720.0f, 0, 1, 1, BackgroundType);
	Background2->CanCollide = FALSE;
	Background2->Position.x = 3840;
	Background2->FlipX = TRUE;

	// Create the shelf sprite and initialize to be collidable
	Shelf = CreatePlatform("TextureFiles/Shelf.png", PlatformType, 11, 123, 245);
	Shelf->Position.x = 475;
	Shelf->Position.y = -115;
	Shelf->PlatformCollider.Position = Shelf->Position;
	Shelf->PlatformSprite->Position = Shelf->Position;
	Shelf->PlatformCollider.Offset.y = Shelf->PlatformSprite->Height / 2 - 30;
	Shelf->PlatformCollider.height = 60;
	
	// Create and initialize the crate sprite
	Crate = CreatePlatform("TextureFiles/Crate.png", PlatformType, 12, 859, 260.5f);
	Crate->Position.x = 1400;
	Crate->Position.y = -180;
	Crate->PlatformCollider.Position = Crate->Position;
	Crate->PlatformSprite->Position = Crate->Position;
	Crate->PlatformCollider.Offset.y = Crate->PlatformSprite->Height / 2 - 60;
	Crate->PlatformCollider.width = Crate->PlatformCollider.width - 100;
	Crate->PlatformCollider.height = 60;

	// Create and initialize the HAM sprite
	Hammy = CreateSprite("Hammy", "TextureFiles/Ham.png", 150.0f, 140.0f, 20, 1, 1, FoodType);
	Hammy->Position.x   = -1000.0f;
	Hammy->SensorType = RectangleCollider;
	Hammy->CollideDebug = TRUE;

	// Add the HUD sprites to the HUDlist
	HUDList.HudItem[0] = HUD;
	HUDList.HudItem[1] = HUDitem;

	// Create the player
	if(NULL != malloc(sizeof(Player)))
		InitializePlayer(&CurrentPlayer);

	CurrentPlayer.Position.x = -1380;
	CurrentPlayer.Position.y = -220;

	// Create the enemy
	if(NULL != malloc(sizeof(Enemy)))
		InitializeEnemy(&CurrentEnemy);

	CurrentEnemy.EnemySprite->CollideDebug = FALSE;
	CurrentEnemy.Position.x = 600;
	CurrentEnemy.EnemySprite->CollideSize.x = CurrentEnemy.EnemySprite->Width  / 1.1;
	CurrentEnemy.EnemySprite->CollideSize.y = CurrentEnemy.EnemySprite->Height / 1.1;

	// Add the enemy and player to the collidable list
	AddCollidable(CurrentEnemy.EnemySprite);
	AddCollidable(CurrentPlayer.PlayerSprite);

	//Show debug box
	CurrentPlayer.PlayerSprite->CollideDebug = TRUE;

	BouncePad = CreatePlatform("TextureFiles/BouncePad.png", PlatformType, 10, 400, 100);
	BouncePad->Position.x = -1000;
	BouncePad->Position.y = -200;
	BouncePad->PlatformCollider.Position = BouncePad->Position;
	BouncePad->PlatformSprite->Position = BouncePad->Position;

	// Set the camera to the starting position
	Vec2Set(&startingCamera, -1280, 0.0f);
	SetCamera(&startingCamera, 250, &HUDList);
}

/*************************************************************************/
/*!
	\brief
	Updates the showcase level
*/
/*************************************************************************/
void UpdateShowcase(void)
{
	// Hide the debug box past X=700
	if(CurrentPlayer.Position.x > 700)
		CurrentPlayer.PlayerSprite->CollideDebug = FALSE;
	else
		CurrentPlayer.PlayerSprite->CollideDebug = TRUE;
	// Run the enemy logic
	EnemyLogic(&CurrentEnemy, &CurrentPlayer);
	UpdateEnemy(&CurrentEnemy);

	// Handle any events such as collision
	EventShowcase();

	// Update the player position
	UpdatePlayerPosition(&CurrentPlayer);

	// Return to main menu with ESC
	if(AEInputCheckTriggered(VK_ESCAPE))
	{
		SetNextState(GS_MainMenu);
	}
}

/*************************************************************************/
/*!
	\brief
	Draws the showcase level
*/
/*************************************************************************/
void DrawShowcase(void)
{
	// Draws the object list and sets the camera to the correct location
	drawObjectList();
	SetCamera(&CurrentPlayer.Position, 250, &HUDList);
}

/*************************************************************************/
/*!
	\brief
	Frees all the objects in the showcase level
*/
/*************************************************************************/
void FreeShowcase(void)
{
	freeObjectList();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the showcase level
*/
/*************************************************************************/
void UnloadShowcase(void)
{
	//Placeholder
}

/*************************************************************************/
/*!
	\brief
	Handles all events in the showcase level
*/
/*************************************************************************/
void EventShowcase(void)
{
	// Check for any collision and handle the results
	DetectCollision();
	DetectPlayerCollision();
	// Handle any input for the current player
	InputPlayer(&CurrentPlayer);

	// Handle input for swapping items
	if(AEInputCheckTriggered('Q'))
	{
		if (HUDitem->ItemType == 0)
		{
			HUDitem->SpriteTexture = AEGfxTextureLoad("TextureFiles/ThrowableHUD.png");
			HUDitem->ItemType = 1;
		}
		else
		{
			HUDitem->SpriteTexture = AEGfxTextureLoad("TextureFiles/HealthPotionHUD.png");
			HUDitem->ItemType = 0;
		}
	}
}




