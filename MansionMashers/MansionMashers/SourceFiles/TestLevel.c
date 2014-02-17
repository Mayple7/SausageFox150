/*****************************************************************************/
/*!
\file				TestLevel.c
\author				Kaden Nugent (kaden.n)
\author				Dan Muller (d.muller)
\date				Jan 8, 2014

\brief				Level functions for the test level

\par				Functions:
\li					LoadTestLevel
\li					InitializeTestLevel
\li					UpdateTestLevel
\li					DrawTestLevel
\li					FreeTestLevel
\li					UnloadTestLevel
\li					EventTestLevel
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/TestLevel.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../FMODHeaders/fmod.h"

// ---------------------------------------------------------------------------
// Globals
int newID;					// ID number

Sprite *HUD1;
Sprite *HUD2;
Sprite *HUD3;
Sprite *HUD4;
Sprite *HUD1item;
Sprite *HUD2item;
Sprite *HUD3item;
Sprite *HUD4item;
Sprite *Background;
Sprite *ColliderInvisible;
Sprite *OverlayGrid;
Enemy *CurrentEnemy;
HUDLayer HUDList;

FoxSound BackgroundSnd;
FoxSound Sound1;
FoxSound Sound2;
FoxChannels ChannelController;
float volume;
TextGlyphs * volumetext;
Sprite *Letters;
char volumestring[4];


Food *Hammy;
Sprite *Hammy2;

/*************************************************************************/
/*!
	\brief
	Loads the assets for the test level
*/
/*************************************************************************/
void LoadTestLevel(void)
{
	//Allocate space for a large texture
	CreateTextureList();
}

/*************************************************************************/
/*!
	\brief
	Initializes the objects for the test level
*/
/*************************************************************************/
void InitializeTestLevel(void)
{	
	int hudLoop;

	//For volume
	Vec3 textTint;
	

	newID = 1;
	// Reset the object list
	ResetObjectList();

	// Set the HUDlist to 0
	for (hudLoop = 0; hudLoop < 20; hudLoop++)
		HUDList.HudItem[hudLoop] = 0;

	OverlayGrid = CreateSprite("TextureFiles/OverlayGrid.png", 2000, 1080, 100, 1, 1, 0, 0);

	// Create the HAMs
	Hammy = CreateFood("TextureFiles/Ham.png", FoodType, 150, 140, newID++, 400, 0);

	// Creating the HUD items
	HUD1 = CreateSprite("TextureFiles/GinkoHUD.png", 320.0f, 137.0f, 200, 1, 1, 0, 0);
	HUD1->isHUD = TRUE;
	HUD2 = CreateSprite("TextureFiles/HollyHUD.png", 320.0f, 137.0f, 200, 1, 1, 0, 0);
	HUD2->isHUD = TRUE;
	HUD3 = CreateSprite("TextureFiles/MaypleHUD.png", 320.0f, 137.0f, 200, 1, 1, 0, 0);
	HUD3->isHUD = TRUE;
	HUD4 = CreateSprite("TextureFiles/KayaHUD.png", 320.0f, 137.0f, 200, 1, 1, 0, 0);
	HUD4->isHUD = TRUE;

	HUD1item = CreateSprite("TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1, 0, 0);
	HUD1item->isHUD = TRUE;
	HUD1item->ItemType = 0;

	HUD2item = CreateSprite("TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1, 0, 0);
	HUD2item->isHUD = TRUE;
	HUD2item->ItemType = 0;

	HUD3item = CreateSprite("TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1, 0, 0);
	HUD3item->isHUD = TRUE;
	HUD3item->ItemType = 0;

	HUD4item = CreateSprite("TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1, 0, 0);
	HUD4item->isHUD = TRUE;
	HUD4item->ItemType = 0;

	// Add HUD items to the list
	HUDList.HudItem[0] = HUD1;
	HUDList.HudItem[2] = HUD2;
	HUDList.HudItem[4] = HUD3;
	HUDList.HudItem[6] = HUD4;
	HUDList.HudItem[1] = HUD1item;
	HUDList.HudItem[3] = HUD2item;
	HUDList.HudItem[5] = HUD3item;
	HUDList.HudItem[7] = HUD4item;

	// Create the background
	Background = CreateSprite("TextureFiles/FoxMansionHall1.png", 1920, 1080, 0, 1, 1, 0, 0);

	// Secret invisible collider GO!
	ColliderInvisible = CreateSprite("TextureFiles/LevelGrassground.png", 300.0f, 80.0f, 0, 1, 1, -1020.0f, -40.0f);
	ColliderInvisible->Visible = FALSE;

	// Creates the player
	InitializePlayer(&CurrentPlayer, newID++, 0, 0);

	// Creates the enemy
	//CurrentEnemy = CreateEnemy("TextureFiles/EasyEnemy.png", EnemyType, 150, 150, newID++, 0, 0);

	//Adding sounds
	CreateSound("Sounds/drumloop.wav", &Sound1, SmallSnd);
	CreateSound("Sounds/jaguar.wav", &Sound2, SmallSnd);
	CreateSound("Sounds/wave.mp3", &BackgroundSnd, SmallSnd);
	CreateChannelGroups(&ChannelController);
	volume = GetSoundVolume(&BackgroundSnd);
	volumestring[0] = '1';
	volumestring[1] = '0';
	volumestring[2] = '0';
	volumestring[3] = '\0';
	Vec3Set(&textTint, 1, 1, 1);
	volumetext = CreateText(volumestring, -300.0f, 350.0f, 150, textTint, Left);
	ChangeTextVisibility(volumetext);
	

	// Reset camera to (0,0)
	ResetCamera();
}

/*************************************************************************/
/*!
	\brief
	Updates the test level
*/
/*************************************************************************/
void UpdateTestLevel(void)
{
	// Handle test level events
	EventTestLevel();

	// Update the player's position
	UpdatePlayerPosition(&CurrentPlayer);

	//Play sounds
	PlayAudio(&BackgroundSnd, &ChannelController);

	if(AEInputCheckTriggered('D'))
		PlayAudio(&Sound1, &ChannelController);
	if(AEInputCheckCurr('A'))
		PlayAudio(&Sound2, &ChannelController);
	if(AEInputCheckTriggered('P'))
	{
		//TogglePauseSound(&BackgroundSnd);
		TogglePauseChannel(&ChannelController, EffectType);
	}
	
	//printf("BG Volume: %i\n", (int)volume);

	if(AEInputCheckCurr(VK_DOWN))
	{
		if(volume > 0)
			volume = volume - 0.01f;
		SetChannelGroupVolume(&ChannelController, EffectType, volume);
		ChangeTextString(volumetext, VolumetoString(volumestring, volume * 100));
	}
	if(AEInputCheckCurr(VK_UP))
	{
		if(volume < 1)
			volume = volume + 0.01f;
		SetChannelGroupVolume(&ChannelController, EffectType, volume * 100);
		ChangeTextString(volumetext, VolumetoString(volumestring, volume * 100));
	}

	//************************************

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

	// Go back to main menu with ESC
	if(AEInputCheckTriggered(VK_ESCAPE))
	{
		SetNextState(GS_MainMenu);
	}
}

/*************************************************************************/
/*!
	\brief
	Draws the test level
*/
/*************************************************************************/
void DrawTestLevel(void)
{
	// Draws the objects in the object list
	DrawObjectList();
	DrawHUD(&HUDList);
	//Camera follows player
	SetCamera(&CurrentPlayer.Position, 250);
	DrawCollisionList();
}

/*************************************************************************/
/*!
	\brief
	Frees all the objects in the test level
*/
/*************************************************************************/
void FreeTestLevel(void)
{
	FreeObjectList();
	ReleaseChannelGroups(&ChannelController);
	ReleaseSound(Sound1.Sound);
	ReleaseSound(Sound2.Sound);
	ReleaseSound(BackgroundSnd.Sound);
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the test level
*/
/*************************************************************************/
void UnloadTestLevel(void)
{
	//Destroy the textures
	DestroyTextureList();
	
}

/*************************************************************************/
/*!
	\brief
	Handles all events for the test level
*/
/*************************************************************************/
void EventTestLevel(void)
{
	// Detect Player collision
	DetectPlayerCollision();

	// Handle any input for the player
	InputPlayer(&CurrentPlayer);

	// Handle item switching for the HUD
	if(AEInputCheckTriggered('Q'))
	{
		if (HUD1item->ItemType == 0)
		{
			HUD1item->SpriteTexture = AEGfxTextureLoad("TextureFiles/ThrowableHUD.png");
			HUD1item->ItemType = 1;
		}
		else
		{
			HUD1item->SpriteTexture = AEGfxTextureLoad("TextureFiles/HealthPotionHUD.png");
			HUD1item->ItemType = 0;
		}
	}
}
