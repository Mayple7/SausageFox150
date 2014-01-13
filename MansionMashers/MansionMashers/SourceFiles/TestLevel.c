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
Sprite *Ham2;
Sprite *objectList;

// ---------------------------------------------------------------------------
// Static function protoypes


// ---------------------------------------------------------------------------
// main
void InitizalizeTestLevel(void)
{
	objectList = (Sprite *)malloc(objectAmount * sizeof(Sprite));
	if (objectList)
	{
		int i;
		for (i = 0; i < objectAmount; i++)
		{
			printf("%i\n", sizeof(objectList[i]));
		}
	}
	Ham2 = objectList;

	if(NULL != malloc(sizeof(Sprite)))
		CreateSprite(Ham2, 344.0f, 340.0f, 1, 1, "TextureFiles\\Ham.png");

	Ham2->XPosition = -100.0f;
	Ham2->YPosition = -150.0f;
}

void DrawLevel(void)
{
	int i;
	for (i = 0; i < objectAmount; i++)
	{
		Sprite* objectNext = (objectList + i);
		if (objectNext && objectNext->Created && objectNext->Created == 1)
			DrawSprite(objectNext);
	}
}

void FreeLevel(void)
{
	// Freeing the objects and textures
	int i;
	for (i = 0; i < objectAmount; i++)
	{
		Sprite* objectNext = (objectList + i);
		if (objectNext && objectNext->Created && objectNext->Created == 1)
			free(objectNext);
	}
}

int LevelLoop(void)
{
	int changeLevel  = 0;
	int LevelRunning = 1;

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
