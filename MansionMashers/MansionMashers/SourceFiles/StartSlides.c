/*
File:				StartSlides.c
Author:				Dan Muller (d.muller)
Creation Date:		Jan 8, 2014

Purpose:			Drawing and logic for the start screen slides

Functions:			FadeLogic - Logic for fading the slides
					InitializeStartScreen - Creates meshes to draw on the screen
					FreeStartScreen - Clears memory of the start screen meshes
					DrawScreen - Draws the start screen to the screen
					StartScreenLoop - Function that runs each iteration of the game loop
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/

// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Sprite.h"

// ---------------------------------------------------------------------------

// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
float alpha = 0.0f;
int slideTextureNum = 0;
int fade = 1;								//0: no fade, 1: fade in, 2: fade out
int slideTimer = 0;

Sprite *Sausage;
Sprite *Title;
Sprite *Digipen;

// ---------------------------------------------------------------------------
// Static function protoypes
int FadeLogic(void);
void DrawScreen(void);

// ---------------------------------------------------------------------------
// main

int FadeLogic(void)
{
	if(fade == 2)
		alpha -= 0.005;
	else if(fade == 1)
		alpha += 0.005;
	else
		alpha = 1.0;

	if(alpha > 1.0)
	{
		alpha = 1.0f;
		fade = 0;
	}
	else if(alpha < 0.0 && slideTextureNum != 2)
	{
		slideTextureNum += 1;
		fade = 1;
		alpha = 0.0;
	}
	else if(alpha < 0.0)
	{
		return 1;
	}



	if(fade == 0)
		slideTimer += 1;

	if(slideTimer == 180)
	{
		fade = 2;
		slideTimer = 0;
	}

	return 0;
}
void InitializeStartScreen(void)
{
	if(NULL != malloc(sizeof(struct Sprite)))
		Title = CreateSprite(1280.0f, 720.0f, 1, 1, "TextureFiles/MansionMashersLogo.png");

	if(NULL != malloc(sizeof(struct Sprite)))
		Digipen = CreateSprite(1024.0f, 248.0f, 1, 1, "TextureFiles/DigipenLogo.png");

	if(NULL != malloc(sizeof(struct Sprite)))
		Sausage = CreateSprite(1280.0f, 720.0f, 1, 1, "TextureFiles/SausageFoxLogoNoBack.png");
}

void FreeStartScreen(void)
{
	// Freeing the objects and textures
}


void DrawScreen(void)
{
	//Digipen Logo
	if(slideTextureNum == 0)
	{
		Digipen->Alpha = alpha;
		DrawSprite(Digipen);
	}
	else
		Digipen->Alpha = 0.0f;

	//Sausage Fox Logo
	if(slideTextureNum == 1)
	{
		Sausage->Alpha = alpha;
		DrawSprite(Sausage);
	}
	else
		Sausage->Alpha = 0.0f;

	//Mansion Mashers Logo
	if(slideTextureNum == 2)
	{
		Title->Alpha = alpha;
		DrawSprite(Title);
	}
	else
		Title->Alpha = 0.0f;

}

int SplashScreenLoop(void)
{
	int changeLevel  = 0;
	int LevelRunning = 1;

	InitializeStartScreen();

	while (LevelRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		// Functions
		changeLevel = FadeLogic();
		DrawScreen();

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (changeLevel == 1 || AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
			LevelRunning = 0;
	}

	FreeStartScreen();
	return changeLevel;
}