/*****************************************************************************/
/*!
\file				Level3.c
\author				Juli Gregg (j.gregg)
\date				Mar 17, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					LoadLevel3
\li					InitializeLevel3
\li					UpdateLevel3
\li					DrawLevel3
\li					FreeLevel3
\li					UnloadLevel3
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// Level Notes!!!!
//
//	Since this level goes into the tree tops. There will be a fade out at
//  the end of Panel one and game will switch to Level3.1 which will place
//	the player at the bottom of the screen and the game will take place 
//	as if in a high spot. Can only access Level3.1 through Level 3.
//	Level3.1 is just a continuation of Level3.
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Level3.1.h"
#include "../HeaderFiles/Level3.h"
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
#define PANELSIZE 1920.0f

#define BACKGROUND_LENGTH 3

#define GLOW_OVERLAY_NUM 6

// ---------------------------------------------------------------------------
// globals
static int levelComplete;
static int beginningAnimation;
static int numPanels;
static int PlayerIsAlive;
static int counter;

Platform *Plat;
Wall *Wall1;

HUD* CurrentHUD;

Sprite* SecondOverlay[3];

Sprite* PlatOverlay[GLOW_OVERLAY_NUM];
static int GlowBool;

// Arrows
Sprite *Arrow1;
Sprite *Arrow2;
Sprite *Arrow3;

Food* Strength1;
Food* Heal1;

static int Arrow1Grow;
static int Arrow2Grow;
static int Arrow3Grow;

static EnemySpawner* Spawners[6];
static Enemy* TopDeckEnemy[4];

Sprite* BlackOverlay;

TextGlyphs *IntelFoxTxtStart;

FoxSound* IntelFoxStart;
FoxSound* IntelFoxEnd;

Sprite* IntelFoxBack;
Sprite* IntelFox;
static float IntelFoxValue;

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
void LoadLevel31(void)
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
void InitializeLevel31(void)
{
	Vec3 TextTint;
	Vec2 SpawnerLocation;
	int i;

	ResetObjectList();
	ResetCamera();
	beginningAnimation = TRUE;
	levelComplete = FALSE;
	numPanels = 3;
	PlayerIsAlive = TRUE;
	ResetEnemyPanelNumber();
	counter = 2 * FRAMERATE;
	GlowBool = TRUE;

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -1300, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	CurrentHUD = CreateHUD(&CurrentPlayer);

	/////////////////////////////////
	//		Text				   //
	/////////////////////////////////
	Vec3Set(&TextTint, 1, 1, 1);
	IntelFoxTxtStart = CreateText("Mash enemies to continue", 0, 150, 100, TextTint, Center, Border);
	ChangeTextZIndex(IntelFoxTxtStart, 500);
	TextProgressiveInit(IntelFoxTxtStart);

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	//Panel 1
	CreateSprite("TextureFiles/Level3Pan2.png", 1920, 1080, 5, 1, 1, 0, 0);
	CreateSprite("TextureFiles/Level3PanOverlay.png", 1920, 1080, 401, 1, 1, 0, 0);
	PlatOverlay[0] = (Sprite*)CreateSprite("TextureFiles/Lvl3Pan1PlatOverlay.png", 1920, 1080, 6, 1, 1, 0, 0);
	PlatOverlay[0]->Alpha = .1f;
	PlatOverlay[1] = (Sprite*)CreateSprite("TextureFiles/Lvl3PanPlatOverlay1.png", 1920, 1080, 402, 1, 1, 0, 0);
	PlatOverlay[1]->Alpha = .1f;
	PlatOverlay[1]->Visible = FALSE;
	//Panel2
	CreateSprite("TextureFiles/Level3Pan3.png", 1920, 1080, 5, 1, 1, 1920, 0);
	CreateSprite("TextureFiles/Level3Pan2Overlay.png", 1920, 1080, 401, 1, 1, 1920, 0);
	PlatOverlay[2] = (Sprite*)CreateSprite("TextureFiles/Lvl3Pan3PlatOverlay.png", 1920, 1080, 6, 1, 1, 1920, 0);
	PlatOverlay[2]->Alpha = .1f;
	PlatOverlay[3] = (Sprite*)CreateSprite("TextureFiles/Lvl3PanPlatOverlay2.png", 1920, 1080, 402, 1, 1, 1920, 0);
	PlatOverlay[3]->Alpha = .1f;
	PlatOverlay[3]->Visible = FALSE;
	//Panel3
	CreateSprite("TextureFiles/Level3Pan4.png", 1920, 1080, 5, 1, 1, (1920 * 2), 0);
	CreateSprite("TextureFiles/Level3Pan2Overlay.png", 1920, 1080, 401, 1, 1, (1920 * 2), 0);
	PlatOverlay[4] = (Sprite*)CreateSprite("TextureFiles/Lvl3Pan4PlatOverlay.png", 1920, 1080, 6, 1, 1, 1920 * 2, 0);
	PlatOverlay[4]->Alpha = .1f;
	PlatOverlay[5] = (Sprite*)CreateSprite("TextureFiles/Lvl3PanPlatOverlay2.png", 1920, 1080, 402, 1, 1, 1920*2, 0);
	PlatOverlay[5]->Alpha = .1f;
	PlatOverlay[5]->Visible = FALSE;


	//Create Upper Deck Overlays
	for(i = 1; i < 3; i++)
	{
		SecondOverlay[i] = (Sprite *)CreateSprite("TextureFiles/Level3Pan2Overlay2.png", 1920, 1080, 400, 1, 1, (1920.0f * i), 0);
	}

	SecondOverlay[0] = (Sprite *)CreateSprite("TextureFiles/Level3Pan1Overlay2.png", 1920, 1080, 400, 1, 1, 0, 0);

	for(i = 0; i < BACKGROUND_LENGTH; i++)
		TreeBackground1[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground5.png", 1920, 1080, 2, 1, 1, 1920.0f * i, 0);

	for(i = 0; i < BACKGROUND_LENGTH; i++)
		TreeBackground2[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground6.png", 1920, 1080, 1, 1, 1, 1920.0f * i, 0);

	for(i = 0; i < BACKGROUND_LENGTH; i++)
		TreeBackground3[i] = (Sprite *)CreateSprite("TextureFiles/TreeBackground7.png", 1920, 1080, 0, 1, 1, 1920.0f * i, 0);

	//Black Overlay
	Vec3Set(&TextTint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = TextTint;

	//Bounding Boxes
	CreateBoundingBoxes();

	/////////////////////////////////
	//		Sounds				   //
	/////////////////////////////////
	IntelFoxStart = CreateSound("Sounds/IntelFoxLvl3Mash.mp3", SmallSnd);
	IntelFoxEnd = CreateSound("Sounds/IntelFoxLvl3End.mp3", SmallSnd);

	/////////////////////////////////
	//		Platforms			   //
	/////////////////////////////////
	//Step 1
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 50.0f, 100.0f, -655, -210);
	Plat->PlatformSprite->Visible = FALSE;
	//Step 2
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 50.0f, 100.0f, -655 + 1920, -210);
	Plat->PlatformSprite->Visible = FALSE;
	//Step 3
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 50.0f, 100.0f, -655 + (1920 * 2), -210);
	Plat->PlatformSprite->Visible = FALSE;

	//Top Walkway
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 1920.0f, 100.0f, 0, -20);
	Plat->PlatformSprite->Visible = FALSE;
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 1920.0f, 100.0f, 1920, -20);
	Plat->PlatformSprite->Visible = FALSE;
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 1920.0f, 100.0f, (1920 * 2), -20);
	Plat->PlatformSprite->Visible = FALSE;

	//Middle Platforms
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 150.0f, 100.0f, 150, -255);
	Plat->PlatformSprite->Visible = FALSE;
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 150.0f, 100.0f, 2350, -255);
	Plat->PlatformSprite->Visible = FALSE;
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 150.0f, 100.0f, 150 + PANELSIZE * 2, -255);
	Plat->PlatformSprite->Visible = FALSE;

	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	//Bounding Walls
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 1920.0f, 100.0f, 0, 590);
	Wall1->WallSprite->Visible = FALSE;

	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 1040.0f, -1018, 0); //Updated for new FR Controller
	Wall1->WallSprite->Visible = FALSE;
	Wall1->enemyNotCollidable = TRUE;

	//Top Cicle Blockers
	//Panel 1
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 100.0f, -880, -20);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 115.0f, 100.0f, -490, -20);
	Wall1->WallSprite->Visible = FALSE;
	//Panel 2
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 160.0f, 100.0f, 1060, -20);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 115.0f, 100.0f, -490 + 1920, -20);
	Wall1->WallSprite->Visible = FALSE;
	//Panel 3
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 160.0f, 100.0f, 1060 + 1920, -20);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 115.0f, 100.0f, -490 + (1920 * 2), -20);
	Wall1->WallSprite->Visible = FALSE;

	//Blocker Boxes
	CreateBlockerBoxes();

	/////////////////////////////////
	//			Spawners		   //
	/////////////////////////////////
	Vec2Set(&SpawnerLocation, 0, 0);
	//1st Left
	Spawners[0] = CreateEnemySpawner(2, BasicMelee, FALSE, 100, 1080, SpawnerLocation, 0);
	//1st Right
	Spawners[1] = CreateEnemySpawner(1, BasicMelee, TRUE, 100, 1080, SpawnerLocation, 0);
	TopDeckEnemy[0] = CreateEnemy(BasicMelee, EnemyType, 300, 110, 0);
	
	Vec2Set(&SpawnerLocation, PANELSIZE, 0);
	//2nd Left
	Spawners[2] = CreateEnemySpawner(1, BasicMelee, FALSE, 100, 1080, SpawnerLocation, 1);
	//2nd Right
	Spawners[3] = CreateEnemySpawner(2, BasicMelee, TRUE, 100, 1080, SpawnerLocation, 1);
	TopDeckEnemy[1] = CreateEnemy(BasicMelee, EnemyType, 300 + PANELSIZE, 110, 1);

	Vec2Set(&SpawnerLocation, PANELSIZE * 2, 0);
	//3rd /eft
	Spawners[4] = CreateEnemySpawner(1, BasicMelee, FALSE, 100, 1080, SpawnerLocation, 2);
	//3rd Right
	Spawners[5] = CreateEnemySpawner(1, BasicMelee, TRUE, 100, 1080, SpawnerLocation, 2);
	TopDeckEnemy[2] = CreateEnemy(BasicMelee, EnemyType, 300 + (2 * PANELSIZE), 110, 2);
	TopDeckEnemy[3] = CreateEnemy(BasicMelee, EnemyType, (2 * PANELSIZE), 110, 2);

	/////////////////////////////////
	//			Objects			   //
	/////////////////////////////////
	// Arrows
	Arrow1 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 180, 165, 90, 1, 1, 0, 200);
	Arrow2 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 180, 165, 90, 1, 1, PANELSIZE, 200);
	Arrow3 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 180, 165, 90, 1, 1, 2 * PANELSIZE, 200);

	Arrow1->Visible = FALSE;
	Arrow2->Visible = FALSE;
	Arrow3->Visible = FALSE;

	Arrow1Grow = FALSE;
	Arrow2Grow = TRUE;
	Arrow3Grow = FALSE;
	
	IntelFoxBack	= (Sprite*)CreateSprite("TextureFiles/IntelFoxHeadBack.png", 256, 256, 300, 1, 1, 740, 380);
	IntelFox		= (Sprite*)CreateSprite("TextureFiles/IntelFoxHead.png", 256, 256, 300, 1, 1, 740, 380);
	IntelFox->Alpha = 0.0f;
	IntelFoxValue	= 0.0f;

	CreatePaperScroll(GetCameraXPosition(), 150);

	Strength1 = CreateFood(Strength, 100, 100, 150, -120);
	Heal1 = CreateFood(Heal, 100, 100, 2350, -120);


	/////////////////////////////////
	//		On Death			   //
	/////////////////////////////////

	///Last thing in initialize
	CreateDeathConfirmObjects();
	CreateUpgradeScreenObjects();
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateLevel31(void)
{
	EventLevel31();

	if(PlayerIsAlive)
		PlayAudio(&Level3BackSnd);


	if(!EnemyPanelNumber[0])
		Arrow1->Visible = TRUE;
	if(!EnemyPanelNumber[1])
		Arrow2->Visible = TRUE;
	if(!EnemyPanelNumber[2])
		Arrow3->Visible = TRUE;

	// Update all the arrows
	UpdateArrow(Arrow1, &Arrow1Grow);
	UpdateArrow(Arrow2, &Arrow2Grow);
	UpdateArrow(Arrow3, &Arrow3Grow);

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
void DrawLevel31(void)
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
void FreeLevel31(void)
{
	// Level complete and not reached level 5 zone
	if(levelComplete && CurrentPlayer.CurrentLevel < GS_Level5)
	{
		CurrentPlayer.levelClearBitFlags |= 8;
		CurrentPlayer.CurrentLevel = GS_Level5;
		CurrentPlayer.armUnlock = TRUE;
	}
	// Level complete and has reached level 5 zone
	else if(levelComplete)
	{
		CurrentPlayer.levelClearBitFlags |= 8;
		CurrentPlayer.armUnlock = TRUE;
	}
	// Level NOT complete
	else if(CurrentPlayer.CurrentLevel < GS_Level3)
		CurrentPlayer.CurrentLevel = GS_Level3;

	//Only save stats if the level was actually completed
	if (levelComplete)
		SavePlayer(&CurrentPlayer);

	//Manually free
	freeSound(&Level3BackSnd);

	FreeAllLists();
	FreeHUD(CurrentHUD);
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadLevel31(void)
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
void EventLevel31(void)
{
	//////////////////////////////////
	//   INPUT & COLLISION FIRST    //
	//////////////////////////////////
	if(FoxInput_KeyTriggered('U'))
	{
		SetDebugMode();
		//OverlayGrid->Visible = TRUE;
	}
	if(FoxInput_KeyTriggered('I'))
	{
		RemoveDebugMode();
		//OverlayGrid->Visible = FALSE;
	}

	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		if(PlayerIsAlive && !levelComplete)
		{
			InitializePause(&DrawLevel31);
			TogglePauseSound(&Level3BackSnd);
			//SetNextState(GS_MainMenu);
			UpdatePause();
			TogglePauseSound(&Level3BackSnd);
		}
	}	

	// Runs if the beginning animation is finished
	if(!beginningAnimation && !levelComplete)
	{
		// Check for any collision and handle the results
		DetectPlayerCollision();
		// Handle any input for the current player
		InputPlayer(&CurrentPlayer);
		//UpdateHUDItems(CurrentHUD, &CurrentPlayer);
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

	SetUpCameraPanAndLock(&levelComplete, PANELSIZE, Spawners, numPanels);
	UpdateBlockerBoxes(PANELSIZE);


	//////////////////////////////////
	//       EVERYTHING ELSE        //
	//////////////////////////////////
	ParticleSystemUpdate();
	BoundingBoxUpdate();
	UpdateFloatingText();
	TreeBackgroundUpdate();
	UpdateAllEnemies();
	ObjectGlowUpdate();
	UpdateAllProjectiles();
	
	//SetUpScrollWithText(IntelFoxTxtStart, &counter);

	//Logic for upper deck overlays
	if(CurrentPlayer.Position.y > 105)
	{
		//Player is on top deck so don't show overlay
		int i;
		for(i = 0; i < 3; i++)
		{
			SecondOverlay[i]->Alpha = 0.0f;
		}
		PlatOverlay[1]->Visible = FALSE;
		PlatOverlay[3]->Visible = FALSE;
		PlatOverlay[5]->Visible = FALSE;
	}
	else 
	{
		//Don't run loop if Alpha is already 1.0f
		if(SecondOverlay[0]->Alpha == 0.0f)
		{
			//Player is on bottom deck so show overlays
			int i;
			for(i = 0; i < 3; i++)
			{
				SecondOverlay[i]->Alpha = 1.0f;
			}
		}
		PlatOverlay[1]->Visible = TRUE;
		PlatOverlay[3]->Visible = TRUE;
		PlatOverlay[5]->Visible = TRUE;
	}

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

	
	if(!EnemyPanelNumber[0] && !EnemyPanelNumber[1] && !EnemyPanelNumber[2])
	{
		if(!IntelFoxEnd->hasPlayed && PlayerIsAlive)
		{
			PlayAudio(IntelFoxEnd);
			IntelFoxEnd->hasPlayed = TRUE;
		}
	}


	//Player Dies
	if(CurrentPlayer.CurrentPlayerStats.CurrentHealth <= 0.0f)
	{
		freeSound(&Level3BackSnd);
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
			if(PlatOverlay[GLOW_OVERLAY_NUM - 1]->Alpha > 0.6f)
				GlowBool = FALSE;
		}
		else 
		{
			PlatOverlay[i]->Alpha -= .005f;
			if(PlatOverlay[GLOW_OVERLAY_NUM -1]->Alpha < 0.2f)
				GlowBool = TRUE;
		}
	}
}
