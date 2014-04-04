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

//Boss HP Bar
Sprite* BossHPBar;
Sprite* BossHPBarBack;

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

	// Arrow Initialize
	Arrow1 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 250, 235, 90, 1, 1, 0, 200);
	Arrow1->Visible = FALSE;
	Arrow1Grow = FALSE;

	// Black Overlay
	Vec3Set(&Tint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = Tint;

	// Boss HP Bar
	BossHPBar = (Sprite *)CreateSprite("TextureFiles/BossHealthBarMid.png", 1, 44, 399, 1, 1, -200, 450);
	BossHPBarBack = (Sprite *)CreateSprite("TextureFiles/BossHealthBarBack.png", 820, 64, 398, 1, 1, 0, 450);

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

		if(CurrentPlayer.Position.x > (1920.0f / 2) + CurrentPlayer.PlayerCollider.width)
		{
			BlackOverlay->Position.x = GetCameraXPosition();
			BlackOverlay->Alpha += 1 * GetDeltaTime();
			if(BlackOverlay->Alpha > 1)
			{
				SetNextState(GS_MapLevel);
			}
		}

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

	//CurrentHUD->HealthBarEnd->Position.x = (CurrentHUD->HUDBackground->Position.x - 52.0f + (CurrentHUD->HealthBar->ScaleX * 2));
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
