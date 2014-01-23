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

Player CurrentPlayer;

void InitizalizeShowcase(void)
{
	MakeShowcase();

	HUD = CreateSprite("HUD", "TextureFiles/MaypleHUD.png", 320.0f, 137.0f, 200, 1, 1);
	HUD->CanCollide = 0;
	HUD->SpriteType = HudType;

	HUDitem = CreateSprite("HUDitem", "TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1);
	HUDitem->CanCollide = 0;
	HUDitem->SpriteType = HudType;
	HUDitem->ItemType = 0;

	Background = CreateSprite("Background", "TextureFiles/LevelBackground.png", 3840.0f, 720.0f, 0, 1, 1);
	Background->CanCollide = 0;

	HUDList.HudItem[0] = HUD;
	HUDList.HudItem[4] = HUDitem;

	if(NULL != malloc(sizeof(Player)))
		InitializePlayer(&CurrentPlayer);
	//CurrentPlayer.Position.x = -1280;
	//CurrentPlayer.Position.y = -225;

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
	SetCamera(&CurrentPlayer.Position, 350, &HUDList);
}

void MakeShowcase(void)
{
	resetObjectList();
}

void EventShowcase(void)
{
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
	EventShowcase();

	if(AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
	{
		SetNextState(GS_MainMenu);
	}
}
