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
#include "../HeaderFiles/StartSlides.h"
#include "../HeaderFiles/MainMenu.h"
#include "../HeaderFiles/TestLevel.h"
#include "../AEEngine.h"
#include "../HeaderFiles/Movement.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"

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
	int nextLevel = 0;
	int Level = 1;
	int Previous;								//Local State Variables
	int Current;								//Local State Variables
	int Next;									//Local State Variables
	
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

	//********************************************************
	//Slowing working GSM into files don't uncomment for now

	//System_Initialize();
	GSMInitialize(GS_ShowcaseLevel);

	while(GameRunning)
	{
		//AESysFrameStart();

		Previous = GetCurrentState();
		Current = GetCurrentState();
		Next = GetNextState();

		if(Current == GS_Quit)
		{
			AESysExit();
			return 0;
		}
		else if(Current == GS_Restart)
		{
			SetCurrentState(Previous);
			SetNextState(Previous);
			Current = Previous;
			Next = Previous;
		}
		else
		{
			GSMUpdate(Current);
			GSMPointers.pLoad();
		}

		GSMPointers.pInit();
		
		while(Current == Next)
		{
			AESysFrameStart();
			AEInputUpdate();
			GSMPointers.pUpdate();
			GSMPointers.pDraw();
			Next = GetNextState();
			AESysFrameEnd();
		}

		GSMPointers.pFree();

		if(Next != GS_Restart)
			GSMPointers.pUnload();
		
		SetPreviousState(Current);
		SetCurrentState(Next);
		Previous = Current;
		Current = Next;

		//AESysFrameEnd();
	}

	return 1;

	//End of GSM
	//******************************************************/
	
	/*
		Read Input
		Handle Input
		Draw Output
		???
		Profit!
	*/
	// Gameloop
	/****************************************************
	while(GameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		//Level List
		/*
			//Interface Levels
		   -1:	Exit Game
			0:	Splash Screen
			1:	Main Menu
			2:	Options Screen
			3:	Credits
			4:	Level Select
			5:	Level 1
			6:	First Shop
			7:	Level 2
			8:	etc. etc.

			enum:
			L1
			L2
			L3
			L_NUM -> Will equal number of levels
		*/

		/******************
		switch(Level)
		{
		case 0:
			nextLevel = SplashScreenLoop();
			break;
		case 1:
			nextLevel = MenuLoop();
			break;
		case 2:
			nextLevel = LevelLoop();
			break;
		default:
			GameRunning = 0;
		}

		Level = nextLevel;
		nextLevel = 0;

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		//if (AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
			//GameRunning = 0;
	}

	// free the system
	AESysExit();
	
	return 1;
	***************************************/
}

// ---------------------------------------------------------------------------
