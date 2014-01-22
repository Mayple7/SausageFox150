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
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"

// ---------------------------------------------------------------------------

// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals

Sprite* StartButton;
Sprite* ExitButton;
Sprite* Selector;

Sprite* HUD2;
Sprite* HUD;

HUDLayer HUDList;

int numMenuButtons = 2;
int selectedButton = 0;								//0: start, 1: exit

// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------

void InitizalizeMainMenu(void)
{
	resetObjectList();

	StartButton = CreateSprite("Start Button", "TextureFiles/StartButton.png", 480.0f, 180.0f, 1, 1, 1);
	StartButton->Position.x = 0.0f;
	StartButton->Position.y = 100.0f;

	ExitButton = CreateSprite("Exit Button", "TextureFiles/ExitButton.png", 480.0f, 180.0f, 1, 1, 1);
	ExitButton->Position.x = 0.0f;
	ExitButton->Position.y = -100.0f;

	Selector = CreateSprite("Selector", "TextureFiles/Selector.png", 500.0f, 200.0f, 0, 1, 1);
	Selector->Position.x = 100.0f;
	Selector->Position.y = 0.0f;
	
	ResetCamera();
	UpdateSelector(Selector);
}

void DrawMainMenu(void)
{
	drawObjectList();
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
	if (AEInputCheckTriggered(VK_UP) || AEInputCheckTriggered('W'))
	{
		if(selectedButton == 0)
			selectedButton = numMenuButtons - 1;
		else
			selectedButton--;

		UpdateSelector(Selector);
	}
	else if(AEInputCheckTriggered(VK_DOWN) || AEInputCheckTriggered('S'))
	{
		if(selectedButton == numMenuButtons - 1)
			selectedButton = 0;
		else
			selectedButton++;

		UpdateSelector(Selector);
	}
	else if(AEInputCheckTriggered(VK_RETURN) || AEInputCheckTriggered(VK_SPACE))
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
		DrawMainMenu();

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
