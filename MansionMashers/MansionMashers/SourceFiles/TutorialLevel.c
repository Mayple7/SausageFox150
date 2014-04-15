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

#define IRONDOORS 3

HUD *CurrentHUD;

Food *HealthPickup;
Food *AgilPickup;

Sprite *TutorialBackground;
Platform *Shelf;
Platform *ShortShelf;
Platform *BouncyBed;
Enemy *StrawDummy;
Enemy *StrawDummy2;

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
Wall *IronDoor[IRONDOORS];
Wall *DummyBlock;

float GettingImpatientTimer;    //A counter to give the player a hint (in seconds)
int   TimerGoingUp;

static int tutorialDone; // Keeps track of how much of the tutorial has been completed (Not a bool)
static int levelComplete;
static int numPanels;

void LoadTutorial(void)
{
	CreateTextureList();
	LoadEnemy(Dummy);
}

void InitializeTutorial(void)
{
	int i;
	Vec3 Tint = { 0, 0, 0 };

	levelComplete = FALSE;
	tutorialDone = 0;
	numPanels = 5;			//Kaden Update this if you add more panels -- what is a panel?
	ResetObjectList();
	ResetCamera();
	ResetEnemyPanelNumber();

	BackSnd = CreateSound("Sounds/TutorialTheme.mp3", LargeSnd);
	GettingImpatientTimer = 0;
	TimerGoingUp = TRUE;

	GongSnd = CreateSound("Sounds/GongHit.mp3", SmallSnd);

	//Set the camera so it currently isn't gated
	ResetGatedCamera();

	InitializePlayer(&CurrentPlayer, Ginko, -700.0f, GROUNDLEVEL + 1);

	CurrentPlayer.CurrentPlayerStats.CurrentHealth -= 40;

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
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 1040.0f, -950, 0); //Updated for new FR Controller
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

	//The great iron doors
	for (i = 0; i < IRONDOORS; ++i)
	{
		if (i != 1)
		{
			IronDoor[i] = CreateWall("TextureFiles/IronDoor.png", PANELSIZE, 1080, PANELSIZE * 2 + PANELSIZE * i, 0);
			UpdateCollider(&IronDoor[i]->WallCollider, 200, 580);
			IronDoor[i]->WallCollider.Offset.x = 870;
			IronDoor[i]->WallCollider.Offset.y = -130;
		}
		else
		{
			IronDoor[i] = CreateWall("TextureFiles/TutorialGrate.png", PANELSIZE, 1080, PANELSIZE * 2 + PANELSIZE * i, -100);
			UpdateCollider(&IronDoor[i]->WallCollider, 300, 580);
			IronDoor[i]->WallCollider.Offset.x = 60;
			IronDoor[i]->WallCollider.Offset.y = -130;
		}
		IronDoor[i]->WallSprite->ZIndex = 80;
	}

	//1 dummy in both panel 3 and panel 4
	StrawDummy = CreateEnemy(Dummy, EnemyType, 240 + PANELSIZE * 2, -250, 2);
	DummyBlock = CreateWall("TextureFiles/BlankPlatform.png", StrawDummy->EnemySprite->Width / 2, StrawDummy->EnemySprite->Height / 2, 
				            StrawDummy->Position.x, StrawDummy->Position.y - 40);
	DummyBlock->WallSprite->Visible = FALSE;

	StrawDummy2 = CreateEnemy(Dummy, EnemyType, 440 + PANELSIZE * 3, -250, 3);
	StrawDummy2->CurrentEnemyStats.CurrentHealth = 10;

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
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 640, -110, 10, -1, 3, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 810, -270, 201, -1, 3, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	Shelf = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 480, 60, 500, 20);
	Shelf->PlatformSprite->Visible = FALSE;

	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 500.0f, 220, -180); //Tall
	WallTemp->WallSprite->Visible = FALSE;
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 350.0f, 50, -350); //Short
	WallTemp->WallSprite->Visible = FALSE;

	/*////////////////////////////////
	//          PANEL TWO           //
	////////////////////////////////*/
	TutorialBackground = (Sprite *) CreateSprite("TextureFiles/TutorialPanel2.png", 1920, 1080, 0, 1, 1, PANELSIZE, 0);
	DoorOverlay = (Sprite *) CreateSprite("TextureFiles/TutorialPanelDoor.png", 1920, 1080, 200, 1, 1, PANELSIZE, 0);

	//Create the shelf sprite and initialize to be collidable
	Shelf = CreatePlatform("TextureFiles/Shelf.png", PlatformType, 184.5f, 367.5, 160 + PANELSIZE, -156);
	UpdateCollider(&Shelf->PlatformCollider, Shelf->PlatformCollider.width, Shelf->PlatformCollider.height * 0.16f);
	Shelf->PlatformCollider.Offset.y = Shelf->PlatformSprite->Height * 3 / 8;

	ShortShelf = CreatePlatform("TextureFiles/ShortShelf.png", PlatformType, 184.5f, 198.75f, -40 + PANELSIZE, -240);
	ShortShelf->PlatformCollider.Offset.y = 5 * ShortShelf->PlatformSprite->Height / 16;
	UpdateCollider(&ShortShelf->PlatformCollider, ShortShelf->PlatformCollider.width, ShortShelf->PlatformCollider.height * 0.2f);

	BouncyBed = CreatePlatform("TextureFiles/BlankPlatform.png", BounceType, 360.0f, 100.0f, -580 + PANELSIZE, -320);
	BouncyBed->PlatformSprite->Visible = FALSE;
	BouncyBed->PlatformRigidBody.Restitution = 2.2f;

	StarterAxe = CreateDroppedWeapon(Axe, Common, 256, 256, -550 + PANELSIZE, -270);
	StarterAxe->WeaponSprite->Rotation = (float)-FOX_PI / 3;

	StarterSword = CreateDroppedWeapon(Sword, Common, 250, 250, 160 + PANELSIZE, 0);
	StarterSword->WeaponSprite->Rotation = FOX_PI /4;

	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 640 + PANELSIZE, -110, 10, -1, 3, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 810 + PANELSIZE, -270, 201, -1, 3, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	/*////////////////////////////////
	//         PANEL THREE          //
	////////////////////////////////*/
	CreateSprite("TextureFiles/TutorialPanel3.png", 1920, 1080, 0, 1, 1, PANELSIZE * 2, 0);
	CreateSprite("TextureFiles/TutorialPanelDoor.png", 1920, 1080, 200, 1, 1, PANELSIZE * 2, 0);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 640 + PANELSIZE * 2, -110, 10, -1, 3, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 810 + PANELSIZE * 2, -270, 201, -1, 3, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	/*////////////////////////////////
	//         PANEL FOUR           //
	////////////////////////////////*/
	CreateSprite("TextureFiles/TutorialPanel4.png", 1920, 1080, 0, 1, 1, PANELSIZE * 3, 0);
	CreateSprite("TextureFiles/TutorialPanel4Door.png", 1920, 1080, 200, 1, 1, PANELSIZE * 3, 0);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 640 + PANELSIZE * 3, -110, 10, -1, 3, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 810 + PANELSIZE * 3, -270, 201, -1, 3, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	/*////////////////////////////////
	//         PANEL FIVE           //
	////////////////////////////////*/
	CreateSprite("TextureFiles/TutorialPanel3.png", 1920, 1080, 0, 1, 1, PANELSIZE * 4, 0);
	CreateSprite("TextureFiles/TutorialPanelDoor.png", 1920, 1080, 200, 1, 1, PANELSIZE * 4, 0);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 640 + PANELSIZE * 4, -110, 10, -1, 3, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 810 + PANELSIZE * 4, -270, 201, -1, 3, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	HealthPickup = CreateFood(Heal, 100, 100, PANELSIZE * 4 + 144, -180);
	AgilPickup = CreateFood(Agility, 100, 100, PANELSIZE * 4 - 144, -260);

	/////////////////////////////////
	//		  WIN AND LOSE		   //
	/////////////////////////////////
	CreateDeathConfirmObjects();
	CreateUpgradeScreenObjects();
}
 
void UpdateTutorial(void)
{
	Vec2 newPosition;
	int i;

	//Handle the special events right off the bat yo
	EventTutorial();

	ParticleSystemUpdate();
	UpdateAllProjectiles();
	BoundingBoxUpdate();
	
	PlayAudio(BackSnd);

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
		TextAllVisible(TutorialText);
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
		TextAllVisible(TutorialText);
	}
	else if (tutorialDone < 3 && CurrentPlayer.PlayerCollider.Position.x > (PANELSIZE / 2))
	{
		tutorialDone = 3;

		//Text
		Vec2Set(&newPosition, 1920, 280);
		ChangeTextString(TutorialText, "Use E to Change Weapons.");
		ChangeTextPosition(TutorialText, newPosition, Center);
		ChangeTextZIndex(TutorialText, 801);
		TextAllVisible(TutorialText);
	}
	else if (tutorialDone < 4 && CurrentPlayer.PlayerCollider.Position.x > (PANELSIZE / 2) * 3)
	{
		tutorialDone = 4;

		//Text
		Vec2Set(&newPosition, 1920 * 2, 220);
		ChangeTextString(TutorialText, "Use Left Mouse or N to Fight!");
		ChangeTextPosition(TutorialText, newPosition, Center);
		ChangeTextZIndex(TutorialText, 801);
		TextAllVisible(TutorialText);
	}
	else if (tutorialDone < 5 && StrawDummy->CurrentEnemyStats.CurrentHealth <= 0)
	{
		tutorialDone = 5;
	}
	else if (tutorialDone < 6 && CurrentPlayer.PlayerCollider.Position.x > (PANELSIZE / 2) * 5)
	{
		tutorialDone = 6;

		//Text
		Vec2Set(&newPosition, 1920 * 3, 220);
		ChangeTextString(TutorialText, "Right Mouse or M to Wind Attack.");
		ChangeTextPosition(TutorialText, newPosition, Center);
		ChangeTextZIndex(TutorialText, 801);
		TextAllVisible(TutorialText);
	}
	else if (tutorialDone < 7 && StrawDummy2->CurrentEnemyStats.CurrentHealth <= 0)
	{
		tutorialDone = 7;

		//Text
		Vec2Set(&newPosition, 1920 * 4, 220);
		ChangeTextString(TutorialText, "Grab the 2 buffs!");
		ChangeTextPosition(TutorialText, newPosition, Center);
		ChangeTextZIndex(TutorialText, 801);
		TextAllVisible(TutorialText);
	}
	else if (tutorialDone < 8 && HealthPickup->objID <= 0 && AgilPickup->objID <= 0)
	{
		tutorialDone = 8;

		//Text
		Vec2Set(&newPosition, 1920 * 4, 300);
		ChangeTextString(TutorialText, "Q to switch buffs.");
		ChangeTextPosition(TutorialText, newPosition, Center);
		ChangeTextZIndex(TutorialText, 801);
		TextAllVisible(TutorialText);

		Vec2Set(&newPosition, 1920 * 4, 220);
		ChangeTextString(TutorialTextra, "Use buffs with F! Heal yourself!");
		ChangeTextPosition(TutorialTextra, newPosition, Center);
		ChangeTextZIndex(TutorialTextra, 801);
		TextAllVisible(TutorialText);
	}
	else if (tutorialDone < 9 && CurrentPlayer.PlayerCollider.Position.x > (PANELSIZE / 2) * 8
		  && CurrentPlayer.CurrentPlayerStats.CurrentHealth == CurrentPlayer.CurrentPlayerStats.MaxHealth
		  && HealthPickup->objID <= 0 && AgilPickup->objID <= 0)
	{
		tutorialDone = 9;

		//Text
		Vec2Set(&newPosition, 1920 * 4, 200);
		ChangeTextString(TutorialText, "Go Right to Continue.");
		ChangeTextPosition(TutorialText, newPosition, Center);
		ChangeTextZIndex(TutorialText, 801);
		TextAllVisible(TutorialText);

		Vec2Set(&newPosition, 1920 * 4, -2000);
		ChangeTextPosition(TutorialTextra, newPosition, Center);
	}

	//Navigation help
	if (TimerGoingUp)
		GettingImpatientTimer += GetDeltaTime();
	else
		GettingImpatientTimer -= GetDeltaTime();
	if (GettingImpatientTimer > 0)
	{
		ChangeTextAlpha(TutorialText, GettingImpatientTimer);
		ChangeTextAlpha(TutorialTextra, GettingImpatientTimer);

		if (GettingImpatientTimer > 1)
			TimerGoingUp = FALSE;
		else if (GettingImpatientTimer < 0.4)
			TimerGoingUp = TRUE;
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
	else 
	{
		//Make the doors rise when needed
		for (i = 0; i < IRONDOORS; ++i)
		{
			if (tutorialDone > 4 + i * 2 && IronDoor[i]->Position.y < 300)
				UpdateWallPosition(IronDoor[i], IronDoor[i]->Position.x, IronDoor[i]->Position.y + 160 * GetDeltaTime());
		}
	}

	//If the dummy exists, prevent the player from moving past
	if(DummyBlock)
	{
		if(StrawDummy->objID > 0)
		{
			//Do nothing
		}
		else
		{
			UpdateWallPosition(DummyBlock, StrawDummy2->Position.x, DummyBlock->Position.y);

			if (StrawDummy2->objID <= 0)
			{
				FreeWall(DummyBlock);
				DummyBlock = NULL;
			}
		}
	}

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
			SelectiveInput(&CurrentPlayer, 0, 0, 0, 0);
			break;
		case 1:
			SelectiveInput(&CurrentPlayer, 1, 0, 0, 0);
			break;
		case 2:
		case 3:
			SelectiveInput(&CurrentPlayer, 1, 1, 0, 0);
			break;
		case 4:
		case 5:
			SelectiveInput(&CurrentPlayer, 1, 1, 1, 0);
			break;
		default:
			SelectiveInput(&CurrentPlayer, 1, 1, 1, 1);
			break;
		}

		UpdateHUDItems(CurrentHUD, &CurrentPlayer);
	}
	else
		LevelCompletion();

	/*////////////////////////////////
	//    CAMERA POSITION SECOND    //
	////////////////////////////////*/

	SetUpCameraPanAndLockNoSpawner(&levelComplete, PANELSIZE, numPanels);
	UpdateBlockerBoxes(PANELSIZE);

	/*////////////////////////////////
	//       EVERYTHING ELSE        //
	////////////////////////////////*/
	UpdateAllEnemies();

	UpdateFloatingText();
}
