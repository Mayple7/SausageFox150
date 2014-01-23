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
HUDLayer HUDList;
Sprite *Background;
Sprite *Background2;

Sprite *Shelf;

Player CurrentPlayer;
Enemy CurrentEnemy;

Sprite *Hammy;

void InitializeShowcase(void)
{
	MakeShowcase();

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
	Shelf->Position.y = -110;
	Shelf->SpriteType = PlatformType;
	Shelf->CollideDebug = TRUE;
	Shelf->CollideSize.y = 80;
	Shelf->CollideOffset.y = Shelf->Height / 2 - 40;

	Hammy = CreateSprite("Hammy", "TextureFiles/Ham.png", 150.0f, 140.0f, 20, 1, 1);
	Hammy->Position.x   = -400.0f;
	Hammy->CanCollide = TRUE;
	Hammy->SensorType = RectangleCollider;
	Hammy->SpriteType = FoodType;
	Hammy->CollideDebug = TRUE;

	HUDList.HudItem[0] = HUD;
	HUDList.HudItem[4] = HUDitem;

	if(NULL != malloc(sizeof(Player)))
		InitializePlayer(&CurrentPlayer);

	CurrentPlayer.Position.x = -1280;
	CurrentPlayer.Position.y = -220;

	if(NULL != malloc(sizeof(Enemy)))
		InitializeEnemy(&CurrentEnemy);

	CurrentEnemy.EnemySprite->CollideDebug = TRUE;
	CurrentEnemy.EnemySprite->CollideSize.x = CurrentEnemy.EnemySprite->Width  / 1.1;
	CurrentEnemy.EnemySprite->CollideSize.y = CurrentEnemy.EnemySprite->Height / 1.1;

	AddCollidable(CurrentEnemy.EnemySprite);
	AddCollidable(CurrentPlayer.PlayerSprite);

	//Player collision box changes (Feel free to mess with all this collision stuff)
	CurrentPlayer.PlayerSprite->CollideSize.x   = 2 * CurrentPlayer.PlayerSprite->Width  / 3;
	CurrentPlayer.PlayerSprite->CollideSize.y   = CurrentPlayer.PlayerSprite->Height / 2;
	CurrentPlayer.PlayerSprite->CollideOffset.x =  0.0f;
	CurrentPlayer.PlayerSprite->CollideOffset.y =  0.0f;
	//Show debug box
	CurrentPlayer.PlayerSprite->CollideDebug = 1;

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
	SetCamera(&CurrentPlayer.Position, 350, &HUDList);
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

int InputHandling(void);

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
	EnemyLogic(&CurrentEnemy, &CurrentPlayer);
	EventShowcase();

	if(AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
	{
		SetNextState(GS_MainMenu);
	}
}
