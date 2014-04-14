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
static int levelComplete;
static int PlayerIsAlive; 
static int beginningAnimation;

Sprite* DebugCircle;
Sprite* BlackOverlay;

Platform* Plat;

HandGuyBoss *Boss;
HUD* CurrentHUD;

Wall* RightWall;

//Boss HP Bar
Sprite* BossHPBar;
Sprite* BossHPBarBack;

Sprite* Arrow1;
static int Arrow1Grow;

static int timer;
static int timerOn;
static int prevPlayed;

FoxSound* BackSnd;
Food* AGKey;
FoxSound* WinTheme;
FoxSound* KeySFX;
static int KeyDropped;

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
	Vec3 Tint;
	int i;
	ResetObjectList();
	ResetCamera();
	levelComplete = FALSE;
	beginningAnimation = TRUE;
	PlayerIsAlive = TRUE;
	timer = 10 * FRAMERATE;
	KeyDropped = FALSE;
	CurrentPlayer.Key2 = FALSE;

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -1260, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	CurrentHUD = CreateHUD(&CurrentPlayer);

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

	// Arrow Initialize
	Arrow1 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 180, 165, 90, 1, 1, 0, 200);
	Arrow1->Visible = FALSE;
	Arrow1Grow = FALSE;

	//Bounding Boxes
	CreateBoundingBoxes();

	// Black Overlay
	Vec3Set(&Tint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = Tint;

	// Boss HP Bar
	BossHPBar = (Sprite *)CreateSprite("TextureFiles/BossHealthBarMid.png", 1, 44, 399, 1, 1, -200, 450);
	BossHPBarBack = (Sprite *)CreateSprite("TextureFiles/BossHealthBarBack.png", 820, 64, 398, 1, 1, 0, 450);

	/////////////////////////////////
	//		Sounds				   //
	/////////////////////////////////
	BackSnd = CreateSound("Sounds/BossMusic.wav", LargeSnd);
	WinTheme = CreateSound("Sounds/CreditTheme.wav", LargeSnd);
	KeySFX = CreateSound("Sounds/KeyDrop.mp3", SmallSnd);

	/////////////////////////////////
	//		Platforms			   //
	/////////////////////////////////
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 400, 50, 0, -195);
	Plat->PlatformSprite->Visible = FALSE;

	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	//Create Bounding Walls
	CreateWall("TextureFiles/BlankPlatform.png", 400.0f, 1040.0f, -1160, 0);
	RightWall = CreateWall("TextureFiles/BlankPlatform.png", 400.0f, 1040.0f, 1160, 0);

	/////////////////////////////////
	//			Boss			   //
	/////////////////////////////////
	Boss = CreateHandGuyBoss(0, 0);

	DebugCircle = (Sprite *)CreateSprite("TextureFiles/DebugCircle.png", Boss->ShoutRadius * 2, Boss->ShoutRadius * 2, 300, 1, 1, Boss->Position.x, Boss->Position.y);
	DebugCircle->Visible = FALSE;

	/////////////////////////////////
	//			Objects			   //
	/////////////////////////////////
	AGKey = CreateFood(Key, 80, 120, 0, 1100);


	/////////////////////////////////
	//		On Death			   //
	/////////////////////////////////
	CreateDeathConfirmObjects();

	CreateUpgradeScreenObjects();
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

	if(PlayerIsAlive && !KeyDropped)
		PlayAudio(BackSnd);
	else if(PlayerIsAlive)
		PlayAudio(WinTheme);

	// This should be the last line in this function
	if(!levelComplete && !KeyDropped)
	{
		UpdateHandGuyBoss(Boss);
	}
	
	UpdatePlayerPosition(&CurrentPlayer);

	UpdateHUDPosition(CurrentHUD);
	UpdateHUDItems(CurrentHUD, &CurrentPlayer);

	UpdateFloatingText();
	BoundingBoxUpdate();
	UpdateAllProjectiles();
	ParticleSystemUpdate();

	// Shows the hit circle for handguy - should change this for final
	if(!levelComplete && Boss->CurrentState == 1 && Boss->InnerState != 2)
	{
		DebugCircle->Visible = TRUE;
		DebugCircle->Position = Boss->Position;
	}
	else
		DebugCircle->Visible = FALSE;

	//When the boss dies
	if(Boss->CurrentHealth <= 0 && AGKey->FoodSprite->Position.y > (GROUNDLEVEL + AGKey->FoodSprite->Height / 2))
	{
		if(!KeyDropped)
		{
			FreeHandGuyBoss(Boss);
			freeSound(BackSnd);
		}
		AGKey->FoodSprite->Position.y -= 650 * GetDeltaTime();
		AGKey->FoodCollider.Position.y -= 650 * GetDeltaTime();
		AGKey->FoodParticle->Position.y -= 650 * GetDeltaTime();
		KeyDropped = TRUE;
		//if(!IntelFoxEnd->hasPlayed && PlayerIsAlive)
		//{
		//	PlayAudio(IntelFoxEnd);
		//	IntelFoxEnd->hasPlayed = TRUE;
		//}
	}

	if(AGKey->FoodSprite->Position.y < 500 && !KeySFX->hasPlayed)
	{
		KeySFX->hasPlayed = TRUE;
		PlayAudio(KeySFX);
	}

	// When the key is achieved
	if(!levelComplete && CurrentPlayer.Key2)
	{
		levelComplete = TRUE;
		Arrow1->Visible = TRUE;
		FreeWall(RightWall);
	}

	// What to do when the boss is dead
	if(levelComplete)
	{
		UpdateArrow(Arrow1, &Arrow1Grow);

		if(CurrentPlayer.Position.x > (1920.0f / 2) + CurrentPlayer.PlayerCollider.width)
		{
			LevelCompletion();
		}
	}

	if(KeyDropped)
	{
		BossHPBar->Visible = FALSE;

		if(BossHPBar->Alpha > 0.0f)
		{
			BossHPBar->Alpha -= GetDeltaTime() / 2.0f;
		}
		else
			BossHPBar->Alpha = 0;

	}
	// Boss health bar logic
	else
	{
		BossHPBar->ScaleX = 800.0f * (Boss->CurrentHealth / (float)Boss->MaxHealth);
		BossHPBar->Position.x = -400.0f * (1 - (Boss->CurrentHealth / (float)Boss->MaxHealth));
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
	else
		FreeHandGuyBoss(Boss);

	//Gotta free that boss sometime
	FreeMyAlloc(Boss);

	FreeAllLists();
	FreeHUD(CurrentHUD);
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
	if(!levelComplete)
		DetectHandGuyBossCollision(Boss);

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
		if(PlayerIsAlive)
		{
			//TogglePauseSound(BackSnd);
			InitializePause(&DrawHandGuy);
			UpdatePause();
			//TogglePauseSound(BackSnd);
		}
	}

	TreeBackgroundUpdate();

	//Player Dies
	if(CurrentPlayer.CurrentPlayerStats.CurrentHealth <= 0.0f)
	{
		//freeSound(BackSnd);
		PlayerIsAlive = FALSE;
		BlackOverlay->Position.x = GetCameraXPosition();
		BlackOverlay->Alpha = 0.5f;

		UpdateDeathConfirmObjects();
	}

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

