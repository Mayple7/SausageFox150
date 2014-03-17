/*****************************************************************************/
/*!
\file				EP2Slides.c
\author				Dan Muller (d.muller
\date				Jan 8, 2014

\brief				Functions for fading in and out all the splash screen slides

\par				Functions:
\li					LoadEP2Screen
\li					InitializeEP2Screen
\li					UpdateEP2Screen
\li					DrawEP2Screen
\li					FreeEP2Screen
\li					UnloadEP2Screen
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
#include "../HeaderFiles/EP2Slides.h"
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
static int slideDir;								//1: Forward, 0: Backward

static Sprite *StretchGoals;
static Sprite *Title;
static Sprite *Risks;
static ParticleSystem *FirstParticle;
static ParticleSystem *SecondParticle;
static ParticleSystem *ThirdParticle;
static ParticleSystem *FourthParticle;



static enum Slides { Slide1, Slide2, Slide3, MaxSlides};

// ---------------------------------------------------------------------------
// Static function protoypes
static int fadeLogic(void);

// ---------------------------------------------------------------------------
// main

/*************************************************************************/
/*!
	\brief
	Loads the assets for the EP2 screen
*/
/*************************************************************************/
void LoadEP2Screen(void)
{
	//Allocate space for a large texture
	CreateTextureList();
}

/*************************************************************************/
/*!
	\brief
	Initializes the objects for the EP2 screen
*/
/*************************************************************************/
void InitializeEP2Screen(void)
{
	// Reset the object list
	ResetObjectList();
	
	alpha = 0.0f;
	slideTextureNum = 0;
	fade = 1;								//0: no fade, 1: fade in, 2: fade out
	slideTimer = 0;
	slideDir = 0;

	// Create the slide sprites
	Title = (Sprite *) CreateSprite("TextureFiles/MansionMashersLogo.png", 1920.0f, 1080.0f, 0, 1, 1, 0, 0);
	StretchGoals = (Sprite *) CreateSprite("TextureFiles/Slide4.png", 1920.0f, 1080.0f, 0, 1, 1, 0, 0);
	Risks = (Sprite *) CreateSprite("TextureFiles/Slide6.png", 1920.0f, 1080.0f, 0, 1, 1, 0, 0);
	FirstParticle	= CreateFoxParticleSystem("TextureFiles/FireParticle.png", -705, 20, 201, -1, 5, 0.01f, 90, 45, 1.5f, -35.0f, 9, 10, 200, 0.25f, 1.0f);
	SecondParticle	= CreateFoxParticleSystem("TextureFiles/FireParticle.png", -310, 20, 201, -1, 5, 0.01f, 90, 45, 1.5f, -35.0f, 9, 10, 200, 0.25f, 1.0f);
	ThirdParticle	= CreateFoxParticleSystem("TextureFiles/FireParticle.png",  195, 20, 201, -1, 5, 0.01f, 90, 45, 1.5f, -35.0f, 9, 10, 200, 0.25f, 1.0f);
	FourthParticle	= CreateFoxParticleSystem("TextureFiles/FireParticle.png",  895, 20, 201, -1, 5, 0.01f, 90, 45, 1.5f, -35.0f, 9, 10, 200, 0.25f, 1.0f);
}

/*************************************************************************/
/*!
	\brief
	Updates the EP2 screen
*/
/*************************************************************************/
void UpdateEP2Screen(void)
{
	int changeLevel;

	// Fades the slides if needed to and returns when to change levels
	changeLevel = fadeLogic();

	if(AEInputCheckTriggered(VK_ESCAPE))
		SetNextState(GS_MainMenu);

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
void DrawEP2Screen(void)
{
	
	//Risks
	if(slideTextureNum == Slide1)
	{
		Risks->Alpha = alpha;
		//DrawSprite(Risks);
	}
	else
		Risks->Alpha = 0.0f;

	//Stretch Goals
	if(slideTextureNum == Slide2)
	{
		StretchGoals->Alpha = alpha;
		//DrawSprite(StretchGoals);
	}
	else
		StretchGoals->Alpha = 0.0f;

		//Mansion Mashers
	if(slideTextureNum == Slide3)
	{
		Title->Alpha = alpha;
		//DrawSprite(Title);
	}
	else
		Title->Alpha = 0.0f;

	FirstParticle->ParticleStartAlpha	= Risks->Alpha;
	SecondParticle->ParticleStartAlpha	= Risks->Alpha;
	ThirdParticle->ParticleStartAlpha	= Risks->Alpha;
	FourthParticle->ParticleStartAlpha	= Risks->Alpha;

	ParticleSystemUpdate();

	DrawObjectList();
}

/*************************************************************************/
/*!
	\brief
	Frees the objects from the level
*/
/*************************************************************************/
void FreeEP2Screen(void)
{
	// Freeing the objects and textures
	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads the assets from the EP2 screen
*/
/*************************************************************************/
void UnloadEP2Screen(void)
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
		alpha -= 0.02f;
	// Fade in the slide
	else if(fade == 1)
		alpha += 0.02f;
	else
		alpha = 1.0f;

	// The fade in is done
	if(alpha > 1.0)
	{
		alpha = 1.0f;
		fade = 0;
	}
	// The fade out is done and it is not the last slide
	else if(alpha < 0.0 && slideTextureNum != MaxSlides - 1)
	{
		if(slideDir)
			slideTextureNum += 1;
		else if(slideTextureNum)
			slideTextureNum -= 1;
		fade = 1;
		alpha = 0.0f;
	}
	// The fade out is done, go to main menu
	else if(alpha < 0.0)
	{
		if(slideDir)
			return 1;
		else
		{
			slideTextureNum -= 1;
			fade = 1;
			alpha = 0.0f;
		}

	}

	// Timer for the slide staying visible
	if(fade == 0)
		slideTimer += 1;

	// Start fading out the slide after some time
	if(AEInputCheckTriggered(VK_SPACE) && alpha > 0.5)
	{
		slideDir = 1;
		fade = 2;
		slideTimer = 0;
	}
	else if(AEInputCheckTriggered(VK_BACK) && alpha > 0.5)
	{
		if(slideTextureNum != Slide1)
		{
			slideDir = 0;
			fade = 2;
			slideTimer = 0;
		}
	}

	// Do not change levels
	return 0;
}
