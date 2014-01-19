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

Sprite* Ham;
Sprite* Fox;
Sprite* StartButton;
Sprite* ExitButton;
Sprite* Selector;
Sprite* AnimationTest;

Player CurrentPlayer;

Sprite* HUD2;
Sprite* HUD;

HUDLayer HUDList;

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

	StartButton = CreateSprite("Start Button", "TextureFiles/StartButton.png", 480.0f, 180.0f, 1, 1, 1);
	StartButton->Position.x = 0.0f;
	StartButton->Position.y = 100.0f;

	ExitButton = CreateSprite("Exit Button", "TextureFiles/ExitButton.png", 480.0f, 180.0f, 1, 1, 1);
	ExitButton->Position.x = 0.0f;
	ExitButton->Position.y = -100.0f;

	Selector = CreateSprite("Selector", "TextureFiles/Selector.png", 500.0f, 200.0f, 0, 1, 1);
	Selector->Position.x = 100.0f;
	Selector->Position.y = 0.0f;
	
	Ham = CreateSprite("Ham", "TextureFiles/Ham.png", 344.0f, 340.0f, 65454, 1, 1);

	Fox = CreateSprite("Fox", "TextureFiles/SausageFox.png", 250.0f, 150.0f, 5, 4, 2);

	AnimationTest = CreateSprite("Animation Test", "TextureFiles/AnimationTest.png", 300.0f, 300.0f, 3, 3, 3);
	AnimationTest->Position.x = -400.0f;
	AnimationTest->Position.y = 300.0f;

	AnimationTest->AnimationActive = 1;
	AnimationTest->AnimationSpeed = 60;

	Ham->Position.x = 100.0f;
	Ham->Position.y = 200.0f;

	Fox->Position.x = -300.0f;
	Fox->Position.y = -150.0f;

	Fox->AnimationSpeed = 6;
	Fox->AnimationActive = 1;

	if(NULL != malloc(sizeof(Player)))
		InitializePlayer(&CurrentPlayer);

	CurrentPlayer.PlayerSprite->ZIndex = 5;

	UpdateSelector(Selector);
}

void DrawMenu(void)
{
	//Camera follows player
	SetCamera(&CurrentPlayer.Position, 350, 3.0f, &HUDList);

	drawObjectList();
	DrawPlayer(&CurrentPlayer);
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
	else if(AEInputCheckCurr(VK_BACK))
	{
		Vec2 force;
		Vec2Set(&force, 0.0f, 30.0f);
		if(CurrentPlayer.Position.y < -225.0)
			Vec2Set(&CurrentPlayer.Position, CurrentPlayer.Position.x, -224.9f);
		ApplyForce(&CurrentPlayer.PlayerRigidBody, &force);
	}
	else if(AEInputCheckTriggered(VK_SPACE))
	{
		Vec2 force;
		Vec2Set(&force, 0.0f, 12.0f);
		if(CurrentPlayer.Position.y < -225.0)
			Vec2Set(&CurrentPlayer.Position, CurrentPlayer.Position.x, -224.9f);
		ApplyVelocity(&CurrentPlayer.PlayerRigidBody, &force);
	}
	else
	{
		CurrentPlayer.PlayerRigidBody.Acceleration.x = 0;
		CurrentPlayer.PlayerRigidBody.Acceleration.y = 0;
	}

	// check if forcing the application to quit
	if (AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
		return -1;
	
	//Moving the player
	InputPlayer(&CurrentPlayer, 'W');
	InputPlayer(&CurrentPlayer, 'A');
	InputPlayer(&CurrentPlayer, 'S');
	InputPlayer(&CurrentPlayer, 'D');

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
