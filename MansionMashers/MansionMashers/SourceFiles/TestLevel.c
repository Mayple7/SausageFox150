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
#include "../HeaderFiles/Sprite.h"
#include "../HeaderFiles/CollisionManager.h"
#include "../HeaderFiles/ObjectManager.h"
#include "../HeaderFiles/Player.h"
#include "../HeaderFiles/Movement.h"
#include "../HeaderFiles/Camera.h"

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals

// ---------------------------------------------------------------------------
// Static function protoypes
Sprite *Ham2;
Sprite *Background;
Sprite *Hammy;

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
}

void FreeLevel(void)
{
	freeObjectList();
}

void EventLevel(void)
{
	detectCollision();

	//Moving the player
	InputPlayer(&CurrentPlayer, 'W');
	InputPlayer(&CurrentPlayer, 'A');
	InputPlayer(&CurrentPlayer, 'S');
	InputPlayer(&CurrentPlayer, 'D');
}

void InitizalizeTestLevel(void)
{
	Background = CreateSprite(3840.0f, 720.0f, 1, 1, "TextureFiles\\LevelBackground.png");
	Background->SensorType = RectangleCollider;

	Hammy = CreateSprite(150.0f, 140.0f, 1, 1, "TextureFiles\\Ham.png");
	Hammy->SensorType = RectangleCollider;
	Hammy->ZIndex = 5;

	Ham2 = CreateSprite(224.0f, 96.0f, 1, 1, "TextureFiles\\PlayerHUD.png");
	Ham2->SensorType = RectangleCollider;

	Ham2->Position.x = -400.0f;
	Ham2->Position.y = 300.0f;

	if(NULL != malloc(sizeof(Player)))
		InitializePlayer(&CurrentPlayer);

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
