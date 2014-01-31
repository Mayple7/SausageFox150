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
#include "../HeaderFiles/FoxObjects.h"

// ---------------------------------------------------------------------------
// Globals
int newID;					// ID number

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
Enemy *CurrentEnemy;
HUDLayer HUDList;

Sprite *LegUpper;
Sprite *LegLower;
Sprite *LegUpper2;
Sprite *LegLower2;
Sprite *Body;
Sprite *ArmUpper;
Sprite *ArmLower;
Sprite *ArmUpper2;
Sprite *ArmLower2;
Sprite *Weapon;

Food *Hammy;
Sprite *Hammy2;

/*************************************************************************/
/*!
	\brief
	Loads the assets for the test level
*/
/*************************************************************************/
void LoadTestLevel(void)
{
	//Allocate space for a large texture
	CreateTextureList();
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

	newID = 1;
	// Reset the object list
	resetObjectList();

	// Set the HUDlist to 0
	for (hudLoop = 0; hudLoop < 20; hudLoop++)
		HUDList.HudItem[hudLoop] = 0;

	// Create the HAMs
	Hammy = CreateFood("TextureFiles/Ham.png", FoodType, 150, 140, newID++);

	// Collision for the first HAM
	UpdateFoodPosition(Hammy, 400, 0);
	 
	// Create the legs of the player

	ArmUpper2 = CreateSprite("TextureFiles/ArmUpper.png", 128.0f, 128.0f, 22, 1, 1);

	ArmLower2 = CreateSprite("TextureFiles/ArmLower.png", 128.0f, 128.0f, 22, 1, 1);

	LegUpper = CreateSprite("TextureFiles/LegUpper.png", 128.0f, 128.0f, 22, 1, 1);

	LegLower = CreateSprite("TextureFiles/LegLower.png", 128.0f, 128.0f, 22, 1, 1);

	LegUpper2 = CreateSprite("TextureFiles/LegUpper.png", 128.0f, 128.0f, 22, 1, 1);

	LegLower2 = CreateSprite("TextureFiles/LegLower.png", 128.0f, 128.0f, 22, 1, 1);

	Body = CreateSprite("TextureFiles/Body.png", 300.0f, 300.0f, 22, 1, 1);

	Weapon = CreateSprite("TextureFiles/Hammer.png", 128.0f, 128.0f, 22, 1, 1);

	ArmUpper = CreateSprite("TextureFiles/ArmUpper.png", 128.0f, 128.0f, 22, 1, 1);

	ArmLower = CreateSprite("TextureFiles/ArmLower.png", 128.0f, 128.0f, 22, 1, 1);



	// Creating the HUD items
	HUD1 = CreateSprite("TextureFiles/GinkoHUD.png", 320.0f, 137.0f, 200, 1, 1);
	HUD2 = CreateSprite("TextureFiles/HollyHUD.png", 320.0f, 137.0f, 200, 1, 1);
	HUD3 = CreateSprite("TextureFiles/MaypleHUD.png", 320.0f, 137.0f, 200, 1, 1);
	HUD4 = CreateSprite("TextureFiles/KayaHUD.png", 320.0f, 137.0f, 200, 1, 1);

	HUD1item = CreateSprite("TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1);
	HUD1item->ItemType = 0;

	HUD2item = CreateSprite("TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1);
	HUD2item->ItemType = 0;

	HUD3item = CreateSprite("TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1);
	HUD3item->ItemType = 0;

	HUD4item = CreateSprite("TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1);
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
	Background = CreateSprite("TextureFiles/FoxMansion1.png", 1920, 1080, 0, 1, 1);

	// Secret invisible collider GO!
	ColliderInvisible = CreateSprite("TextureFiles/LevelGrassground.png", 300.0f, 80.0f, 0, 1, 1);
	ColliderInvisible->Visible = FALSE;
	ColliderInvisible->Position.x = -1020.0f;
	ColliderInvisible->Position.y = -40.0f;

	// Creates the player
	if(NULL != malloc(sizeof(Player)))
		InitializePlayer(&CurrentPlayer, newID++);

	// Creates the enemy
	CurrentEnemy = CreateEnemy("TextureFiles/EasyEnemy.png", EnemyType, 150, 150, newID++);

	// Adds the player and enemy to the collilde list
	AddCollidable(CurrentPlayer.PlayerSprite);

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
	DetectPlayerCollision();
	EnemyLogic(CurrentEnemy, &CurrentPlayer);
	UpdateEnemy(CurrentEnemy);

	// Handle test level events
	EventTestLevel();

	// Update the player's position
	UpdatePlayerPosition(&CurrentPlayer);

	// Update/Draw them legs
	Animation(&CurrentPlayer, LegUpper, LegUpper2, LegLower, LegLower2, Body, ArmUpper, ArmLower, ArmUpper2, ArmLower2, Weapon);

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
	//Destroy the textures
	DestroyTextureList();
}

/*************************************************************************/
/*!
	\brief
	Handles all events for the test level
*/
/*************************************************************************/
void EventTestLevel(void)
{
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

	// check if forcing the application to quit
	if (0 == AESysDoesWindowExist())
		SetNextState(GS_Quit);
}
