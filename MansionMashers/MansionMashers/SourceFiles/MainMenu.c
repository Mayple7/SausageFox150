/*
File:				MainMenu.c
Author:				Dan Muller (d.muller)
Creation Date:		Jan 8, 2014

Purpose:			MainMenu

Functions:			MenuLoop - Main loop for the main menu
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 



// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/MainMenu.h"
#include "../HeaderFiles/Sprite.h"
#include "../HeaderFiles/Player.h"
#include "../HeaderFiles/Movement.h"
#include "../HeaderFiles/Camera.h"

// ---------------------------------------------------------------------------

// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals

AEGfxVertexList*	meshStartButton;				// Pointer to Mesh (Model)
AEGfxVertexList*	meshExitButton;					// Pointer to Digipen Logo Mesh
AEGfxVertexList*	meshSelector;					// Selector Mesh
AEGfxVertexList*	meshFox;					// Selector Mesh

AEGfxTexture *startButtonTexture;					// Pointer to Texture (Image)
AEGfxTexture *exitButtonTexture;					// Pointer to Digipen logo texture
AEGfxTexture *selectorTexture;						// Pointer to selector texture
AEGfxTexture *foxTexture;						// Pointer to selector texture

Sprite Ham;
Sprite Bektor;

struct Player Player;

float *foxOffsetX;
float *foxOffsetY;

int foxCurrentFrame = 0;
int foxTotalFrames = 8;

int numMenuButtons = 2;
int selectedButton = 0;								//0: start, 1: exit

float startButtonx = 0;
float startButtony = 100;

float exitButtonx = 0;
float exitButtony = -100;

float selectorX = 0;
float selectorY = 0;

float animationSpeed = 60.0f / 16.0f;
int animationTimer = 0;

// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------

void InitizalizeMainMenu(void)
{
	meshStartButton = CreateSpriteTexture(480.0f, 180.0f, 1.0f, 1.0f);
	AE_ASSERT_MESG(meshStartButton, "Failed to create start button!!");

	// Texture 1: From file
	startButtonTexture = AEGfxTextureLoad("Textures\\StartButton.png");
	AE_ASSERT_MESG(startButtonTexture, "Failed to create Start Button Texture!!");

	meshExitButton = CreateSpriteTexture(480.0f, 180.0f, 1.0f, 1.0f);
	AE_ASSERT_MESG(meshExitButton, "Failed to create start button!!");

	// Texture 1: From file
	exitButtonTexture = AEGfxTextureLoad("Textures\\ExitButton.png");
	AE_ASSERT_MESG(exitButtonTexture, "Failed to create Exit Button Texture!!");

	meshSelector = CreateSpriteTexture(500.0f, 200.0f, 1.0f, 1.0f);
	AE_ASSERT_MESG(meshSelector, "Failed to create selector!!");

	// Texture 1: From file
	selectorTexture = AEGfxTextureLoad("Textures\\Selector.png");
	AE_ASSERT_MESG(selectorTexture, "Failed to create Start Button Texture!!");

	if(NULL != malloc(sizeof(struct Sprite)))
		CreateSprite(&Ham, 344.0f, 340.0f, 1, 1, "TextureFiles\\Ham.png");

	if(NULL != malloc(sizeof(struct Sprite)))
		CreateSprite(&Bektor, 150.0f, 150.0f, 4, 1, "TextureFiles\\Bektor.png");

	Ham.XPosition = 100.0f;
	Ham.YPosition = 200.0f;

	Bektor.XPosition = -300.0f;
	Bektor.YPosition = -300.0f;

	Bektor.TotalFrames = 4;
	Bektor.AnimationSpeed = 12;
	Bektor.AnimationActive = 1;

	if(NULL != malloc(sizeof(struct Player)))
		InitializePlayer(&Player);

	UpdateSelector();
}

void DrawMenu(void)
{
	// Drawing Selector
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set poisition for object 2
	AEGfxSetPosition(selectorX, selectorY);
	// Drawing the mesh (list of triangles)
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(selectorTexture, 0.0f, 0.0f);
	AEGfxMeshDraw(meshSelector, AE_GFX_MDM_TRIANGLES);

	// Drawing Start Button
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set poisition for object 1
	AEGfxSetPosition(startButtonx, startButtony);
	// Drawing the mesh (list of triangles)
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(startButtonTexture, 0.0f, 0.0f);
	AEGfxMeshDraw(meshStartButton, AE_GFX_MDM_TRIANGLES);

	// Drawing Exit Button
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set poisition for object 2
	AEGfxSetPosition(exitButtonx, exitButtony);
	// Drawing the mesh (list of triangles)
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(exitButtonTexture, 0.0f, 0.0f);
	AEGfxMeshDraw(meshExitButton, AE_GFX_MDM_TRIANGLES);

	//Camera follows ham
	SF_SetCamera(Ham.XPosition, 350, 3.0f);

	DrawSprite(&Ham);
	DrawSprite(&Bektor);
	DrawPlayer(&Player);
}

		
void FreeMainMenu(void)
{
	// Freeing the objects and textures
	AEGfxMeshFree(meshStartButton);
	AEGfxMeshFree(meshExitButton);
	
	AEGfxTextureUnload(startButtonTexture);
	AEGfxTextureUnload(exitButtonTexture);
}

//INPUT, PRESS DEM KEYS BOI
int InputHandling(void)
{
	// check if forcing the application to quit
	if (AEInputCheckTriggered(VK_UP))
	{
		if(selectedButton == 0)
			selectedButton = numMenuButtons - 1;
		else
			selectedButton--;

		UpdateSelector();
	}
	else if(AEInputCheckTriggered(VK_DOWN))
	{
		if(selectedButton == numMenuButtons - 1)
			selectedButton = 0;
		else
			selectedButton++;

		UpdateSelector();
	}
	else if(AEInputCheckTriggered(VK_RETURN))
	{
		printf("%i button: %i\n", numMenuButtons - 1, selectedButton);

		// Play le gaem
		if(selectedButton == 0)
			return 2;
		// Exit button
		else if(selectedButton == 1)
			return -1;
	}

	// check if forcing the application to quit
	if (AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
		return -1;

	//Moving the Ham with WASD
	JG_move('W', &Ham.XPosition, &Ham.YPosition, 3.0f,  UP);
	JG_move('S', &Ham.XPosition, &Ham.YPosition, 3.0f, DOWN);
	JG_move('A', &Ham.XPosition, &Ham.YPosition, 3.0f, LEFT);
	JG_move('D', &Ham.XPosition, &Ham.YPosition, 3.0f, RIGHT);

	return 0;
}

void UpdateSelector(void)
{
	switch(selectedButton)
	{
		case 0:
			selectorX = startButtonx;
			selectorY = startButtony;
			break;
		case 1:
			selectorX = exitButtonx;
			selectorY = exitButtony;
			break;
		default:
			selectorX = startButtonx;
			selectorY = startButtony;
			break;
	}
}

// main

int MenuLoop(void)
{
	int changeLevel  = 0;
	int LevelRunning = 1;

	InitizalizeMainMenu();

	printf("Running Menu\n");

	while (LevelRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();
		// Functions
		changeLevel = InputHandling();
		DrawMenu();

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (changeLevel != 0 || AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
			LevelRunning = 0;
	}

	FreeMainMenu();
	return changeLevel;
}



// ---------------------------------------------------------------------------
