/*****************************************************************************/
/*!
\file				YeahGuy.c
\author				Dan Muller (d.muller)
\date				Feb 15, 2014

\brief				Functions for the showcase level

\par				Functions:
\li					LoadYeahGuy
\li					InitializeYeahGuy
\li					UpdateYeahGuy
\li					DrawYeahGuy
\li					FreeYeahGuy
\li					UnloadYeahGuy
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/YeahGuy.h"
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
static int levelComplete = FALSE;
static int CurrentBuff;
float buffTimer;
float playerBuffTimer;

// Fake Bools
int buffsShown;
int redHead;
int greenHead;
int blueHead;

Sprite* CurrentBuffSprite;

// Buff Sprites and Collision boxes
Sprite* RedBuff;
CollisionBox* RedBuffCollider;

Sprite* GreenBuff;
CollisionBox* GreenBuffCollider;

Sprite* BlueBuff;
CollisionBox* BlueBuffCollider;

Sprite* DebugCircle;

YeahGuyBoss *Boss;

static enum BossBuff { None, Red, Green, Blue };

/*************************************************************************/
/*!
	\brief
	Loads assets for the showcase level
*/
/*************************************************************************/
void LoadYeahGuy(void)
{
	//Allocate space for a large texture
	CreateTextureList();

	LoadTexture("TextureFiles/RedBuff.png");
	LoadTexture("TextureFiles/GreenBuff.png");
	LoadTexture("TextureFiles/BlueBuff.png");
}

/*************************************************************************/
/*!
	\brief
	Initializes the objects for the level
*/
/*************************************************************************/
void InitializeYeahGuy(void)
{
	Vec2 Position;
	ResetObjectList();
	ResetCamera();

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, 0, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	CurrentBuff = None;
	buffsShown = FALSE;
	buffTimer = 0.0f;
	playerBuffTimer = 0.0f;

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	//Panel1
	CreateSprite("TextureFiles/ArmGuyBackground.png", 1920, 1080, 1, 1, 1, 0, 0);

	//Bounding Boxes
	CreateBoundingBoxes();

	/////////////////////////////////
	//		Platforms			   //
	/////////////////////////////////
	CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 300, 50, -550, -170);
	CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 300, 50, 550, -170);
	CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 300, 50, 0, 100);

	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	//Create bounding walls
	CreateWall("TextureFiles/BlankPlatform.png", 400.0f, 1040.0f, -1160, 0);
	CreateWall("TextureFiles/BlankPlatform.png", 400.0f, 1040.0f, 1160, 0);

	/////////////////////////////////
	//			Boss			   //
	/////////////////////////////////
	Boss = CreateYeahGuyBoss(0, 0);
	Boss->BossCollider.collisionDebug = TRUE;

	DebugCircle = (Sprite *)CreateSprite("TextureFiles/DebugCircle.png", Boss->YeahAOERadius * 2, Boss->YeahAOERadius * 2, 300, 1, 1, Boss->Position.x, Boss->Position.y);
	DebugCircle->Visible = FALSE;

	redHead = TRUE;
	greenHead = TRUE;
	blueHead = TRUE;

	/////////////////////////////////
	//			Buffs			   //
	/////////////////////////////////
	Vec2Set(&Position, 0, 0);

	CurrentBuffSprite = (Sprite *)CreateSprite("TextureFiles/RedBuff.png", 50, 50, 200, 1, 1, 920, 500);

	// Creates the buffs, do not add these to 
	RedBuffCollider = (CollisionBox *) CallocMyAlloc(1, sizeof(CollisionBox));
	GreenBuffCollider = (CollisionBox *) CallocMyAlloc(1, sizeof(CollisionBox));
	BlueBuffCollider = (CollisionBox *) CallocMyAlloc(1, sizeof(CollisionBox));

	RedBuff = (Sprite *)CreateSprite("TextureFiles/RedBuff.png", 200, 200, 8, 1, 1, 0, 0);
	CreateCollisionBox(RedBuffCollider, &Position, FoodType, 200, 200, GetObjectID());

	GreenBuff = (Sprite *)CreateSprite("TextureFiles/GreenBuff.png", 200, 200, 8, 1, 1, 0, 0);
	CreateCollisionBox(GreenBuffCollider, &Position, FoodType, 200, 200, GetObjectID());

	BlueBuff = (Sprite *)CreateSprite("TextureFiles/BlueBuff.png", 200, 200, 8, 1, 1, 0, 0);
	CreateCollisionBox(BlueBuffCollider, &Position, FoodType, 200, 200, GetObjectID());

	HideBuffs();

	/////////////////////////////////
	//		On Death			   //
	/////////////////////////////////
	CreateDeathConfirmObjects();

	CreateUpgradeScreenObjects();
}

/*************************************************************************/
/*!
	\brief
	Updates the level
*/
/*************************************************************************/
void UpdateYeahGuy(void)
{
	redHead = Boss->redHead;
	greenHead = Boss->greenHead;
	blueHead = Boss->blueHead;

	if(CurrentBuff == None)
	{
		buffTimer += GetDeltaTime();
		CurrentBuffSprite->Visible = FALSE;
	}
	else
	{
		CurrentBuffSprite->Visible = TRUE;
		if(CurrentBuff == Red)
			CurrentBuffSprite->SpriteTexture = LoadTexture("TextureFiles/RedBuff.png");
		else if(CurrentBuff == Green)
			CurrentBuffSprite->SpriteTexture = LoadTexture("TextureFiles/GreenBuff.png");
		else if(CurrentBuff == Blue)
			CurrentBuffSprite->SpriteTexture = LoadTexture("TextureFiles/BlueBuff.png");
	}


	UpdatePlayerBuff();

	// Update all buffs if needed
	if(buffTimer >= 10.0f)
	{
		HideBuffs();
		buffTimer = 0.0f;
	}
	else if(buffTimer >= 2.0f && !buffsShown)
	{
		ShowBuffs();
	}

	// Make buffs invisible if the head does not exist
	if(!redHead)
		RedBuff->Visible = FALSE;
	if(!greenHead)
		GreenBuff->Visible = FALSE;
	if(!blueHead)
		BlueBuff->Visible = FALSE;

	DebugCircle->Width = Boss->YeahAOERadius * 2;
	if(DebugCircle->Height > DebugCircle->Width)
	{
		DebugCircle->Height = Boss->YeahAOERadius * 2;
		UpdateMesh(DebugCircle);
	}

	printf("%f\n", DebugCircle->Width);

	EventYeahGuy();
	// This should be the last line in this function
	UpdateYeahGuyBoss(Boss);
	UpdatePlayerPosition(&CurrentPlayer);

	UpdateFloatingText();
	BoundingBoxUpdate();

	UpdateAllProjectiles();
	ParticleSystemUpdate();

	if(Boss->CurrentState == 1 && Boss->InnerState != 2)
	{
		DebugCircle->Visible = TRUE;
		DebugCircle->Position = Boss->Position;
	}
	else
		DebugCircle->Visible = FALSE;

	if(!Boss->redHead && !Boss->greenHead && !Boss->blueHead)
	{
		levelComplete = TRUE;
	}

	if (levelComplete)
		LevelCompletion();
}

/*************************************************************************/
/*!
	\brief
	Draws the level
*/
/*************************************************************************/
void DrawYeahGuy(void)
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
void FreeYeahGuy(void)
{
	if(levelComplete && CurrentPlayer.CurrentLevel < GS_Level7)
	{
		CurrentPlayer.levelClearBitFlags |= 128;
		CurrentPlayer.CurrentLevel = GS_Level7;
	}
	else if(CurrentPlayer.CurrentLevel < GS_YeahGuy)
		CurrentPlayer.CurrentLevel = GS_YeahGuy;

	FreeMyAlloc(RedBuffCollider);
	FreeMyAlloc(GreenBuffCollider);
	FreeMyAlloc(BlueBuffCollider);

	//Only save stats if the level was actually completed
	if (levelComplete)
		SavePlayer(&CurrentPlayer);

	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets in the level
*/
/*************************************************************************/
void UnloadYeahGuy(void)
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
void EventYeahGuy(void)
{
	// Check for any collision and handle the results
	DetectPlayerCollision();
	DetectYeahGuyBossCollision(Boss, CurrentBuff);
	// Handle any input for the current player
	InputPlayer(&CurrentPlayer);

	if(FoxInput_KeyTriggered('U'))
	{
		SetDebugMode();
	}
	if(FoxInput_KeyTriggered('I'))
	{
		RemoveDebugMode();
	}
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		InitializePause(&DrawYeahGuy);
		//TogglePauseSound(&BackSnd);
		UpdatePause();
		//TogglePauseSound(&BackSnd);
	}
}

void ShowBuffs(void)
{
	int buffLocations = rand() % 6;

	if(CurrentBuff == None)
	{
		switch(buffLocations)
		{
		case 0:
			// 1-2-3
			// Sets up red buff if needed
			if(redHead)
			{
				Vec2Set(&RedBuff->Position, -550, 0);
				RedBuff->Visible = TRUE;
				Vec2Set(&RedBuffCollider->Position, -550, 0);
			}
			// Sets up green buff if needed
			if(greenHead)
			{
				Vec2Set(&GreenBuff->Position, 0, 270);
				RedBuff->Visible = TRUE;
				Vec2Set(&GreenBuffCollider->Position, 0, 270);
			}
			// Sets up blue buff if needed
			if(blueHead)
			{
				Vec2Set(&BlueBuff->Position, 550, 0);
				RedBuff->Visible = TRUE;
				Vec2Set(&BlueBuffCollider->Position, 550, 0);
			}
			break;
		case 1:
			// 1-3-2
			// Sets up red buff if needed
			if(redHead)
			{
				Vec2Set(&RedBuff->Position, -550, 0);
				RedBuff->Visible = TRUE;
				Vec2Set(&RedBuffCollider->Position, -550, 0);
			}
			// Sets up green buff if needed
			if(greenHead)
			{
				Vec2Set(&GreenBuff->Position, 550, 0);
				RedBuff->Visible = TRUE;
				Vec2Set(&GreenBuffCollider->Position, 550, 0);
			}
			// Sets up blue buff if needed
			if(blueHead)
			{
				Vec2Set(&BlueBuff->Position, 0, 270);
				RedBuff->Visible = TRUE;
				Vec2Set(&BlueBuffCollider->Position, 0, 270);
			}
			break;
		case 2:
			// 2-1-3
			// Sets up red buff if needed
			if(redHead)
			{
				Vec2Set(&RedBuff->Position, 550, 0);
				RedBuff->Visible = TRUE;
				Vec2Set(&RedBuffCollider->Position, 550, 0);
			}
			// Sets up green buff if needed
			if(greenHead)
			{
				Vec2Set(&GreenBuff->Position, -550, 0);
				RedBuff->Visible = TRUE;
				Vec2Set(&GreenBuffCollider->Position, -550, 0);
			}
			// Sets up blue buff if needed
			if(blueHead)
			{
				Vec2Set(&BlueBuff->Position, 0, 270);
				RedBuff->Visible = TRUE;
				Vec2Set(&BlueBuffCollider->Position, 0, 270);
			}
			break;
		case 3:
			// 2-3-1
			// Sets up red buff if needed
			if(redHead)
			{
				Vec2Set(&RedBuff->Position, 550, 0);
				RedBuff->Visible = TRUE;
				Vec2Set(&RedBuffCollider->Position, 550, 0);
			}
			// Sets up green buff if needed
			if(greenHead)
			{
				Vec2Set(&GreenBuff->Position, 0, 270);
				RedBuff->Visible = TRUE;
				Vec2Set(&GreenBuffCollider->Position, 0, 270);
			}
			// Sets up blue buff if needed
			if(blueHead)
			{
				Vec2Set(&BlueBuff->Position, -550, 0);
				RedBuff->Visible = TRUE;
				Vec2Set(&BlueBuffCollider->Position, -550, 0);
			}
			break;
		case 4:
			// 3-1-2
			// Sets up red buff if needed
			if(redHead)
			{
				Vec2Set(&RedBuff->Position, 550, 0);
				RedBuff->Visible = TRUE;
				Vec2Set(&RedBuffCollider->Position, 550, 0);
			}
			// Sets up green buff if needed
			if(greenHead)
			{
				Vec2Set(&GreenBuff->Position, -550, 0);
				RedBuff->Visible = TRUE;
				Vec2Set(&GreenBuffCollider->Position, -550, 0);
			}
			// Sets up blue buff if needed
			if(blueHead)
			{
				Vec2Set(&BlueBuff->Position, 0, 270);
				RedBuff->Visible = TRUE;
				Vec2Set(&BlueBuffCollider->Position, 0, 270);
			}
			break;
		case 5:
			// 3-2-1
			// Sets up red buff if needed
			if(redHead)
			{
				Vec2Set(&RedBuff->Position, 550, 0);
				RedBuff->Visible = TRUE;
				Vec2Set(&RedBuffCollider->Position, 550, 0);
			}
			// Sets up green buff if needed
			if(greenHead)
			{
				Vec2Set(&GreenBuff->Position, 0, 270);
				RedBuff->Visible = TRUE;
				Vec2Set(&GreenBuffCollider->Position, 0, 270);
			}
			// Sets up blue buff if needed
			if(blueHead)
			{
				Vec2Set(&BlueBuff->Position, -550, 0);
				RedBuff->Visible = TRUE;
				Vec2Set(&BlueBuffCollider->Position, -550, 0);
			}
			break;
		}
		buffsShown = TRUE;
	}
	else
		HideBuffs();
}

void HideBuffs(void)
{
	// Moves everything down below
	Vec2Set(&RedBuff->Position, 550, -999);
	RedBuff->Visible = TRUE;
	Vec2Set(&RedBuffCollider->Position, 550, -999);
	
	Vec2Set(&GreenBuff->Position, 0, -999);
	RedBuff->Visible = TRUE;
	Vec2Set(&GreenBuffCollider->Position, 0, -999);
	
	Vec2Set(&BlueBuff->Position, -550, -999);
	RedBuff->Visible = TRUE;
	Vec2Set(&BlueBuffCollider->Position, -550, -999);

	buffsShown = FALSE;
}

void UpdatePlayerBuff(void)
{
	// If a buff is already active ignore
	if(CurrentBuff != None)
	{
		playerBuffTimer += GetDeltaTime();

		// Buff time is up
		if(playerBuffTimer >= 10)
		{
			CurrentBuff = None;
			playerBuffTimer = 0.0f;
		}
	}
	// If there is a redHead, check collision with red buff
	else if(redHead && RectCircleCollision(&RedBuffCollider->Position, RedBuffCollider->width / 2, &CurrentPlayer.PlayerCollider))
	{
		CurrentBuff = Red;
		playerBuffTimer = 0.0f;
		HideBuffs();
		buffTimer = 0.0f;
	}
	// If there is a greenHead, check collision with green buff
	else if(greenHead && RectCircleCollision(&GreenBuffCollider->Position, GreenBuffCollider->width / 2, &CurrentPlayer.PlayerCollider))
	{
		CurrentBuff = Green;
		playerBuffTimer = 0.0f;
		HideBuffs();
		buffTimer = 0.0f;
	}
	// If there is a blueHead, check collision with blue buff
	else if(blueHead && RectCircleCollision(&BlueBuffCollider->Position, BlueBuffCollider->width / 2, &CurrentPlayer.PlayerCollider))
	{
		CurrentBuff = Blue;
		playerBuffTimer = 0.0f;
		HideBuffs();
		buffTimer = 0.0f;
	}

}
