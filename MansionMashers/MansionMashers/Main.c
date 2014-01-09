
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

	// reset the system modules
	AESysReset();

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// 1 triangle at a time
	// X, Y, Color, texU, texV
	AEGfxTriAdd(
		-518.0f, -84.0f, 0x00FFFFFF, 0.0f, 1.0f, 
		518.0f,  -84.0f, 0x00FFFFFF, 1.0f, 1.0f,
		-518.0f,  84.0f, 0x00FFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		518.0f, -84.0f, 0x00FFFFFF, 1.0f, 1.0f, 
		518.0f,  84.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-518.0f,  84.0f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1

	meshTitle = AEGfxMeshEnd();
	AE_ASSERT_MESG(meshTitle, "Failed to create mesh 1!!");

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// 1 triangle at a time
	// X, Y, Color, texU, texV
	AEGfxTriAdd(
		-512.0f, -124.0f, 0x00FFFFFF, 0.0f, 1.0f, 
		512.0f,  -124.0f, 0x00FFFFFF, 1.0f, 1.0f,
		-512.0f,  124.0f, 0x00FFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		512.0f, -124.0f, 0x00FFFFFF, 1.0f, 1.0f, 
		512.0f,  124.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-512.0f,  124.0f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1

	meshDigipen = AEGfxMeshEnd();
	AE_ASSERT_MESG(meshDigipen, "Failed to create mesh 1!!");


	// Texture 1: From file
	titleTexture = AEGfxTextureLoad("Textures\\MansionMashers_Title.png");
	AE_ASSERT_MESG(titleTexture, "Failed to create Title Texture!!");

	// Texture 2: From file
	digipenLogo = AEGfxTextureLoad("Textures\\DigipenLogo.png");
	AE_ASSERT_MESG(titleTexture, "Failed to create Digipen Logo!!");

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

		// Drawing object 1
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set poisition for object 1
		AEGfxSetPosition(0.0f, 0.0f);
		// Set texture for object 2
		
		
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
		else if(alpha < 0.0)
		{
			slideTextureNum = 1;
			fade = 1;
		}


		if(fade == 0 && slideTextureNum != 1)
			slideTimer += 1;

		if(slideTimer == 180)
		{
			fade = 2;
			slideTimer = 0;
		}
		
		if(slideTextureNum == 1)
			AEGfxSetTransparency(alpha);
		else
			AEGfxSetTransparency(0.0f);
		// Drawing the mesh (list of triangles)
		AEGfxTextureSet(titleTexture, 0.0f, 0.0f);
		AEGfxMeshDraw(meshTitle, AE_GFX_MDM_TRIANGLES);

		if(slideTextureNum == 0)
			AEGfxSetTransparency(alpha);
		else
			AEGfxSetTransparency(0.0f);
		AEGfxTextureSet(digipenLogo, 0.0f, 0.0f);
		AEGfxMeshDraw(meshDigipen, AE_GFX_MDM_TRIANGLES);

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
			GameRunning = 0;
	}

	// Freeing the objects and textures
	AEGfxMeshFree(meshTitle);
	AEGfxMeshFree(meshDigipen);
	
	AEGfxTextureUnload(titleTexture);
	AEGfxTextureUnload(digipenLogo);

	// free the system
	AESysExit();

	return 1;
}

// ---------------------------------------------------------------------------
