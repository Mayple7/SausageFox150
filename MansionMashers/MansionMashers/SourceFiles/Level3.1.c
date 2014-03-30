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
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"
#include "../HeaderFiles/EasyEdit.h"


// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// defines
#define PANELSIZE 1920.0f

#define BACKGROUND_LENGTH 3

// ---------------------------------------------------------------------------
// globals
static int newID;					// ID number
static int levelComplete;
static int beginningAnimation;
static int numPanels;
static int PlayerIsAlive;
static int counter;

Platform *Plat;
Wall *Wall1;

HUD* CurrentHUD;

Sprite* SecondOverlay[3];

// Arrows
Sprite *Arrow1;
Sprite *Arrow2;
Sprite *Arrow3;

static int Arrow1Grow;
static int Arrow2Grow;
static int Arrow3Grow;

static EnemySpawner* Spawners[6];
static Enemy* TopDeckEnemy[4];

Sprite* BlackOverlay;

TextGlyphs *IntelFoxTxtStart;

FoxSound* IntelFoxStart;
FoxSound* IntelFoxEnd;

Sprite* TreeBackground1[BACKGROUND_LENGTH];
Sprite* TreeBackground2[BACKGROUND_LENGTH];
Sprite* TreeBackground3[BACKGROUND_LENGTH];
static void TreeBackgroundUpdate(void);

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
	newID = 10;
	ResetObjectList();
	ResetCamera();
	beginningAnimation = TRUE;
	levelComplete = FALSE;
	numPanels = 3;
	PlayerIsAlive = TRUE;
	ResetEnemyPanelNumber();
	counter = 2 * FRAMERATE;

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
	CreateSprite("TextureFiles/Level3Pan2Overlay.png", 1920, 1080, 401, 1, 1, 0, 0);
	//Panel2
	CreateSprite("TextureFiles/Level3Pan3.png", 1920, 1080, 5, 1, 1, 1920, 0);
	CreateSprite("TextureFiles/Level3Pan2Overlay.png", 1920, 1080, 401, 1, 1, 1920, 0);
	//Panel3
	CreateSprite("TextureFiles/Level3Pan4.png", 1920, 1080, 5, 1, 1, (1920 * 2), 0);
	CreateSprite("TextureFiles/Level3Pan2Overlay.png", 1920, 1080, 401, 1, 1, (1920 * 2), 0);

	//Create Upper Deck Overlays
	for(i = 0; i < 3; i++)
	{
		SecondOverlay[i] = (Sprite *)CreateSprite("TextureFiles/Level3Pan2Overlay2.png", 1920, 1080, 400, 1, 1, (1920.0f * i), 0);
	}

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
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 50.0f, 100.0f, newID++, -655, -210);
	Plat->PlatformSprite->Visible = FALSE;
	//Step 2
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 50.0f, 100.0f, newID++, -655 + 1920, -210);
	Plat->PlatformSprite->Visible = FALSE;
	//Step 3
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 50.0f, 100.0f, newID++, -655 + (1920 * 2), -210);
	Plat->PlatformSprite->Visible = FALSE;

	//Top Walkway
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 1920.0f, 100.0f, newID++, 0, -20);
	Plat->PlatformSprite->Visible = FALSE;
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 1920.0f, 100.0f, newID++, 1920, -20);
	Plat->PlatformSprite->Visible = FALSE;
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 1920.0f, 100.0f, newID++, (1920 * 2), -20);
	Plat->PlatformSprite->Visible = FALSE;

	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	//Bounding Walls
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 1920.0f, 100.0f, newID++, 0, 590);
	Wall1->WallSprite->Visible = FALSE;

	//Top Cicle Blockers
	//Panel 1
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 100.0f, newID++, -880, -20);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 115.0f, 100.0f, newID++, -490, -20);
	Wall1->WallSprite->Visible = FALSE;
	//Panel 2
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 160.0f, 100.0f, newID++, 1060, -20);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 115.0f, 100.0f, newID++, -490 + 1920, -20);
	Wall1->WallSprite->Visible = FALSE;
	//Panel 3
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 160.0f, 100.0f, newID++, 1060 + 1920, -20);
	Wall1->WallSprite->Visible = FALSE;
	Wall1 = CreateWall("TextureFiles/BlankPlatform.png", 115.0f, 100.0f, newID++, -490 + (1920 * 2), -20);
	Wall1->WallSprite->Visible = FALSE;

	//Blocker Boxes
	CreateBlockerBoxes(&newID);

	/////////////////////////////////
	//			Spawners		   //
	/////////////////////////////////
	Vec2Set(&SpawnerLocation, 0, 0);
	//1st Left
	Spawners[0] = CreateEnemySpawner(2, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 0);
	//1st Right
	Spawners[1] = CreateEnemySpawner(1, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 0);
	TopDeckEnemy[0] = CreateEnemy(BasicMelee, EnemyType, newID++, 300, 110, 0);
	
	Vec2Set(&SpawnerLocation, PANELSIZE, 0);
	//2nd Left
	Spawners[2] = CreateEnemySpawner(1, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 1);
	//2nd Right
	Spawners[3] = CreateEnemySpawner(2, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 1);
	TopDeckEnemy[1] = CreateEnemy(BasicMelee, EnemyType, newID++, 300 + PANELSIZE, 110, 1);

	Vec2Set(&SpawnerLocation, PANELSIZE * 2, 0);
	//3rd /eft
	Spawners[4] = CreateEnemySpawner(1, BasicMelee, FALSE, 100, 1080, SpawnerLocation, &newID, 2);
	//3rd Right
	Spawners[5] = CreateEnemySpawner(1, BasicMelee, TRUE, 100, 1080, SpawnerLocation, &newID, 2);
	TopDeckEnemy[2] = CreateEnemy(BasicMelee, EnemyType, newID++, 300 + (2 * PANELSIZE), 110, 2);
	TopDeckEnemy[3] = CreateEnemy(BasicMelee, EnemyType, newID++, (2 * PANELSIZE), 110, 2);

	/////////////////////////////////
	//			Objects			   //
	/////////////////////////////////
	// Arrows
	Arrow1 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 250, 235, 90, 1, 1, 0, 200);
	Arrow2 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 250, 235, 90, 1, 1, PANELSIZE, 200);
	Arrow3 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 250, 235, 90, 1, 1, 2 * PANELSIZE, 200);

	Arrow1->Visible = FALSE;
	Arrow2->Visible = FALSE;
	Arrow3->Visible = FALSE;

	Arrow1Grow = FALSE;
	Arrow2Grow = TRUE;
	Arrow3Grow = FALSE;

	CreatePaperScroll(GetCameraXPosition(), 150);

	/////////////////////////////////
	//		On Death			   //
	/////////////////////////////////

	///Last thing in initialize
	CreateDeathConfirmObjects(&newID);

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

	//EasyEditPlatform(Plat, 10);
	//EasyEditWall(Wall1 ,10);

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
		InitializePause(&DrawLevel31);
		//TogglePauseSound(&BackgroundSnd);
		//SetNextState(GS_MainMenu);
		UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
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
	{
		BlackOverlay->Position.x = GetCameraXPosition();
		BlackOverlay->Alpha += 1 * GetDeltaTime();
		if(BlackOverlay->Alpha > 1)
			SetNextState(GS_MapLevel);
	}



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
	}

	//Intel Fox Starting Narrative
	if(beginningAnimation == FALSE && !IntelFoxStart->hasPlayed)
	{
		PlayAudio(IntelFoxStart);
		IntelFoxStart->hasPlayed = TRUE;
	}
	
	if(!EnemyPanelNumber[0] && !EnemyPanelNumber[1] && !EnemyPanelNumber[2])
	{
		if(!IntelFoxEnd->hasPlayed && PlayerIsAlive)
		{
			PlayAudio(IntelFoxEnd);
			IntelFoxEnd->hasPlayed = TRUE;
		}
	}


	//If player dies
	if(CurrentPlayer.CurrentPlayerStats.CurrentHealth <= 0.0f)
	{
		PlayerIsAlive = FALSE;
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
