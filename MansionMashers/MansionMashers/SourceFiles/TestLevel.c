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
Sprite *HUD1;
Sprite *HUD2;
Sprite *HUD3;
Sprite *HUD4;
Sprite *HUD1item;
Sprite *HUD2item;
Sprite *HUD3item;
Sprite *HUD4item;
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
	DetectCollision();

	InputPlayer(&CurrentPlayer);


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

void InitizalizeTestLevel(void)
{	
	Sprite *Hammy = CreateSprite("Hammy", "TextureFiles/Ham.png", 150.0f, 140.0f, 20, 1, 1);
	Sprite *Hammy2 = CreateSprite("Hammy2", "TextureFiles/Ham.png", 150.0f, 140.0f, 22, 1, 1);

	Hammy->SensorType = RectangleCollider;
	Hammy->Position.x = 400.0f;
	Hammy->SpriteType = FoodType;

	Hammy2->SensorType = RectangleCollider;
	Hammy2->Position.y = -100.0f;
	Hammy2->Position.x = -500.0f;
	Hammy2->SpriteType = FoodType;

	HUD1 = CreateSprite("HUD1", "TextureFiles/GinkoHUD.png", 320.0f, 137.0f, 200, 1, 1);
	HUD1->SensorType = RectangleCollider;
	HUD1->CanCollide = 0;
	HUD1->SpriteType = HudType;

	HUD2 = CreateSprite("HUD2", "TextureFiles/HollyHUD.png", 320.0f, 137.0f, 200, 1, 1);
	HUD2->SensorType = RectangleCollider;
	HUD2->CanCollide = 0;
	HUD2->SpriteType = HudType;
	
	HUD3 = CreateSprite("HUD3", "TextureFiles/MaypleHUD.png", 320.0f, 137.0f, 200, 1, 1);
	HUD3->SensorType = RectangleCollider;
	HUD3->CanCollide = 0;
	HUD3->SpriteType = HudType;

	HUD4 = CreateSprite("HUD4", "TextureFiles/KayaHUD.png", 320.0f, 137.0f, 200, 1, 1);
	HUD4->SensorType = RectangleCollider;
	HUD4->CanCollide = 0;
	HUD4->SpriteType = HudType;

	HUD1item = CreateSprite("HUDitem", "TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1);
	HUD1item->SensorType = RectangleCollider;
	HUD1item->CanCollide = 0;
	HUD1item->SpriteType = HudType;
	HUD1item->ItemType = 0;

	HUD2item = CreateSprite("HUDitem", "TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1);
	HUD2item->SensorType = RectangleCollider;
	HUD2item->CanCollide = 0;
	HUD2item->SpriteType = HudType;
	HUD2item->ItemType = 0;

	HUD3item = CreateSprite("HUDitem", "TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1);
	HUD3item->SensorType = RectangleCollider;
	HUD3item->CanCollide = 0;
	HUD3item->SpriteType = HudType;
	HUD3item->ItemType = 0;

	HUD4item = CreateSprite("HUDitem", "TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1);
	HUD4item->SensorType = RectangleCollider;
	HUD4item->CanCollide = 0;
	HUD4item->SpriteType = HudType;
	HUD4item->ItemType = 0;

	HUDList.HudItem[0] = HUD1;
	HUDList.HudItem[1] = HUD2;
	HUDList.HudItem[2] = HUD3;
	HUDList.HudItem[3] = HUD4;
	HUDList.HudItem[4] = HUD1item;
	HUDList.HudItem[5] = HUD2item;
	HUDList.HudItem[6] = HUD3item;
	HUDList.HudItem[7] = HUD4item;

	Background = CreateSprite("Background", "TextureFiles/LevelBackground.png", 3840.0f, 720.0f, 0, 1, 1);
	Background->CanCollide = 0;

	if(NULL != malloc(sizeof(Player)))
		InitializePlayer(&CurrentPlayer);

	if(NULL != malloc(sizeof(Enemy)))
		InitializeEnemy(&CurrentEnemy);

	AddCollidable(CurrentEnemy.EnemySprite);

	CurrentPlayer.PlayerSprite->SpriteType = PlayerType;
	AddCollidable(CurrentPlayer.PlayerSprite);

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
		//if (changeLevel != 0 || AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
			//LevelRunning = 0;
		if(AEInputCheckTriggered(VK_ESCAPE))
		{
			LevelRunning = 0;
			changeLevel = 1;
		}

	}

	FreeLevel();
	return changeLevel;
}

// ---------------------------------------------------------------------------
