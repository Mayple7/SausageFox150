/*****************************************************************************/
/*!
\file				Shop3.c
\author				Kaden Nugent (kaden.n)
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
#include "../HeaderFiles/Shop1.h"
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
static int newID;					// ID number
static int levelComplete;
static int beginningAnimation;
HUD* CurrentHUD;
Sprite* BlackOverlay;

FoxSound *BackSnd;

static int timer;
static int timerOn;
static int prevPlayed;

static int MooseWelcomeSaid;

static int PlayerIsAlive; 

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadShop3(void)
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
void InitializeShop3(void)
{
	Vec3 Tint;
	int randNum, randType;

	levelComplete = FALSE;
	PlayerIsAlive = TRUE;
	beginningAnimation = TRUE;
	newID = 10;
	ResetObjectList();
	ResetCamera();

	//Weapon/Shop
	randNum = (int)((rand() / (float)RAND_MAX) * 70 + 30);
	randType = (int)((rand() / (float)RAND_MAX) * FoxWeapon);

	// First shop plackard
	if(randNum > 98)
		CreateWeaponShop(-400, -140, newID++, randType, Sausage);
	else if(randNum > 85)
		CreateWeaponShop(-400, -140, newID++, randType, Rare);
	else if(randNum > 50)
		CreateWeaponShop(-400, -140, newID++, randType, Uncommon);
	else
		CreateWeaponShop(-400, -140, newID++, randType, Common);
	
	randNum = (int)((rand() / (float)RAND_MAX) * 70 + 30);
	randType = (int)((rand() / (float)RAND_MAX) * FoxWeapon);

	// Second shop plackard
	if(randNum > 98)
		CreateWeaponShop(600, -140, newID++, randType, Sausage);
	else if(randNum > 85)
		CreateWeaponShop(600, -140, newID++, randType, Rare);
	else if(randNum > 50)
		CreateWeaponShop(600, -140, newID++, randType, Uncommon);
	else
		CreateWeaponShop(600, -140, newID++, randType, Common);

	

	CreateSprite("TextureFiles/LevelGrassGround.png", 5760.0f, 1080.0f, 1, 1, 1, 0, 0);

	CreateSprite("TextureFiles/ShopKeeper.png", 350.0f, 350.0f, 80, 1, 1, 0, -250);

	Vec3Set(&Tint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = Tint;

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -1200, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	//Bounding Boxes
	CreateBoundingBoxes();

	BackSnd = CreateSound("Sounds/ShopTheme.wav", LargeSnd);

	CurrentHUD = CreateHUD(&CurrentPlayer);
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateShop3(void)
{
	EventLevel();

	PlayAudio(BackSnd);

	if(levelComplete)
	{
		BlackOverlay->Position.x = GetCameraXPosition();
		BlackOverlay->Alpha += 1 * GetDeltaTime();
		if(BlackOverlay->Alpha > 1)
		{
			SetNextState(GS_MapLevel);
		}
	}

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
void DrawShop3(void)
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
void FreeShop3(void)
{
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
void UnloadShop3(void)
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
	int i;

	/*////////////////////////////////
	//   INPUT & COLLISION FIRST    //
	////////////////////////////////*/
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		if(PlayerIsAlive == TRUE)
		{
			InitializePause(&DrawShop1);
			TogglePauseSound(BackSnd);
			//SetNextState(GS_MainMenu);
			UpdatePause();
			TogglePauseSound(BackSnd);
		}
	}
	if(FoxInput_KeyTriggered('U'))
		SetDebugMode();
	if(FoxInput_KeyTriggered('I'))
		RemoveDebugMode();

	if(!beginningAnimation && !levelComplete)
	{
		// Check for any collision and handle the results
		DetectPlayerCollision();
		// Handle any input for the current player
		InputPlayer(&CurrentPlayer);
		UpdateHUDItems(CurrentHUD, &CurrentPlayer);

		if(CurrentPlayer.Position.x > (1920.0f / 2) + CurrentPlayer.PlayerCollider.width || CurrentPlayer.Position.x < -(1920.0f / 2) - CurrentPlayer.PlayerCollider.width)
		{
			levelComplete = TRUE;
		}

	}
	else if(!levelComplete)
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
			CurrentPlayer.FlipX = 1;
			CurrentPlayer.PlayerDirection = RIGHT;
			CurrentPlayer.Speed = CurrentPlayer.CurrentPlayerStats.MoveSpeed * GetDeltaTime();
			
			// Threshold to give control back to the player
			if(CurrentPlayer.Position.x > -500)
			{
				beginningAnimation = FALSE;
			}
		}
		//Always animate the player otherwise the sprites get stuck in the middle
		Animation(&CurrentPlayer);
		UpdateCollisionPosition(&CurrentPlayer.PlayerWeapon->WeaponAttack, &CurrentPlayer.PlayerWeapon->WeaponAttackPosition);
		MoveObject(&CurrentPlayer.Position, CurrentPlayer.PlayerDirection, CurrentPlayer.Speed);
	}
	else
	{
		BlackOverlay->Position.x = GetCameraXPosition();
		BlackOverlay->Alpha += 1 * GetDeltaTime();
		if(BlackOverlay->Alpha > 1)
		{
			SetNextState(GS_MapLevel);
		}
	}

	UpdateFloatingText();

	//Play a random welcome saying
	if(beginningAnimation == FALSE && !CurrentPlayer.CurrentPlayerSounds.MooseWelcome[0]->hasPlayed && !CurrentPlayer.CurrentPlayerSounds.MooseWelcome[1]->hasPlayed)
	{
		int randNum = ((int)((rand() / (float)RAND_MAX) * 60)) % 2;
		
		PlayAudio(CurrentPlayer.CurrentPlayerSounds.MooseWelcome[randNum]);
		CurrentPlayer.CurrentPlayerSounds.MooseWelcome[randNum]->hasPlayed = TRUE;

		MooseWelcomeSaid = TRUE;
	}

	CurrentPlayer.CurrentPlayerSounds.MoosePlay = FALSE;

	for(i = 0; i < 5; i++)
	{
		if(FoxSoundCheckIsPlaying(CurrentPlayer.CurrentPlayerSounds.MooseRandom[i]))
			CurrentPlayer.CurrentPlayerSounds.MoosePlay = TRUE;
	}
	if(FoxSoundCheckIsPlaying(CurrentPlayer.CurrentPlayerSounds.MooseWelcome[0]) || FoxSoundCheckIsPlaying(CurrentPlayer.CurrentPlayerSounds.MooseWelcome[1])
    || FoxSoundCheckIsPlaying(CurrentPlayer.CurrentPlayerSounds.MoosePurchase[0]) || FoxSoundCheckIsPlaying(CurrentPlayer.CurrentPlayerSounds.MoosePurchase[1])
	|| FoxSoundCheckIsPlaying(CurrentPlayer.CurrentPlayerSounds.MooseNotEnoughCoins))
		CurrentPlayer.CurrentPlayerSounds.MoosePlay = TRUE;

	//Say Random Phrases Randomly
	if(MooseWelcomeSaid)
	{
		//Get RandNum to choose rand Sound and a random time
		int randNum = ((int)((rand() / (float)RAND_MAX) * 60)) % 5;
		int randInstance = ((int)((rand() / (float)RAND_MAX) * 720)) % 360;

		//Randomly go but wait if a phrase was just said
		if(randInstance > 356 && !timerOn)
		{
			//Start timer
			timerOn = TRUE;

			//If a sound is not playing let's say something
			if(CurrentPlayer.CurrentPlayerSounds.MoosePlay == FALSE)
			{
				//Check if phrase your about to say was just said
				if(randNum == prevPlayed)
				{
					//if it was the same phrase say a differnt one
					if(randNum == 0)
					{
						PlayAudio(CurrentPlayer.CurrentPlayerSounds.MooseRandom[randNum + 1]);
						prevPlayed = randNum + 1;
					}
					else
					{
						PlayAudio(CurrentPlayer.CurrentPlayerSounds.MooseRandom[randNum - 1]);
						prevPlayed = randNum - 1;
					}

				}
				//Wasn't the same so just say what you wanna say
				else
				{
					PlayAudio(CurrentPlayer.CurrentPlayerSounds.MooseRandom[randNum]);
					prevPlayed = randNum;
				}
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
}
