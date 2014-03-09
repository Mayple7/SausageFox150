/*****************************************************************************/
/*!
\file				TutorialLevel.c
\author				Dan Muller (d.muller)
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

Weapon *StarterAxe;
Weapon *StarterSword;

FoxSound *BackSnd;
FoxSound *GongSnd;

Wall *WallTemp;

static int newID;
static int levelComplete;

void LoadTutorial(void)
{
	CreateTextureList();
}

void InitializeTutorial(void)
{
	levelComplete = FALSE;
	newID = 10;
	ResetObjectList();
	ResetCamera();

	BackSnd = CreateSound("Sounds/wave.mp3", SmallSnd);
	GongSnd = CreateSound("Sounds/GongHit.wav", SmallSnd);

	InitializePlayer(&CurrentPlayer, Ginko, 50.0f, GROUNDLEVEL * GetLoadRatio() + 1);

	CurrentHUD = CreateHUD(&CurrentPlayer);

	//Bounding Boxes
	CreateBoundingBoxes();

	//Black fade part
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 1920, 0);
	BlackOverlay->Alpha = 0;
	Vec3Set(&BlackOverlay->Tint, 0, 0, 0);

	//Mansion mashers' logo
	GameLogo = (Sprite *) CreateSprite("TextureFiles/MansionMashersLogo.png", 1920, 1080, 4001, 1, 1, 1920, 0);
	GameLogo->Alpha = 0;

	//Invisible walls
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 160.0f, 500.0f, newID++, 865, 130);
	WallTemp->WallSprite->Visible = FALSE;
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 1040.0f, newID++, -900, 0);
	WallTemp->WallSprite->Visible = FALSE;
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 160.0f, 500.0f, newID++, 2785, 130);
	WallTemp->WallSprite->Visible = FALSE;

	//Sound volume
	SetChannelGroupVolume(EffectType, SFXVolume);
	SetChannelGroupVolume(MusicType, BGMVolume);

	/*////////////////////////////////
	//          PANEL ONE           //
	////////////////////////////////*/
	TutorialBackground = (Sprite *) CreateSprite("TextureFiles/MansionBedRoom1.png", 1920, 1080, 0, 1, 1, 0, 0);
	DoorOverlay = (Sprite *) CreateSprite("TextureFiles/MansionBedRoomDoor.png", 1920, 1080, 200, 1, 1, 0, 0);

	//Create the shelf sprite and initialize to be collidable
	Shelf = CreatePlatform("TextureFiles/Shelf.png", PlatformType, 184.5f, 367.5, newID++, 160, -156);
	UpdateCollider(&Shelf->PlatformCollider, Shelf->PlatformCollider.width, Shelf->PlatformCollider.height * 0.16f); 
	Shelf->PlatformCollider.Offset.y = Shelf->PlatformSprite->Height * 3 / 8;

	ShortShelf = CreatePlatform("TextureFiles/ShortShelf.png", PlatformType, 184.5f, 198.75f, newID++, -40, -240);
	ShortShelf->PlatformCollider.Offset.y = 5 * ShortShelf->PlatformSprite->Height / 16;
	UpdateCollider(&ShortShelf->PlatformCollider, ShortShelf->PlatformCollider.width, ShortShelf->PlatformCollider.height * 0.2f); 

	BouncyBed = CreatePlatform("TextureFiles/BlankPlatform.png", BounceType, 375.0f, 100.0f, newID++, -580, -280);
	BouncyBed->PlatformSprite->Visible = FALSE;
	BouncyBed->PlatformRigidBody.Restitution = 2.2f;

	StarterAxe = CreateDroppedWeapon(Axe, Common, 256, 256, newID++, -550, -220);
	StarterAxe->WeaponSprite->Rotation = (float)-FOX_PI / 3;

	StarterSword = CreateDroppedWeapon(Sword, Common, 250, 250, newID++, 160, 0);
	StarterSword->WeaponSprite->Rotation = (float)FOX_PI /4;

	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 690, -110, 10, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 810, -280, 201, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	/*////////////////////////////////
	//          PANEL TWO           //
	////////////////////////////////*/
	CreateSprite("TextureFiles/MansionBedRoom2.png", 1920, 1080, 0, 1, 1, 1920.0f, 0);
	CreateSprite("TextureFiles/MansionBedRoomDoor.png", 1920, 1080, 200, 1, 1, 1920.0f, 0);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 690 + 1920.0f, -110, 10, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 810 + 1920.0f, -280, 201, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	StrawDummy = CreateEnemy(Dummy, EnemyType, newID++, 750 + 1920.0f, -250);
}
 
void UpdateTutorial(void)
{
	//Handle the special events right off the bat yo
	EventTutorial();

	ParticleSystemUpdate();
	BoundingBoxUpdate();
	
	PlayAudio(BackSnd);

	if(FoxInput_KeyTriggered('J'))
	{
		FreeEnemy(StrawDummy);
	}

	//This should be the last line in this function
	UpdatePlayerPosition(&CurrentPlayer);

	//If the dummy exists, prevent the player from moving past
	if(StrawDummy->objID > 0)
	{
		if(CurrentPlayer.PlayerCollider.Position.x + CurrentPlayer.PlayerCollider.width / 2 > StrawDummy->Position.x - StrawDummy->EnemyCollider.width / 2)
		{
			CurrentPlayer.Position.x = (StrawDummy->Position.x - StrawDummy->EnemyCollider.width / 2) - (CurrentPlayer.PlayerCollider.width / 2) - 1;
		}
		else if(CurrentPlayer.PlayerCollider.Position.x - CurrentPlayer.PlayerCollider.width / 2 < -7 * TutorialBackground->Width / 16)
		{
			CurrentPlayer.Position.x = (-7 * TutorialBackground->Width / 16) + (CurrentPlayer.PlayerCollider.width / 2) + 1;
		}
	}

	UpdateHUDPosition(CurrentHUD);
}

void DrawTutorial(void)
{
	DrawObjectList();
	DrawCollisionList();
}

void FreeTutorial(void)
{
	if(levelComplete)
		CurrentPlayer.CurrentLevel = GS_Level1;
	else
		CurrentPlayer.CurrentLevel = GS_Tutorial;

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
	//     HANDLE INPUT FIRST       //
	////////////////////////////////*/
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		InitializePause(&DrawTutorial);
		UpdatePause();
	}
	if(FoxInput_KeyTriggered('U'))
		SetDebugMode();
	if(FoxInput_KeyTriggered('I'))
		RemoveDebugMode();

	InputPlayer(&CurrentPlayer);

	/*////////////////////////////////
	//    CAMERA POSITION SECOND    //
	////////////////////////////////*/
	if(CurrentPlayer.Position.x > -(PANELSIZE / 2) * GetLoadRatio() && CurrentPlayer.Position.x < (PANELSIZE / 2) * GetLoadRatio())
		SetCameraPan(0.0f, PANELSIZE);
	else if(CurrentPlayer.Position.x > (PANELSIZE / 2) * GetLoadRatio() && CurrentPlayer.Position.x < (PANELSIZE + (PANELSIZE / 2)) * GetLoadRatio())
		SetCameraPan(PANELSIZE * GetLoadRatio(), PANELSIZE);
	else if(CurrentPlayer.Position.x > (PANELSIZE / 2) * 2 * GetLoadRatio() + CurrentPlayer.PlayerCollider.width)
	{
		levelComplete = TRUE;
	}

	/*////////////////////////////////
	//       EVERYTHING ELSE        //
	////////////////////////////////*/
	if(!levelComplete)
	{
		// Check for any collision and handle the results
		DetectPlayerCollision();
		UpdateHUDItems(CurrentHUD, &CurrentPlayer);
	}
	else
		fadeToEnd();

	if(StrawDummy->objID > 0)
		UpdateEnemy(StrawDummy);

	UpdateFloatingText();
}

void fadeToEnd(void)
{
	if(BlackOverlay->Alpha >= 1.0f)
	{
		BlackOverlay->Alpha = 1.0f;
		SetNextState(GS_MapLevel);
	}
	else
	{
		BlackOverlay->Alpha += GetDeltaTime();
	}
}
