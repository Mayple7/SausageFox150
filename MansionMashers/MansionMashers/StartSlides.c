/*
File:				StartSlides.c
Author:				Dan Muller (d.muller)
Creation Date:		Jan 8, 2014

Purpose:			Drawing and logic for the start screen slides

Functions:			FadeLogic - Logic for fading the slides
 
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
float alpha = 0.0f;
int slideTextureNum = 0;
int fade = 1;								//0: no fade, 1: fade in, 2: fade out
int slideTimer = 0;

// ---------------------------------------------------------------------------
// Static function protoypes

float FadeLogic(void);
void DrawScreen(void);
void SplashScreenLoop(void);

// ---------------------------------------------------------------------------
// main

float FadeLogic(void)
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
}

void DrawScreen(void)
{
	AEGfxVertexList*	meshTitle;				// Pointer to Mesh (Model)
	AEGfxVertexList*	meshDigipen;			// Pointer to Digipen Logo Mesh
	AEGfxTexture *titleTexture;					// Pointer to Texture (Image)
	AEGfxTexture *digipenLogo;					// Pointer to Digipen logo texture

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

	// Drawing object 1
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set poisition for object 1
	AEGfxSetPosition(0.0f, 0.0f);		
		
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
}

void SplashScreenLoop(void)
{
	FadeLogic();
	DrawScreen();
}