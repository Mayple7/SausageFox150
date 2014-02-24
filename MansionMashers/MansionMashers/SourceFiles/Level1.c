/*****************************************************************************/
/*!
\file				Level1.c
\author				Dan Muller (d.muller)
					Juli Gregg (j.gregg)
\date				Feb 15, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					LoadLevel1
\li					InitializeLevel1
\li					UpdateLevel1
\li					DrawLevel1
\li					FreeLevel1
\li					UnloadLevel1
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Level1.h"
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
// globals
int newID;					// ID number
TextGlyphs* LevelName;

Sprite* Level1Panel1;
Sprite* Level1Door1;
Sprite* Level1Panel2;
Sprite* Level1Panel3;
Sprite* Various;

Platform* Table1;






/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadLevel1(void)
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
void InitializeLevel1(void)
{
	Vec3 TextTint;

	newID = 1;
	ResetObjectList();
	ResetCamera();

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, newID++, 0, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	Vec3Set(&TextTint, 1, 1, 1);
	LevelName = CreateText("Level 1", 0, 500, 100, TextTint, Center);
	ChangeTextVisibility(LevelName);

	//Bounding Boxes
	CreateBoundingBoxes();

	//Background: Panel 1
	Level1Panel1 = (Sprite *)CreateSprite("TextureFiles/FoxMansion2.png", 1920, 1080, 0, 1, 1, 0, 0);
	Level1Door1 = (Sprite *)CreateSprite("TextureFiles/OverlayDoorTorch.png", 1920, 1080, 200, 1, 1, 0, 0);
	Level1Panel2 = (Sprite *)CreateSprite("TextureFiles/FoxMansionHall1.png", 1920, 1080, 0, 1, 1, 1920, 0);
	Level1Door1 = (Sprite *)CreateSprite("TextureFiles/OverlayDoor.png", 1920, 1080, 300, 1, 1, 1920, 0);
	Level1Panel3 = (Sprite *)CreateSprite("TextureFiles/FoxMansion1.png", 1920, 1080, 0, 1, 1, (1920 * 2), 0);


	//Platforms
	Table1 = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 40.0f, newID++, -628, -285);
	Table1->PlatformSprite->Visible = FALSE;

	Table1 = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 40.0f, newID++, -288, -285);
	Table1->PlatformSprite->Visible = FALSE;

	Table1 = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 40.0f, newID++, 68, -285);
	Table1->PlatformSprite->Visible = FALSE;

	Table1 = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 100.0f, 40.0f, newID++, 450, -285);
	Table1->PlatformSprite->Visible = FALSE;

	//Table1 = CreatePlatform("TextureFiles/BlankPlatform.png", WallType, 200.0f, 250.0f, newID++, 450, -100);
	//Table1->PlatformSprite->Visible = FALSE;

	CreateEnemy(BasicMelee, EnemyType, newID++, 750, 250);

	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 695, -140, 10, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 806, -235, 201, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);


}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateLevel1(void)
{
	EventLevel1();

	ParticleSystemUpdate();
	BoundingBoxUpdate();

	// This should be the last line in this function
	UpdatePlayerPosition(&CurrentPlayer);

	if(CurrentPlayer.PlayerCollider.Position.x - CurrentPlayer.PlayerCollider.width / 2 < -7 * Level1Panel1->Width / 16)
			CurrentPlayer.Position.x = (-7 * Level1Panel1->Width / 16) + (CurrentPlayer.PlayerCollider.width / 2) + 1;
	else if(CurrentPlayer.PlayerCollider.Position.x + CurrentPlayer.PlayerCollider.width / 2 > 7 * Level1Panel1->Width / 16 && CurrentPlayer.Position.y + CurrentPlayer.PlayerCollider.height / 2 > -Level1Panel1->Height / 8)
	{
		CurrentPlayer.Position.x = (7 * Level1Panel1->Width / 16) - (CurrentPlayer.PlayerCollider.width / 2) - 1;
	}
	else if(CurrentPlayer.PlayerCollider.Position.x + CurrentPlayer.PlayerCollider.width / 2 > 7 * Level1Panel1->Width / 16)
	{
		if(CurrentPlayer.PlayerRigidBody.Velocity.y > 0 && CurrentPlayer.Position.y + CurrentPlayer.PlayerCollider.height / 2 > -Level1Panel1->Height / 7)
			ZeroVelocity(&CurrentPlayer.PlayerRigidBody);
	}
}

/*************************************************************************/
/*!
	\brief
	Draws the level
*/
/*************************************************************************/
void DrawLevel1(void)
{
	// Draws the object list and sets the camera to the correct location
	DrawObjectList();
	//DrawHUD(&HUDList);
	DrawCollisionList();
	SetCamera(&CurrentPlayer.Position, 250);

}

/*************************************************************************/
/*!
	\brief
	Frees all the objects in the level
*/
/*************************************************************************/
void FreeLevel1(void)
{
	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadLevel1(void)
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
void EventLevel1(void)
{
	int i;
	// Check for any collision and handle the results
	DetectPlayerCollision();
	// Handle any input for the current player
	InputPlayer(&CurrentPlayer);

	//Update the enemies
	for(i = 0; i < COLLIDEAMOUNT; i++)
	{
		if (enemyList[i].objID == -1)
			break;
		if (enemyList[i].objID == 0)
			continue;

		UpdateEnemy(&enemyList[i]);
	}

	i = 0;
	while(i < COLLIDEAMOUNT)
	{
		if(floatTextList[i] > 0)
			UpdateFloatingText(floatTextList[i]);
		i++;
	}

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
		//InitializePause(&DrawLevel1);
		//TogglePauseSound(&BackgroundSnd);
		SetNextState(GS_MainMenu);
		//UpdatePause();
		//TogglePauseSound(&BackgroundSnd);
	}
}
