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
static int levelComplete;
static int PlayerIsAlive;
static int beginningAnimation;
static int CurrentBuff;
float buffTimer;
float playerBuffTimer;

// Fake Bools
int buffsShown;
int redHead;
int greenHead;
int blueHead;
int moveBars;

FoxSound* IntelFoxStart;
FoxSound* IntelFoxEnd;

static int prevPlayed;
static int DieSound1;
static int DieSound2;
static int DieSound3;

Sprite* CurrentBuffSprite;

HUD* CurrentHUD;

Wall* RightWall;

Sprite* BlackOverlay;
Sprite* Arrow1;
static int Arrow1Grow;

//Boss HP Bar
Sprite* BossHPBarRed;
Sprite* BossHPBarBackRed;

Sprite* BossHPBarGreen;
Sprite* BossHPBarBackGreen;

Sprite* BossHPBarBlue;
Sprite* BossHPBarBackBlue;

// Buff Sprites and Collision boxes
Sprite* RedBuff;
CollisionBox* RedBuffCollider;

Sprite* GreenBuff;
CollisionBox* GreenBuffCollider;

Sprite* BlueBuff;
CollisionBox* BlueBuffCollider;

Sprite* IntelFoxBack;
Sprite* IntelFox;
static float IntelFoxValue;

Platform* Plat;

FoxSound* BackSnd;

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

	LoadTexture("TextureFiles/YGBuffRed.png");
	LoadTexture("TextureFiles/YGBuffGreen.png");
	LoadTexture("TextureFiles/YGBuffBlue.png");
	LoadPlayerSprites(CurrentPlayer.Princess);
	LoadYeahGuyBoss();
}

/*************************************************************************/
/*!
	\brief
	Initializes the objects for the level
*/
/*************************************************************************/
void InitializeYeahGuy(void)
{
	Vec3 Tint;
	Vec2 Position;
	ResetObjectList();
	ResetCamera();
	levelComplete = FALSE;
	beginningAnimation = TRUE;
	PlayerIsAlive = TRUE;

	DieSound1 = FALSE;
	DieSound2 = FALSE;
	DieSound3 = FALSE;

	// Initialize the player
	InitializePlayer(&CurrentPlayer, Mayple, -1260, -220);
	CurrentPlayer.PlayerCollider.Position = CurrentPlayer.Position;

	CurrentHUD = CreateHUD(&CurrentPlayer);

	CurrentBuff = None;
	buffsShown = FALSE;
	buffTimer = 0.0f;
	playerBuffTimer = 0.0f;

	/////////////////////////////////
	//		Backgrounds			   //
	/////////////////////////////////
	//Panel1
	CreateSprite("TextureFiles/YeahGuyBG.png", 1920, 1080, 1, 1, 1, 0, 0);
	CreateSprite("TextureFiles/TreeBackground4.png", 1920, 1080, 0, 1, 1, 0, 0);

	//Bounding Boxes
	CreateBoundingBoxes();

	// Arrow Initialize
	Arrow1 = (Sprite *)CreateSprite("TextureFiles/Arrow.png", 180, 165, 90, 1, 1, 0, 300);
	Arrow1->Visible = FALSE;
	Arrow1Grow = FALSE;

	// Black Overlay
	Vec3Set(&Tint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4000, 1, 1, 0, 0);
	BlackOverlay->Tint = Tint;

	// Boss HP Bar
	BossHPBarRed = (Sprite *)CreateSprite("TextureFiles/BossRedHealthBarMid.png", 1, 22, 3, 1, 1, -200, 470);
	BossHPBarBackRed = (Sprite *)CreateSprite("TextureFiles/BossYeahHealthBarBack.png", 820, 34, 2, 1, 1, 0, 470);

	BossHPBarGreen = (Sprite *)CreateSprite("TextureFiles/BossGreenHealthBarMid.png", 1, 22, 3, 1, 1, -200, 430);
	BossHPBarBackGreen = (Sprite *)CreateSprite("TextureFiles/BossYeahHealthBarBack.png", 820, 34, 2, 1, 1, 0, 430);

	BossHPBarBlue = (Sprite *)CreateSprite("TextureFiles/BossBlueHealthBarMid.png", 1, 22, 3, 1, 1, -200, 390);
	BossHPBarBackBlue = (Sprite *)CreateSprite("TextureFiles/BossYeahHealthBarBack.png", 820, 34, 2, 1, 1, 0, 390);

	/////////////////////////////////
	//		Platforms			   //
	/////////////////////////////////
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 300, 50, -550, -170);
	Plat->PlatformSprite->Visible = FALSE;
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 300, 50, 550, -170);
	Plat->PlatformSprite->Visible = FALSE;
	Plat = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 300, 50, 0, 100);
	Plat->PlatformSprite->Visible = FALSE;

	/////////////////////////////////
	//		Sounds				   //
	/////////////////////////////////
	IntelFoxStart = CreateSound("Sounds/IntelFoxYeahGuy.mp3", SmallSnd);
	IntelFoxEnd = CreateSound("Sounds/IntelFoxYeahGuyEnd.mp3", SmallSnd);

	BackSnd = CreateSound("Sounds/BossMusic.mp3", LargeSnd);
	/////////////////////////////////
	//			Walls			   //
	/////////////////////////////////
	//Create bounding walls
	CreateWall("TextureFiles/BlankPlatform.png", 400.0f, 1040.0f, -1160, 0);
	RightWall = CreateWall("TextureFiles/BlankPlatform.png", 400.0f, 1040.0f, 1160, 0);

	/////////////////////////////////
	//			Boss			   //
	/////////////////////////////////
	Boss = CreateYeahGuyBoss(0, 0);

	redHead = TRUE;
	greenHead = TRUE;
	blueHead = TRUE;

	/////////////////////////////////
	//			Objects			   //
	/////////////////////////////////
	IntelFoxBack	= (Sprite*)CreateSprite("TextureFiles/IntelFoxHeadBack.png", 256, 256, 300, 1, 1, 740, 380);
	IntelFox		= (Sprite*)CreateSprite("TextureFiles/IntelFoxHead.png", 256, 256, 300, 1, 1, 740, 380);
	IntelFox->Alpha = 0.0f;
	IntelFoxValue	= 0.0f;

	/////////////////////////////////
	//			Buffs			   //
	/////////////////////////////////
	Vec2Set(&Position, 0, 0);

	CurrentBuffSprite = (Sprite *)CreateSprite("TextureFiles/YGBuffRed.png", 50, 50, 200, 1, 1, 920, 500);

	// Creates the buffs, do not add these to 
	RedBuffCollider = (CollisionBox *) CallocMyAlloc(1, sizeof(CollisionBox));
	GreenBuffCollider = (CollisionBox *) CallocMyAlloc(1, sizeof(CollisionBox));
	BlueBuffCollider = (CollisionBox *) CallocMyAlloc(1, sizeof(CollisionBox));

	RedBuff = (Sprite *)CreateSprite("TextureFiles/YGBuffRed.png", 200, 200, 8, 1, 1, 0, 0);
	CreateCollisionBox(RedBuffCollider, &Position, FoodType, 200, 200, GetObjectID());

	GreenBuff = (Sprite *)CreateSprite("TextureFiles/YGBuffGreen.png", 200, 200, 8, 1, 1, 0, 0);
	CreateCollisionBox(GreenBuffCollider, &Position, FoodType, 200, 200, GetObjectID());

	BlueBuff = (Sprite *)CreateSprite("TextureFiles/YGBuffBlue.png", 200, 200, 8, 1, 1, 0, 0);
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
	PlayAudio(BackSnd);

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
		if(CurrentBuff == Red && redHead)
			CurrentBuffSprite->SpriteTexture = LoadTexture("TextureFiles/YGBuffRed.png");
		else if(CurrentBuff == Red && !redHead)
		{
			moveBars = TRUE;
			CurrentBuff = None;
		}
		else if(CurrentBuff == Green && greenHead)
			CurrentBuffSprite->SpriteTexture = LoadTexture("TextureFiles/YGBuffGreen.png");
		else if(CurrentBuff == Green && !greenHead)
		{
			moveBars = TRUE;
			CurrentBuff = None;
		}
		else if(CurrentBuff == Blue && blueHead)
			CurrentBuffSprite->SpriteTexture = LoadTexture("TextureFiles/YGBuffBlue.png");
		else if(CurrentBuff == Blue && !blueHead)
		{
			moveBars = TRUE;
			CurrentBuff = None;
		}
	}

	UpdatePlayerBuff();

	if(levelComplete || moveBars)
		MoveHPBars();

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

	EventYeahGuy();
	// This should be the last line in this function
	if(!levelComplete)
	{
		UpdateYeahGuyBoss(Boss);
	}
	UpdatePlayerPosition(&CurrentPlayer);

	UpdateHUDPosition(CurrentHUD);
	UpdateHUDItems(CurrentHUD, &CurrentPlayer);

	UpdateFloatingText();
	BoundingBoxUpdate();
	UpdateAllProjectiles();
	ParticleSystemUpdate();

	// When the boss dies
	if(!levelComplete && !Boss->redHead && !Boss->greenHead && !Boss->blueHead)
	{
		levelComplete = TRUE;
		Arrow1->Visible = TRUE;
		FreeWall(RightWall);
		FreeYeahGuyBoss(Boss);
	}

	// What to do when the boss is dead
	if(levelComplete)
	{
		UpdateArrow(Arrow1, &Arrow1Grow);

		if(CurrentPlayer.Position.x > (1920.0f / 2) + CurrentPlayer.PlayerCollider.width)
		{
			LevelCompletion();
		}

		BossHPBarRed->Visible = FALSE;
		BossHPBarGreen->Visible = FALSE;
		BossHPBarBlue->Visible = FALSE;
	}
	// Boss health bar logic
	else
	{
		BossHPBarRed->ScaleX = 800.0f * (Boss->CurrentRedHealth / (float)Boss->MaxHealth);
		BossHPBarRed->Position.x = -400.0f * (1 - (Boss->CurrentRedHealth / (float)Boss->MaxHealth));

		BossHPBarGreen->ScaleX = 800.0f * (Boss->CurrentGreenHealth / (float)Boss->MaxHealth);
		BossHPBarGreen->Position.x = -400.0f * (1 - (Boss->CurrentGreenHealth / (float)Boss->MaxHealth));

		BossHPBarBlue->ScaleX = 800.0f * (Boss->CurrentBlueHealth / (float)Boss->MaxHealth);
		BossHPBarBlue->Position.x = -400.0f * (1 - (Boss->CurrentBlueHealth / (float)Boss->MaxHealth));
	}
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
	else
		FreeYeahGuyBoss(Boss);

	//Yeah Guy is finally free
	FreeMyAlloc(Boss);

	FreeAllLists();
	FreeHUD(CurrentHUD);
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
	int i;

	// Check for any collision and handle the results
	DetectPlayerCollision();
	if(!levelComplete)
		DetectYeahGuyBossCollision(Boss, CurrentBuff);

	// Handle any input for the current player
	if(!beginningAnimation)
		InputPlayer(&CurrentPlayer);
	else if(!levelComplete)
	{
		// Make sure the boss stays put during the start
		Boss->cooldownTimer = 0;

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
			{
				beginningAnimation = FALSE;
			}
		}
		//Always animate the player otherwise the sprites get stuck in the middle
		Animation(&CurrentPlayer);
		UpdateCollisionPosition(&CurrentPlayer.PlayerWeapon->WeaponAttack, &CurrentPlayer.PlayerWeapon->WeaponAttackPosition);
		MoveObject(&CurrentPlayer.Position, CurrentPlayer.PlayerDirection, CurrentPlayer.Speed);
	}

#if defined _DEBUG
	if(FoxInput_KeyTriggered('U'))
	{
		SetDebugMode();
	}
	if(FoxInput_KeyTriggered('I'))
	{
		RemoveDebugMode();
	}
#endif

	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		if(PlayerIsAlive)
		{
			TogglePauseSound(BackSnd);
			InitializePause(&DrawYeahGuy);
			UpdatePause();
			TogglePauseSound(BackSnd);
		}
	}

	//Player Dies
	if(CurrentPlayer.CurrentPlayerStats.CurrentHealth <= 0.0f)
	{
		freeSound(BackSnd);
		PlayerIsAlive = FALSE;
		BlackOverlay->Position.x = GetCameraXPosition();
		BlackOverlay->Alpha = 0.5f;

		UpdateDeathConfirmObjects();
	}

	//Check if the boss is already saying something
	Boss->YeahGuySoundsPlay = FALSE;
	for(i = 0; i < 4; i++)
	{
		if(FoxSoundCheckIsPlaying(Boss->YeahGuyHit[i]))
			Boss->YeahGuySoundsPlay = TRUE;
	}
	if(FoxSoundCheckIsPlaying(Boss->YeahGuyYell) || FoxSoundCheckIsPlaying(Boss->YeahGuyDie))
		Boss->YeahGuySoundsPlay = TRUE;

	//Play sound if boss died
	if(Boss->CurrentBlueHealth <= 0 && !DieSound1 && !Boss->YeahGuySoundsPlay && (Boss->CurrentGreenHealth > 0 || Boss->CurrentRedHealth > 0))
	{
		DieSound1 = TRUE;
		PlayAudio(Boss->YeahGuyDie);
	}
	if(Boss->CurrentGreenHealth <= 0 && !DieSound2 && !Boss->YeahGuySoundsPlay && (Boss->CurrentGreenHealth > 0 || Boss->CurrentRedHealth > 0))
	{
		DieSound2 = TRUE;
		PlayAudio(Boss->YeahGuyDie);
	}
	if(Boss->CurrentRedHealth <= 0 && !DieSound3 && !Boss->YeahGuySoundsPlay && (Boss->CurrentGreenHealth > 0 || Boss->CurrentRedHealth > 0))
	{
		DieSound3 = TRUE;
		PlayAudio(Boss->YeahGuyDie);
	}

	//Intel Fox Starting Narrative
	if(!IntelFoxStart->hasPlayed)
	{
		PlayAudio(IntelFoxStart);
		IntelFoxStart->hasPlayed = TRUE;
	}

	if(Boss->CurrentBlueHealth <= 0 && Boss->CurrentGreenHealth <= 0 && Boss->CurrentRedHealth <= 0)
	{
		if(!IntelFoxEnd->hasPlayed && PlayerIsAlive)
		{
			PlayAudio(IntelFoxEnd);
			IntelFoxEnd->hasPlayed = TRUE;
		}
	}

	//When sound is play show Intel Fox in da corner
	if(FoxSoundCheckIsPlaying(IntelFoxStart) || FoxSoundCheckIsPlaying(IntelFoxEnd))
	{
		if(IntelFox->Alpha < 1)
			IntelFox->Alpha += 3 * GetDeltaTime();
	}
	else
	{
		if(IntelFox->Alpha > 0)
			IntelFox->Alpha -= 3 * GetDeltaTime();
	}

	//Always update intel foxes position you need him
	IntelFox->Position.x = GetCameraXPosition() + 740;
	
	IntelFoxValue += GetDeltaTime() * 8.0f;
	IntelFox->Rotation = sinf(IntelFoxValue) / 4.0f;

	IntelFoxBack->Position = IntelFox->Position;
	IntelFoxBack->Alpha = IntelFox->Alpha;
}

/*************************************************************************/
/*!
	\brief
	Shows all the buffs in a random location
*/
/*************************************************************************/
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

/*************************************************************************/
/*!
	\brief
	Hides all the buffs and moves the colliders away
*/
/*************************************************************************/
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

/*************************************************************************/
/*!
	\brief
	Updates the player's buff
*/
/*************************************************************************/
void UpdatePlayerBuff(void)
{
	// If a buff is already active ignore
	if(CurrentBuff != None)
	{
		int i;

		playerBuffTimer += GetDeltaTime();

		if(CurrentBuff == Green)
			i = 0;
		else if(CurrentBuff == Blue)
			i = 1;
		else if(CurrentBuff == Red)
			i = 2;

		CurrentPlayer.YGBuffParticles[i]->Position = CurrentPlayer.Position;
		CurrentPlayer.YGBuffParticles[i]->amountTotal = -1;

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

	if(CurrentBuff == None)
	{
		CurrentPlayer.YGBuffParticles[0]->amountTotal = 0;
		CurrentPlayer.YGBuffParticles[1]->amountTotal = 0;
		CurrentPlayer.YGBuffParticles[2]->amountTotal = 0;
	}
}

/*************************************************************************/
/*!
	\brief
	Moves and fades any bars that need to when a head dies
*/
/*************************************************************************/
void MoveHPBars(void)
{
	// Fake bools for fade
	int redFade = FALSE;
	int greenFade = FALSE;
	int blueFade = FALSE;

	// Fake bools for movement
	int movementDone = FALSE;

	// Red bar has completed movement
	if(!redHead && BossHPBarRed->Alpha <= 0)
	{
		BossHPBarRed->Alpha = 0;
		BossHPBarBackRed->Alpha = 0;
		redFade = TRUE;
	}

	// Green bar has completed movement
	if(!greenHead && BossHPBarGreen->Alpha <= 0)
	{
		BossHPBarGreen->Alpha = 0;
		BossHPBarBackGreen->Alpha = 0;
		greenFade = TRUE;
	}

	// Blue bar has completed movement
	if(!blueHead && BossHPBarBlue->Alpha <= 0)
	{
		BossHPBarBlue->Alpha = 0;
		BossHPBarBackBlue->Alpha = 0;
		blueFade = TRUE;
	}

	// Incoming huge if statement!!!!!
	// Fade out any and all hp bars if needed
	if(!redHead && BossHPBarRed->Alpha > 0)
	{
		BossHPBarRed->Alpha -= GetDeltaTime();
		BossHPBarBackRed->Alpha -= GetDeltaTime();
	}
	if(!greenHead && BossHPBarGreen->Alpha > 0)
	{
		BossHPBarGreen->Alpha -= GetDeltaTime();
		BossHPBarBackGreen->Alpha -= GetDeltaTime();
	}
	if(!blueHead && BossHPBarBlue->Alpha > 0)
	{
		BossHPBarBlue->Alpha -= GetDeltaTime();
		BossHPBarBackBlue->Alpha -= GetDeltaTime();
	}

	// Red is dead
	if(!redHead)
	{
		// Green and Red are dead
		if(!greenHead && BossHPBarBlue->Position.y < BossHPBarRed->Position.y)
		{
			BossHPBarBlue->Position.y += 40 * GetDeltaTime() * 2;
			BossHPBarBackBlue->Position.y += 40 * GetDeltaTime() * 2;
		}
		// Blue bar movement is done
		else if(!greenHead)
		{
			movementDone = TRUE;
		}
		// Only Red or Red and blue are dead
		else if(BossHPBarGreen->Position.y < BossHPBarRed->Position.y)
		{
			// Move blue and green bars
			BossHPBarBlue->Position.y += 40 * GetDeltaTime() * 2;
			BossHPBarBackBlue->Position.y += 40 * GetDeltaTime() * 2;

			BossHPBarGreen->Position.y += 40 * GetDeltaTime() * 2;
			BossHPBarBackGreen->Position.y += 40 * GetDeltaTime() * 2;
		}
		// All movement should be done
		else
		{
			movementDone = TRUE;
		}
	}
	// Red is alive
	else
	{
		// Only green is dead
		if(!greenHead && BossHPBarBlue->Position.y < BossHPBarGreen->Position.y)
		{
			BossHPBarBlue->Position.y += 40 * GetDeltaTime() * 2;
			BossHPBarBackBlue->Position.y += 40 * GetDeltaTime() * 2;
		}
		// Movement for blue is done
		else if(!greenHead)
		{
			movementDone = TRUE;
		}
		// Green and red are alive, don't need to move anything
		else
		{
			movementDone = TRUE;
		}
	}

	// All fade and movement is completed
	if(redFade + greenFade + blueFade == 3 && movementDone)
	{
		moveBars = FALSE;
	}
}
