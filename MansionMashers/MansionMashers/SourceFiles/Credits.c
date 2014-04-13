/*****************************************************************************/
/*!
\file				Credits.c
\author				Juli Gregg (j.gregg)
\date				Feb 15, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					LoadLevel
\li					InitializeLevel
\li					UpdateLevel
\li					DrawLevel
\li					FreeLevel
\li					UnloadLevel
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/TemplateLevel.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// defines
#define PANELSIZE 2880

// ---------------------------------------------------------------------------
// globals			
static int levelComplete;
static int beginningAnimation;

Sprite* BlackOverlay;

Sprite* Skip;
float SkipTimer;
static int TimerGoingUp;

FoxSound* BackSnd;

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadCredits(void)
{
	//Allocate space for a large texture
	CreateTextureList();
}

/*************************************************************************/
/*!
	\brief
	Initializes the objects for the level
*/
/*************************************************************************/
void InitializeCredits(void)
{
	Vec3 TextTint;

	ResetObjectList();
	ResetCamera();

	levelComplete = FALSE;
	beginningAnimation = TRUE;

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	CreateSprite("TextureFiles/CreditsBG.png", 2880, 1080, 20, 1, 1, 1820, 0);

	//Bounding Boxes
	CreateBoundingBoxes();

	//Black Overlay
	Vec3Set(&TextTint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = TextTint;

	SkipTimer = -2.5;
	TimerGoingUp = TRUE;
	Skip = (Sprite *)CreateSprite("TextureFiles/Skip.png", 1920, 1080, 600, 1, 1, 0, 0);
	Skip->Alpha = -2.5;

	BackSnd = CreateSound("Sounds/CreditTheme.wav", LargeSnd);
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateCredits(void)
{
	EventLevel();
	PlayAudio(BackSnd);
}

/*************************************************************************/
/*!
	\brief
	Draws the level
*/
/*************************************************************************/
void DrawCredits(void)
{
	// Draws the object list and sets the camera to the correct location
	DrawObjectList();
	DrawCollisionList();
}

/*************************************************************************/
/*!
	\brief
	Frees all the objects in the level
*/
/*************************************************************************/
void FreeCredits(void)
{
	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadCredits(void)
{
	//Destroy the textures
	DestroyTextureList();
}

/*************************************************************************/
/*!
	\brief
	Handles all events in the level
*/
/*************************************************************************/
void EventLevel(void)
{
	//////////////////////////////
	//		Input				//
	//////////////////////////////
	
	if(FoxInput_KeyTriggered(VK_SPACE))
	{
		SetNextState(GS_MainMenu);
	}



	//////////////////////////////
	//		Camera				//
	//////////////////////////////

	if(GetCameraXPosition() < PANELSIZE + 800)
		SetCameraXPosition(GetCameraXPosition() + ((PANELSIZE / 10.0f) * GetLoadRatio()) * GetDeltaTime());
	else
	{
		SetCameraXPosition(PANELSIZE + 800);
	}

	if(GetCameraXPosition() >= PANELSIZE + 400)
		levelComplete = TRUE;


	//////////////////////////////
	//		Everything Else		//
	//////////////////////////////
	BoundingBoxUpdate();
	ParticleSystemUpdate();
	
	if(beginningAnimation)
	{
		if(BlackOverlay->Alpha > 0)
		{
			BlackOverlay->Alpha -= 1 * GetDeltaTime();
		}
		else 
			beginningAnimation = FALSE;
	}

	//Level Transition
	BlackOverlay->Position.x = GetCameraXPosition();
	if(levelComplete)
	{
		BlackOverlay->Alpha += 1 * GetDeltaTime();
		if(BlackOverlay->Alpha > 1)
			SetNextState(GS_MainMenu);
	}

	if (TimerGoingUp)
		SkipTimer += GetDeltaTime();
	else
		SkipTimer -= GetDeltaTime();
	if (SkipTimer > 0)
	{
		Skip->Alpha = SkipTimer;

		if (SkipTimer > 1)
			TimerGoingUp = FALSE;
		else if (SkipTimer < 0.4)
			TimerGoingUp = TRUE;
	}

	Skip->Position.x = GetCameraXPosition();
	
}
