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

Sprite *Seclude1;
Sprite *Seclude2;
Sprite *Seclude3;
Sprite *Seclude4;

Weapon *StarterAxe;
Weapon *StarterSword;

FoxSound *BackSnd;
FoxSound *GongSnd;

Wall *WallTemp;

Wall* BBWallLeft;
Wall* BBWallRight;

static int tutorialDone; // Keeps track of how much of the tutorial has been completed (Not a bool)
static int newID;
static int levelComplete;

void LoadTutorial(void)
{
	CreateTextureList();
}

void InitializeTutorial(void)
{
	Vec3 Tint = { 0, 0, 0 };

	levelComplete = FALSE;
	tutorialDone = 0;
	newID = 10;
	ResetObjectList();
	ResetCamera();
	ResetEnemyPanelNumber();

	BackSnd = CreateSound("Sounds/wave.mp3", SmallSnd);
	GongSnd = CreateSound("Sounds/GongHit.wav", SmallSnd);

	InitializePlayer(&CurrentPlayer, Ginko, -700.0f, GROUNDLEVEL * GetLoadRatio() + 1);

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
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 100.0f, 1040.0f, newID++, -900, 0);
	WallTemp->WallSprite->Visible = FALSE;
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 240.0f, 500.0f, newID++, 840, 200);
	WallTemp->WallSprite->Visible = FALSE;
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 240.0f, 500.0f, newID++, 840 + 1920, 200);
	WallTemp->WallSprite->Visible = FALSE;
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 240.0f, 500.0f, newID++, 840 + 1920 * 2, 200);
	WallTemp->WallSprite->Visible = FALSE;

	// Bounding Box Walls
	BBWallLeft = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 1080.0f, newID++, 0, 0);
	BBWallLeft->WallSprite->Visible = FALSE;
	BBWallLeft->enemyNotCollidable = TRUE;
	BBWallRight = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 1080.0f, newID++, 0, 0);
	BBWallRight->WallSprite->Visible = FALSE;
	BBWallRight->enemyNotCollidable = TRUE;

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

	//Sound volume
	SetChannelGroupVolume(EffectType, SFXVolume);
	SetChannelGroupVolume(MusicType, BGMVolume);

	/*////////////////////////////////
	//          PANEL ONE           //
	////////////////////////////////*/
	CreateSprite("TextureFiles/TutorialPanel1.png", 1920, 1080, 0, 1, 1, 0, 0);
	CreateSprite("TextureFiles/TutorialPanelDoor.png", 1920, 1080, 200, 1, 1, 0, 0);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 640, -110, 10, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 810, -270, 201, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	Shelf = CreatePlatform("TextureFiles/BlankPlatform.png", PlatformType, 480, 60, newID++, 500, 20);
	Shelf->PlatformSprite->Visible = FALSE;

	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 500.0f, newID++, 220, -180); //Tall
	WallTemp->WallSprite->Visible = FALSE;
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 500.0f, newID++, 300, -280); //Tall (back)
	WallTemp->WallSprite->Visible = FALSE;
	WallTemp = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 350.0f, newID++, 50, -350); //Short
	WallTemp->WallSprite->Visible = FALSE;

	/*////////////////////////////////
	//          PANEL TWO           //
	////////////////////////////////*/
	TutorialBackground = (Sprite *) CreateSprite("TextureFiles/TutorialPanel2.png", 1920, 1080, 0, 1, 1, 1920.0f, 0);
	DoorOverlay = (Sprite *) CreateSprite("TextureFiles/TutorialPanelDoor.png", 1920, 1080, 200, 1, 1, 1920.0f, 0);

	//Create the shelf sprite and initialize to be collidable
	Shelf = CreatePlatform("TextureFiles/Shelf.png", PlatformType, 184.5f, 367.5, newID++, 160 + 1920.0f, -156);
	UpdateCollider(&Shelf->PlatformCollider, Shelf->PlatformCollider.width, Shelf->PlatformCollider.height * 0.16f); 
	Shelf->PlatformCollider.Offset.y = Shelf->PlatformSprite->Height * 3 / 8;

	ShortShelf = CreatePlatform("TextureFiles/ShortShelf.png", PlatformType, 184.5f, 198.75f, newID++, -40 + 1920.0f, -240);
	ShortShelf->PlatformCollider.Offset.y = 5 * ShortShelf->PlatformSprite->Height / 16;
	UpdateCollider(&ShortShelf->PlatformCollider, ShortShelf->PlatformCollider.width, ShortShelf->PlatformCollider.height * 0.2f); 

	BouncyBed = CreatePlatform("TextureFiles/BlankPlatform.png", BounceType, 360.0f, 100.0f, newID++, -580 + 1920.0f, -320);
	BouncyBed->PlatformSprite->Visible = FALSE;
	BouncyBed->PlatformRigidBody.Restitution = 2.2f;

	StarterAxe = CreateDroppedWeapon(Axe, Common, 256, 256, newID++, -550 + 1920.0f, -270);
	StarterAxe->WeaponSprite->Rotation = (float)-FOX_PI / 3;

	StarterSword = CreateDroppedWeapon(Sword, Common, 250, 250, newID++, 160 + 1920.0f, 0);
	StarterSword->WeaponSprite->Rotation = (float)FOX_PI /4;

	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 640 + 1920.0f, -110, 10, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 810 + 1920.0f, -270, 201, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	/*////////////////////////////////
	//         PANEL THREE          //
	////////////////////////////////*/
	CreateSprite("TextureFiles/TutorialPanel3.png", 1920, 1080, 0, 1, 1, 1920.0f * 2, 0);
	CreateSprite("TextureFiles/TutorialPanelDoor.png", 1920, 1080, 200, 1, 1, 1920.0f * 2, 0);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 640 + 1920.0f * 2, -110, 10, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);
	CreateFoxParticleSystem("TextureFiles/FireParticle.png", 810 + 1920.0f * 2, -270, 201, -1, 5, 0.01f, 90, 45, 0.5f, -30.0f, 9, 10, 200, 0.25f, 1.0f);

	StrawDummy = CreateEnemy(Dummy, EnemyType, newID++, 750 + 1920.0f * 2, -250, 2);
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

	/*////////////////////////////////
	//     TUTORIAL COMPLETION      //
	////////////////////////////////*/
	if (tutorialDone < 1 && CurrentPlayer.PlayerCollider.Position.x > -300)
	{
		tutorialDone = 1;

		Seclude1->Position.y = 490 * GetLoadRatio();//Top
		Seclude1->Height     = 100 * GetLoadRatio();
		UpdateMesh(Seclude1);

		Seclude3->Position.y = 0;//Left
		Seclude3->Height     = 880 * GetLoadRatio();
		UpdateMesh(Seclude3);

		Seclude4->Position.x = 610 * GetLoadRatio();//Right
		Seclude4->Position.y = 0;
		Seclude4->Height     = 880 * GetLoadRatio();
		Seclude4->Width      = 700 * GetLoadRatio();
		UpdateMesh(Seclude4);

		//Seclude1 = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 600, 800, 1, 1, 0, 240); //Top
		//Seclude2 = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 100, 800, 1, 1, 0, -490); //Bottom
		//Seclude3 = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 100, 380, 800, 1, 1, -960, -250); //Left
		//Seclude4 = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1200, 380, 800, 1, 1, 360, -250); //Right
	}
	else if (tutorialDone < 2 && CurrentPlayer.PlayerCollider.Position.x > 150 * GetLoadRatio())
	{
		tutorialDone = 2;

		Seclude4->Position.x = 910 * GetLoadRatio();//Right
		Seclude4->Width      = 100 * GetLoadRatio();
		UpdateMesh(Seclude4);
	}
	else if (tutorialDone < 3 && CurrentPlayer.PlayerCollider.Position.x > (1920.0f / 2) * GetLoadRatio())
	{
		tutorialDone = 3;
	}

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

	if (tutorialDone > 2)
		UpdateHUDPosition(CurrentHUD);
	else
	{
		//DONT SHOW THE HUD YET
		Vec2 newPosition;
		Vec2Set(&newPosition, (GetCameraXPosition() - 685 * GetLoadRatio() - 1000), 300 * GetLoadRatio());

		//HUD items update position
		CurrentHUD->HUDBackground->Position.x = (GetCameraXPosition() - 735 * GetLoadRatio() - 1000);

		CurrentHUD->HUDItem[0]->Position.x = (GetCameraXPosition() - 800 * GetLoadRatio() - 1000);
		CurrentHUD->HUDItem[1]->Position.x = (GetCameraXPosition() - 800 * GetLoadRatio() - 1000);
		CurrentHUD->HUDItem[2]->Position.x = (GetCameraXPosition() - 800 * GetLoadRatio() - 1000);
		CurrentHUD->HUDItem[3]->Position.x = (GetCameraXPosition() - 800 * GetLoadRatio() - 1000);

		CurrentHUD->HUDItem[4]->ScaleX = 71.0f * (CurrentPlayer.CurrentPlayerStats.CurrentHealth / (float)CurrentPlayer.CurrentPlayerStats.MaxHealth);
		CurrentHUD->HUDItem[4]->Position.x = (CurrentHUD->HUDBackground->Position.x - (2 * GetLoadRatio()) + (CurrentHUD->HUDItem[4]->ScaleX * GetLoadRatio() - 1000));
		CurrentHUD->HUDItem[5]->Position.x = (CurrentHUD->HUDBackground->Position.x + (CurrentHUD->HUDItem[4]->ScaleX * GetLoadRatio() * 2) - 1000);

		//Coin text update position
		ChangeTextPosition(CurrentHUD->CoinText, newPosition, Center);
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
	/*////////////////////////////////
	//   INPUT & COLLISION FIRST    //
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
		Animation(&CurrentPlayer);
		UpdateCollisionPosition(&CurrentPlayer.PlayerWeapon->WeaponAttack, &CurrentPlayer.PlayerWeapon->WeaponAttackPosition);
		// Move left if A is pressed
		if(FoxInput_KeyDown('A'))
		{
			CurrentPlayer.FlipX = 0;
			CurrentPlayer.PlayerDirection = LEFT;
			CurrentPlayer.Speed = CurrentPlayer.CurrentPlayerStats.MoveSpeed * GetLoadRatio() * GetDeltaTime();
		}
		// Move right if D is pressed
		else if(FoxInput_KeyDown('D'))
		{
			CurrentPlayer.FlipX = 1;
			CurrentPlayer.PlayerDirection = RIGHT;
			CurrentPlayer.Speed = CurrentPlayer.CurrentPlayerStats.MoveSpeed * GetLoadRatio() * GetDeltaTime();
		}
		else
		{
			if (!(CurrentPlayer.Position.y > GROUNDLEVEL * GetLoadRatio()) && !CurrentPlayer.PlayerRigidBody.onGround)
			{
				if (CurrentPlayer.Speed - 48.0f * GetDeltaTime() >= 0.0f)
				{
					CurrentPlayer.Speed -= 48.0f * GetDeltaTime();
				}
				else
				{
					CurrentPlayer.Speed = 0.0f;
					CurrentPlayer.LegSinValue = 0;
				}
			}
			else
			{
				if (CurrentPlayer.Speed - 48.0f * GetDeltaTime() >= 0.0f)
				{
					CurrentPlayer.Speed -= 48.0f * GetDeltaTime();
				}
				else
				{
					CurrentPlayer.Speed = 0.0f;
					CurrentPlayer.LegSinValue = 0;
				}
			}
		}

		// TUTORIAL PART 1
		if (tutorialDone > 0)
		{
			//Jump when space is pushed or drop down if S is pushed as well
			if(FoxInput_KeyTriggered(VK_SPACE))
			{
				Vec2 velocity;
		
				if(FoxInput_KeyDown('S') && CurrentPlayer.PlayerRigidBody.onGround && tutorialDone > 1) //TUTORIAL PART 2
				{
					CurrentPlayer.PlayerRigidBody.onGround = FALSE;
					CurrentPlayer.dropDown = TRUE;
					CurrentPlayer.dropdownTimer = 0.25f;
				}

				Vec2Set(&velocity, 0.0f, 1080.0f * GetLoadRatio());
				if(CurrentPlayer.Position.y <= GROUNDLEVEL * GetLoadRatio() || CurrentPlayer.PlayerRigidBody.onGround)
				{
					Vec2Set(&CurrentPlayer.Position, CurrentPlayer.Position.x, CurrentPlayer.Position.y + 300.0f * GetDeltaTime() * GetLoadRatio());
					CurrentPlayer.PlayerRigidBody.onGround = FALSE;
					ApplyVelocity(&CurrentPlayer.PlayerRigidBody, &velocity);
				}
			}
		}
		
		// TUTORIAL PART 3
		if (tutorialDone > 2)
		{
			if (FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT) && !CurrentPlayer.isAttacking)
			{
				//Pick a random swing sound to play
				if (rand() % 2)
				{
					if (!FoxSoundCheckIsPlaying(CurrentPlayer.CurrentPlayerSounds.Swing2))
						PlayAudio(CurrentPlayer.CurrentPlayerSounds.Swing1);
				}
				else
				{
					if (!FoxSoundCheckIsPlaying(CurrentPlayer.CurrentPlayerSounds.Swing1))
						PlayAudio(CurrentPlayer.CurrentPlayerSounds.Swing2);
				}

				//Set the attacking necessaries
				CurrentPlayer.isAttacking = TRUE;
				CurrentPlayer.PlayerSpriteParts.AttackRotation = 0;
				CurrentPlayer.PlayerSpriteParts.AttackRotationArm = 0;
				CurrentPlayer.PlayerSpriteParts.AttackRotationArmLower = 0;
				UpdateCollider(&CurrentPlayer.PlayerCollider,CurrentPlayer.PlayerCollider.width, CurrentPlayer.PlayerCollider.height);
			}

			//Cycle through the buffs
			if(FoxInput_KeyTriggered('Q'))
			{
				//The starting buff so we don't infini-loop
				int startingBuff = CurrentPlayer.BuffSelected++;

				//Loop back around
				if(CurrentPlayer.BuffSelected > 3)
					CurrentPlayer.BuffSelected = 0;

				//Go to the next acquired buff, or stop looping
				while(!CurrentPlayer.BuffHeld[CurrentPlayer.BuffSelected] && startingBuff != CurrentPlayer.BuffSelected)
				{
					CurrentPlayer.BuffSelected++;
					if(CurrentPlayer.BuffSelected > 3)
					{
						CurrentPlayer.BuffSelected = 0;
					}
				}
			}

			//Use the buff
			if(FoxInput_KeyTriggered('F'))
			{
				if(CurrentPlayer.BuffHeld[CurrentPlayer.BuffSelected])
				{
					int startingBuff;
					// Currently going with a 20% increase
					// Sets the timer for 10 seconds
					switch(CurrentPlayer.BuffSelected)
					{
					// Buffs the player's attack speed
					case Agility:
						CurrentPlayer.CurrentPlayerStats.AttackSpeed *= 1.2f;
						CurrentPlayer.CurrentPlayerStats.AgilityTimer = 10;
						break;
					// Buffs the player's damage
					case Strength:
						CurrentPlayer.CurrentPlayerStats.Damage = (int)(CurrentPlayer.CurrentPlayerStats.Damage * 1.2f);
						CurrentPlayer.CurrentPlayerStats.StrengthTimer = 10;
						break;
					// Buffs the player's damage reduction
					case Defense:
						CurrentPlayer.CurrentPlayerStats.DamageReduction *= 1.2f;
						CurrentPlayer.CurrentPlayerStats.DefenseTimer = 10;
						break;
					// Buffs the player's move speed
					case Haste:
						CurrentPlayer.CurrentPlayerStats.MoveSpeed *= 1.2f;
						CurrentPlayer.CurrentPlayerStats.HasteTimer = 10;
						break;
					}
					CurrentPlayer.BuffHeld[CurrentPlayer.BuffSelected] = FALSE;
					//Cycles automatically to the next available buff
					//The starting buff so we don't infini-loop
					startingBuff = CurrentPlayer.BuffSelected++;
					//Loop back around
					if(CurrentPlayer.BuffSelected > 3)
						CurrentPlayer.BuffSelected = 0;

					//Go to the next acquired buff, or stop looping
					while(!CurrentPlayer.BuffHeld[CurrentPlayer.BuffSelected] && startingBuff != CurrentPlayer.BuffSelected)
					{
						CurrentPlayer.BuffSelected++;
						if(CurrentPlayer.BuffSelected > 3)
						{
							CurrentPlayer.BuffSelected = 0;
						}
					}
				}
			}
		}

		CurrentPlayer.PlayerRigidBody.Acceleration.x = 0;
		CurrentPlayer.PlayerRigidBody.Acceleration.y = 0;

		// Move the direction based on the speed
		MoveObject(&CurrentPlayer.Position, CurrentPlayer.PlayerDirection, CurrentPlayer.Speed);

		UpdateHUDItems(CurrentHUD, &CurrentPlayer);
	}
	else
		fadeToEnd();

	/*////////////////////////////////
	//    CAMERA POSITION SECOND    //
	////////////////////////////////*/
	SetCameraLockState(FALSE);
	//Panel1
	if(CurrentPlayer.Position.x > -(PANELSIZE / 2) * GetLoadRatio() && CurrentPlayer.Position.x < (PANELSIZE / 2) * GetLoadRatio())
	{
		if(EnemyPanelNumber[0] > 0 && GetCameraMovedState())
			SetCameraLockState(TRUE);
		SetCameraPan(0.0f, PANELSIZE);
	}
	//Panel2
	else if(CurrentPlayer.Position.x > (PANELSIZE / 2) * GetLoadRatio() && CurrentPlayer.Position.x < (PANELSIZE + (PANELSIZE / 2)) * GetLoadRatio())
	{
		if(EnemyPanelNumber[1] > 0 && GetCameraMovedState())
			SetCameraLockState(TRUE);
		SetCameraPan(PANELSIZE * GetLoadRatio(), PANELSIZE);
	}
	//Panel3
	else if(CurrentPlayer.Position.x > (PANELSIZE + (PANELSIZE / 2)) * GetLoadRatio() && CurrentPlayer.Position.x < ((PANELSIZE * 2) + (PANELSIZE / 2)) * GetLoadRatio())
	{
		if(EnemyPanelNumber[2] > 0 && GetCameraMovedState())
			SetCameraLockState(TRUE);
		SetCameraPan((PANELSIZE * 2) * GetLoadRatio(), PANELSIZE);
	}
	else if(CurrentPlayer.Position.x > (PANELSIZE / 2) * 5 * GetLoadRatio() + CurrentPlayer.PlayerCollider.width)
	{
			levelComplete = TRUE;
	}

	BBWallLeft->Position.y = -1080 * GetLoadRatio() + 1080 * GetLoadRatio() * GetCameraLockState();
	BBWallLeft->Position.x = GetCameraXPosition() - (PANELSIZE / 2 * GetLoadRatio());
	UpdateCollisionPosition(&BBWallLeft->WallCollider, &BBWallLeft->Position);
	BBWallRight->Position.y = -1080 * GetLoadRatio() + 1080 * GetLoadRatio() * GetCameraLockState();
	BBWallRight->Position.x = GetCameraXPosition() + (PANELSIZE / 2 * GetLoadRatio());
	UpdateCollisionPosition(&BBWallRight->WallCollider, &BBWallRight->Position);

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
