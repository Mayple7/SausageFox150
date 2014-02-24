/*****************************************************************************/
/*!
\file				StartSlides.c
\author				Dan Muller (d.muller
\date				Jan 8, 2014

\brief				Functions for fading in and out all the splash screen slides

\par				Functions:
\li					LoadStartScreen
\li					InitializeStartScreen
\li					UpdateStartScreen
\li					DrawStartScreen
\li					FreeStartScreen
\li					UnloadStartScreen
\li					fadeLogic
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/StartSlides.h"
#include "../HeaderFiles/FoxEngine.h"

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
static float alpha = 0.0f;
static int slideTextureNum = 0;
static int fade = 1;								//0: no fade, 1: fade in, 2: fade out
static int slideTimer = 0;

Sprite *SausageFox;
Sprite *Title;
Sprite *Digipen;

// ---------------------------------------------------------------------------
// Static function protoypes
static int fadeLogic(void);

// ---------------------------------------------------------------------------
// main

/*************************************************************************/
/*!
	\brief
	Loads the assets for the start screen
*/
/*************************************************************************/
void LoadStartScreen(void)
{
	//Allocate space for a large texture
	CreateTextureList();
}

/*************************************************************************/
/*!
	\brief
	Initializes the objects for the start screen
*/
/*************************************************************************/
void InitializeStartScreen(void)
{
	// Reset the object list
	ResetObjectList();
	alpha = 0.0f;
	slideTextureNum = 0;
	fade = 1;								//0: no fade, 1: fade in, 2: fade out
	slideTimer = 0;
	// Create the slide sprites
	Title = CreateSprite("TextureFiles/MansionMashersLogo.png", 1920.0f, 1080.0f, 0, 1, 1, 0, 0);
	Digipen = CreateSprite("TextureFiles/DigipenLogo.png", 1024.0f, 248.0f, 0, 1, 1, 0, 0);
	SausageFox = CreateSprite("TextureFiles/SausageFoxLogoNoBack.png", 1920.0f, 1080.0f, 0, 1, 1, 0, 0);
}

/*************************************************************************/
/*!
	\brief
	Updates the start screen
*/
/*************************************************************************/
void UpdateStartScreen(void)
{
	int changeLevel;

	// Fades the slides if needed to and returns when to change levels
	changeLevel = fadeLogic();

	if(changeLevel == 1)
		SetNextState(GS_Tutorial);
	else if(changeLevel == -1)
		SetNextState(GS_Quit);
}

/*************************************************************************/
/*!
	\brief
	Draws the slides to the screen
*/
/*************************************************************************/
void DrawStartScreen(void)
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
		SausageFox->Alpha = alpha;
		DrawSprite(SausageFox);
	}
	else
		SausageFox->Alpha = 0.0f;

	//Mansion Mashers Logo
	if(slideTextureNum == 2)
	{
		Title->Alpha = alpha;
		DrawSprite(Title);
	}
	else
		Title->Alpha = 0.0f;

}

/*************************************************************************/
/*!
	\brief
	Frees the objects from the level
*/
/*************************************************************************/
void FreeStartScreen(void)
{
	// Freeing the objects and textures
	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads the assets from the start screen
*/
/*************************************************************************/
void UnloadStartScreen(void)
{
	//Destroy the textures
	DestroyTextureList();
}

/*************************************************************************/
/*!
	\brief
	Logic for fading in and out the different slides

	\return
	Returns a value for what level should be loaded if any or to continue
*/
/*************************************************************************/
static int fadeLogic(void)
{
	// Fade out the slide
	if(fade == 2)
		alpha -= 0.01f;
	// Fade in the slide
	else if(fade == 1)
		alpha += 0.01f;
	else
		alpha = 1.0f;

	// The fade in is done
	if(alpha > 1.0)
	{
		alpha = 1.0f;
		fade = 0;
	}
	// The fade out is done and it is not the last slide
	else if(alpha < 0.0 && slideTextureNum != 2)
	{
		slideTextureNum += 1;
		fade = 1;
		alpha = 0.0f;
	}
	// The fade out is done, go to main menu
	else if(alpha < 0.0)
	{
		return 1;
	}

	// Timer for the slide staying visible
	if(fade == 0)
		slideTimer += 1;

	// Start fading out the slide after some time
	if(slideTimer == 120)
	{
		fade = 2;
		slideTimer = 0;
	}

	// Do not change levels
	return 0;
}
