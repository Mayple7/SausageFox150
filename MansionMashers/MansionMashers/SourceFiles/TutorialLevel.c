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
Vec3 TextColor;
TextGlyphs* TestText;

FoxSound BackgroundSnd;
FoxChannels ChannelController;
float volume;
char volumestring[4];
int newID;

void LoadTutorial(void)
{
	CreateTextureList();
}

void InitializeTutorial(void)
{
	newID = 1;
	resetObjectList();
	CreateChannelGroups(&ChannelController);

	volumestring[0] = '1';
	volumestring[1] = '0';
	volumestring[2] = '0';
	volumestring[3] = '\0';
	CreateSound("Sounds/wave.mp3", &BackgroundSnd, SmallSnd);

	InitializePlayer(&CurrentPlayer, newID++, 0, GROUNDLEVEL + 1);

	TutorialBackground = CreateSprite("TextureFiles/TutorialBackground.png", 1920, 1080, 0, 1, 1, 0, 0);
	OverlayGrid = CreateSprite("TextureFiles/OverlayGrid.png", 2000, 1080, 100, 1, 1, 0, 0);

	// Create the shelf sprite and initialize to be collidable
	Shelf = CreatePlatform("TextureFiles/Shelf.png", PlatformType, 184.5f, 367.5, newID++, 475, -170);
	UpdateCollider(&Shelf->PlatformCollider, Shelf->PlatformCollider.width, Shelf->PlatformCollider.height * 0.16f); 
	Shelf->PlatformCollider.Offset.y = Shelf->PlatformSprite->Height * 3 / 8;

	ShortShelf = CreatePlatform("TextureFiles/ShortShelf.png", PlatformType, 184.5f, 198.75f, newID++, 280, -280);
	ShortShelf->PlatformCollider.Offset.y = 5 * ShortShelf->PlatformSprite->Height / 16;
	UpdateCollider(&ShortShelf->PlatformCollider, ShortShelf->PlatformCollider.width, ShortShelf->PlatformCollider.height * 0.2f); 

	BouncyBed = CreatePlatform("TextureFiles/BouncePad.png", BounceType, 375.0f, 100.0f, newID++, -225, -350);
	BouncyBed->PlatformSprite->Visible = FALSE;
	BouncyBed->PlatformRigidBody.Restitution = 2.2f;

	StarterAxe = CreateDroppedWeapon(Axe, Common, 256, 256, newID++, -200, -300);
	StarterAxe->WeaponSprite->Rotation = (float)-FOX_PI / 3;

	StarterSword = CreateDroppedWeapon(Sword, Common, 250, 250, newID++, 475, 0);
	StarterSword->WeaponSprite->Rotation = (float)FOX_PI /4;

	StrawDummy = CreateEnemy(Dummy, EnemyType, newID++, 750, -300);

	BlackOverlay = CreateSprite("TextureFiles/BouncePad.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Alpha = 0;
	Vec3Set(&BlackOverlay->Tint, 0, 0, 0);

	GameLogo = CreateSprite("TextureFiles/MansionMashersLogo.png", 1920, 1080, 4001, 1, 1, 0, 0);
	GameLogo->Alpha = 0;

	DoorOverlay = CreateSprite("TextureFiles/DoorOverlay.png", 1920, 1080, 200, 1, 1, 0, 0);

	Vec3Set(&TextColor, 0, 0, 0);
	TestText = CreateText(volumestring, -500, 350, 100, TextColor);
	SetChannelGroupVolume(&ChannelController, EffectType, 0);
	ChangeTextVisibility(TestText);

	RemoveDebugMode();
	OverlayGrid->Visible = FALSE;

	ResetCamera();
}

void UpdateTutorial(void)
{
	// Handle any events such as collision
	EventTutorial();
	
	// Update the player position
	UpdatePlayerPosition(&CurrentPlayer);
	
	if(StrawDummy->objID > 0)
	{
		if(CurrentPlayer.PlayerCollider.Position.x + CurrentPlayer.PlayerCollider.width / 2 > StrawDummy->Position.x)
		{
			CurrentPlayer.Position.x = (StrawDummy->Position.x) - (CurrentPlayer.PlayerCollider.width / 2) - 1;
		}

	}
	else
	{
		if(CurrentPlayer.PlayerCollider.Position.x - CurrentPlayer.PlayerCollider.width / 2 < -7 * TutorialBackground->Width / 16)
		{
			CurrentPlayer.Position.x = (-7 * TutorialBackground->Width / 16) + (CurrentPlayer.PlayerCollider.width / 2) + 1;
		}
		/*else if(CurrentPlayer.PlayerCollider.Position.x + CurrentPlayer.PlayerCollider.width / 2 > 7 * TutorialBackground->Width / 16)
		{
			CurrentPlayer.Position.x = (7 * TutorialBackground->Width / 16) - (CurrentPlayer.PlayerCollider.width / 2) - 1;
		}*/
		else if(CurrentPlayer.PlayerCollider.Position.x - CurrentPlayer.PlayerCollider.width > TutorialBackground->Width / 2)
		{
			CurrentPlayer.Position.x = 50000;
			fadeToEnd();
		}

	}

	if(AEInputCheckTriggered('U'))
	{
		SetDebugMode();
		OverlayGrid->Visible = TRUE;
	}
	if(AEInputCheckTriggered('I'))
	{
		RemoveDebugMode();
		OverlayGrid->Visible = FALSE;
	}
	PlayAudio(&BackgroundSnd, &ChannelController);

	if(AEInputCheckCurr(VK_DOWN))
	{
		if(volume > 0)
			volume = volume - 0.01f;
		SetChannelGroupVolume(&ChannelController, EffectType, volume);
		ChangeTextString(TestText, VolumetoString(volumestring, volume * 100));
	}
	if(AEInputCheckCurr(VK_UP))
	{
		if(volume < 1)
			volume = volume + 0.01f;
		SetChannelGroupVolume(&ChannelController, EffectType, (volume * 100));
		ChangeTextString(TestText, VolumetoString(volumestring, (volume * 100)));
	}
	// Return to main menu with RSHIFT
	// Pause with ESCAPE
	if(AEInputCheckTriggered(VK_RSHIFT))
	{
		SetNextState(GS_MainMenu);
	}
	if(AEInputCheckTriggered(VK_ESCAPE))
	{
		if(BlackOverlay->Alpha < 0.1)
		{
			InitializePause(&DrawTutorial);
			UpdatePause();
		}
		else if(GameLogo->Alpha > 0.8)
		{
			SetNextState(GS_EPMenu);
		}

	}
	if(AEInputCheckTriggered('J'))
	{
		FreeEnemy(StrawDummy);
	}

}

void DrawTutorial(void)
{
	DrawObjectList();
	DrawCollisionList();
}

void FreeTutorial(void)
{
	freeObjectList();
	ReleaseChannelGroups(&ChannelController);
	ReleaseSound(BackgroundSnd.Sound);
}

void UnloadTutorial(void)
{
	DestroyTextureList();
}

void EventTutorial(void)
{
	// Check for any collision and handle the results
	DetectPlayerCollision();
	// Handle any input for the current player
	InputPlayer(&CurrentPlayer);
}

void fadeToEnd(void)
{
	if(BlackOverlay->Alpha >= 1.0f)
	{
		BlackOverlay->Alpha = 1.0f;
		if(GameLogo->Alpha >= 1.0f)
		{
			GameLogo->Alpha = 1.0f;
			if(AEInputCheckTriggered(VK_SPACE))
			{
				SetNextState(GS_EPMenu);
			}
		}
		else
		{
			GameLogo->Alpha += GetDeltaTime();
		}

	}
	else
	{
		BlackOverlay->Alpha += GetDeltaTime();
	}


}
