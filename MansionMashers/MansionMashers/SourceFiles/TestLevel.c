/*
File:				TestLevel.c
Author:				Kaden Nugent (kaden.n)
Creation Date:		Jan 8, 2014

Purpose:			Level me a level

Functions:			LevelLoop - Main loop for this Level
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 



// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/TestLevel.h"
#include "../HeaderFiles/FoxEngine.h"

// ---------------------------------------------------------------------------
// Libraries

// ---------------------------------------------------------------------------
// globals

// ---------------------------------------------------------------------------
// Static function protoypes
Sprite *HUD;
Sprite *HUDitem;
Sprite *HUD2;
Sprite *HUD3;
Sprite *HUD4;
Sprite *Background;
Player CurrentPlayer;
Enemy CurrentEnemy;
HUDLayer HUDList;

// ---------------------------------------------------------------------------
// main
void MakeLevel(void)
{
	resetObjectList();
}

void DrawLevel(void)
{
	drawObjectList();
	DrawPlayer(&CurrentPlayer);
	DrawEnemy(&CurrentEnemy);

	//Camera follows player
	SetCamera(&CurrentPlayer.Position, 350, &HUDList);
}

void FreeLevel(void)
{
	freeObjectList();
}

void EventLevel(void)
{
	detectCollision();

	InputPlayer(&CurrentPlayer);


	if(AEInputCheckTriggered('Q'))
	{
		if (HUDitem->ItemType == 0)
		{
			HUDitem->SpriteTexture = AEGfxTextureLoad("TextureFiles\\ThrowableHUD.png");
			HUDitem->ItemType = 1;
		}
		else
		{
			HUDitem->SpriteTexture = AEGfxTextureLoad("TextureFiles\\HealthPotionHUD.png");
			HUDitem->ItemType = 0;
		}
	}
}

void InitizalizeTestLevel(void)
{	
	Sprite *Hammy = CreateSprite("Hammy", "TextureFiles\\Ham.png", 150.0f, 140.0f, 20, 1, 1);
	Sprite *Hammy2 = CreateSprite("Hammy2", "TextureFiles\\Ham.png", 150.0f, 140.0f, 22, 1, 1);

	Hammy->SensorType = RectangleCollider;
	Hammy->Position.x = 400.0f;
	Hammy->SpriteType = FoodType;

	Hammy2->SensorType = RectangleCollider;
	Hammy2->Position.y = -100.0f;
	Hammy2->Position.x = -500.0f;
	Hammy2->SpriteType = FoodType;

	HUD = CreateSprite("HUD", "TextureFiles\\PlayerHUD.png", 330.0f, 140.0f, 200, 1, 1);
	HUD->SensorType = RectangleCollider;
	HUD->CanCollide = 0;
	HUD->SpriteType = HudType;

	HUDitem = CreateSprite("HUDitem", "TextureFiles\\HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1);
	HUDitem->SensorType = RectangleCollider;
	HUDitem->CanCollide = 0;
	HUDitem->SpriteType = HudType;
	HUDitem->ItemType = 0;

	HUD2 = CreateSprite("HUD2", "TextureFiles\\PlayerHUD.png", 330.0f, 140.0f, 200, 1, 1);
	HUD2->SensorType = RectangleCollider;
	HUD2->CanCollide = 0;
	HUD2->SpriteType = HudType;
	
	HUD3 = CreateSprite("HUD3", "TextureFiles\\PlayerHUD.png", 330.0f, 140.0f, 200, 1, 1);
	HUD3->SensorType = RectangleCollider;
	HUD3->CanCollide = 0;
	HUD3->SpriteType = HudType;

	HUD4 = CreateSprite("HUD4", "TextureFiles\\PlayerHUD.png", 330.0f, 140.0f, 200, 1, 1);
	HUD4->SensorType = RectangleCollider;
	HUD4->CanCollide = 0;
	HUD4->SpriteType = HudType;

	HUDList.HudItem[0] = HUD;
	HUDList.HudItem[1] = HUD2;
	HUDList.HudItem[2] = HUD3;
	HUDList.HudItem[3] = HUD4;
	HUDList.HudItem[4] = HUDitem;

	Background = CreateSprite("Background", "TextureFiles\\LevelBackground.png", 3840.0f, 720.0f, 0, 1, 1);
	Background->CanCollide = 0;

	HUD->Position.x = -504.0f;
	HUD->Position.y = 250.0f;

	HUD2->Position.x = 504.0f;
	HUD2->Position.y = 250.0f;

	if(NULL != malloc(sizeof(Player)))
		InitializePlayer(&CurrentPlayer);

	if(NULL != malloc(sizeof(Enemy)))
		InitializeEnemy(&CurrentEnemy);

	CurrentPlayer.PlayerSprite->SpriteType = PlayerType;

	ResetCamera();
}

int LevelLoop(void)
{
	int changeLevel  = 0;
	int LevelRunning = 1;

	MakeLevel();
	InitizalizeTestLevel();

	printf("Running Level\n");

	while (LevelRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		// Functions
		EnemyLogic(&CurrentEnemy, &CurrentPlayer);
		DrawLevel();
		EventLevel();

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (changeLevel != 0 || AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
			LevelRunning = 0;
	}

	FreeLevel();
	return changeLevel;
}

// ---------------------------------------------------------------------------
