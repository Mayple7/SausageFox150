/*****************************************************************************/
/*!
\file				ArmGuy.c
\author				Dan Muller (d.muller)
\date				Feb 15, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					LoadArmGuy
\li					InitializeArmGuy
\li					UpdateArmGuy
\li					DrawArmGuy
\li					FreeArmGuy
\li					UnloadArmGuy
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/ArmGuy.h"
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
// Libraries
#define BACKGROUND_LENGTH 3

#define GLOW_OVERLAY_NUM 1

// ---------------------------------------------------------------------------
// globals
static int newID;					// ID number
static int levelComplete;
static int PlayerIsAlive; 
static int beginningAnimation;

ArmGuyBoss *Boss;
HUD* CurrentHUD;

Wall* RightWall;

Sprite* Arrow1;
static int Arrow1Grow;

Sprite* BlackOverlay;
Sprite* PlatOverlay[GLOW_OVERLAY_NUM];
static int GlowBool;

FoxSound* IntelFoxStart;
FoxSound* IntelFoxEnd;

FoxSound* ArmGuyDie;

Sprite* IntelFoxBack;
Sprite* IntelFox;
static float IntelFoxValue;

static int timer;
static int timerOn;
static int prevPlayed;

Sprite* TreeBackground1[BACKGROUND_LENGTH];
Sprite* TreeBackground2[BACKGROUND_LENGTH];
Sprite* TreeBackground3[BACKGROUND_LENGTH];
static void TreeBackgroundUpdate(void);
static void ObjectGlowUpdate(void);
/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadArmGuy(void)
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
void InitializeArmGuy(void)
{
	Vec3 Tint;
	int i;
	newID = 10;
	ResetObjectList();
	ResetCamera();
	levelComplete = FALSE;
	GlowBool = TRUE;
	beginningAnimation = TRUE;
	PlayerIsAlive = TRUE;

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -1260, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	CurrentHUD = CreateHUD(&CurrentPlayer);

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	//Panel1
	CreateSprite("TextureFiles/ArmGuyPan.png", 1920, 1080, 5, 1, 1, 0, 0);
	PlatOverlay[0] = (Sprite*)CreateSprite("TextureFiles/AGPlatOverlay.png", 1920, 1080, 6, 1, 1, 0, 0);
	PlatOverlay[0]->Alpha = .1f;

	//Trees in Background
	for(i = 0; i < BACKGROUND_LENGTH; i++)
		TreeBackground1[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground5.png", 1920, 1080, 2, 1, 1, 1920.0f * i, 0);

	for(i = 0; i < BACKGROUND_LENGTH; i++)
		TreeBackground2[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground6.png", 1920, 1080, 1, 1, 1, 1920.0f * i, 0);

	for(i = 0; i < BACKGROUND_LENGTH; i++)
		TreeBackground3[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground7.png", 1920, 1080, 0, 1, 1, 1920.0f * i, 0);

	//Bounding Boxes
	CreateBoundingBoxes();

	// Black Overlay
	Vec3Set(&Tint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = Tint;

	/////////////////////////////////
	//		Sounds				   //
	/////////////////////////////////
	IntelFoxStart = CreateSound("Sounds/IntelFoxBossStart.mp3", SmallSnd);
	IntelFoxEnd = CreateSound("Sounds/IntelFoxBossDogWellDone.mp3", SmallSnd);
	ArmGuyDie = CreateSound("Sounds/ArmGuyDie.mp3", SmallSnd);

	/////////////////////////////////
	//		Platforms			   //
	/////////////////////////////////
	CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 300, 50, newID++, -400, -170);
	CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 300, 50, newID++, 400, -170);

	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	//Create Bounding Walls
	CreateWall("TextureFiles/BlankPlatform.png", 400.0f, 1040.0f, newID++, -1160, 0);
	RightWall = CreateWall("TextureFiles/BlankPlatform.png", 400.0f, 1040.0f, newID++, 1160, 0);

	/////////////////////////////////
	//			Objects			   //
	/////////////////////////////////
	// Arrow Initialize
	Arrow1 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 180, 165, 90, 1, 1, 0, 200);
	Arrow1->Visible = FALSE;
	Arrow1Grow = FALSE;

	IntelFoxBack	= (Sprite*)CreateSprite("TextureFiles/IntelFoxHeadBack.png", 256, 256, 300, 1, 1, 740, 380);
	IntelFox		= (Sprite*)CreateSprite("TextureFiles/IntelFoxHead.png", 256, 256, 300, 1, 1, 740, 380);
	IntelFox->Alpha = 0.0f;
	IntelFoxValue	= 0.0f;

	/////////////////////////////////
	//			Boss			   //
	/////////////////////////////////
	Boss = CreateArmGuyBoss(0, 0, &newID);

	/////////////////////////////////
	//		On Death			   //
	/////////////////////////////////
	CreateDeathConfirmObjects(&newID);

}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateArmGuy(void)
{
	EventArmGuy();

	// This should be the last line in this function
	if(!levelComplete)
	{
		UpdateArmGuyBoss(Boss);
	}

	UpdatePlayerPosition(&CurrentPlayer);

	UpdateHUDPosition(CurrentHUD);
	UpdateHUDItems(CurrentHUD, &CurrentPlayer);

	UpdateFloatingText();
	UpdateAllProjectiles();
	ParticleSystemUpdate();
	BoundingBoxUpdate();

	// When the boss dies
	if(!levelComplete && Boss->CurrentHealth <= 0)
	{
		levelComplete = TRUE;
		Arrow1->Visible = TRUE;
		FreeWall(RightWall);
		FreeArmGuyBoss(Boss);
	}

	// What to do when the boss is dead
	if(levelComplete)
	{
		UpdateArrow(Arrow1, &Arrow1Grow);

		/*if(!ArmGuyDie->hasPlayed && PlayerIsAlive)
		{
			PlayAudio(ArmGuyDie);
			ArmGuyDie->hasPlayed = TRUE;
		}*/

		if(!IntelFoxEnd->hasPlayed && PlayerIsAlive)
		{
			PlayAudio(IntelFoxEnd);
			IntelFoxEnd->hasPlayed = TRUE;
		}


		if(CurrentPlayer.Position.x > (1920.0f / 2) + CurrentPlayer.PlayerCollider.width)
		{
			BlackOverlay->Position.x = GetCameraXPosition();
			BlackOverlay->Alpha += 1 * GetDeltaTime();
			if(BlackOverlay->Alpha > 1)
			{
				SetNextState(GS_MapLevel);
			}
		}
	}

}

/*************************************************************************/
/*!
	\brief
	Draws the level
*/
/*************************************************************************/
void DrawArmGuy(void)
{
	// Draws the object list and sets the camera to the correct location
	DrawObjectList();
	//DrawHUD(&HUDList);
	DrawCollisionList();
	
}

/*************************************************************************/
/*!
	\brief
	Frees all the objects in the level
*/
/*************************************************************************/
void FreeArmGuy(void)
{
	if(levelComplete && CurrentPlayer.CurrentLevel < GS_Level5)
	{
		CurrentPlayer.CurrentLevel = GS_Level5;
	}
	else if(CurrentPlayer.CurrentLevel < GS_ArmGuy)
		CurrentPlayer.CurrentLevel = GS_ArmGuy;

	//Only save stats if the level was actually completed
	if (levelComplete)
	{
		CurrentPlayer.armClear = TRUE;
		SavePlayer(&CurrentPlayer);
	}
	else
		FreeArmGuyBoss(Boss);

	FreeAllLists();
	FreeHUD(CurrentHUD);
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadArmGuy(void)
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
void EventArmGuy(void)
{
	//////////////////////////////////
	//   INPUT & COLLISION FIRST    //
	//////////////////////////////////
	// Check for any collision and handle the results
	DetectPlayerCollision();
	if(!levelComplete)
		DetectArmGuyBossCollision(Boss);
	// Handle any input for the current player
	if(!beginningAnimation)
		InputPlayer(&CurrentPlayer);
	else if(!levelComplete)
	{
		// Make sure the boss stays put during the start
		Boss->cooldownTimer = 0;

		// Fade in the level
		if(BlackOverlay->Alpha > 0)
		{
			BlackOverlay->Alpha -= 1 * GetDeltaTime();
		}
		// Makes the player walk into view
		else
		{
			BlackOverlay->Alpha = 0.0f;
			CurrentPlayer.FlipX = TRUE;
			CurrentPlayer.PlayerDirection = RIGHT;
			CurrentPlayer.Speed = CurrentPlayer.CurrentPlayerStats.MoveSpeed * GetDeltaTime();
			
			// Threshold to give control back to the player
			if(CurrentPlayer.Position.x > -800)
			{
				beginningAnimation = FALSE;
			}
		}
		//Always animate the player otherwise the sprites get stuck in the middle
		Animation(&CurrentPlayer);
		UpdateCollisionPosition(&CurrentPlayer.PlayerWeapon->WeaponAttack, &CurrentPlayer.PlayerWeapon->WeaponAttackPosition);
		MoveObject(&CurrentPlayer.Position, CurrentPlayer.PlayerDirection, CurrentPlayer.Speed);
	}
	

#if defined _DEBUG
	if(FoxInput_KeyTriggered('U'))
	{
		SetDebugMode();
	}
	if(FoxInput_KeyTriggered('I'))
	{
		RemoveDebugMode();
	}
#endif
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		//TogglePauseSound(BackSnd);
		InitializePause(&DrawArmGuy);
		UpdatePause();
		//TogglePauseSound(BackSnd);
	}

	//////////////////////////////////
	//    CAMERA POSITION SECOND    //
	//////////////////////////////////

	//////////////////////////////////
	//       EVERYTHING ELSE        //
	//////////////////////////////////
	TreeBackgroundUpdate();
	ObjectGlowUpdate();

	//Intel Fox Starting Narrative
	if(beginningAnimation == FALSE && !IntelFoxStart->hasPlayed)
	{
		PlayAudio(IntelFoxStart);
		IntelFoxStart->hasPlayed = TRUE;
	}

	//When sound is play show Intel Fox in da corner
	if(FoxSoundCheckIsPlaying(IntelFoxStart) || FoxSoundCheckIsPlaying(IntelFoxEnd))
	{
		if(IntelFox->Alpha < 1)
			IntelFox->Alpha += 3 * GetDeltaTime();
	}
	else
	{
		if(IntelFox->Alpha > 0)
			IntelFox->Alpha -= 3 * GetDeltaTime();
	}

	//Always update intel foxes position you need him
	IntelFox->Position.x = GetCameraXPosition() + 740;
	
	IntelFoxValue += GetDeltaTime() * 8.0f;
	IntelFox->Rotation = sinf(IntelFoxValue) / 4.0f;

	IntelFoxBack->Position = IntelFox->Position;
	IntelFoxBack->Alpha = IntelFox->Alpha;


	//Player Dies
	if(CurrentPlayer.CurrentPlayerStats.CurrentHealth <= 0.0f)
	{
		PlayerIsAlive = FALSE;
		BlackOverlay->Position.x = GetCameraXPosition();
		BlackOverlay->Alpha = 0.5f;

		UpdateDeathConfirmObjects();
	}

}

void TreeBackgroundUpdate(void)
{
	int i;

	for(i = 0; i < BACKGROUND_LENGTH; i++)
		TreeBackground2[i]->Position.x = (1920.0f * i) + (GetCameraXPosition() / 30.0f);

	for(i = 0; i < BACKGROUND_LENGTH; i++)
		TreeBackground3[i]->Position.x = (1920.0f * i) + (GetCameraXPosition() / 15.0f);
}

void ObjectGlowUpdate(void)
{
	int i;

	for(i = 0; i < GLOW_OVERLAY_NUM; i++)
	{
		if(GlowBool)
		{
			PlatOverlay[i]->Alpha += .005f;
			if(PlatOverlay[GLOW_OVERLAY_NUM - 1]->Alpha > 0.3f)
				GlowBool = FALSE;
		}
		else 
		{
			PlatOverlay[i]->Alpha -= .005f;
			if(PlatOverlay[GLOW_OVERLAY_NUM -1]->Alpha < 0.1f)
				GlowBool = TRUE;
		}
	}
}

/* Still working on this I don't think I can get it in for beta
void UpdateArmGuySounds(void)
{
	//Get RandNum to choose rand Sound and a random time
	int randNum = ((int)((rand() / (float)RAND_MAX) * 60)) % 3;
	int randInstance = ((int)((rand() / (float)RAND_MAX) * 720)) % 360;


	//Randomly go but wait if a phrase was just said
	if(randInstance > 356 && !timerOn)
	{
		//Start timer
		timerOn = TRUE;

		//If a sound is not playing let's say something
		if(Boss->ArmGuySoundsPlay == FALSE)
		{
			//Check if phrase your about to say was just said
			if(randNum == prevPlayed)
			{
				//if it was the same phrase say a differnt one
				if(randNum == 0)
				{
					PlayAudio(Boss->ArmGuyPhrase[randNum + 1]);
					prevPlayed = randNum + 1;
				}
				else
				{
					PlayAudio(Boss->ArmGuyPhrase[randNum - 1]);
					prevPlayed = randNum - 1;
				}

			}
			//Wasn't the same so just say what you wanna say
			else
			{
				PlayAudio(Boss->ArmGuyPhrase[randNum]);
				prevPlayed = randNum;
			}
		}
	}

	//For Random Phrase Timer (see above)
	if(timerOn)
		timer -= 1;
	if(timer < 0)
	{
		timer = 5 * FRAMERATE;
		timerOn = FALSE;
	}

}*/

