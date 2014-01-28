/*****************************************************************************/
/*!
\file				Main.c
\author				Dan Muller (d.muller)
\date				Jan 7, 2014

\brief				The main function for the game and holds the game loop

\par				Functions:
\li					SetCamera
\li					ResetCamera
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../HeaderFiles/StartSlides.h"
#include "../HeaderFiles/MainMenu.h"
#include "../HeaderFiles/TestLevel.h"
#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"

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
	int Previous;								//Local State Variables
	int Current;								//Local State Variables
	int Next;									//Local State Variables
	long Time1 = 0;								//For framerate controller
	long Time2 = 0;								//for framerate controller
	long DeltaTime = 0;
	int FrameRate = 60;							//Make a define in the future
	AESysInitInfo sysInitInfo;

	//Creates the console window
	if(AllocConsole())
	{
		FILE* file;
 
		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);
		freopen_s(&file, "CONOUT$", "wt", stdin);

		SetConsoleTitle("Alpha Engine - Console");
	}

	// Initialize the system 
	sysInitInfo.mAppInstance		= instanceH;
	sysInitInfo.mShow				= show;
	sysInitInfo.mWinWidth			= 1280;
	sysInitInfo.mWinHeight			= 720;
	sysInitInfo.mCreateConsole		= 0;
	sysInitInfo.mMaxFrameRate		= 0;
	sysInitInfo.mpWinCallBack		= NULL;//MyWinCallBack;
	sysInitInfo.mClassStyle			= CS_HREDRAW | CS_VREDRAW;											
	sysInitInfo.mWindowStyle		= WS_OVERLAPPEDWINDOW;//WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;;	
	AESysInit (&sysInitInfo);
	AESysSetWindowTitle("Mansion Mashers!");
	
	
	// reset the system modules
	AESysReset();

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	//GSM Start

	GSMInitialize(GS_MainMenu);

	while(GameRunning)
	{
		//Setting local states
		Previous = GetCurrentState();
		Current = GetCurrentState();
		Next = GetNextState();

		//Checking if wanting to quit
		if(Current == GS_Quit)
		{
			AESysExit();
			return 0;
		}
		//Checking if wanting to restart
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
			Time1 = timeGetTime();							//in Milliseconds

			AEInputUpdate();
			GSMPointers.pUpdate();
			GSMPointers.pDraw();
			Next = GetNextState();

			Time2 = timeGetTime();							//in Milliseconds
			DeltaTime = Time2 - Time1;						//in Milliseconds
			
			//Framerate controller
			while(DeltaTime < (1000.0 / FrameRate))
			{
				Time2 = timeGetTime();
				DeltaTime = Time2 - Time1;
			}
			//printf("%lu\n", DeltaTime);

			AESysFrameEnd();
		}

		GSMPointers.pFree();

		if(Next != GS_Restart)
			GSMPointers.pUnload();
		
		SetPreviousState(Current);
		SetCurrentState(Next);

	}

	return 1;

	//End of GSM
}
