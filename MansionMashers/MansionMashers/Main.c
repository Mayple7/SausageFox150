
/*
File:				Main.c
Author:				Dan Muller (d.muller)
Creation Date:		Jan 7, 2014

Purpose:			Houses the main gameloop

Functions:			WinMain - Main function
					Foo - Another function
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 


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

	// Variable declaration	
	AEGfxVertexList*	pMesh1;				// Pointer to Mesh (Model)

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

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// 1 triangle at a time
	// X, Y, Color, texU, texV
	AEGfxTriAdd(
		-25.5f, -25.5f, 0xFFFFFFFF, 0.0f, 0.0f, 
		25.5f,  0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
		-25.5f,  25.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1

	pMesh1 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh1, "Failed to create mesh 1!!");

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Gameloop
	while(gameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		// Drawing object 1
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Set poisition for object 1
		AEGfxSetPosition(0.0f, 0.0f);
		// No texture for object 1
		AEGfxTextureSet(NULL, 0, 0);
		



		// check if forcing the application to quit
		if (AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
			gameRunning = 0;
		else if(AEInputCheckTriggered(VK_SPACE) && backgroundColor == 0)
		{
			backgroundColor = 1;
			AEGfxSetBackgroundColor(0.0f, 0.5f, 0.0f);
			AEGfxSetBlendColor(0.5f, 0.0f, 0.0f, 1.0f);
		}
		else if(AEInputCheckTriggered(VK_SPACE) && backgroundColor == 1)
		{
			backgroundColor = 2;
			AEGfxSetBackgroundColor(0.0f, 0.0f, 0.5f);
			AEGfxSetBlendColor(0.5f, 0.5f, 0.0f, 1.0f);
		}
		else if(AEInputCheckTriggered(VK_SPACE))
		{
			AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
			backgroundColor = 0;
			AEGfxSetBlendColor(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// Drawing the mesh (list of triangles)
		AEGfxSetTransparency(1.0f);
		AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);
		
		// Informing the system about the loop's end
		AESysFrameEnd();
	}

	// Freeing the objects and textures
	AEGfxMeshFree(pMesh1);

	// free the system
	AESysExit();

	return 1;
}

// ---------------------------------------------------------------------------
