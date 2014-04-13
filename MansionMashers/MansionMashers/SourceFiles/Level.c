/*****************************************************************************/
/*!
\file				Level.c
\author				Kaden Nugent (kaden.n)
\date				Mar 11, 2014

\brief				

\par				Functions:
\li					
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"

#define UPGRADE_MAX 8

static Sprite *IndiOverlay; //Just for fun
static Sprite *HurtOverlay; //Warns the player they've been hit
static float lastHealth;
static int lastDefense;
static Sprite *GuudJub;
static float holdTime;

static int UpgradesToUpgrade; //If the player even has points to spend
static int lastUpgrades;      //As to not update the text every frame
static int allUpgradesFull;   //If there aren't any upgrades left

enum PopUp { PopUp_Rise, PopUp_Hold, PopUp_Fall };

static TextGlyphs *UpgradesName;
static TextGlyphs *UpgradesLeft;
static Button *MainMapButton;

static Sprite *BlackOverlay;
static Sprite *WhiteOverlay;

static Button *UpgradeButton1;
static Button *UpgradeButton2;
static Button *UpgradeButton3;

static Sprite *UpgradeBar1;
static Sprite *UpgradeBar2;
static Sprite *UpgradeBar3;

static Sprite *UpgradeBarColor1;
static Sprite *UpgradeBarColor2;
static Sprite *UpgradeBarColor3;

static TextGlyphs *UpgradeAmount1;
static TextGlyphs *UpgradeAmount2;
static TextGlyphs *UpgradeAmount3;

static Sprite* HazeBackground;
static ParticleSystem* SystemOne;

/*************************************************************************/
/*!
	\brief
	Gets a fresh object ID from the ObjectID pool
*/
/*************************************************************************/
int GetObjectID(void)
{
	return ++ObjectID;
}

/*************************************************************************/
/*!
	\brief
	Creates the upgrade screen
*/
/*************************************************************************/
void CreateUpgradeScreenObjects(void)
{
	float camX = GetCameraXPosition();
	Vec3 NewTint;

	lastUpgrades      = -1;
	UpgradeComplete   = FALSE;
	UpgradesToUpgrade = FALSE;
	allUpgradesFull   = FALSE;

	//Particle Background
	HazeBackground = (Sprite *)CreateSprite("TextureFiles/MapHaze.png", 4000, 1080, 4000, 1, 1, 480, -200);
	HazeBackground->Alpha = 0;
	SystemOne = CreateFoxParticleSystem("TextureFiles/MapParticle.png", 0, -2000, 4001, -1, 15, 0.5f, 0, 100, 20.0f, 5.0f, 4000, 1080, 50, 2.0f, 2.0f);
	SystemOne->FadeIn = TRUE;

	//Title
	Vec3Set(&NewTint, 1, 1, 1);
	UpgradesName = CreateText("Upgrades Available!", 0, -2000, 140, NewTint, Center, Border);
	ChangeTextVisibility(UpgradesName);
	ChangeTextZIndex(UpgradesName, 4003);

	//Upgrades remaining
	Vec3Set(&NewTint, 0.32f, 0.11f, 0.47f);
	UpgradesLeft = CreateText("Sweg", 0, -2000, 100, NewTint, Center, Border);
	ChangeTextVisibility(UpgradesLeft);
	ChangeTextZIndex(UpgradesLeft, 4003);

	//Overlays
	Vec3Set(&NewTint, 0, 0, 0);
	BlackOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 4010, 1, 1, 0, 0);
	BlackOverlay->Tint = NewTint;
	BlackOverlay->Alpha = 0;
	Vec3Set(&NewTint, 0.9f, 0.9f, 0.9f);
	WhiteOverlay = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 3999, 1, 1, 0, 0);
	WhiteOverlay->Tint = NewTint;
	WhiteOverlay->Alpha = 0;

	//Upgrade buttons, all 3 (Not really sure why Agility is in the middle, but that is how the rest of the game is)
	UpgradeButton1 = CreateButton("TextureFiles/UpgradeButtonStr.png", 545 + camX, -2000, 347, 146); //Strength
	UpgradeButton1->ButtonSprite->ZIndex = 4003;
	UpgradeButton2 = CreateButton("TextureFiles/UpgradeButtonAgi.png", 545 + camX, -2000, 347, 146); //Agility
	UpgradeButton2->ButtonSprite->ZIndex = 4003;
	UpgradeButton3 = CreateButton("TextureFiles/UpgradeButtonDef.png", 545 + camX, -2000, 347, 146); //Defense
	UpgradeButton3->ButtonSprite->ZIndex = 4003;

	//Upgrade bars
	UpgradeBar1 = (Sprite *) CreateSprite("TextureFiles/UpgradeBar.png", 1920, 1080, 4003, 1, 1, 0, -2000);
	UpgradeBar2 = (Sprite *) CreateSprite("TextureFiles/UpgradeBar.png", 1920, 1080, 4003, 1, 1, 0, -2000);
	UpgradeBar3 = (Sprite *) CreateSprite("TextureFiles/UpgradeBar.png", 1920, 1080, 4003, 1, 1, 0, -2000);

	//Upgrade bar colors
	UpgradeBarColor1 = (Sprite *) CreateSprite("TextureFiles/UpgradeBarColor.png", 816, 85, 4002, 1, 1, 0, -2000);
	UpgradeBarColor2 = (Sprite *) CreateSprite("TextureFiles/UpgradeBarColor.png", 816, 85, 4002, 1, 1, 0, -2000);
	UpgradeBarColor3 = (Sprite *) CreateSprite("TextureFiles/UpgradeBarColor.png", 816, 85, 4002, 1, 1, 0, -2000);

	//How many upgrades have been used for each skill type
	Vec3Set(&NewTint, 0.32f, 0.11f, 0.47f);

	UpgradeAmount1 = CreateText("Swog", 0, -2000, 120, NewTint, Center, Border);
	ChangeTextVisibility(UpgradeAmount1);
	ChangeTextZIndex(UpgradeAmount1, 4004);

	UpgradeAmount2 = CreateText("Swug", 0, -2000, 120, NewTint, Center, Border);
	ChangeTextVisibility(UpgradeAmount2);
	ChangeTextZIndex(UpgradeAmount2, 4004);

	UpgradeAmount3 = CreateText("Swyg", 0, -2000, 120, NewTint, Center, Border);
	ChangeTextVisibility(UpgradeAmount3);
	ChangeTextZIndex(UpgradeAmount3, 4004);

	//Done upgrading button
	MainMapButton = CreateButton("TextureFiles/GoToMapButton.png", 400 + camX, -2000, 300, 112.5f);
	MainMapButton->ButtonSprite->ZIndex = 4003;
}

/*************************************************************************/
/*!
	\brief
	Updates the upgrade screen, allows the player to upgrade
*/
/*************************************************************************/
void UpdateUpgradeScreenObjects(void)
{
	float camX = GetCameraXPosition();
	char CharTemp[32];

	int worldX, worldY;
	Vec2 MouseClick;
	Vec2 NewPosition;

	FoxInput_GetWorldPosition(&worldX, &worldY);
	Vec2Set(&MouseClick, (float)worldX, (float)worldY);

	//Update the particles
	ParticleSystemUpdate();

	//Check if there are upgrades
	if (!UpgradesToUpgrade)
	{
		if (CurrentPlayer.CurrentPlayerStats.Upgrades > 0)
			UpgradesToUpgrade = TRUE;
		else if (CurrentPlayer.CurrentPlayerStats.Upgrades < 1)
		{
			//No upgrade points, get out
			UpgradeComplete = TRUE;
			return;
		}
	}

	//Title
	if (CurrentPlayer.CurrentPlayerStats.Strength >= UPGRADE_MAX
	 && CurrentPlayer.CurrentPlayerStats.Agility >= UPGRADE_MAX
	 && CurrentPlayer.CurrentPlayerStats.Defense >= UPGRADE_MAX
	 && !allUpgradesFull)
	{
		//If no more upgrades remain, let them know
		allUpgradesFull = TRUE;
		ChangeTextString(UpgradesName, "All Upgrades Complete!");
		ChangeTextZIndex(UpgradesName, 4003);
	}
	Vec2Set(&NewPosition, camX, 470);
	ChangeTextPosition(UpgradesName, NewPosition, Center);

	//Upgrades remaining
	if (lastUpgrades != CurrentPlayer.CurrentPlayerStats.Upgrades)
	{
		//Do some flavor text warnings for differnt UP amounts
		if (allUpgradesFull)
			sprintf(CharTemp, "Good Work! Keep Mashing!");
		else if (CurrentPlayer.CurrentPlayerStats.Upgrades == 1)
			sprintf(CharTemp, "%i Point Remaining!", CurrentPlayer.CurrentPlayerStats.Upgrades);
		else if (CurrentPlayer.CurrentPlayerStats.Upgrades < 1)
			sprintf(CharTemp, "No Points! Level Up More!");
		else
			sprintf(CharTemp, "%i Points to Spend!", CurrentPlayer.CurrentPlayerStats.Upgrades);
		ChangeTextString(UpgradesLeft, CharTemp);
		ChangeTextZIndex(UpgradesLeft, 4003);
		lastUpgrades = CurrentPlayer.CurrentPlayerStats.Upgrades;
	}
	Vec2Set(&NewPosition, camX, 380);
	ChangeTextPosition(UpgradesLeft, NewPosition, Center);

	//Upgrade buttons, all 3
	UpdateButtonPosition(UpgradeButton1, 545 + camX, 200);
	UpdateCollisionPosition(&UpgradeButton1->ButtonCollider, &UpgradeButton1->ButtonSprite->Position);
	UpdateButtonPosition(UpgradeButton2, 545 + camX, 0);
	UpdateCollisionPosition(&UpgradeButton2->ButtonCollider, &UpgradeButton2->ButtonSprite->Position);
	UpdateButtonPosition(UpgradeButton3, 545 + camX, -200);
	UpdateCollisionPosition(&UpgradeButton3->ButtonCollider, &UpgradeButton3->ButtonSprite->Position);

	//Upgrade bars
	UpgradeBar1->Position.x = camX;
	UpgradeBar1->Position.y = 200;
	UpgradeBar2->Position.x = camX;
	UpgradeBar2->Position.y = 0;
	UpgradeBar3->Position.x = camX;
	UpgradeBar3->Position.y = -200;

	//Particle / Haze back
	HazeBackground->Position.x = camX + 480;
	HazeBackground->Position.y = 0;
	SystemOne->Position.y = 0;
	SystemOne->Position.x = camX;
	SystemOne->amountTotal = -1;

	if (HazeBackground->Alpha < 1)
		HazeBackground->Alpha += 2 * GetDeltaTime();
	else
		HazeBackground->Alpha = 1;

	//Upgrade bar colors
	//Str --
	UpgradeBarColor1->Position.x = camX - 61 - 52.0f * (UPGRADE_MAX - CurrentPlayer.CurrentPlayerStats.Strength);
	UpgradeBarColor1->Position.y = 200;
	UpgradeBarColor1->ScaleX     = 1.0f * ((float)CurrentPlayer.CurrentPlayerStats.Strength / UPGRADE_MAX);
	UpgradeBarColor1->ScaleY     = 1.1f;
	//Agi --
	UpgradeBarColor2->Position.x = camX - 61 - 52.0f * (UPGRADE_MAX - CurrentPlayer.CurrentPlayerStats.Agility);
	UpgradeBarColor2->Position.y = 0;
	UpgradeBarColor2->ScaleX     = 1.0f * ((float)CurrentPlayer.CurrentPlayerStats.Agility / UPGRADE_MAX);
	UpgradeBarColor2->ScaleY     = 1.1f;
	//Def --
	UpgradeBarColor3->Position.x = camX - 61 - 52.0f * (UPGRADE_MAX - CurrentPlayer.CurrentPlayerStats.Defense);
	UpgradeBarColor3->Position.y = -200;
	UpgradeBarColor3->ScaleX     = 1.0f * ((float)CurrentPlayer.CurrentPlayerStats.Defense / UPGRADE_MAX);
	UpgradeBarColor3->ScaleY     = 1.1f;

	//Upgrades used placement
	//Str --
	sprintf(CharTemp, "%i", CurrentPlayer.CurrentPlayerStats.Strength);
	ChangeTextString(UpgradeAmount1, CharTemp);
	ChangeTextZIndex(UpgradeAmount1, 4003);
	Vec2Set(&NewPosition, camX - 590, 200);
	ChangeTextPosition(UpgradeAmount1, NewPosition, Center);
	//Agi --
	sprintf(CharTemp, "%i", CurrentPlayer.CurrentPlayerStats.Agility);
	ChangeTextString(UpgradeAmount2, CharTemp);
	ChangeTextZIndex(UpgradeAmount2, 4003);
	Vec2Set(&NewPosition, camX - 590, 0);
	ChangeTextPosition(UpgradeAmount2, NewPosition, Center);
	//Def --
	sprintf(CharTemp, "%i", CurrentPlayer.CurrentPlayerStats.Defense);
	ChangeTextString(UpgradeAmount3, CharTemp);
	ChangeTextZIndex(UpgradeAmount3, 4003);
	Vec2Set(&NewPosition, camX - 590, -200);
	ChangeTextPosition(UpgradeAmount3, NewPosition, Center);

	//Done upgrading button
	UpdateButtonPosition(MainMapButton, 400 + camX, -400);
	UpdateCollisionPosition(&MainMapButton->ButtonCollider, &MainMapButton->ButtonSprite->Position);

	//STRENGTH (No up allowed if maxed)
	if (CurrentPlayer.CurrentPlayerStats.Upgrades > 0 && PointRectCollision(&UpgradeButton1->ButtonCollider, &MouseClick)
	 && CurrentPlayer.CurrentPlayerStats.Strength < UPGRADE_MAX)
	{
		UpgradeButton1->ButtonSprite->ScaleX = 1.12f;
		UpgradeButton1->ButtonSprite->ScaleY = 1.12f;

		//Make the button react
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			CurrentPlayer.CurrentPlayerStats.Upgrades--;
			CurrentPlayer.CurrentPlayerStats.Strength++;

			UpgradeButton1->ButtonSprite->ScaleX = 1.16f;
			UpgradeButton1->ButtonSprite->ScaleY = 1.16f;
		}
	}
	else
	{
		if (UpgradeButton1->ButtonSprite->ScaleX > 1.0f)
		{
			UpgradeButton1->ButtonSprite->ScaleX -= GetDeltaTime() * 2;
			UpgradeButton1->ButtonSprite->ScaleY -= GetDeltaTime() * 2;
		}
	}
	//AGILITY (No up allowed if maxed)
	if (CurrentPlayer.CurrentPlayerStats.Upgrades > 0 && PointRectCollision(&UpgradeButton2->ButtonCollider, &MouseClick)
	 && CurrentPlayer.CurrentPlayerStats.Agility < UPGRADE_MAX)
	{
		UpgradeButton2->ButtonSprite->ScaleX = 1.12f;
		UpgradeButton2->ButtonSprite->ScaleY = 1.12f;

		//Make the button react
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			CurrentPlayer.CurrentPlayerStats.Upgrades--;
			CurrentPlayer.CurrentPlayerStats.Agility++;

			UpgradeButton2->ButtonSprite->ScaleX = 1.16f;
			UpgradeButton2->ButtonSprite->ScaleY = 1.16f;
		}
	}
	else
	{
		if (UpgradeButton2->ButtonSprite->ScaleX > 1.0f)
		{
			UpgradeButton2->ButtonSprite->ScaleX -= GetDeltaTime() * 2;
			UpgradeButton2->ButtonSprite->ScaleY -= GetDeltaTime() * 2;
		}
	}
	//DEFENSE (No up allowed if maxed)
	if (CurrentPlayer.CurrentPlayerStats.Upgrades > 0 && PointRectCollision(&UpgradeButton3->ButtonCollider, &MouseClick)
	 && CurrentPlayer.CurrentPlayerStats.Defense < UPGRADE_MAX)
	{
		UpgradeButton3->ButtonSprite->ScaleX = 1.12f;
		UpgradeButton3->ButtonSprite->ScaleY = 1.12f;

		//Make the button react
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			CurrentPlayer.CurrentPlayerStats.Upgrades--;
			CurrentPlayer.CurrentPlayerStats.Defense++;

			UpgradeButton3->ButtonSprite->ScaleX = 1.16f;
			UpgradeButton3->ButtonSprite->ScaleY = 1.16f;
		}
	}
	else
	{
		if (UpgradeButton3->ButtonSprite->ScaleX > 1.0f)
		{
			UpgradeButton3->ButtonSprite->ScaleX -= GetDeltaTime() * 2;
			UpgradeButton3->ButtonSprite->ScaleY -= GetDeltaTime() * 2;
		}
	}

	//MAIN MENU
	if(PointRectCollision(&MainMapButton->ButtonCollider, &MouseClick))
	{
		MainMapButton->ButtonSprite->ScaleX = 1.2f;
		MainMapButton->ButtonSprite->ScaleY = 1.2f;

		//Make the button react
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			UpgradeComplete = TRUE;
		}
	}
	else
	{
		MainMapButton->ButtonSprite->ScaleX = 1.0f;
		MainMapButton->ButtonSprite->ScaleY = 1.0f;
	}
}

/*************************************************************************/
/*!
	\brief
	Shows a hurt overlay if the player is damaged
*/
/*************************************************************************/
void InitializePlayerHurt(Player *CurrentPlayer)
{
	IndiOverlay = (Sprite *) CreateSprite("TextureFiles/HurtOverlay.png", 1920, 1080, 1401, 1, 1, 0, 0);
	IndiOverlay->Tint.x = 0;
	IndiOverlay->Tint.y = 0;
	IndiOverlay->Tint.z = 0;
	IndiOverlay->ScaleX = 3.0f;
	IndiOverlay->ScaleY = 6.0f;

	HurtOverlay = (Sprite *) CreateSprite("TextureFiles/HurtOverlay.png", 1920, 1080, 1400, 1, 1, 0, 0);
	HurtOverlay->ScaleX = 6.0f;
	HurtOverlay->ScaleY = 6.0f;
}

/*************************************************************************/
/*!
	\brief
	Shows a hurt overlay if the player is damaged
*/
/*************************************************************************/
void UpdatePlayerHurt(Player *CurrentPlayer)
{
	IndiOverlay->Position.x = GetCameraXPosition();

	HurtOverlay->Position.x = GetCameraXPosition();

	if (HurtOverlay->ScaleX < 6.0f)
	{
		HurtOverlay->ScaleX += 2 * GetDeltaTime();
		HurtOverlay->ScaleY += 2 * GetDeltaTime();
	}

	//Displayer that player damage layer
	if (lastHealth > CurrentPlayer->CurrentPlayerStats.CurrentHealth && lastHealth > 0 && lastDefense == CurrentPlayer->PlayerWeapon->BonusDefense)
	{
		float overlayScalar = 1.0f + 1.5f * ((float)CurrentPlayer->CurrentPlayerStats.CurrentHealth / CurrentPlayer->CurrentPlayerStats.MaxHealth);
		if (overlayScalar < 1)
			overlayScalar = 1.0f;
		HurtOverlay->ScaleX = overlayScalar;
		HurtOverlay->ScaleY = overlayScalar;
	}

	lastDefense = CurrentPlayer->PlayerWeapon->BonusDefense; //Player won't get hurt when picking weapon with less defense
	lastHealth  = CurrentPlayer->CurrentPlayerStats.CurrentHealth;
}

/*************************************************************************/
/*!
	\brief
	Checks if the player is ranking up
*/
/*************************************************************************/
void InitializePlayerRank(Player *CurrentPlayer)
{
	GuudJub = (Sprite *) CreateSprite("TextureFiles/RankedUp.png", 1920, 1080, 1600, 1, 1, 0, -180);
	GuudJub->ItemType = PopUp_Fall;
}

/*************************************************************************/
/*!
	\brief
	Checks if the player is ranking up
*/
/*************************************************************************/
void UpdatePlayerRank(Player *CurrentPlayer)
{
	int nextExperience = GetMaxExperience(CurrentPlayer);

	//Increase the rank if XP has mer it's goal
	if (CurrentPlayer->CurrentPlayerStats.Experience >= nextExperience)
	{
		CurrentPlayer->CurrentPlayerStats.Rank++;
		CurrentPlayer->CurrentPlayerStats.Experience -= nextExperience;

		//This will be the amount a player has leveled up.
		CurrentPlayer->CurrentPlayerStats.Upgrades++;

		//Pop up
		GuudJub->ItemType = PopUp_Rise;
		holdTime = 2;
	}

	GuudJub->Position.x = GetCameraXPosition();
	switch (GuudJub->ItemType)
	{
	case PopUp_Rise:
		if (GuudJub->Position.y < 0)
			GuudJub->Position.y += 360 * GetDeltaTime();
		else
		{
			GuudJub->ItemType = PopUp_Hold;
			GuudJub->Position.y = 0;
		}
		break;
	case PopUp_Hold:
		if (holdTime > 0)
			holdTime -= GetDeltaTime();
		else
			GuudJub->ItemType = PopUp_Fall;
		break;
	case PopUp_Fall:
		if (GuudJub->Position.y > -182)
			GuudJub->Position.y -= 360 * GetDeltaTime();
		break;
	}
}

/*************************************************************************/
/*!
	\brief
	Gets the player's next XP goal
*/
/*************************************************************************/
int GetMaxExperience(Player *CurrentPlayer)
{
	int currRank = CurrentPlayer->CurrentPlayerStats.Rank;
	int nextRank = currRank + 1;

	//Some arbitrary math, may be capricious for a little while
	return currRank * 60 + currRank * (nextRank * 20);
}

/*************************************************************************/
/*!
	\brief
	Runs all needed tasks when a level is successfully completed
*/
/*************************************************************************/
void LevelCompletion(void)
{
	WhiteOverlay->Position.x = GetCameraXPosition();
	BlackOverlay->Position.x = GetCameraXPosition();

	if (WhiteOverlay->Alpha > 1)
	{
		//Allow player to upgrade their player if upgrades are available
		if (UpgradeComplete)
		{
			//Continue onto the map
			if (BlackOverlay->Alpha > 1)
			{
				if(GetCurrentState() == GS_Level1)
					SetNextState(GS_Narr1);
				else
					SetNextState(GS_MapLevel);
			}
			else
				BlackOverlay->Alpha += GetDeltaTime();
		}
		else if (!UpgradeComplete)
			UpdateUpgradeScreenObjects();
	}
	else
		WhiteOverlay->Alpha += 2 * GetDeltaTime();
}
