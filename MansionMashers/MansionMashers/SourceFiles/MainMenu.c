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

Player CurrentPlayer;

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

	StartButton = CreateSprite(480.0f, 180.0f, 1, 1, "TextureFiles/StartButton.png");
	StartButton->Position.x = 0.0f;
	StartButton->Position.y = 100.0f;

	ExitButton = CreateSprite(480.0f, 180.0f, 1, 1, "TextureFiles/ExitButton.png");
	ExitButton->Position.x = 0.0f;
	ExitButton->Position.y = -100.0f;

	Selector = CreateSprite(500.0f, 200.0f, 1, 1, "TextureFiles/Selector.png");
	Selector->Position.x = 100.0f;
	Selector->Position.y = 0.0f;
	
	Ham = CreateSprite(344.0f, 340.0f, 1, 1, "TextureFiles/Ham.png");

	Bektor = CreateSprite(150.0f, 150.0f, 4, 1, "TextureFiles/Bektor.png");

	AnimationTest = CreateSprite(300.0f, 300.0f, 3, 3, "TextureFiles/AnimationTest.png");
	AnimationTest->Position.x = -400.0f;
	AnimationTest->Position.y = 300.0f;

	AnimationTest->AnimationActive = 1;
	AnimationTest->AnimationSpeed = 60;

	Ham->Position.x = 100.0f;
	Ham->Position.y = 200.0f;

	Bektor->Position.x = -300.0f;
	Bektor->Position.y = -300.0f;

	Bektor->AnimationSpeed = 12;
	Bektor->AnimationActive = 1;

	if(NULL != malloc(sizeof(Player)))
		InitializePlayer(&CurrentPlayer);

	UpdateSelector(Selector);
}

void DrawMenu(void)
{
	//Camera follows ham
	SF_SetCamera((&CurrentPlayer)->PlayerSprite.Position.x, 350, 3.0f);

	DrawSprite(Selector);
	DrawSprite(StartButton);
	DrawSprite(ExitButton);
	DrawSprite(Ham);
	DrawSprite(Bektor);
	DrawPlayer(&CurrentPlayer);

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
	
	//Moving the player
	Input_Player(&Player, 'W');
	Input_Player(&Player, 'A');
	Input_Player(&Player, 'S');
	Input_Player(&Player, 'D');

	return 0;
}

void UpdateSelector(struct Sprite *Selector)
{
	switch(selectedButton)
	{
		case 0:
			Selector->Position.x = 0.0f;//StartButton.Position.x;
			Selector->Position.y = 100.0f;//StartButton.Position.y;
			break;
		case 1:
			Selector->Position.x = 0.0f;//ExitButton.Position.x;
			Selector->Position.y = -100.0f;//ExitButton.Position.y;
			break;
		default:
			Selector->Position.x = 0.0f;//StartButton.Position.x;
			Selector->Position.y = 100.0f;//StartButton.Position.y;
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
