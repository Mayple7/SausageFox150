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

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

HUD* CurrentHUD;

//Bounding Boxes
Sprite *BoundTop;
Sprite *BoundBottom;
Sprite *BoundLeft;
Sprite *BoundRight;

Sprite* TutorialBackground;
Sprite* OverlayGrid;
Platform* Shelf;
Platform* ShortShelf;
Platform* BouncyBed;
Enemy* StrawDummy;

Sprite* DoorOverlay;
Sprite* BlackOverlay;
Sprite* GameLogo;

Weapon* StarterAxe;
Weapon* StarterSword;

FoxSound *BackSnd;
FoxSound *GongSnd;

static int newID;
static int levelComplete;

void LoadTutorial(void)
{
	CreateTextureList();
}

void InitializeTutorial(void)
{
	Vec3 BoundingTint;

	levelComplete = FALSE;
	FoxInput_Update();
	Vec3Set(&BoundingTint, 0.0f, 0.0f, 0.0f);

	newID = 10;
	ResetObjectList();

	BackSnd = CreateSound("Sounds/wave.mp3", SmallSnd);
	GongSnd = CreateSound("Sounds/GongHit.wav", SmallSnd);

	InitializePlayer(&CurrentPlayer, Ginko, 50.0f, GROUNDLEVEL * GetLoadRatio() + 1);

	CurrentHUD = CreateHUD(&CurrentPlayer);

	TutorialBackground = (Sprite *) CreateSprite("TextureFiles/TutorialBackground.png", 1920, 1080, 0, 1, 1, 0, 0);
	OverlayGrid = (Sprite *) CreateSprite("TextureFiles/OverlayGrid.png", 2000, 1080, 100, 1, 1, 0, 0);

	//Bounding Boxes
	BoundTop = (Sprite *) CreateSprite("TextureFiles/BoundingBox.png", 1920, 1080, 5000, 1, 1, 0, 1080);
	BoundBottom = (Sprite *) CreateSprite("TextureFiles/BoundingBox.png", 1920, 1080, 5000, 1, 1, 0, -1080);
	BoundLeft = (Sprite *) CreateSprite("TextureFiles/BoundingBox.png", 1920, 1080, 5000, 1, 1, -1920, 0);
	BoundRight = (Sprite *) CreateSprite("TextureFiles/BoundingBox.png", 1920, 1080, 5000, 1, 1, 1920, 0);

	BoundTop->Tint = BoundingTint;
	BoundBottom->Tint = BoundingTint;
	BoundLeft->Tint = BoundingTint;
	BoundRight->Tint = BoundingTint;

	// Create the shelf sprite and initialize to be collidable
	Shelf = CreatePlatform("TextureFiles/Shelf.png", PlatformType, 184.5f, 367.5, newID++, 475, -170);
	UpdateCollider(&Shelf->PlatformCollider, Shelf->PlatformCollider.width, Shelf->PlatformCollider.height * 0.16f); 
	Shelf->PlatformCollider.Offset.y = Shelf->PlatformSprite->Height * 3 / 8;

	ShortShelf = CreatePlatform("TextureFiles/ShortShelf.png", PlatformType, 184.5f, 198.75f, newID++, 280, -280);
	ShortShelf->PlatformCollider.Offset.y = 5 * ShortShelf->PlatformSprite->Height / 16;
	UpdateCollider(&ShortShelf->PlatformCollider, ShortShelf->PlatformCollider.width, ShortShelf->PlatformCollider.height * 0.2f); 

	BouncyBed = CreatePlatform("TextureFiles/BlankPlatform.png", BounceType, 375.0f, 100.0f, newID++, -225, -350);
	BouncyBed->PlatformSprite->Visible = FALSE;
	BouncyBed->PlatformRigidBody.Restitution = 2.2f;

	StarterAxe = CreateDroppedWeapon(Axe, Common, 256, 256, newID++, -200, -300);
	StarterAxe->WeaponSprite->Rotation = (float)-FOX_PI / 3;

	StarterSword = CreateDroppedWeapon(Sword, Common, 250, 250, newID++, 475, 0);
	StarterSword->WeaponSprite->Rotation = (float)FOX_PI /4;

	StrawDummy = CreateEnemy(Dummy, EnemyType, newID++, 750, -250);

	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Alpha = 0;
	Vec3Set(&BlackOverlay->Tint, 0, 0, 0);

	GameLogo = (Sprite *) CreateSprite("TextureFiles/MansionMashersLogo.png", 1920, 1080, 4001, 1, 1, 0, 0);
	GameLogo->Alpha = 0;

	DoorOverlay = (Sprite *) CreateSprite("TextureFiles/DoorOverlay.png", 1920, 1080, 200, 1, 1, 0, 0);

	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 745, -85, 10, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 900, -205, 201, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	SetChannelGroupVolume(EffectType, SFXVolume);
	SetChannelGroupVolume(MusicType, BGMVolume);

	RemoveDebugMode();
	OverlayGrid->Visible = FALSE;

	ResetCamera();
}
 
void UpdateTutorial(void)
{
	// Handle any events such as collision
	EventTutorial();
	UpdateHUDPosition(CurrentHUD);
	// Update the player position
	UpdatePlayerPosition(&CurrentPlayer);

	ParticleSystemUpdate();
	
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
	else
	{
		if(CurrentPlayer.PlayerCollider.Position.x - CurrentPlayer.PlayerCollider.width / 2 < -7 * TutorialBackground->Width / 16)
		{
			CurrentPlayer.Position.x = (-7 * TutorialBackground->Width / 16) + (CurrentPlayer.PlayerCollider.width / 2) + 1;
		}
		else if(CurrentPlayer.PlayerCollider.Position.x + CurrentPlayer.PlayerCollider.width / 2 > 7 * TutorialBackground->Width / 16 && CurrentPlayer.Position.y + CurrentPlayer.PlayerCollider.height / 2 > -TutorialBackground->Height / 8)
		{
			CurrentPlayer.Position.x = (7 * TutorialBackground->Width / 16) - (CurrentPlayer.PlayerCollider.width / 2) - 1;
		}
		else if(CurrentPlayer.PlayerCollider.Position.x + CurrentPlayer.PlayerCollider.width / 2 > 7 * TutorialBackground->Width / 16)
		{
			if(CurrentPlayer.PlayerRigidBody.Velocity.y > 0 && CurrentPlayer.Position.y + CurrentPlayer.PlayerCollider.height / 2 > -TutorialBackground->Height / 7)
				ZeroVelocity(&CurrentPlayer.PlayerRigidBody);
			
			if(CurrentPlayer.PlayerCollider.Position.x - CurrentPlayer.PlayerCollider.width > TutorialBackground->Width / 2)
			{
				CurrentPlayer.Position.x = 50000;
				RemoveDebugMode();
				OverlayGrid->Visible = FALSE;

				fadeToEnd();
			}
		}
	}
	
	if(FoxInput_KeyTriggered('U'))
	{
		SetDebugMode();
		OverlayGrid->Visible = TRUE;
	}
	if(FoxInput_KeyTriggered('I'))
	{
		RemoveDebugMode();
		OverlayGrid->Visible = FALSE;
	}
	PlayAudio(BackSnd);

	// Return to main menu with RSHIFT
	// Pause with ESCAPE
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		if(BlackOverlay->Alpha < 0.1)
		{
			TogglePauseSound(BackSnd);
			InitializePause(&DrawTutorial);
			UpdatePause();
			SetChannelGroupVolume(EffectType, SFXVolume);
			SetChannelGroupVolume(MusicType, BGMVolume);
			TogglePauseSound(BackSnd);
		}
		else if(GameLogo->Alpha > 0.8)
		{
			SetNextState(GS_EPMenu);
		}
	}

	if(FoxInput_KeyTriggered('J'))
	{
		FreeEnemy(StrawDummy);
	}
	if(FoxInput_KeyTriggered(VK_RSHIFT))
	{
		SetNextState(GS_MainMenu);
	}
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
	// Check for any collision and handle the results
	DetectPlayerCollision();
	if(StrawDummy->objID > 0)
		UpdateEnemy(StrawDummy);

	UpdateFloatingText();

	// Handle any input for the current player
	InputPlayer(&CurrentPlayer);
	UpdateHUDItems(CurrentHUD, &CurrentPlayer);
}

void fadeToEnd(void)
{
	if(BlackOverlay->Alpha >= 1.0f)
	{
		BlackOverlay->Alpha = 1.0f;
		levelComplete = TRUE;
		SetNextState(GS_MapLevel);
	}
	else
	{
		BlackOverlay->Alpha += GetDeltaTime();
	}
}
