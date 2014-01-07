// ---------------------------------------------------------------------------
// Project Name		:	Simple Demo
// File Name		:	Main.cpp
// Author			:	Antoine Abi Chacra
// Creation Date	:	2012/01/11
// Purpose			:	main entry point2 for the test program
// History			:
// - 2012/01/11		:	- initial implementation
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"

// ---------------------------------------------------------------------------

// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
int gameRunning = 1;

// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------
// main


int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
	int backgroundColor = 0;

	// Initialize the system 
	AESysInitInfo sysInitInfo;
	sysInitInfo.mAppInstance		= instanceH;
	sysInitInfo.mShow				= show;
	sysInitInfo.mWinWidth			= 800;
	sysInitInfo.mWinHeight			= 600;
	sysInitInfo.mCreateConsole		= 1;
	sysInitInfo.mMaxFrameRate		= 60;
	sysInitInfo.mpWinCallBack		= NULL;//MyWinCallBack;
	sysInitInfo.mClassStyle			= CS_HREDRAW | CS_VREDRAW;											
	sysInitInfo.mWindowStyle		= WS_OVERLAPPEDWINDOW;//WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;;	
	AESysInit (&sysInitInfo);

	// reset the system modules
	AESysReset();

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Gameloop
	while(gameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();


		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
			gameRunning = 0;
		else if(AEInputCheckTriggered(VK_SPACE) && backgroundColor == 0)
		{
			backgroundColor = 1;
			AEGfxSetBackgroundColor(0.0f, 1.0f, 0.0f);
		}
		else if(AEInputCheckTriggered(VK_SPACE) && backgroundColor == 1)
		{
			backgroundColor = 2;
			AEGfxSetBackgroundColor(0.0f, 0.0f, 1.0f);
		}
		else if(AEInputCheckTriggered(VK_SPACE))
		{
			AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
			backgroundColor = 0;
		}
	}

	// free the system
	AESysExit();

	return 1;
}

// ---------------------------------------------------------------------------

