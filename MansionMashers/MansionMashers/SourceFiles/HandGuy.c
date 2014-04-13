/*****************************************************************************/
/*!
\file				HandGuy.c
\author				Dan Muller (d.muller)
\date				Feb 15, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					LoadHandGuy
\li					InitializeHandGuy
\li					UpdateHandGuy
\li					DrawHandGuy
\li					FreeHandGuy
\li					UnloadHandGuy
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/HandGuy.h"
#include "../HeaderFiles/HandGuyBoss.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"
#include "../HeaderFiles/BoundingBox.h"


// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
static int levelComplete = FALSE;

Sprite* DebugCircle;

Platform* Plat;

HandGuyBoss *Boss;

static int timer;
static int timerOn;
static int prevPlayed;

// Tree Background
Sprite* TreeBackground1[4];
Sprite* TreeBackground2[4];
Sprite* TreeBackground3[4];
static void TreeBackgroundUpdate(void);

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadHandGuy(void)
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
void InitializeHandGuy(void)
{
	int i;
	ResetObjectList();
	ResetCamera();
	timer = 10 * FRAMERATE;

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, 0, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	CreateSprite("TextureFiles/HandGuyBG.png", 1920, 1080, 4, 1, 1, 0, 0);

	//Tree Backgrounds
	for(i = 0; i < 4; i++)
		TreeBackground1[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground1.png", 1920, 1080, 2, 1, 1, 1920.0f * i, 0);

	for(i = 0; i < 4; i++)
		TreeBackground2[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground2.png", 1920, 1080, 1, 1, 1, 1920.0f * i, 0);

	for(i = 0; i < 4; i++)
		TreeBackground3[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground3.png", 1920, 1080, 0, 1, 1, 1920.0f * i, 0);

	//Bounding Boxes
	CreateBoundingBoxes();

	/////////////////////////////////
	//		Platforms			   //
	/////////////////////////////////
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 400, 50, 0, -195);
	Plat->PlatformSprite->Visible = FALSE;

	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	CreateWall("TextureFiles/BlankPlatform.png", 400.0f, 1040.0f, -1160, 0);
	CreateWall("TextureFiles/BlankPlatform.png", 400.0f, 1040.0f, 1160, 0);

	/////////////////////////////////
	//			Boss			   //
	/////////////////////////////////
	Boss = CreateHandGuyBoss(0, 0);

	DebugCircle = (Sprite *)CreateSprite("TextureFiles/DebugCircle.png", Boss->ShoutRadius * 2, Boss->ShoutRadius * 2, 300, 1, 1, Boss->Position.x, Boss->Position.y);
	DebugCircle->Visible = FALSE;

	/////////////////////////////////
	//		On Death			   //
	/////////////////////////////////
	CreateDeathConfirmObjects();

}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateHandGuy(void)
{
	EventHandGuy();
	// This should be the last line in this function
	UpdateHandGuyBoss(Boss);
	UpdatePlayerPosition(&CurrentPlayer);

	UpdateFloatingText();
	BoundingBoxUpdate();

	UpdateAllProjectiles();
	ParticleSystemUpdate();

	if(Boss->CurrentState == 1 && Boss->InnerState != 2)
	{
		DebugCircle->Visible = TRUE;
		DebugCircle->Position = Boss->Position;
	}
	else
		DebugCircle->Visible = FALSE;

	if(Boss->CurrentHealth <= 0)
	{
		levelComplete = TRUE;
		SetNextState(GS_MapLevel);
	}
}

/*************************************************************************/
/*!
	\brief
	Draws the level
*/
/*************************************************************************/
void DrawHandGuy(void)
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
void FreeHandGuy(void)
{
	if(levelComplete && CurrentPlayer.CurrentLevel < GS_Level5)
	{
		CurrentPlayer.CurrentLevel = GS_Level5;
	}
	else if(CurrentPlayer.CurrentLevel < GS_HandGuy)
		CurrentPlayer.CurrentLevel = GS_HandGuy;

	//Only save stats if the level was actually completed
	if (levelComplete)
	{
		CurrentPlayer.handClear = TRUE;
		SavePlayer(&CurrentPlayer);
	}

	//Gotta free that boss sometime
	FreeMyAlloc(Boss);

	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadHandGuy(void)
{
	//Destroy the textures
	DestroyTextureList();
	DestroyPlayer(&CurrentPlayer);
}

/*************************************************************************/
/*!
	\brief
	Handles all events in the level
*/
/*************************************************************************/
void EventHandGuy(void)
{
	int i;
	// Check for any collision and handle the results
	DetectPlayerCollision();
	DetectHandGuyBossCollision(Boss);
	// Handle any input for the current player
	InputPlayer(&CurrentPlayer);

	if(FoxInput_KeyTriggered('U'))
	{
		SetDebugMode();
	}
	if(FoxInput_KeyTriggered('I'))
	{
		RemoveDebugMode();
	}
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		//TogglePauseSound(BackSnd);
		InitializePause(&DrawHandGuy);
		UpdatePause();
		//TogglePauseSound(BackSnd);
	}

	TreeBackgroundUpdate();

	if(Boss->CurrentHealth > 0)
	{
		//Check if the boss is already saying something
		Boss->HandGuySoundsPlay = FALSE;
		for(i = 0; i < 6; i++)
		{
			if(FoxSoundCheckIsPlaying(Boss->HandGuyHit[i]))
				Boss->HandGuySoundsPlay = TRUE;
		}
		if(FoxSoundCheckIsPlaying(Boss->HandGuyYell) || FoxSoundCheckIsPlaying(Boss->HandGuyPhrase))
			Boss->HandGuySoundsPlay = TRUE;

		//Say Phrase Randomly when not beginning animation, intel fox talking, or smashing
		if(/*!beginningAnimation && !FoxSoundCheckIsPlaying(IntelFoxStart) 
			&& !FoxSoundCheckIsPlaying(IntelFoxEnd) &&*/ !FoxSoundCheckIsPlaying(Boss->HandGuyYell))
		{
			//Get RandNum to choose rand Sound and a random time
			int randInstance = ((int)((rand() / (float)RAND_MAX) * 720)) % 540;

			//Randomly go but wait if a phrase was just said
			if(randInstance > 536 && !timerOn)
			{
				//Start timer
				timerOn = TRUE;

				//If a sound is not playing let's say something
				if(Boss->HandGuySoundsPlay == FALSE)
					PlayAudio(Boss->HandGuyPhrase);		
			}
		}
	}

	//For Random Phrase Timer (see above)
	if(timerOn)
		timer -= 1;
	if(timer < 0)
	{
		timer = 10 * FRAMERATE;
		timerOn = FALSE;
	}


}

void TreeBackgroundUpdate(void)
{
	int i;

	for(i = 0; i < 4; i++)
		TreeBackground2[i]->Position.x = (1920.0f * i) + (GetCameraXPosition() / 30.0f);

	for(i = 0; i < 4; i++)
		TreeBackground3[i]->Position.x = (1920.0f * i) + (GetCameraXPosition() / 15.0f);
}

