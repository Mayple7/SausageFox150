/*****************************************************************************/
/*!
\file				Level5.c
\author				Juli Gregg (j.gregg)	
\date				Mar 8, 2014

\brief				Functions for Level5

\par				Functions:
\li					LoadLevel5
\li					InitializeLevel5
\li					UpdateLevel5
\li					DrawLevel5
\li					FreeLevel5
\li					UnloadLevel5
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Level5.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// define
#define PANELSIZE 1920.0f

// ---------------------------------------------------------------------------
// globals
static int levelComplete;
static int beginningAnimation;
static int keysObtained;
static int goingBack;
static int counter;

Sprite* BlackOverlay;
HUD* CurrentHUD;

FoxSound* BackSnd;

TextGlyphs* IntelFoxTxtTwoKeys;
TextGlyphs* IntelFoxTxtOneKeys;
TextGlyphs* IntelFoxTxtEnd;

FoxSound* IntelFoxTwoKeys;
FoxSound* IntelFoxLook;
FoxSound* IntelFoxOneKey;
FoxSound* IntelFoxEnd;

Sprite* IntelFoxBack;
Sprite* IntelFox;
static float IntelFoxValue;

Platform* Plat;
Wall* Wall1;

Wall* RightBarrier;

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
void LoadLevel5(void)
{
	//Allocate space for a large texture
	CreateTextureList();
	LoadPlayerSprites(CurrentPlayer.Princess);
}

/*************************************************************************/
/*!
	\brief
	Initializes the objects for the level
*/
/*************************************************************************/
void InitializeLevel5(void)
{
	Vec3 TextTint;
	int i;
	ResetObjectList();
	ResetCamera();
	ResetEnemyPanelNumber();
	ResetGatedCamera();
	levelComplete = FALSE;
	beginningAnimation = TRUE;
	keysObtained = FALSE;
	goingBack = FALSE;
	counter = 2 * FRAMERATE;
	

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -1300, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	CurrentHUD = CreateHUD(&CurrentPlayer);

	/////////////////////////////////
	//		Text				   //
	/////////////////////////////////
	Vec3Set(&TextTint, 1, 1, 1);
	IntelFoxTxtTwoKeys = CreateText("You need two keys!", 0, 150, 100, TextTint, Center, Border);
	ChangeTextZIndex(IntelFoxTxtTwoKeys, 500);
	TextProgressiveInit(IntelFoxTxtTwoKeys);

	IntelFoxTxtOneKeys = CreateText("You need one more key!", 0, 150, 100, TextTint, Center, Border);
	ChangeTextZIndex(IntelFoxTxtOneKeys, 500);
	TextProgressiveInit(IntelFoxTxtOneKeys);

	IntelFoxTxtEnd = CreateText("Head Underground!", 0, 150, 100, TextTint, Center, Border);
	ChangeTextZIndex(IntelFoxTxtEnd, 500);
	TextProgressiveInit(IntelFoxTxtEnd);

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	//Panel1
	CreateSprite("TextureFiles/Level5Pan1.png", 1920, 1080, 5, 1, 1, 0, 0);

	//Tree Backgrounds
	for(i = 0; i < 4; i++)
		TreeBackground1[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground1.png", 1920, 1080, 2, 1, 1, 1920.0f * i, 0);

	for(i = 0; i < 4; i++)
		TreeBackground2[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground2.png", 1920, 1080, 1, 1, 1, 1920.0f * i, 0);

	for(i = 0; i < 4; i++)
		TreeBackground3[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground3.png", 1920, 1080, 0, 1, 1, 1920.0f * i, 0);

	//Black Overlay
	Vec3Set(&TextTint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = TextTint;

	//Bounding Boxes
	CreateBoundingBoxes();

	/////////////////////////////////
	//		Sounds				   //
	/////////////////////////////////
	BackSnd = CreateSound("Sounds/TutorialTheme.mp3", LargeSnd);

	IntelFoxTwoKeys = CreateSound("Sounds/IntelFoxLvl5TwoKeys.mp3", SmallSnd);
	IntelFoxLook = CreateSound("Sounds/IntelFoxLvl5Look.mp3", SmallSnd);
	IntelFoxOneKey = CreateSound("Sounds/IntelFoxLvl5OneKey.mp3", SmallSnd);
	IntelFoxEnd = CreateSound("Sounds/IntelFoxLvl5End.mp3", SmallSnd);

	/////////////////////////////////
	//		Platforms			   //
	/////////////////////////////////

	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	//Right Blocker Barrier
	RightBarrier = CreateWall("TextureFiles/BlankPlatform.png", 200, 1080.0f, 870, 0); //Updated for new FR Controller
	RightBarrier->WallSprite->Visible = FALSE;

	/////////////////////////////////
	//		Objects				   //
	/////////////////////////////////
	IntelFoxBack	= (Sprite*)CreateSprite("TextureFiles/IntelFoxHeadBack.png", 256, 256, 300, 1, 1, 740, 380);
	IntelFox		= (Sprite*)CreateSprite("TextureFiles/IntelFoxHead.png", 256, 256, 300, 1, 1, 740, 380);
	IntelFox->Alpha = 0.0f;
	IntelFoxValue	= 0.0f;

	CreatePaperScroll(0, 150);

	CreateUpgradeScreenObjects();
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateLevel5(void)
{
	EventLevel();
	PlayAudio(BackSnd);

	ParticleSystemUpdate();
	UpdateAllProjectiles();

	// This should be the last line in this function
	UpdatePlayerPosition(&CurrentPlayer);

	UpdateHUDPosition(CurrentHUD);
	UpdateHUDItems(CurrentHUD, &CurrentPlayer);
}

/*************************************************************************/
/*!
	\brief
	Draws the level
*/
/*************************************************************************/
void DrawLevel5(void)
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
void FreeLevel5(void)
{
	if(levelComplete && CurrentPlayer.CurrentLevel < GS_Level6)
	{
		CurrentPlayer.levelClearBitFlags |= 32;
		CurrentPlayer.CurrentLevel = GS_Level6;
	}
	else if(CurrentPlayer.CurrentLevel < GS_Level5)
		CurrentPlayer.CurrentLevel = GS_Level5;

	//Only save stats if the level was actually completed
	if (levelComplete)
		SavePlayer(&CurrentPlayer);

	FreeAllLists();
	FreeHUD(CurrentHUD);
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadLevel5(void)
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
void EventLevel(void)
{
	//////////////////////////////////
	//   INPUT & COLLISION FIRST    //
	//////////////////////////////////
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
		InitializePause(&DrawLevel5);
		TogglePauseSound(BackSnd);
		UpdatePause();
		TogglePauseSound(BackSnd);

	}

	// Runs if the beginning animation is finished
	if(!beginningAnimation && !levelComplete && !goingBack)
	{
		// Check for any collision and handle the results
		DetectPlayerCollision();
		// Handle any input for the current player
		InputPlayer(&CurrentPlayer);
		//UpdateHUDItems(CurrentHUD, &CurrentPlayer);
	}
	else if(!levelComplete && !goingBack)
	{
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
				beginningAnimation = FALSE;
		}
		//Always animate the player otherwise the sprites get stuck in the middle
		Animation(&CurrentPlayer);
		UpdateCollisionPosition(&CurrentPlayer.PlayerWeapon->WeaponAttack, &CurrentPlayer.PlayerWeapon->WeaponAttackPosition);
		MoveObject(&CurrentPlayer.Position, CurrentPlayer.PlayerDirection, CurrentPlayer.Speed);
	}
	else
		LevelCompletion();

	//////////////////////////////////
	//    CAMERA POSITION SECOND    //
	//////////////////////////////////

	//////////////////////////////////
	//       EVERYTHING ELSE        //
	//////////////////////////////////
	BoundingBoxUpdate();
	ParticleSystemUpdate();
	TreeBackgroundUpdate();

	//Player Has two keys 
	if(CurrentPlayer.armClear && CurrentPlayer.handClear && !beginningAnimation)
	{
		//Print Appropriate text and play certain audio
		SetUpScrollWithText(IntelFoxTxtEnd, &counter);
		if(!IntelFoxEnd->hasPlayed)
		{
			PlayAudio(IntelFoxEnd);
			IntelFoxEnd->hasPlayed = TRUE;
		}
	}
	//Player has one key
	else if((CurrentPlayer.armClear || CurrentPlayer.handClear) && !beginningAnimation)
	{
		//Print Appropriate text and play certain audio
		SetUpScrollWithText(IntelFoxTxtOneKeys, &counter);
		if(!IntelFoxOneKey->hasPlayed)
		{
			PlayAudio(IntelFoxOneKey);
			IntelFoxOneKey->hasPlayed = TRUE;
		}
	}
	//Player has no keys
	else if(!beginningAnimation)
	{
		//Print Appropriate text and play certain audio
		SetUpScrollWithText(IntelFoxTxtTwoKeys, &counter);
		if(!IntelFoxTwoKeys->hasPlayed)
		{
			PlayAudio(IntelFoxTwoKeys);
			IntelFoxTwoKeys->hasPlayed = TRUE;
		}
		if(IntelFoxTwoKeys->hasPlayed && !FoxSoundCheckIsPlaying(IntelFoxTwoKeys) && !IntelFoxLook->hasPlayed)
		{
			PlayAudio(IntelFoxLook);
			IntelFoxLook->hasPlayed = TRUE;
		}
	}

	//When sound is play show Intel Fox in da corner
	if(FoxSoundCheckIsPlaying(IntelFoxTwoKeys) || FoxSoundCheckIsPlaying(IntelFoxEnd)
		|| FoxSoundCheckIsPlaying(IntelFoxOneKey) || FoxSoundCheckIsPlaying(IntelFoxLook))
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

	//Key Checking
	if(CurrentPlayer.armClear && CurrentPlayer.handClear && !beginningAnimation)
	{
		RightBarrier->WallCollider.Position.x = 930;
		keysObtained = TRUE;
	}

	//Level Transition if complete
	BlackOverlay->Position.x = GetCameraXPosition();
	if(CurrentPlayer.Position.x >= 720 + CurrentPlayer.PlayerCollider.width / 2 && keysObtained)
	{
		levelComplete = TRUE;
	}
	//Level Transition if not complete
	if(CurrentPlayer.Position.x <= -PANELSIZE /2 - CurrentPlayer.PlayerCollider.width / 2 && !beginningAnimation)
	{
		goingBack = TRUE;
		BlackOverlay->Alpha += 1 * GetDeltaTime();
		if(BlackOverlay->Alpha > 1)
			SetNextState(GS_MapLevel);

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
