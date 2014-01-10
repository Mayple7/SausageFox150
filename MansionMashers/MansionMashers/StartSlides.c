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

#include "AEEngine.h"
#include "Sprite.h"

// ---------------------------------------------------------------------------

// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
float alpha = 0.0f;
int slideTextureNum = 0;
int fade = 1;								//0: no fade, 1: fade in, 2: fade out
int slideTimer = 0;

AEGfxVertexList*	meshTitle;				// Pointer to Mesh (Model)
AEGfxVertexList*	meshDigipen;			// Pointer to Digipen Logo Mesh
AEGfxVertexList*	meshSausage;			// Pointer to Digipen Logo Mesh
AEGfxTexture *titleTexture;					// Pointer to Texture (Image)
AEGfxTexture *digipenLogo;					// Pointer to Digipen logo texture
AEGfxTexture *sausageLogo;					// Pointer to Digipen logo texture

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
	meshTitle = CreateSpriteTexture(1280.0f, 720.0f);
	AE_ASSERT_MESG(meshTitle, "Failed to create mesh 1!!");

	meshDigipen = CreateSpriteTexture(1024.0f, 248.0f);
	AE_ASSERT_MESG(meshDigipen, "Failed to create mesh 1!!");

	meshSausage = CreateSpriteTexture(1280.0f, 720.0f);
	AE_ASSERT_MESG(meshSausage, "Failed to create mesh 1!!");


	// Texture 1: From file
	titleTexture = AEGfxTextureLoad("Textures\\MansionMashersLogo.png");
	AE_ASSERT_MESG(titleTexture, "Failed to create Title Texture!!");

	// Texture 2: From file
	digipenLogo = AEGfxTextureLoad("Textures\\DigipenLogo.png");
	AE_ASSERT_MESG(titleTexture, "Failed to create Digipen Logo!!");

	// Texture 3: From file
	sausageLogo = AEGfxTextureLoad("Textures\\SausageFoxLogoNoBack.png");
	AE_ASSERT_MESG(sausageLogo, "Failed to create Digipen Logo!!");

}

void FreeStartScreen(void)
{
	// Freeing the objects and textures
	AEGfxMeshFree(meshTitle);
	AEGfxMeshFree(meshDigipen);
	AEGfxMeshFree(meshSausage);
	
	AEGfxTextureUnload(titleTexture);
	AEGfxTextureUnload(digipenLogo);
	AEGfxTextureUnload(sausageLogo);
}


void DrawScreen(void)
{
	// Drawing object 1
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set poisition for object 1
	AEGfxSetPosition(0.0f, 0.0f);		
		
	if(slideTextureNum == 2)
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

	if(slideTextureNum == 1)
		AEGfxSetTransparency(alpha);
	else
		AEGfxSetTransparency(0.0f);
	AEGfxTextureSet(sausageLogo, 0.0f, 0.0f);
	AEGfxMeshDraw(meshSausage, AE_GFX_MDM_TRIANGLES);
}

int SplashScreenLoop(void)
{
	int changeLevel;
	changeLevel = FadeLogic();
	DrawScreen();

	return changeLevel;
}