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

enum PopUp { PopUp_Rise, PopUp_Hold, PopUp_Fall };

/*static Sprite *DeathConfirm;
static Button *RestartButton;
static Button *MainMapButton;*/

void CreateUpgradeScreenObjects(int *newID)
{
}

void UpdateUpgradeScreenObjects(void)
{
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
	HurtOverlay->ScaleX = 1.4f;
	HurtOverlay->ScaleY = 1.4f;
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

	if (HurtOverlay->ScaleX < 1.4f)
	{
		HurtOverlay->ScaleX += GetDeltaTime();
		HurtOverlay->ScaleY += GetDeltaTime();
	}

	//Displayer that player damage layer
	if (lastHealth > CurrentPlayer->CurrentPlayerStats.CurrentHealth && lastHealth > 0 && lastDefense == CurrentPlayer->PlayerWeapon->BonusDefense)
	{
		float overlayScalar = 1.0f + 0.18f * ((float)CurrentPlayer->CurrentPlayerStats.CurrentHealth / CurrentPlayer->CurrentPlayerStats.MaxHealth);
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
	if (CurrentPlayer->CurrentPlayerStats.Upgrades)
	{
		CurrentPlayer->CurrentPlayerStats.Upgrades = 0;
	}

	SavePlayer(CurrentPlayer);
}
