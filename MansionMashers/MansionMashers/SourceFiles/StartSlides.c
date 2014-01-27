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
#include "../HeaderFiles/FoxEngine.h"

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
float alpha = 0.0f;
int slideTextureNum = 0;
int fade = 1;								//0: no fade, 1: fade in, 2: fade out
int slideTimer = 0;

Sprite *Sausage;
Sprite *Title;
Sprite *Digipen;

// ---------------------------------------------------------------------------
// Static function protoypes
int fadeLogic(void);

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
	//Placeholder?
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
	resetObjectList();

	// Create the slide sprites
	Title = CreateSprite("Title", "TextureFiles/MansionMashersLogo.png", 1280.0f, 720.0f, 0, 1, 1);
	Digipen = CreateSprite("Digipen", "TextureFiles/DigipenLogo.png", 1024.0f, 248.0f, 0, 1, 1);
	Sausage = CreateSprite("Sausage", "TextureFiles/SausageFoxLogoNoBack.png", 1280.0f, 720.0f, 0, 1, 1);
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
		SetNextState(GS_MainMenu);
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
		Sausage->Alpha = alpha;
		DrawSprite(Sausage);
	}
	else
		Sausage->Alpha = 0.0f;

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
	freeObjectList();
}

/*************************************************************************/
/*!
	\brief
	Unloads the assets from the start screen
*/
/*************************************************************************/
void UnloadStartScreen(void)
{
	//Placeholder?
}

/*************************************************************************/
/*!
	\brief
	Logic for fading in and out the different slides

	\return
	Returns a value for what level should be loaded if any or to continue
*/
/*************************************************************************/
int fadeLogic(void)
{
	// Fade out the slide
	if(fade == 2)
		alpha -= 0.01;
	// Fade in the slide
	else if(fade == 1)
		alpha += 0.01;
	else
		alpha = 1.0;

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
		alpha = 0.0;
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
