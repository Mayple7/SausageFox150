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
#include "../HeaderFiles/ObjectManager.h"

// ---------------------------------------------------------------------------

// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals

Sprite* Ham;
Sprite* Bektor;
Sprite* StartButton;
Sprite* ExitButton;
Sprite* Selector;
Sprite* AnimationTest;

struct Player Player;

int numMenuButtons = 2;
int selectedButton = 0;								//0: start, 1: exit

float animationSpeed = 60.0f / 16.0f;
int animationTimer = 0;

// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------

void InitizalizeMainMenu(void)
{
	resetObjectList();

	if(NULL != malloc(sizeof(Sprite)))
		StartButton = CreateSprite(480.0f, 180.0f, 1, 1, "TextureFiles/StartButton.png");
	StartButton->XPosition = 0.0f;
	StartButton->YPosition = 100.0f;

	if(NULL != malloc(sizeof(Sprite)))
		ExitButton = CreateSprite(480.0f, 180.0f, 1, 1, "TextureFiles/ExitButton.png");
	ExitButton->XPosition = 0.0f;
	ExitButton->YPosition = -100.0f;

	if(NULL != malloc(sizeof(Sprite)))
		Selector = CreateSprite(500.0f, 200.0f, 1, 1, "TextureFiles/Selector.png");
	Selector->XPosition = 100.0f;
	Selector->YPosition = 0.0f;
	
	if(NULL != malloc(sizeof(Sprite)))
		Ham = CreateSprite(344.0f, 340.0f, 1, 1, "TextureFiles/Ham.png");

	if(NULL != malloc(sizeof(Sprite)))
		Bektor = CreateSprite(150.0f, 150.0f, 4, 1, "TextureFiles/Bektor.png");

	if(NULL != malloc(sizeof(Sprite)))
		AnimationTest = CreateSprite(300.0f, 300.0f, 3, 3, "TextureFiles/AnimationTest.png");
	AnimationTest->XPosition = -400.0f;
	AnimationTest->YPosition = 300.0f;

	AnimationTest->AnimationActive = 1;
	AnimationTest->AnimationSpeed = 60;

	Ham->XPosition = 100.0f;
	Ham->YPosition = 200.0f;

	Bektor->XPosition = -300.0f;
	Bektor->YPosition = -300.0f;

	Bektor->AnimationSpeed = 12;
	Bektor->AnimationActive = 1;

	if(NULL != malloc(sizeof(Player)))
		InitializePlayer(&Player);

	UpdateSelector(Selector);
}

void DrawMenu(void)
{
	//Camera follows ham
	//SF_SetCamera(Ham->Position, 350, 3.0f);

	DrawSprite(Selector);
	DrawSprite(StartButton);
	DrawSprite(ExitButton);
	DrawSprite(Ham);
	DrawSprite(Bektor);
	DrawPlayer(&Player);

	DrawSprite(AnimationTest);
}

		
void FreeMainMenu(void)
{
	// Freeing the objects and textures
	freeObjectList();
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

		UpdateSelector(Selector);
	}
	else if(AEInputCheckTriggered(VK_DOWN))
	{
		if(selectedButton == numMenuButtons - 1)
			selectedButton = 0;
		else
			selectedButton++;

		UpdateSelector(Selector);
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
	JG_move('W', &Ham->XPosition, &Ham->YPosition, 3.0f,  UP);
	JG_move('S', &Ham->XPosition, &Ham->YPosition, 3.0f, DOWN);
	JG_move('A', &Ham->XPosition, &Ham->YPosition, 3.0f, LEFT);
	JG_move('D', &Ham->XPosition, &Ham->YPosition, 3.0f, RIGHT);

	return 0;
}

void UpdateSelector(struct Sprite *Selector)
{
	switch(selectedButton)
	{
		case 0:
			Selector->XPosition = 0.0f;//StartButton.XPosition;
			Selector->YPosition = 100.0f;//StartButton.YPosition;
			break;
		case 1:
			Selector->XPosition = 0.0f;//ExitButton.XPosition;
			Selector->YPosition = -100.0f;//ExitButton.YPosition;
			break;
		default:
			Selector->XPosition = 0.0f;//StartButton.XPosition;
			Selector->YPosition = 100.0f;//StartButton.YPosition;
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
