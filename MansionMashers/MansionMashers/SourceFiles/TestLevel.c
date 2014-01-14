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
#include "../HeaderFiles/ObjectManager.h"

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals

// ---------------------------------------------------------------------------
// Static function protoypes
Sprite *Ham2;

// ---------------------------------------------------------------------------
// main
void MakeLevel(void)
{
	resetObjectList();
}

void DrawLevel(void)
{
	drawObjectList();
}

void FreeLevel(void)
{
	freeObjectList();
}

void InitizalizeTestLevel(void)
{
	Sprite *Hammy = CreateSprite(344.0f, 340.0f, 1, 1, "TextureFiles\\Ham.png");

	Ham2 = CreateSprite(344.0f, 340.0f, 1, 1, "TextureFiles\\Ham.png");

	Ham2->Position.x = -200.0f;
	Ham2->Position.y = -150.0f;
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
