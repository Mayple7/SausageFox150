/*****************************************************************************/
/*!
\file				ShowcaseLevel.c
\author				Dan Muller (d.muller)
\date				Jan 22, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					LoadShowcase
\li					InitializeShowcase
\li					UpdateShowcase
\li					DrawShowcase
\li					FreeShowcase
\li					UnloadShowcase
\li					EventShowcase
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/LevelShowcase.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"


// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
int newID;					// ID number

Sprite *HUD;
Sprite *HUDitem;
Sprite *Background;
Sprite *Background2;
HUDLayer HUDList;
Platform *BouncePad;
Platform *Shelf;
Platform *Crate;
Sprite *OverlayGrid;
Sprite *FoxBig;
Sprite *FoxSmall;
Sprite *Letters;

TextGlyphs *Juli, *Luke, *Kaden, *Dan, *Fox, *subText;

//FoxSound BackgroundSnd;

Food *Ham;
Food *Hammy;

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadShowcase(void)
{
	//Allocate space for a large texture
	CreateTextureList();
}

/*************************************************************************/
/*!
	\brief
	Initializes the objects for the showcase level
*/
/*************************************************************************/
void InitializeShowcase(void)
{
	Vec2 startingCamera;
	Vec3 textTint;
	int hudLoop;

	newID = 1;
	resetObjectList();

	// Create the player
	InitializePlayer(&CurrentPlayer, newID++, 0, -220);

	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	for (hudLoop = 0; hudLoop < 20; hudLoop++)
		HUDList.HudItem[hudLoop] = 0;

	OverlayGrid = CreateSprite("TextureFiles/OverlayGrid.png", 2000, 1080, 100, 1, 1, 0, 0);

	// Create single player HUD sprite
	HUD = CreateSprite("TextureFiles/MaypleHUD.png", 320.0f, 137.0f, 200, 1, 1, 0, 0);
	HUD->isHUD = TRUE;

	// Create single player HUD item sprite
	HUDitem = CreateSprite("TextureFiles/HealthPotionHUD.png", 44.0f, 44.0f, 200, 1, 1, 0, 0);
	HUDitem->ItemType = 0;
	HUDitem->isHUD = TRUE;

	// Create the background sprite
	Background = CreateSprite("TextureFiles/LevelBackground.png", 5760, 1080, 0, 1, 1, 0, 0);
	
	// Create the offset background sprite
	Background2 = CreateSprite("TextureFiles/LevelBackground.png", 5760, 1080, 0, 1, 1, 5760, 0);
	Background2->FlipX = TRUE;

	// Create the shelf sprite and initialize to be collidable
	Shelf = CreatePlatform("TextureFiles/Shelf.png", PlatformType, 123, 245, newID++, 475, -115);
	Shelf->PlatformCollider.Offset.y = Shelf->PlatformSprite->Height * GetLoadRatio() / 2 - 30;
	UpdateCollider(&Shelf->PlatformCollider, Shelf->PlatformCollider.width, 60);
	
	// Create and initialize the crate sprite
	Crate = CreatePlatform("TextureFiles/Crate.png", PlatformType, 859, 260.5f, newID++, 1400, -180);
	Crate->PlatformCollider.Offset.y = Crate->PlatformSprite->Height * GetLoadRatio() / 2 - 60;
	UpdateCollider(&Crate->PlatformCollider, Crate->PlatformCollider.width - 100, 60);
	Crate->PlatformCollider.height = 60;

	Ham = CreateFood("TextureFiles/Ham.png", FoodType, 150, 140, newID++, 0, 0);

	// Create and initialize the HAM sprite
	Hammy = CreateFood("TextureFiles/Ham.png", FoodType, 150.0f, 140.0f, newID++, -1000, 0);

	// Add the HUD sprites to the HUDlist
	HUDList.HudItem[0] = HUD;
	HUDList.HudItem[1] = HUDitem;

	BouncePad = CreatePlatform("TextureFiles/BouncePad.png", BounceType, 400, 100, newID++, -1000, -200);
	BouncePad->PlatformCollider.Position = BouncePad->Position;
	BouncePad->PlatformRigidBody.Restitution = 1.8f;

	FoxBig = CreateSprite("Textures/GinkoArt.png", 148.5, 270, 20, 1, 1, -300, 0);
	FoxSmall = CreateSprite("Textures/GinkoSmall.png", 148.5, 270, 20, 1, 1, -800, 0);

	Letters = CreateSprite("TextureFiles/Rumple_TextSheet.png", 92, 100, 20, 11, 4, -1200, 100);
	Letters->AnimationSpeed = 60;

	Vec3Set(&textTint, 0.6f, 0.169f, 0.8f);
	Juli = CreateText("Hi Juli, This is dynamic!", 0, 300, 100, textTint, Right);
	ChangeTextVisibility(Juli);

	Vec3Set(&textTint, 1.0f, 1.0f, 0.0f);
	Luke = CreateText("Luke is muy grande.", 100, 200, 100, textTint, Left);
	ChangeTextVisibility(Luke);

	Vec3Set(&textTint, 1.0f, 0.0f, 0.0f);
	Kaden = CreateText("+1 to kaden for being 100% less of a kevin this week", 100, 100, 100, textTint, Left);
	ChangeTextVisibility(Kaden);

	Vec3Set(&textTint, 0.01f, 0.6f, 0.39f);
	Dan = CreateText("Dan is just way too awesome, hahahaha!!!", 300, 0, 150, textTint, Left);
	ChangeTextVisibility(Dan);

	Vec3Set(&textTint, 1.0f, 1.0f, 1.0f);
	Fox = CreateText("The quick brown fox jumps over the lazy dog! -+!.,%()", 100, -200, 200, textTint, Left);
	ChangeTextVisibility(Fox);

	Vec3Set(&textTint, 0.0f, 0.0f, 0.0f);
	subText = CreateText("(These are the letters and symbols that you can display)", 300, -300, 50, textTint, Left);
	ChangeTextVisibility(subText);

	//Sounds
	//CreateSound("Sounds/wave.mp3", &BackgroundSnd, SmallSnd);

	// Set the camera to the starting position
	Vec2Set(&startingCamera, -1280 * GetLoadRatio(), 0.0f);
	SetCamera(&startingCamera, 250);
}

/*************************************************************************/
/*!
	\brief
	Updates the showcase level
*/
/*************************************************************************/
void UpdateShowcase(void)
{
	int XPos, YPos;
	FoxInput_GetMousePosition(&XPos, &YPos);
	// Handle any events such as collision
	EventShowcase();
	printf("%i : %i\n", XPos, YPos);
	// Update the player position
	UpdatePlayerPosition(&CurrentPlayer);

	//Sound
	//PlayAudio(&BackgroundSnd);


	// Return to main menu with ESC
	if(AEInputCheckTriggered(VK_RSHIFT))
	{
		SetNextState(GS_MainMenu);
	}
	if(AEInputCheckTriggered(VK_ESCAPE))
	{
		InitializePause(&DrawShowcase);
		//TogglePauseSound(&BackgroundSnd);
		UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
	}

}

/*************************************************************************/
/*!
	\brief
	Draws the showcase level
*/
/*************************************************************************/
void DrawShowcase(void)
{
	// Draws the object list and sets the camera to the correct location
	DrawObjectList();
	DrawHUD(&HUDList);
	SetCamera(&CurrentPlayer.Position, 250);

	DrawCollisionList();
	SetCamera(&CurrentPlayer.Position, 250);
}

/*************************************************************************/
/*!
	\brief
	Frees all the objects in the showcase level
*/
/*************************************************************************/
void FreeShowcase(void)
{
	FreeText(Juli);
	FreeText(Luke);
	FreeText(Kaden);
	FreeText(Dan);
	FreeText(Fox);
	FreeText(subText);
	freeObjectList();
	//ReleaseSound(BackgroundSnd.Sound);

}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the showcase level
*/
/*************************************************************************/
void UnloadShowcase(void)
{
	//Destroy the textures
	DestroyTextureList();
}

/*************************************************************************/
/*!
	\brief
	Handles all events in the showcase level
*/
/*************************************************************************/
void EventShowcase(void)
{
	// Check for any collision and handle the results
	DetectPlayerCollision();
	// Handle any input for the current player
	InputPlayer(&CurrentPlayer);

	// Handle input for swapping items
	if(AEInputCheckTriggered('Q'))
	{
		if (HUDitem->ItemType == 0)
		{
			HUDitem->SpriteTexture = AEGfxTextureLoad("TextureFiles/ThrowableHUD.png");
			HUDitem->ItemType = 1;
		}
		else
		{
			HUDitem->SpriteTexture = AEGfxTextureLoad("TextureFiles/HealthPotionHUD.png");
			HUDitem->ItemType = 0;
		}
	}
	if(AEInputCheckTriggered('9'))
	{
		AEGfxSetViewportPositionAndDimensions(1000, 0, 800, 600);
		printf("4:3\n");
	}
	if(AEInputCheckTriggered('0'))
	{
		AEGfxSetViewportPositionAndDimensions(0, 0, 1920, 1080);
		printf("16:9\n");
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
}




