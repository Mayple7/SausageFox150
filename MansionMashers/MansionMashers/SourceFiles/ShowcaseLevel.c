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
int newID;					// ID number

Sprite *HUD;
Sprite *HUDitem;
Sprite *Background;
Sprite *Background2;
HUDLayer HUDList;
Platform *BouncePad;
Platform *Shelf;
Platform *Crate;
Sprite *OverlayGrid;

Player CurrentPlayer;
Enemy CurrentEnemy;

Food *Ham;
Food *Hammy;

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadShowcase(void)
{
	//Allocate space for a large texture
	CreateTextureList();
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

	newID = 1;
	resetObjectList();

	// Create the player
	if(NULL != malloc(sizeof(Player)))
		InitializePlayer(&CurrentPlayer, newID++);

	CurrentPlayer.Position.x = -1380;
	CurrentPlayer.Position.y = -220;
	CurrentPlayer.PlayerSprite->Position = CurrentPlayer.Position;
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	for (hudLoop = 0; hudLoop < 20; hudLoop++)
		HUDList.HudItem[hudLoop] = 0;

	OverlayGrid = CreateSprite("TextureFiles/OverlayGrid.png", 2000, 1080, 100, 1, 1, BackgroundType);

	// Create single player HUD sprite
	HUD = CreateSprite("TextureFiles/MaypleHUD.png", 320.0f, 137.0f, 200, 1, 1, HudType);

	// Create single player HUD item sprite
	HUDitem = CreateSprite("TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1, HudType);
	HUDitem->ItemType = 0;

	// Create the background sprite
	Background = CreateSprite("TextureFiles/LevelBackground.png", 3840.0f, 720.0f, 0, 1, 1, BackgroundType);
	
	// Create the offset background sprite
	Background2 = CreateSprite("TextureFiles/LevelBackground.png", 3840.0f, 720.0f, 0, 1, 1, BackgroundType);
	Background2->Position.x = 3840;
	Background2->FlipX = TRUE;

	// Create the shelf sprite and initialize to be collidable
	Shelf = CreatePlatform("TextureFiles/Shelf.png", PlatformType, 123, 245, newID++);
	Shelf->Position.x = 475;
	Shelf->Position.y = -115;
	Shelf->PlatformCollider.Position = Shelf->Position;
	Shelf->PlatformSprite->Position = Shelf->Position;
	Shelf->PlatformCollider.Offset.y = Shelf->PlatformSprite->Height / 2 - 30;
	Shelf->PlatformCollider.height = 60;
	
	// Create and initialize the crate sprite
	Crate = CreatePlatform("TextureFiles/Crate.png", PlatformType, 859, 260.5f, newID++);
	Crate->Position.x = 1400;
	Crate->Position.y = -180;
	Crate->PlatformCollider.Position = Crate->Position;
	Crate->PlatformSprite->Position = Crate->Position;
	Crate->PlatformCollider.Offset.y = Crate->PlatformSprite->Height / 2 - 60;
	Crate->PlatformCollider.width = Crate->PlatformCollider.width - 100;
	Crate->PlatformCollider.height = 60;

	Ham = CreateFood("TextureFiles/Ham.png", FoodType, 150, 140, newID++);
	Ham->Position.x = 0.0f;

	// Create and initialize the HAM sprite
	Hammy = CreateFood("TextureFiles/Ham.png", FoodType, 150.0f, 140.0f, newID++);
	UpdateFoodPosition(Hammy, -1000.0f, 0.0f);

	// Add the HUD sprites to the HUDlist
	HUDList.HudItem[0] = HUD;
	HUDList.HudItem[1] = HUDitem;

	// Create the enemy
	if(NULL != malloc(sizeof(Enemy)))
		InitializeEnemy(&CurrentEnemy);

	CurrentEnemy.EnemySprite->CollideDebug = FALSE;
	CurrentEnemy.Position.x = 600;
	CurrentEnemy.EnemySprite->CollideSize.x = CurrentEnemy.EnemySprite->Width  / 1.1f;
	CurrentEnemy.EnemySprite->CollideSize.y = CurrentEnemy.EnemySprite->Height / 1.1f;

	// Add the enemy and player to the collidable list
	AddCollidable(CurrentEnemy.EnemySprite);
	AddCollidable(CurrentPlayer.PlayerSprite);

	BouncePad = CreatePlatform("TextureFiles/BouncePad.png", BounceType, 400, 100, newID++);
	BouncePad->Position.x = -1000;
	BouncePad->Position.y = -200;
	BouncePad->PlatformCollider.Position = BouncePad->Position;
	BouncePad->PlatformSprite->Position = BouncePad->Position;
	BouncePad->PlatformRigidBody.Restitution = 1.9f;

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
	//Destroy the textures
	DestroyTextureList();
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

	// check if forcing the application to quit
	if (0 == AESysDoesWindowExist())
		SetNextState(GS_Quit);
}




