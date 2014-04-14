/*****************************************************************************/
/*!
\file				Level1.c

\author				Juli Gregg (j.gregg)	

\date				April 12, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					Load
\li					Initialize
\li					Update
\li					Draw
\li					Free
\li					Unload
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Narr1.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"



// ---------------------------------------------------------------------------
// defines
#define PANELSIZE 1920.0f


// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
static int levelComplete;
static int fadeInGoing;
static int TimerGoingUp;
FoxSound* KingFox;

Sprite* Subtitles;
Sprite* Light;

ParticleSystem* SystemOne;

Sprite *BlackOverlay;

static int timer;
static int timerOn;

TextGlyphs* Skip;
float SkipTimer;

FoxSound* BackSnd;

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadNarr1(void)
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
void InitializeNarr1(void)
{
	Vec3 Tint;
	Vec3 TextTint;
	levelComplete = FALSE;
	ResetObjectList();
	ResetCamera();
	ResetEnemyPanelNumber();
	fadeInGoing = TRUE;
	timer = 2 * FRAMERATE;

	//Set the camera so it currently isn't gated
	ResetGatedCamera();

	// Initialize the player
	//InitializePlayer(&CurrentPlayer, Mayple, -1300, -220);
	//CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;



	/////////////////////////////////
	//		Text				   //
	/////////////////////////////////
	SkipTimer = -2.5;
	TimerGoingUp = TRUE;
	Vec3Set(&Tint, .5, .5, 0);
	Skip = CreateText("Hit Space to Skip!", -425, -480, 75, Tint, Center, Border);
	ChangeTextZIndex(Skip, 600);
	ChangeTextAlpha(Skip, 0);
	ChangeTextVisibility(Skip);

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	//Panel 1
	CreateSprite("TextureFiles/NarrBG.png", PANELSIZE, 1080, 12, 1, 1, 0, 0);
	Light = (Sprite *)CreateSprite("TextureFiles/NarrBGOverlay.png", PANELSIZE, 1080, 12, 1, 1, 0, 0);

	
	//Black Overlay
	Vec3Set(&TextTint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = TextTint;

	//Bounding Boxes
	CreateBoundingBoxes();

	/////////////////////////////////
	//		Sounds				   //
	/////////////////////////////////
	//Background Sound

	KingFox = CreateSound("Sounds/KingFoxSpeech.mp3", SmallSnd);

	BackSnd = CreateSound("Sounds/Narr1MusicCut.mp3", LargeSnd);


	/////////////////////////////////
	//		Platforms			   //
	/////////////////////////////////

	
	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	CreateBlockerBoxes();

	/////////////////////////////////
	//			Particles		   //
	/////////////////////////////////
	SystemOne = CreateFoxParticleSystem("TextureFiles/MapParticle.png", 0, 0, 10, -1, 15, 0.5f, 0, 100, 20.0f, 5.0f, 4000, 1080, 50, 2.0f, 2.0f);
	SystemOne->FadeIn = TRUE;

	/////////////////////////////////
	//			Spawners		   //
	/////////////////////////////////

	/////////////////////////////////
	//			Objects			   //
	/////////////////////////////////
	Subtitles = (Sprite *)CreateSprite("TextureFiles/TextNarrScroll.png", 1920, 2048, 30, 1, 1, 0, -1540);

	/////////////////////////////////
	//		On Death			   //
	/////////////////////////////////

}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateNarr1(void)
{
	//Handle the special events right off the bat yo
	EventNarr1();
	if(!BackSnd->hasPlayed)
	{
		BackSnd->hasPlayed = TRUE;
		PlayAudio(BackSnd);
	}
}

/*************************************************************************/
/*!
	\brief
	Draws the level
*/
/*************************************************************************/
void DrawNarr1(void)
{
	// Draws the object list and sets the camera to the correct location
	DrawObjectList();
	DrawCollisionList();

		// This should be the last line in this function
	//UpdatePlayerPosition(&CurrentPlayer);
}

/*************************************************************************/
/*!
	\brief
	Frees all the objects in the level
*/
/*************************************************************************/
void FreeNarr1(void)
{


	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadNarr1(void)
{
	//Destroy the textures
	DestroyTextureList();
	//DestroyPlayer(&CurrentPlayer);
}

/*************************************************************************/
/*!
	\brief
	Handles all events in the level
*/
/*************************************************************************/
void EventNarr1(void)
{
	//////////////////////////////////
	//   INPUT & COLLISION FIRST    //
	//////////////////////////////////
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		if(!levelComplete)
		{
			InitializePause(&DrawNarr1);
			//TogglePauseSound(BackSnd);
			TogglePauseSound(KingFox);
			UpdatePause();
			TogglePauseSound(KingFox);

			//TogglePauseSound(BackSnd);
		}
	}

	if(BlackOverlay->Alpha > 0 && fadeInGoing)
	{
		BlackOverlay->Alpha -= GetDeltaTime();
	}
	else if(levelComplete)
	{
		BlackOverlay->Alpha += GetDeltaTime();
	}
	else
		fadeInGoing = FALSE;

	if(FoxInput_KeyTriggered(' '))
		SetNextState(GS_MapLevel);

	//////////////////////////////////
	//    CAMERA POSITION SECOND    //
	//////////////////////////////////


	//////////////////////////////////
	//       EVERYTHING ELSE        //
	//////////////////////////////////
	UpdateFloatingText();
	ParticleSystemUpdate();
	BoundingBoxUpdate();

	if(!fadeInGoing && !KingFox->hasPlayed)
	{
		PlayAudio(KingFox);
		KingFox->hasPlayed = TRUE;
	}

	if(!FoxSoundCheckIsPlaying(KingFox) && KingFox->hasPlayed)
	{
		levelComplete = TRUE;
	}

	if(levelComplete && BlackOverlay->Alpha > 1)
		SetNextState(GS_MapLevel);

	if(!fadeInGoing)
	{
		Subtitles->Position.y += 100 * GetDeltaTime();
	}

	if (TimerGoingUp)
		SkipTimer += GetDeltaTime();
	else
		SkipTimer -= GetDeltaTime();
	if (SkipTimer > 0)
	{
		ChangeTextAlpha(Skip, SkipTimer);

		if (SkipTimer > 1)
			TimerGoingUp = FALSE;
		else if (SkipTimer < 0.4)
			TimerGoingUp = TRUE;
	}

}

