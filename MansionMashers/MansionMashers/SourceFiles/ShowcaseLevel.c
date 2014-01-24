/*
File:				ShowcaseLevel.c
Author:				Dan Muller (d.muller)
Creation Date:		Jan 22, 2014

Purpose:			Showcase demo

Functions:			MenuLoop - Main loop for the main menu
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 



// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/LevelShowcase.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
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
Sprite *Shelf;
Sprite *Crate;

Player CurrentPlayer;
Enemy CurrentEnemy;

Sprite *Hammy;

void InitializeShowcase(void)
{
	Vec2 startingCamera;
	int hudLoop;
	MakeShowcase();

	for (hudLoop = 0; hudLoop < 20; hudLoop++)
		HUDList.HudItem[hudLoop] = 0;

	HUD = CreateSprite("HUD", "TextureFiles/MaypleHUD.png", 320.0f, 137.0f, 200, 1, 1);
	HUD->CanCollide = FALSE;
	HUD->SpriteType = HudType;

	HUDitem = CreateSprite("HUDitem", "TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1);
	HUDitem->CanCollide = FALSE;
	HUDitem->SpriteType = HudType;
	HUDitem->ItemType = 0;

	Background = CreateSprite("Background", "TextureFiles/LevelBackground.png", 3840.0f, 720.0f, 0, 1, 1);
	Background->CanCollide = FALSE;
	
	Background2 = CreateSprite("Background", "TextureFiles/LevelBackground.png", 3840.0f, 720.0f, 0, 1, 1);
	Background2->CanCollide = FALSE;
	Background2->Position.x = 3840;
	Background2->FlipX = TRUE;

	Shelf = CreateSprite("Shelf", "TextureFiles/Shelf.png", 123.0f, 245.5f, 8, 1, 1);
	Shelf->CanCollide = TRUE;
	Shelf->Position.x = 475;
	Shelf->Position.y = -115;
	Shelf->SpriteType = PlatformType;
	Shelf->CollideDebug = TRUE;
	Shelf->CollideSize.y = 60;
	Shelf->CollideOffset.y = Shelf->Height / 2 - 30;

	Crate = CreateSprite("Crate", "TextureFiles/Crate.png", 859.0f, 260.5f, 9, 1, 1);
	Crate->CanCollide = TRUE;
	Crate->Position.x = 1400;
	Crate->Position.y = -180;
	Crate->SpriteType = PlatformType;
	Crate->CollideDebug = FALSE;
	Crate->CollideSize.x = Crate->Width - 100;
	Crate->CollideSize.y = 60;
	Crate->CollideOffset.x = 0.0f;
	Crate->CollideOffset.y = Crate->Height / 2 - 60;

	Hammy = CreateSprite("Hammy", "TextureFiles/Ham.png", 150.0f, 140.0f, 20, 1, 1);
	Hammy->Position.x   = -1000.0f;
	Hammy->CanCollide = TRUE;
	Hammy->SensorType = RectangleCollider;
	Hammy->SpriteType = FoodType;
	Hammy->CollideDebug = TRUE;

	HUDList.HudItem[0] = HUD;
	HUDList.HudItem[1] = HUDitem;

	if(NULL != malloc(sizeof(Player)))
		InitializePlayer(&CurrentPlayer);

	CurrentPlayer.Position.x = -1380;
	CurrentPlayer.Position.y = -220;

	if(NULL != malloc(sizeof(Enemy)))
		InitializeEnemy(&CurrentEnemy);

	CurrentEnemy.EnemySprite->CollideDebug = FALSE;
	CurrentEnemy.Position.x = 600;
	CurrentEnemy.EnemySprite->CollideSize.x = CurrentEnemy.EnemySprite->Width  / 1.1;
	CurrentEnemy.EnemySprite->CollideSize.y = CurrentEnemy.EnemySprite->Height / 1.1;

	AddCollidable(CurrentEnemy.EnemySprite);
	AddCollidable(CurrentPlayer.PlayerSprite);

	//Player collision box changes (Feel free to mess with all this collision stuff)
	CurrentPlayer.PlayerSprite->CollideSize.x   = 2 * CurrentPlayer.PlayerSprite->Width  / 3;
	CurrentPlayer.PlayerSprite->CollideSize.y   = CurrentPlayer.PlayerSprite->Height / 2;
	CurrentPlayer.PlayerSprite->CollideOffset.x =  0.0f;
	CurrentPlayer.PlayerSprite->CollideOffset.y =  -20.0f;
	//Show debug box
	CurrentPlayer.PlayerSprite->CollideDebug = TRUE;

	
	Vec2Set(&startingCamera, -1280, 0.0f);
	SetCamera(&startingCamera, 250, &HUDList);
}

void FreeShowcase(void)
{
	freeObjectList();
}

void DrawShowcase(void)
{
	drawObjectList();
	DrawPlayer(&CurrentPlayer);
	DrawEnemy(&CurrentEnemy);
	SetCamera(&CurrentPlayer.Position, 250, &HUDList);
}

void MakeShowcase(void)
{
	resetObjectList();
}

void EventShowcase(void)
{
	CurrentPlayer.PlayerRigidBody.onGround = FALSE;
	DetectCollision();

	InputPlayer(&CurrentPlayer);


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

void UnloadShowcase(void)
{
	//Placeholder
}

void LoadShowcase(void)
{
	//Placeholder
}

void UpdateShowcase(void)
{
	if(CurrentPlayer.Position.x > 700)
		CurrentPlayer.PlayerSprite->CollideDebug = FALSE;
	else
		CurrentPlayer.PlayerSprite->CollideDebug = TRUE;
	EnemyLogic(&CurrentEnemy, &CurrentPlayer);
	EventShowcase();

	if(AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
	{
		SetNextState(GS_MainMenu);
	}
}
