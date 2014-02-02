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
Sprite *FoxBig;
Sprite *FoxSmall;
Sprite *Letters;

Player CurrentPlayer;

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
	Vec3 textTint;
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

	OverlayGrid = CreateSprite("TextureFiles/OverlayGrid.png", 2000, 1080, 100, 1, 1);

	// Create single player HUD sprite
	HUD = CreateSprite("TextureFiles/MaypleHUD.png", 320.0f, 137.0f, 200, 1, 1);
	HUD->isHUD = TRUE;

	// Create single player HUD item sprite
	HUDitem = CreateSprite("TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1);
	HUDitem->ItemType = 0;
	HUDitem->isHUD = TRUE;

	// Create the background sprite
	Background = CreateSprite("TextureFiles/LevelBackground.png", 3840.0f, 720.0f, 0, 1, 1);
	
	// Create the offset background sprite
	Background2 = CreateSprite("TextureFiles/LevelBackground.png", 3840.0f, 720.0f, 0, 1, 1);
	Background2->Position.x = 3840;
	Background2->FlipX = TRUE;

	// Create the shelf sprite and initialize to be collidable
	Shelf = CreatePlatform("TextureFiles/Shelf.png", PlatformType, 123, 245, newID++);
	UpdatePlatformPosition(Shelf, 475, -115);
	Shelf->PlatformCollider.Offset.y = Shelf->PlatformSprite->Height / 2 - 30;
	Shelf->PlatformCollider.height = 60;
	
	// Create and initialize the crate sprite
	Crate = CreatePlatform("TextureFiles/Crate.png", PlatformType, 859, 260.5f, newID++);
	UpdatePlatformPosition(Crate, 1400, -180);
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

	// Add the enemy and player to the collidable list
	AddCollidable(CurrentPlayer.PlayerSprite);
	CurrentPlayer.PlayerSprite->Visible = TRUE;

	BouncePad = CreatePlatform("TextureFiles/BouncePad.png", BounceType, 400, 100, newID++);
	BouncePad->Position.x = -1000;
	BouncePad->Position.y = -200;
	BouncePad->PlatformCollider.Position = BouncePad->Position;
	BouncePad->PlatformSprite->Position = BouncePad->Position;
	BouncePad->PlatformRigidBody.Restitution = 1.9f;

	FoxBig = CreateSprite("Textures/GinkoArt.png", 148.5, 270, 20, 1, 1);
	FoxBig->Position.x = -300;
	FoxSmall = CreateSprite("Textures/GinkoSmall.png", 148.5, 270, 20, 1, 1);
	FoxSmall->Position.x = -800;

	Letters = CreateSprite("TextureFiles/Rumple_TextSheet.png", 92, 100, 20, 11, 4);
	Letters->AnimationSpeed = 60;
	Letters->Position.x = -1200;
	Letters->Position.y = 100;

	Vec3Set(&textTint, 0.6f, 0.169f, 0.8f);
	CreateText("Hi Juli, This is dynamic!", 100, 300, 100, textTint);

	Vec3Set(&textTint, 1.0f, 1.0f, 0.0f);
	CreateText("Luke is muy grande.", 100, 200, 100, textTint);

	Vec3Set(&textTint, 1.0f, 0.0f, 0.0f);
	CreateText("+1 to kaden for being 100% less of a kevin this week", 100, 100, 100, textTint);

	Vec3Set(&textTint, 0.01f, 0.6f, 0.39f);
	CreateText("Dan is just way too awesome, hahahaha!!!", 300, 0, 150, textTint);

	Vec3Set(&textTint, 1.0f, 1.0f, 1.0f);
	CreateText("The quick brown fox jumps over the lazy dog! -+!.,%()", 100, -200, 200, textTint);

	Vec3Set(&textTint, 0.0f, 0.0f, 0.0f);
	CreateText("(These are the letters and symbols that you can display)", 300, -300, 50, textTint);

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

	/*
	TODO: 
		- Make loop for drawing collidebox
		- Make loop for removing collide box on free
		- Make loop for freeing all collision items
	*/
	displayCollisionDebug(&Ham->FoodCollider);
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




