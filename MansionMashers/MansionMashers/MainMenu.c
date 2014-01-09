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

#include "AEEngine.h"
#include "MainMenu.h"

// ---------------------------------------------------------------------------

// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals

AEGfxVertexList*	meshStartButton;				// Pointer to Mesh (Model)
//AEGfxVertexList*	meshQuitButton;			// Pointer to Digipen Logo Mesh

AEGfxTexture *startButtonTexture;					// Pointer to Texture (Image)
//AEGfxTexture *digipenLogo;					// Pointer to Digipen logo texture

// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------
// main

int MenuLoop(void)
{
	InputHandling();
	DrawMenu();

	return 0;
}

void InitizalizeMainMenu(void)
{
	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// 1 triangle at a time
	// X, Y, Color, texU, texV
	AEGfxTriAdd(
		-240.0f, -90.0f, 0x00FFFFFF, 0.0f, 1.0f, 
		240.0f,  -90.0f, 0x00FFFFFF, 1.0f, 1.0f,
		-240.0f,  90.0f, 0x00FFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		240.0f, -90.0f, 0x00FFFFFF, 1.0f, 1.0f, 
		240.0f,  90.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-240.0f,  90.0f, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1

	meshStartButton = AEGfxMeshEnd();
	AE_ASSERT_MESG(meshStartButton, "Failed to create start button!!");

	// Texture 1: From file
	startButtonTexture = AEGfxTextureLoad("Textures\\StartButton.png");
	AE_ASSERT_MESG(startButtonTexture, "Failed to create Start Button Texture!!");
}

void DrawMenu(void)
{
	// Drawing object 1
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set poisition for object 1
	AEGfxSetPosition(0.0f, 0.0f);
	// Drawing the mesh (list of triangles)
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(startButtonTexture, 0.0f, 0.0f);
	AEGfxMeshDraw(meshStartButton, AE_GFX_MDM_TRIANGLES);
}

		
void FreeMainMenu(void)
{
	// Freeing the objects and textures
	AEGfxMeshFree(meshStartButton);
	
	AEGfxTextureUnload(startButtonTexture);
}

void InputHandling(void)
{

}


// ---------------------------------------------------------------------------
