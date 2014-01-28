/*****************************************************************************/
/*!
\file				TestLevel.c
\author				Kaden Nugent (kaden.n)
\author				Dan Muller (d.muller)
\date				Jan 8, 2014

\brief				Level functions for the test level

\par				Functions:
\li					LoadTestLevel
\li					InitializeTestLevel
\li					UpdateTestLevel
\li					DrawTestLevel
\li					FreeTestLevel
\li					UnloadTestLevel
\li					EventTestLevel
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/TestLevel.h"
#include "../HeaderFiles/FoxEngine.h"

// ---------------------------------------------------------------------------
// Globals
Sprite *HUD1;
Sprite *HUD2;
Sprite *HUD3;
Sprite *HUD4;
Sprite *HUD1item;
Sprite *HUD2item;
Sprite *HUD3item;
Sprite *HUD4item;
Sprite *Background;
Sprite *ColliderInvisible;
Player CurrentPlayer;
Enemy CurrentEnemy;
HUDLayer HUDList;

Sprite *Hammy;
Sprite *Hammy2;

/*************************************************************************/
/*!
	\brief
	Loads the assets for the test level
*/
/*************************************************************************/
void LoadTestLevel(void)
{
	//Placeholder
}

/*************************************************************************/
/*!
	\brief
	Initializes the objects for the test level
*/
/*************************************************************************/
void InitializeTestLevel(void)
{	
	int hudLoop;

	// Reset the object list
	resetObjectList();

	// Set the HUDlist to 0
	for (hudLoop = 0; hudLoop < 20; hudLoop++)
		HUDList.HudItem[hudLoop] = 0;

	// Create the HAMs
	Hammy = CreateSprite("Hammy", "TextureFiles/Ham.png", 150.0f, 140.0f, 20, 1, 1, FoodType);
	Hammy2 = CreateSprite("Hammy2", "TextureFiles/Ham.png", 150.0f, 140.0f, 22, 1, 1, FoodType);

	// Collision for the first HAM
	Hammy->SensorType   = RectangleCollider;
	Hammy->Position.x   = 400.0f;
	Hammy->CollideDebug = 1;
	Hammy->CollideOffset.x =  200.0f;
	Hammy->CollideOffset.y =  0.0f;

	// Collision for the second HAM
	Hammy2->SensorType = RectangleCollider;
	Hammy2->Position.y = -100.0f;
	Hammy2->Position.x = -500.0f;

	// Creating the HUD items
	HUD1 = CreateSprite("HUD1", "TextureFiles/GinkoHUD.png", 320.0f, 137.0f, 200, 1, 1, HudType);
	HUD1->CanCollide = 0;

	HUD2 = CreateSprite("HUD2", "TextureFiles/HollyHUD.png", 320.0f, 137.0f, 200, 1, 1, HudType);
	HUD2->CanCollide = 0;
	
	HUD3 = CreateSprite("HUD3", "TextureFiles/MaypleHUD.png", 320.0f, 137.0f, 200, 1, 1, HudType);
	HUD3->CanCollide = 0;

	HUD4 = CreateSprite("HUD4", "TextureFiles/KayaHUD.png", 320.0f, 137.0f, 200, 1, 1, HudType);
	HUD4->CanCollide = 0;

	HUD1item = CreateSprite("HUDitem", "TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1, HudType);
	HUD1item->CanCollide = 0;
	HUD1item->ItemType = 0;

	HUD2item = CreateSprite("HUDitem", "TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1, HudType);
	HUD2item->CanCollide = 0;
	HUD2item->ItemType = 0;

	HUD3item = CreateSprite("HUDitem", "TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1, HudType);
	HUD3item->CanCollide = 0;
	HUD3item->ItemType = 0;

	HUD4item = CreateSprite("HUDitem", "TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1, HudType);
	HUD4item->CanCollide = 0;
	HUD4item->ItemType = 0;

	// Add HUD items to the list
	HUDList.HudItem[0] = HUD1;
	HUDList.HudItem[2] = HUD2;
	HUDList.HudItem[4] = HUD3;
	HUDList.HudItem[6] = HUD4;
	HUDList.HudItem[1] = HUD1item;
	HUDList.HudItem[3] = HUD2item;
	HUDList.HudItem[5] = HUD3item;
	HUDList.HudItem[7] = HUD4item;

	// Create the background
	Background = CreateSprite("Background", "TextureFiles/LevelGrassground.png", 3840.0f, 720.0f, 0, 1, 1, BackgroundType);
	Background->CanCollide = 0;

	// Secret invisible collider GO!
	ColliderInvisible = CreateSprite("ColliderInvisible", "TextureFiles/LevelGrassground.png", 300.0f, 80.0f, 0, 1, 1, PlatformType);
	ColliderInvisible->Visible = FALSE;
	ColliderInvisible->CollideDebug = TRUE;
	ColliderInvisible->Position.x = -1020.0f;
	ColliderInvisible->Position.y = -40.0f;

	// Creates the player
	if(NULL != malloc(sizeof(Player)))
		InitializePlayer(&CurrentPlayer);

	// Creates the enemy
	if(NULL != malloc(sizeof(Enemy)))
		InitializeEnemy(&CurrentEnemy);
	CurrentEnemy.EnemySprite->CollideDebug = TRUE;
	CurrentEnemy.EnemySprite->CollideSize.x = CurrentEnemy.EnemySprite->Width  / 1.1;
	CurrentEnemy.EnemySprite->CollideSize.y = CurrentEnemy.EnemySprite->Height / 1.1;


	// Adds the player and enemy to the collilde list
	AddCollidable(CurrentEnemy.EnemySprite);
	AddCollidable(CurrentPlayer.PlayerSprite);

	// Show debug box
	CurrentPlayer.PlayerSprite->CollideDebug = TRUE;

	// Reset camera to (0,0)
	ResetCamera();
}

/*************************************************************************/
/*!
	\brief
	Updates the test level
*/
/*************************************************************************/
void UpdateTestLevel(void)
{
	// Run the enemy logic and update the enemy
	EnemyLogic(&CurrentEnemy, &CurrentPlayer);
	UpdateEnemy(&CurrentEnemy);

	// Handle test level events
	EventTestLevel();

	// Update the player's position
	UpdatePlayerPosition(&CurrentPlayer);

	// Go back to main menu with ESC
	if(AEInputCheckTriggered(VK_ESCAPE))
	{
		SetNextState(GS_MainMenu);
	}
}

/*************************************************************************/
/*!
	\brief
	Draws the test level
*/
/*************************************************************************/
void DrawTestLevel(void)
{
	// Draws the objects in the object list
	drawObjectList();

	//Camera follows player
	SetCamera(&CurrentPlayer.Position, 350, &HUDList);
}

/*************************************************************************/
/*!
	\brief
	Frees all the objects in the test level
*/
/*************************************************************************/
void FreeTestLevel(void)
{
	freeObjectList();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the test level
*/
/*************************************************************************/
void UnloadTestLevel(void)
{
	//Placeholder
}

/*************************************************************************/
/*!
	\brief
	Handles all events for the test level
*/
/*************************************************************************/
void EventTestLevel(void)
{
	// Detect and handle collisions
	DetectCollision();

	// Handle any input for the player
	InputPlayer(&CurrentPlayer);

	// Handle item switching for the HUD
	if(AEInputCheckTriggered('Q'))
	{
		if (HUD1item->ItemType == 0)
		{
			HUD1item->SpriteTexture = AEGfxTextureLoad("TextureFiles/ThrowableHUD.png");
			HUD1item->ItemType = 1;
		}
		else
		{
			HUD1item->SpriteTexture = AEGfxTextureLoad("TextureFiles/HealthPotionHUD.png");
			HUD1item->ItemType = 0;
		}
	}
}
