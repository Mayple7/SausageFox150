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

static Sprite *GuudJub;
static float holdTime;

enum PopUp { PopUp_Rise, PopUp_Hold, PopUp_Fall };

/*static Sprite *DeathConfirm;
static Button *RestartButton;
static Button *MainMapButton;*/

void CreateUpgradeScreenObjects(int *newID)
{
	/*Vec3 TextTint;

	Vec3Set(&TextTint, 0, 0, 0);
	DeathConfirm = (Sprite *) CreateSprite("TextureFiles/DeathConfirm.png", 639, 204, 4020, 1, 1, -1920, 100);

	MainMapButton = CreateButton("TextureFiles/GoToMapButton.png", -1920, -130, 300, 112.5f, (*newID)++);
	MainMapButton->ButtonSprite->ZIndex = 4021;

	RestartButton = CreateButton("TextureFiles/RestartButton.png", -1920, -130, 300, 112.5f, (*newID)++);
	RestartButton->ButtonSprite->ZIndex = 4022;*/
}

void UpdateUpgradeScreenObjects(void)
{
	/*int worldX, worldY;
	Vec2 MouseClick;

	CurrentPlayer.Position.x = -1920;
	UpdateCollisionPosition(&CurrentPlayer.PlayerCollider, &CurrentPlayer.Position);

	FoxInput_GetWorldPosition(&worldX, &worldY);
	Vec2Set(&MouseClick, (float)worldX, (float)worldY);

	UpdateButtonPosition(RestartButton, (-250) + GetCameraXPosition(), -130);
	UpdateCollisionPosition(&RestartButton->ButtonCollider, &RestartButton->ButtonSprite->Position);

	UpdateButtonPosition(MainMapButton, (250) + GetCameraXPosition(), -130);
	UpdateCollisionPosition(&MainMapButton->ButtonCollider, &MainMapButton->ButtonSprite->Position);

	DeathConfirm->Position.x = GetCameraXPosition();

	if(PointRectCollision(&RestartButton->ButtonCollider, &MouseClick))
	{
		RestartButton->ButtonSprite->ScaleX = 1.2f;
		RestartButton->ButtonSprite->ScaleY = 1.2f;

		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			FreeButton(RestartButton);
			FreeButton(MainMapButton);
			FreeSprite(DeathConfirm);
			if(GetCurrentState() == GS_Level31)
				SetNextState(GS_Level3);
			else
				SetNextState(GS_Restart);
		}
	}
	else
	{
		RestartButton->ButtonSprite->ScaleX = 1.0f;
		RestartButton->ButtonSprite->ScaleY = 1.0f;
	}

	if(PointRectCollision(&MainMapButton->ButtonCollider, &MouseClick))
	{
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			FreeButton(RestartButton);
			FreeButton(MainMapButton);
			FreeSprite(DeathConfirm);
			SetNextState(GS_MapLevel);
		}
		MainMapButton->ButtonSprite->ScaleX = 1.2f;
		MainMapButton->ButtonSprite->ScaleY = 1.2f;
	}
	else
	{
		MainMapButton->ButtonSprite->ScaleX = 1.0f;
		MainMapButton->ButtonSprite->ScaleY = 1.0f;
	}*/
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
