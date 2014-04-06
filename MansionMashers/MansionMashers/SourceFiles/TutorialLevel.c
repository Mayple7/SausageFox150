/*****************************************************************************/
/*!
\file				TutorialLevel.c
\author				Kaden Nugent (kaden.n)
\date				Jan 31, 2014

\brief				Functions for the tutorial level

\par				Functions:
\li					LoadTutorial
\li					InitializeTutorial
\li					UpdateTutorial
\li					DrawTutorial
\li					FreeTutorial
\li					UnloadTutorial
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../AEEngine.h"
#include "../HeaderFiles/TutorialLevel.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"
#include "../HeaderFiles/BoundingBox.h"

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

#define PANELSIZE 1920.0f

HUD *CurrentHUD;

Sprite *TutorialBackground;
Platform *Shelf;
Platform *ShortShelf;
Platform *BouncyBed;
Enemy *StrawDummy;

Sprite *DoorOverlay;
Sprite *BlackOverlay;
Sprite *GameLogo;

Sprite *Seclude1;
Sprite *Seclude2;
Sprite *Seclude3;
Sprite *Seclude4;

TextGlyphs *TutorialText;
TextGlyphs *TutorialTextra; //For extra stuff

Weapon *StarterAxe;
Weapon *StarterSword;

FoxSound *BackSnd;
FoxSound *GongSnd;

Wall *WallTemp;
Wall *IronDoor;
Wall *DummyBlock;

static int tutorialDone; // Keeps track of how much of the tutorial has been completed (Not a bool)
static int levelComplete;
static int numPanels;

void LoadTutorial(void)
{
	CreateTextureList();
}

void InitializeTutorial(void)
{
	Vec3 Tint = { 0, 0, 0 };

	levelComplete = FALSE;
	tutorialDone = 0;
	numPanels = 3;			//Kaden Update this if you add more panels -- what is a panel?
	ResetObjectList();
	ResetCamera();
	ResetEnemyPanelNumber();

	BackSnd = CreateSound("Sounds/Temp.mp3", LargeSnd);
	GongSnd = CreateSound("Sounds/GongHit.wav", SmallSnd);

	//Set the camera so it currently isn't gated
	ResetGatedCamera();

	InitializePlayer(&CurrentPlayer, Ginko, -700.0f, GROUNDLEVEL + 1);

	CurrentHUD = CreateHUD(&CurrentPlayer);

	//Bounding Boxes
	CreateBoundingBoxes();

	//Black fade part
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	Vec3Set(&BlackOverlay->Tint, 0, 0, 0);

	//Mansion mashers' logo
	GameLogo = (Sprite *) CreateSprite("TextureFiles/MansionMashersLogo.png", 1920, 1080, 4001, 1, 1, 1920 * 2, 0);
	GameLogo->Alpha = 0;

	//Invisible walls
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 1040.0f, -900, 0);
	WallTemp->WallSprite->Visible = FALSE;
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 240.0f, 500.0f, 840, 200);
	WallTemp->WallSprite->Visible = FALSE;
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 240.0f, 500.0f, 840 + 1920, 200);
	WallTemp->WallSprite->Visible = FALSE;
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 240.0f, 500.0f, 840 + 1920 * 2, 200);
	WallTemp->WallSprite->Visible = FALSE;

	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 320.0f, 340.0f, 800, 280);            //Above arch, room 1
	WallTemp->WallSprite->Visible = FALSE;
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 320.0f, 340.0f, 800 + 1920, 280);     //Above arch, room 2
	WallTemp->WallSprite->Visible = FALSE;
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 320.0f, 340.0f, 800 + 1920 * 2, 280); //Above arch, room 3
	WallTemp->WallSprite->Visible = FALSE;

	//The great iron door
	IronDoor = CreateWall("TextureFiles/IronDoor.png", PANELSIZE, 1080, PANELSIZE * 2, 0);
	IronDoor->WallSprite->ZIndex = 80;
	UpdateCollider(&IronDoor->WallCollider, 200, 380);
	IronDoor->WallCollider.Offset.x = 870;
	IronDoor->WallCollider.Offset.y = -230;

	// Bounding Box Walls
	CreateBlockerBoxes();

	// Seclusion overlay for the player to focus on things in the tutorial
	Seclude1 = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 600, 800, 1, 1, 0, 240); //Top
	Seclude1->Alpha = 0.9f;
	Seclude1->Tint  = Tint;
	Seclude2 = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 100, 800, 1, 1, 0, -490); //Bottom
	Seclude2->Alpha = 0.9f;
	Seclude2->Tint  = Tint;
	Seclude3 = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 100, 380, 800, 1, 1, -910, -250); //Left
	Seclude3->Alpha = 0.9f;
	Seclude3->Tint  = Tint;
	Seclude4 = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1200, 380, 800, 1, 1, 360, -250); //Right
	Seclude4->Alpha = 0.9f;
	Seclude4->Tint  = Tint;

	//Some text to use for whatever tutorial stuff
	Vec3Set(&Tint, 1, 1, 1);
	TutorialText = CreateText("Use A,D to move Left,Right.", -300, 200, 100, Tint, Center, Border);
	ChangeTextZIndex(TutorialText, 801);
	ChangeTextVisibility(TutorialText);

	TutorialTextra = CreateText("Extra Text.", -300, -2000, 100, Tint, Center, Border);
	ChangeTextZIndex(TutorialTextra, 801);
	ChangeTextVisibility(TutorialTextra);

	//Sound volume
	SetChannelGroupVolume(EffectType, SFXVolume);
	SetChannelGroupVolume(MusicType, BGMVolume);

	/*////////////////////////////////
	//          PANEL ONE           //
	////////////////////////////////*/
	CreateSprite("TextureFiles/TutorialPanel1.png", 1920, 1080, 0, 1, 1, 0, 0);
	CreateSprite("TextureFiles/TutorialPanel1Door.png", 1920, 1080, 200, 1, 1, 0, 0);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 640, -110, 10, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 810, -270, 201, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	Shelf = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 480, 60, 500, 20);
	Shelf->PlatformSprite->Visible = FALSE;

	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 500.0f, 220, -180); //Tall
	WallTemp->WallSprite->Visible = FALSE;
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 350.0f, 50, -350); //Short
	WallTemp->WallSprite->Visible = FALSE;

	/*////////////////////////////////
	//          PANEL TWO           //
	////////////////////////////////*/
	TutorialBackground = (Sprite *) CreateSprite("TextureFiles/TutorialPanel2.png", 1920, 1080, 0, 1, 1, 1920.0f, 0);
	DoorOverlay = (Sprite *) CreateSprite("TextureFiles/TutorialPanelDoor.png", 1920, 1080, 200, 1, 1, 1920.0f, 0);

	//Create the shelf sprite and initialize to be collidable
	Shelf = CreatePlatform("TextureFiles/Shelf.png", PlatformType, 184.5f, 367.5, 160 + 1920.0f, -156);
	UpdateCollider(&Shelf->PlatformCollider, Shelf->PlatformCollider.width, Shelf->PlatformCollider.height * 0.16f);
	Shelf->PlatformCollider.Offset.y = Shelf->PlatformSprite->Height * 3 / 8;

	ShortShelf = CreatePlatform("TextureFiles/ShortShelf.png", PlatformType, 184.5f, 198.75f, -40 + 1920.0f, -240);
	ShortShelf->PlatformCollider.Offset.y = 5 * ShortShelf->PlatformSprite->Height / 16;
	UpdateCollider(&ShortShelf->PlatformCollider, ShortShelf->PlatformCollider.width, ShortShelf->PlatformCollider.height * 0.2f);

	BouncyBed = CreatePlatform("TextureFiles/BlankPlatform.png", BounceType, 360.0f, 100.0f, -580 + 1920.0f, -320);
	BouncyBed->PlatformSprite->Visible = FALSE;
	BouncyBed->PlatformRigidBody.Restitution = 2.2f;

	StarterAxe = CreateDroppedWeapon(Axe, Common, 256, 256, -550 + 1920.0f, -270);
	StarterAxe->WeaponSprite->Rotation = (float)-FOX_PI / 3;

	StarterSword = CreateDroppedWeapon(Sword, Common, 250, 250, 160 + 1920.0f, 0);
	StarterSword->WeaponSprite->Rotation = FOX_PI /4;

	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 640 + 1920.0f, -110, 10, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 810 + 1920.0f, -270, 201, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	/*////////////////////////////////
	//         PANEL THREE          //
	////////////////////////////////*/
	CreateSprite("TextureFiles/TutorialPanel3.png", 1920, 1080, 0, 1, 1, 1920.0f * 2, 0);
	CreateSprite("TextureFiles/TutorialPanelDoor.png", 1920, 1080, 200, 1, 1, 1920.0f * 2, 0);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 640 + 1920.0f * 2, -110, 10, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 810 + 1920.0f * 2, -270, 201, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	StrawDummy = CreateEnemy(Dummy, EnemyType, 240 + 1920.0f * 2, -250, 2);
	DummyBlock = CreateWall("TextureFiles/BlankPlatform.png", StrawDummy->EnemySprite->Width / 2, StrawDummy->EnemySprite->Height / 2, 
				            StrawDummy->Position.x, StrawDummy->Position.y - 40);
	DummyBlock->WallSprite->Visible = FALSE;
}
 
void UpdateTutorial(void)
{
	Vec2 newPosition;

	//Handle the special events right off the bat yo
	EventTutorial();

	ParticleSystemUpdate();
	UpdateAllProjectiles();
	BoundingBoxUpdate();
	
	PlayAudio(BackSnd);

	if(FoxInput_KeyTriggered('J'))
	{
		FreeEnemy(StrawDummy);
	}

	//This should be the last line in this function
	UpdatePlayerPosition(&CurrentPlayer);

	/*////////////////////////////////
	//     TUTORIAL COMPLETION      //
	////////////////////////////////*/
	if (tutorialDone < 1 && CurrentPlayer.PlayerCollider.Position.x > -320)
	{
		tutorialDone = 1;

		Seclude1->Position.y = 490;//Top
		Seclude1->Height     = 100;
		UpdateMesh(Seclude1);

		Seclude3->Position.y = 0;//Left
		Seclude3->Height     = 880;
		UpdateMesh(Seclude3);

		Seclude4->Position.x = 610;//Right
		Seclude4->Position.y = 0;
		Seclude4->Height     = 880;
		Seclude4->Width      = 700;
		UpdateMesh(Seclude4);

		//Text
		Vec2Set(&newPosition, -100, 400);
		ChangeTextString(TutorialText, "Use SPACE to Jump.");
		ChangeTextPosition(TutorialText, newPosition, Center);
		ChangeTextZIndex(TutorialText, 801);

		//Seclude1 = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 600, 800, 1, 1, 0, 240); //Top
		//Seclude2 = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 100, 800, 1, 1, 0, -490); //Bottom
		//Seclude3 = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 100, 380, 800, 1, 1, -960, -250); //Left
		//Seclude4 = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1200, 380, 800, 1, 1, 360, -250); //Right
	}
	else if (tutorialDone < 2 && CurrentPlayer.PlayerCollider.Position.x > 150)
	{
		tutorialDone = 2;

		Seclude4->Position.x = 910;//Right
		Seclude4->Width      = 100;
		UpdateMesh(Seclude4);

		//Text
		Vec2Set(&newPosition, 100, 400);
		ChangeTextString(TutorialText, "Use S to Fall Through.");
		ChangeTextPosition(TutorialText, newPosition, Center);
		ChangeTextZIndex(TutorialText, 801);
	}
	else if (tutorialDone < 3 && CurrentPlayer.PlayerCollider.Position.x > (1920.0f / 2))
	{
		tutorialDone = 3;

		//Text
		Vec2Set(&newPosition, 1920, 280);
		ChangeTextString(TutorialText, "Use E to Change Weapons.");
		ChangeTextPosition(TutorialText, newPosition, Center);
		ChangeTextZIndex(TutorialText, 801);
	}
	else if (tutorialDone < 4 && CurrentPlayer.PlayerCollider.Position.x > (1920.0f / 2) * 3)
	{
		tutorialDone = 4;

		//Text
		Vec2Set(&newPosition, 1920 * 2, 300);
		ChangeTextString(TutorialText, "Use Left Mouse or N to Fight!");
		ChangeTextPosition(TutorialText, newPosition, Center);
		ChangeTextZIndex(TutorialText, 801);

		Vec2Set(&newPosition, 1920 * 2, 220);
		ChangeTextString(TutorialTextra, "Right Mouse or M to Wind Attack.");
		ChangeTextPosition(TutorialTextra, newPosition, Center);
		ChangeTextZIndex(TutorialTextra, 801);
	}
	else if (tutorialDone < 5 && StrawDummy->CurrentEnemyStats.CurrentHealth <= 0)
	{
		tutorialDone = 5;

		//Text
		Vec2Set(&newPosition, 1920 * 2, 200);
		ChangeTextString(TutorialText, "Go Right to Continue.");
		ChangeTextPosition(TutorialText, newPosition, Center);
		ChangeTextZIndex(TutorialText, 801);

		Vec2Set(&newPosition, 1920 * 2, -2000);
		ChangeTextPosition(TutorialTextra, newPosition, Center);
	}

	//Make the bars slide away naturally
	Seclude1->Position.x = GetCameraXPosition();
	Seclude2->Position.x = GetCameraXPosition();

	if (tutorialDone > 2 && Seclude1->Visible && Seclude2->Visible)
	{
		Seclude1->Position.y += 100 * GetDeltaTime();
		Seclude2->Position.y -= 100 * GetDeltaTime();
		Seclude3->Position.x  = GetCameraXPosition() - 910;
		Seclude4->Position.x  = GetCameraXPosition() + 910;
		Seclude3->Height     += 200 * GetDeltaTime();
		Seclude4->Height     += 200 * GetDeltaTime();
		Seclude3->Alpha      -= 1.0f * GetDeltaTime();
		Seclude4->Alpha      -= 1.0f * GetDeltaTime();
		UpdateMesh(Seclude3);
		UpdateMesh(Seclude4);

		if (Seclude1->Position.y > 600)
		{
			Seclude1->Visible = FALSE;
			Seclude2->Visible = FALSE;
			Seclude3->Visible = FALSE;
			Seclude4->Visible = FALSE;
		}
	}
	else if (tutorialDone > 4 && IronDoor->Position.y < 300)
		UpdateWallPosition(IronDoor, IronDoor->Position.x, IronDoor->Position.y + 160 * GetDeltaTime());

	//If the dummy exists, prevent the player from moving past
	if(StrawDummy->objID > 0)
	{
		/*if(CurrentPlayer.PlayerCollider.Position.x + CurrentPlayer.PlayerCollider.width / 2 > StrawDummy->Position.x - StrawDummy->EnemyCollider.width / 2)
		{
			CurrentPlayer.Position.x = (StrawDummy->Position.x - StrawDummy->EnemyCollider.width / 2) - (CurrentPlayer.PlayerCollider.width / 2) - 1;
		}
		else if(CurrentPlayer.PlayerCollider.Position.x - CurrentPlayer.PlayerCollider.width / 2 < -7 * TutorialBackground->Width / 16)
		{
			CurrentPlayer.Position.x = (-7 * TutorialBackground->Width / 16) + (CurrentPlayer.PlayerCollider.width / 2) + 1;
		}*/
	}
	else
		FreeWall(DummyBlock);

	if (tutorialDone > 2)
		UpdateHUDPosition(CurrentHUD);
	else
	{
		//DONT SHOW THE HUD YET
		Vec2Set(&newPosition, (GetCameraXPosition() - 685 - 1000), 300);

		//HUD items update position
		CurrentHUD->HUDBackground->Position.x = (GetCameraXPosition() - 735 - 1000);

		CurrentHUD->HUDItem[0]->Position.x = (GetCameraXPosition() - 800 - 1000);
		CurrentHUD->HUDItem[1]->Position.x = (GetCameraXPosition() - 800 - 1000);
		CurrentHUD->HUDItem[2]->Position.x = (GetCameraXPosition() - 800 - 1000);
		CurrentHUD->HUDItem[3]->Position.x = (GetCameraXPosition() - 800 - 1000);

		CurrentHUD->AbilityBar->Position.x = (GetCameraXPosition() - 800 - 1000);
		CurrentHUD->AbilityBarEnd->Position.x = (GetCameraXPosition() - 800 - 1000);
		CurrentHUD->HealthBar->Position.x = (GetCameraXPosition() - 800 - 1000);
		CurrentHUD->HealthBarEnd->Position.x = (GetCameraXPosition() - 800 - 1000);
		CurrentHUD->ExpBar->Position.x = (GetCameraXPosition() - 800 - 1000);
		CurrentHUD->ExpBarEnd->Position.x = (GetCameraXPosition() - 800 - 1000);

		//Text update position
		ChangeTextPosition(CurrentHUD->CoinText, newPosition, Center);
		ChangeTextPosition(CurrentHUD->FPSText, newPosition, Center);
		ChangeTextPosition(CurrentHUD->RankText, newPosition, Center);
	}
}

void DrawTutorial(void)
{
	DrawObjectList();
	DrawCollisionList();
}

void FreeTutorial(void)
{
	if(levelComplete && CurrentPlayer.CurrentLevel < GS_Level1)
	{
		CurrentPlayer.levelClearBitFlags |= 1;
		CurrentPlayer.CurrentLevel = GS_Level1;
	}
	else if(CurrentPlayer.CurrentLevel < GS_Tutorial)
		CurrentPlayer.CurrentLevel = GS_Tutorial;

	//Only save stats if the level was actually completed
	if (levelComplete)
		SavePlayer(&CurrentPlayer);

	FreeAllLists();
	FreeHUD(CurrentHUD);
}

void UnloadTutorial(void)
{
	DestroyTextureList();
	DestroyPlayer(&CurrentPlayer);
}

void EventTutorial(void)
{
	/*////////////////////////////////
	//   INPUT & COLLISION FIRST    //
	////////////////////////////////*/
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		TogglePauseSound(BackSnd);
		InitializePause(&DrawTutorial);
		UpdatePause();
		TogglePauseSound(BackSnd);
	}
	if(FoxInput_KeyTriggered('U'))
		SetDebugMode();
	if(FoxInput_KeyTriggered('I'))
		RemoveDebugMode();

	if(!levelComplete)
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
		}

		// Check for any collision and handle the results
		DetectPlayerCollision();

		// Handle any input for the current player [SPECIAL FOR TUTORIAL LEVEL]
		switch(tutorialDone)
		{
		case 0:
			SelectiveInput(&CurrentPlayer, 0, 0, 0);
			break;
		case 1:
			SelectiveInput(&CurrentPlayer, 1, 0, 0);
			break;
		case 2:
			SelectiveInput(&CurrentPlayer, 1, 1, 0);
			break;
		default:
			SelectiveInput(&CurrentPlayer, 1, 1, 1);
			break;
		}

		UpdateHUDItems(CurrentHUD, &CurrentPlayer);
	}
	else
		fadeToEnd();

	/*////////////////////////////////
	//    CAMERA POSITION SECOND    //
	////////////////////////////////*/

	SetUpCameraPanAndLockNoSpawner(&levelComplete, PANELSIZE, numPanels);
	UpdateBlockerBoxes(PANELSIZE);

	/*////////////////////////////////
	//       EVERYTHING ELSE        //
	////////////////////////////////*/
	if(StrawDummy->objID > 0)
		UpdateEnemy(StrawDummy);

	UpdateFloatingText();
}

void fadeToEnd(void)
{
	BlackOverlay->Position.x = GetCameraXPosition();
	BlackOverlay->Alpha += 1 * GetDeltaTime();
	if(BlackOverlay->Alpha > 1)
		SetNextState(GS_MapLevel);
}
