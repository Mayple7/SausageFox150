
/*
File:				Main.c
Author:				Dan Muller (d.muller)
Creation Date:		Jan 7, 2014

Purpose:			Starts the game up

Functions:			WinMain - Main function
					Foo - Another function
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 



// ---------------------------------------------------------------------------
// includes
#include "StartSlides.h"
#include "MainMenu.h"
#include "TestLevel.h"
#include "AEEngine.h"

// ---------------------------------------------------------------------------

// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
int GameRunning = 1;

// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------
// main


int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
	// Variable declaration	
	int slideTextureNum = 0;					// Number for the slide texture being shown
	float alpha = 0.0f;
	int slideTimer = 0;
	int fade = 1;								//0: no fade, 1: fade in, 2: fade out
	int initializeLevel = 0;
	int nextLevel = 0;
	int Level = 0;

	AEGfxVertexList*	meshTitle;				// Pointer to Mesh (Model)
	AEGfxVertexList*	meshDigipen;			// Pointer to Digipen Logo Mesh
	AEGfxTexture *titleTexture;					// Pointer to Texture (Image)
	AEGfxTexture *digipenLogo;					// Pointer to Digipen logo texture

	// Initialize the system 
	AESysInitInfo sysInitInfo;
	sysInitInfo.mAppInstance		= instanceH;
	sysInitInfo.mShow				= show;
	sysInitInfo.mWinWidth			= 1280;
	sysInitInfo.mWinHeight			= 720;
	sysInitInfo.mCreateConsole		= 1;
	sysInitInfo.mMaxFrameRate		= 60;
	sysInitInfo.mpWinCallBack		= NULL;//MyWinCallBack;
	sysInitInfo.mClassStyle			= CS_HREDRAW | CS_VREDRAW;											
	sysInitInfo.mWindowStyle		= WS_OVERLAPPEDWINDOW;//WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;;	
	AESysInit (&sysInitInfo);
	AESysSetWindowTitle("Mansion Mashers!");

	// reset the system modules
	AESysReset();

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	/*
		Read Input
		Handle Input
		Draw Output
		???
		Profit!
	*/
	// Gameloop
	while(GameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		if(nextLevel)
		{
			nextLevel = 0;
			Level += 1;
			initializeLevel = 0;
		}

		if(Level == 0)
		{
			if(!initializeLevel)
			{
				InitializeStartScreen();
				initializeLevel = 1;
			}		
			else
				nextLevel = SplashScreenLoop();
		}
		else if(Level == 1)
		{
			if(!initializeLevel)
			{
				InitizalizeMainMenu();
				initializeLevel = 1;
			}
			else
				nextLevel = MenuLoop();
		}
		else if(Level == 2)
		{
			if(!initializeLevel)
			{
				InitizalizeTestLevel();
				initializeLevel = 1;
			}
			else
				nextLevel = LevelLoop();
		}

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
			GameRunning = 0;
	}

	// free the system
	FreeStartScreen();
	AESysExit();

	return 1;
}

// ---------------------------------------------------------------------------
