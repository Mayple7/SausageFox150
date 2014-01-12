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

// ---------------------------------------------------------------------------

// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals

AEGfxVertexList*	meshtest;					// Selector Mesh

AEGfxTexture *testTexture;						// Pointer to selector texture

float *testOffsetX;
float *testOffsetY;

int testCurrentFrame = 0;
int testTotalFrames = 8;

float testAnimationSpeed = 60.0f / 16.0f;
int testAnimationTimer = 0;

// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------

void InitizalizeTestLevel(void)
{
	float testFirstOffsetX = 0.0f;
	float testFirstOffsetY = 0.0f;

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// 1 triangle at a time
	// X, Y, Color, texU, texV
	AEGfxTriAdd(
		-125.0f, -75.0f, 0x00FFFFFF, 0.0f, 0.5f, 
		125.0f,  -75.0f, 0x00FFFFFF, 0.25f, 0.5f,
		-125.0f,  75.0f, 0x00FFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		125.0f, -75.0f, 0x00FFFFFF, 0.25f, 0.5f, 
		125.0f,  75.0f, 0x00FFFFFF, 0.25f, 0.0f,
		-125.0f,  75.0f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1

	meshtest = AEGfxMeshEnd();
	AE_ASSERT_MESG(meshtest, "Failed to create selector!!");

	testTexture = AEGfxTextureLoad("Textures\\SausageFox.png");
	AE_ASSERT_MESG(testTexture, "Failed to create test!!");


	testOffsetX = &testFirstOffsetX;
	testOffsetY = &testFirstOffsetY;
}

void DrawLevel(void)
{
	if(testAnimationTimer >= testAnimationSpeed)
	{
		testCurrentFrame = UpdateFrame(testTotalFrames, testCurrentFrame, 1, testOffsetX, testOffsetY);
		testAnimationTimer = 0;
	}
	else
	{
		testCurrentFrame = UpdateFrame(testTotalFrames, testCurrentFrame, 0, testOffsetX, testOffsetY);
		testAnimationTimer++;
	}
	printf("X: %f, Y: %f\n", *testOffsetX, *testOffsetY);
	// Drawing Exit Button
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set poisition for object 2
	AEGfxSetPosition(0.0f, 0.0f);
	// Drawing the mesh (list of triangles)
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(testTexture, *testOffsetX, *testOffsetY);
	AEGfxMeshDraw(meshtest, AE_GFX_MDM_TRIANGLES);
}

void FreeLevel(void)
{
	// Freeing the objects and textures
	AEGfxMeshFree(meshtest);

	AEGfxTextureUnload(testTexture);
}

// main

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
