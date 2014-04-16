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
#include "../HeaderFiles/Narr2.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"



// ---------------------------------------------------------------------------
// defines
#define PANELSIZE 1920.0f
#define HEAD_SCALE 2


// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
static int levelComplete;
static int fadeInGoing;
static int TimerGoingUp;

ParticleSystem* SystemOne;

Sprite *BlackOverlay;

static float IntelFoxValue;

TextGlyphs* Skip;
float SkipTimer;

FoxSound* BackSnd;

Sprite* Kevin1;
static int KevinIsIn;
static int KevinIsOut;
FoxSound* KPoof;
ParticleSystem* KevinDie;
Sprite* HeadBack;

Sprite* Oakley;
Sprite* PlayerHead;
Sprite* PlayerBack;


Sprite* DogMan;
Sprite* DogMan1;
Sprite* FoxMan;

FoxSound* KevinDefeat;
FoxSound* OakleySaved;

Sprite* Medal;

static float timer;
static int timerOn;

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadNarr2(void)
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
void InitializeNarr2(void)
{
	Vec3 Tint;
	Vec3 TextTint;
	levelComplete = FALSE;
	ResetObjectList();
	ResetCamera();
	ResetEnemyPanelNumber();
	fadeInGoing = TRUE;
	KevinIsIn = TRUE;
	KevinIsOut = FALSE;
	timer = 6;
	timerOn = FALSE;

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
	Skip = CreateText("Hit Space to Skip!", 0, -480, 75, Tint, Center, Border);
	ChangeTextZIndex(Skip, 600);
	ChangeTextAlpha(Skip, 0);
	ChangeTextVisibility(Skip);

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	//Panel 1
	//CreateSprite("TextureFiles/NarrBG.png", PANELSIZE, 1080, 12, 1, 1, 0, 0);

	
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
	KevinDefeat = CreateSound("Sounds/KevinDefeat.mp3", SmallSnd);
	OakleySaved = CreateSound("Sounds/IntelFoxEndGame.mp3", SmallSnd);
	KPoof = CreateSound("Sounds/Poof1.mp3", SmallSnd);

	BackSnd = CreateSound("Sounds/Narr1MusicCut.mp3", LargeSnd);
	
	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	CreateBlockerBoxes();

	/////////////////////////////////
	//			Particles		   //
	/////////////////////////////////
	SystemOne = CreateFoxParticleSystem("TextureFiles/MapParticle.png", 0, 0, 10, -1, 15, 0.5f, 0, 100, 20.0f, 5.0f, 4000, 1080, 50, 2.0f, 2.0f);
	SystemOne->FadeIn = TRUE;

	KevinDie = CreateFoxParticleSystem("TextureFiles/Particle.png", 310, 310, 35, 0, 20, 0.0f, 0, 360, 1.0f, 15.0f, 200, 200, 20, 2.0f, 0.5f);


	/////////////////////////////////
	//			Spawners		   //
	/////////////////////////////////

	/////////////////////////////////
	//			Objects			   //
	/////////////////////////////////
	Kevin1 = (Sprite*)CreateSprite("TextureFiles/KevinTalking.png", 300, 240, 30, 1, 1, 310, 310);
	Oakley = (Sprite*)CreateSprite("TextureFiles/OakleyHead.png", 400, 250, 32, 1, 1, 0, 0);
	HeadBack = (Sprite*) CreateSprite("TextureFiles/IntelFoxHeadBack.png", 540, 500, 28, 1, 1, 310, 320);
	PlayerBack = (Sprite*) CreateSprite("TextureFiles/IntelFoxHeadBack.png", 540, 500, 28, 1, 1, -310, 320);
	Medal = (Sprite*) CreateSprite("TextureFiles/PawOfHonor.png", 800, 1080, 28, 1, 1, -470, 0);



	//To fade in characters
	Oakley->Alpha = 0.0f; 
	Kevin1->Alpha = 0.0f;
	HeadBack->Alpha = 0.0f;
	PlayerBack->Alpha = 0.0f;
	Medal->Alpha = 0.0f;


	IntelFoxValue	= 0.0f;

	//For Correct Character head
	switch (CurrentPlayer.Princess)
	{
	case Mayple:
		PlayerHead = (Sprite*)CreateSprite("TextureFiles/MaypleHead.png", HEAD_SCALE * 102.75f, HEAD_SCALE * 119.25f, 30, 1, 1, -310, 310);
		break;
	case Kaya:
		PlayerHead = (Sprite*)CreateSprite("TextureFiles/KayaHead.png", HEAD_SCALE * 104.8f, HEAD_SCALE * 108, 30, 1, 1, -310, 310);
		break;
	case Ginko:
		PlayerHead = (Sprite*)CreateSprite("TextureFiles/GinkoHead.png", HEAD_SCALE * 129.75f, HEAD_SCALE * 101.25f, 30, 1, 1, -310, 310);
		break;
	case Holly:
		PlayerHead = (Sprite*)CreateSprite("TextureFiles/MaypleHead.png", HEAD_SCALE * 140.25f, HEAD_SCALE * 108.75f, 30, 1, 1, -310, 310);
		break;
	}

	PlayerHead->Alpha = 0.0f;
	PlayerHead->FlipX = TRUE;

	DogMan = (Sprite*)CreateSprite("TextureFiles/DogMan.png", 487, 441, 15, 1, 1, 0, -50);
	DogMan1 = (Sprite*)CreateSprite("TextureFiles/DogMan1.png", 487, 441, 15, 1, 1, 0, -50);

	FoxMan = (Sprite*)CreateSprite("TextureFiles/FoxMan.png", 474, 444, 15, 1, 1, 0, -50);

	DogMan->Alpha = 0.0f;
	FoxMan->Alpha = 0.0f;
	DogMan1->Alpha = 0.0f;


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
void UpdateNarr2(void)
{
	//Handle the special events right off the bat yo
	EventNarr2();
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
void DrawNarr2(void)
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
void FreeNarr2(void)
{
	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadNarr2(void)
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
void EventNarr2(void)
{
	//////////////////////////////////
	//   INPUT & COLLISION FIRST    //
	//////////////////////////////////
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		if(!levelComplete)
		{
			InitializePause(&DrawNarr2);
			TogglePauseSound(BackSnd);
			TogglePauseChannel(EffectType);
			UpdatePause();
			TogglePauseChannel(EffectType);
			TogglePauseSound(BackSnd);
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
		SetNextState(GS_Credits);

	//////////////////////////////////
	//    CAMERA POSITION SECOND    //
	//////////////////////////////////


	//////////////////////////////////
	//       EVERYTHING ELSE        //
	//////////////////////////////////
	UpdateFloatingText();
	ParticleSystemUpdate();
	BoundingBoxUpdate();

	if(levelComplete && BlackOverlay->Alpha > 1)
		SetNextState(GS_Credits);

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


	//Fade in kevin and backs and player head
	if(!fadeInGoing && Kevin1->Alpha < 1 && KevinIsIn)
	{
		float dt = GetDeltaTime();
		Kevin1->Alpha += dt;
		HeadBack->Alpha += dt;
		PlayerHead->Alpha += dt;
		PlayerBack->Alpha += dt;
		DogMan->Alpha += dt;
	}
	else if(!fadeInGoing)
		KevinIsIn = FALSE;

	//Talk that smack kevin
	if(!fadeInGoing && !KevinDefeat->hasPlayed)
	{
		PlayAudio(KevinDefeat);
		KevinDefeat->hasPlayed = TRUE;
	}

	//rotate kevin and oakley
	IntelFoxValue += GetDeltaTime() * 4.0f;
	Kevin1->Rotation = sinf(IntelFoxValue) / 4.0f;
	Oakley->Rotation = sinf(IntelFoxValue) / 4.0f;


	if(!FoxSoundCheckIsPlaying(KevinDefeat) && KevinDefeat->hasPlayed == TRUE && !KPoof->hasPlayed)
	{
		PlayAudio(KPoof);				//Poof Sound
		KevinDie->amountTotal = -1;		//Show Particles
		KPoof->hasPlayed = TRUE;		//Set bool
		Kevin1->Visible = FALSE;		//Set Invisible

		HeadBack->Alpha = 0.0f;			//Update HeadBack for oakley
		HeadBack->Position = Oakley->Position;


		HeadBack->ScaleX = 0.1f;		//Set Scales
		HeadBack->ScaleY = 0.1f;
		Oakley->ScaleX = 0.1f;
		Oakley->ScaleY = 0.1f;
		FoxMan->ScaleX = 0.5f;
		FoxMan->ScaleY = 0.5f;

		DogMan1->Alpha = 1.0f;			//Set Alpha for Dog Mansions
		DogMan->Alpha = 0.0f;

		KevinIsOut = TRUE;				//Kevin is done

	}
	else
		KevinDie->amountTotal = 0;		//Bye particles

	if(KevinIsOut)
	{
		float dt = GetDeltaTime();

		timerOn = TRUE;					//Timer for medal to appear

		if(Oakley->Alpha < 1)
		{
			Oakley->Alpha += dt;			//Get Oakley visible with his back
			HeadBack->Alpha += dt;
			DogMan1->Alpha -= dt;
			FoxMan->Alpha += dt;
		}

		if(Oakley->Position.y < 310)	//Move him & back too
		{
			Oakley->Position.x += 500 * dt;
			Oakley->Position.y += 500 * dt;
		}
		HeadBack->Position = Oakley->Position;

		if(HeadBack->ScaleX < 1)	//Scale them both
		{
			HeadBack->ScaleX += dt;
			HeadBack->ScaleY += dt;
			Oakley->ScaleX += dt;
			Oakley->ScaleY += dt;
		}
		else if(!OakleySaved->hasPlayed)  //Play the audio
		{
			PlayAudio(OakleySaved);
			OakleySaved->hasPlayed = TRUE;
		}

		if(FoxMan->ScaleX < 1)		//Scale the mansion (originally starts at .5f)
		{
			FoxMan->ScaleX += dt;
			FoxMan->ScaleY += dt;
		}

	}

	//Timer for medal
	if(timerOn)
		timer -= GetDeltaTime();
	if(timer < 0)
		Medal->Alpha = 1.0f;

	//Everything done lets get out of here
	if(KevinIsOut && OakleySaved->hasPlayed && !FoxSoundCheckIsPlaying(OakleySaved))
		levelComplete = TRUE;
}

