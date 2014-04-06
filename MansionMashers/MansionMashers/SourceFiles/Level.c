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

static Sprite *HurtOverlay;
static float lastHealth;
static int lastDefense;
static Sprite *GuudJub;
static float holdTime;

static int UpgradesToUpgrade; //If the player even has points to spend
static int lastUpgrades;      //As to not update the text every frame

enum PopUp { PopUp_Rise, PopUp_Hold, PopUp_Fall };

static TextGlyphs *UpgradesName;
static TextGlyphs *UpgradesLeft;
static Button *MainMapButton;

static Button *UpgradeButton1;
static Button *UpgradeButton2;
static Button *UpgradeButton3;

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
	Vec3 TextColor;

	lastUpgrades = -1;
	UpgradeComplete = FALSE;
	UpgradesToUpgrade = FALSE;

	//Title
	Vec3Set(&TextColor, 1, 1, 1);
	UpgradesName = CreateText("Upgrades Available!", 0, -2000, 140, TextColor, Center, Border);
	ChangeTextVisibility(UpgradesName);
	ChangeTextZIndex(UpgradesName, 4002);

	Vec3Set(&TextColor, 0.6f, 0.0f, 0.6f);
	UpgradesLeft = CreateText("Sweg", 0, -2000, 100, TextColor, Center, Border);
	ChangeTextVisibility(UpgradesLeft);
	ChangeTextZIndex(UpgradesLeft, 4002);

	//Upgrade buttons, all 3
	UpgradeButton1 = CreateButton("TextureFiles/GinkoHead.png", 400 + camX, -2000, 200, 200); //Strength
	UpgradeButton1->ButtonSprite->ZIndex = 4002;
	UpgradeButton2 = CreateButton("TextureFiles/KayaHead.png", 400 + camX, -2000, 200, 200); //Defense
	UpgradeButton2->ButtonSprite->ZIndex = 4002;
	UpgradeButton3 = CreateButton("TextureFiles/HollyHead.png", 400 + camX, -2000, 200, 200); //Agility
	UpgradeButton3->ButtonSprite->ZIndex = 4002;

	//Done upgrading button
	MainMapButton = CreateButton("TextureFiles/GoToMapButton.png", 400 + camX, -2000, 300, 112.5f);
	MainMapButton->ButtonSprite->ZIndex = 4002;
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
	Vec2Set(&NewPosition, camX, 480);
	ChangeTextPosition(UpgradesName, NewPosition, Center);

	Vec2Set(&NewPosition, camX, 400);
	ChangeTextPosition(UpgradesLeft, NewPosition, Center);
	if (lastUpgrades != CurrentPlayer.CurrentPlayerStats.Upgrades)
	{
		if (CurrentPlayer.CurrentPlayerStats.Upgrades == 1)
			sprintf(CharTemp, "%i Point Remaining!", CurrentPlayer.CurrentPlayerStats.Upgrades);
		else if (CurrentPlayer.CurrentPlayerStats.Upgrades < 1)
			sprintf(CharTemp, "No Points! Level Up More!");
		else
			sprintf(CharTemp, "%i Points to Spend!", CurrentPlayer.CurrentPlayerStats.Upgrades);
		ChangeTextString(UpgradesLeft, CharTemp);
		ChangeTextZIndex(UpgradesLeft, 4002);
		lastUpgrades = CurrentPlayer.CurrentPlayerStats.Upgrades;
	}

	//Upgrade buttons, all 3
	UpdateButtonPosition(UpgradeButton1, 300 + camX, 200);
	UpdateCollisionPosition(&UpgradeButton1->ButtonCollider, &UpgradeButton1->ButtonSprite->Position);
	UpdateButtonPosition(UpgradeButton2, 300 + camX, 0);
	UpdateCollisionPosition(&UpgradeButton2->ButtonCollider, &UpgradeButton2->ButtonSprite->Position);
	UpdateButtonPosition(UpgradeButton3, 300 + camX, -200);
	UpdateCollisionPosition(&UpgradeButton3->ButtonCollider, &UpgradeButton3->ButtonSprite->Position);

	//Done upgrading button
	UpdateButtonPosition(MainMapButton, 400 + camX, -400);
	UpdateCollisionPosition(&MainMapButton->ButtonCollider, &MainMapButton->ButtonSprite->Position);

	//STRENGTH
	if (CurrentPlayer.CurrentPlayerStats.Upgrades > 0 && PointRectCollision(&UpgradeButton1->ButtonCollider, &MouseClick))
	{
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			CurrentPlayer.CurrentPlayerStats.Upgrades--;
			CurrentPlayer.CurrentPlayerStats.Strength++;
		}
		UpgradeButton1->ButtonSprite->ScaleX = 1.2f;
		UpgradeButton1->ButtonSprite->ScaleY = 1.2f;
	}
	else
	{
		UpgradeButton1->ButtonSprite->ScaleX = 1.0f;
		UpgradeButton1->ButtonSprite->ScaleY = 1.0f;
	}
	//DEFENSE
	if (CurrentPlayer.CurrentPlayerStats.Upgrades > 0 && PointRectCollision(&UpgradeButton2->ButtonCollider, &MouseClick))
	{
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			CurrentPlayer.CurrentPlayerStats.Upgrades--;
			CurrentPlayer.CurrentPlayerStats.Defense++;
		}
		UpgradeButton2->ButtonSprite->ScaleX = 1.2f;
		UpgradeButton2->ButtonSprite->ScaleY = 1.2f;
	}
	else
	{
		UpgradeButton2->ButtonSprite->ScaleX = 1.0f;
		UpgradeButton2->ButtonSprite->ScaleY = 1.0f;
	}
	//AGILITY
	if (CurrentPlayer.CurrentPlayerStats.Upgrades > 0 && PointRectCollision(&UpgradeButton3->ButtonCollider, &MouseClick))
	{
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			CurrentPlayer.CurrentPlayerStats.Upgrades--;
			CurrentPlayer.CurrentPlayerStats.Agility++;
		}
		UpgradeButton3->ButtonSprite->ScaleX = 1.2f;
		UpgradeButton3->ButtonSprite->ScaleY = 1.2f;
	}
	else
	{
		UpgradeButton3->ButtonSprite->ScaleX = 1.0f;
		UpgradeButton3->ButtonSprite->ScaleY = 1.0f;
	}

	//MAIN MENU
	if(PointRectCollision(&MainMapButton->ButtonCollider, &MouseClick))
	{
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			UpgradeComplete = TRUE;
		}
		MainMapButton->ButtonSprite->ScaleX = 1.2f;
		MainMapButton->ButtonSprite->ScaleY = 1.2f;
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
	HurtOverlay = (Sprite *) CreateSprite("TextureFiles/HurtOverlay.png", 1920, 1080, 1400, 1, 1, 0, 0);
	HurtOverlay->ScaleX = 5.0f;
	HurtOverlay->ScaleY = 5.0f;
}

/*************************************************************************/
/*!
	\brief
	Shows a hurt overlay if the player is damaged
*/
/*************************************************************************/
void UpdatePlayerHurt(Player *CurrentPlayer)
{
	HurtOverlay->Position.x = GetCameraXPosition();

	if (HurtOverlay->ScaleX < 5.0f)
	{
		HurtOverlay->ScaleX += 2 * GetDeltaTime();
		HurtOverlay->ScaleY += 2 * GetDeltaTime();
	}

	//Displayer that player damage layer
	if (lastHealth > CurrentPlayer->CurrentPlayerStats.CurrentHealth && lastHealth > 0 && lastDefense == CurrentPlayer->PlayerWeapon->BonusDefense)
	{
		float overlayScalar = 1.0f + 1.5f * ((float)CurrentPlayer->CurrentPlayerStats.CurrentHealth / CurrentPlayer->CurrentPlayerStats.MaxHealth);
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
void LevelCompletion(Player *CurrentPlayer)
{
	SavePlayer(CurrentPlayer);
}
