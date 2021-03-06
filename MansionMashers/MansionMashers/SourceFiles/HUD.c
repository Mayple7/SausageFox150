/*****************************************************************************/
/*!
\file				HUD.c
\author				Dan Muller (d.muller)
\author				Lucas Conlon (l.conlon)
\date				Feb 26, 2024

\brief				Functions for the heads up display

\par				Functions:
\li					CreateHUD
  
\par 
<b> Copyright (C) 2024 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
static int DebugHUD;

/*************************************************************************/
/*!
	\brief
	Creates the HUD for the level

	\param CurrentPlayer
	The current player to customize the HUD for

	\return
	Returns a pointer to the HUD created
*/
/*************************************************************************/
HUD* CreateHUD(Player* CurrentPlayer)
{
	//Allocate memory for the HUD
	HUD *CurrentHUD = (HUD *)CallocMyAlloc(1, sizeof(HUD));
	char CharTemp[32];
	Vec3 TextTint;

	//Only show FPS in debug mode
#ifndef _DEBUG
	DebugHUD = FALSE;
#else
	DebugHUD = TRUE;
#endif

	Vec3Set(&TextTint, 0.8705f, 0.5216f, 0.0196f);

	// Create the status text for specific levels
	if(GetCurrentState() == GS_Level2 || GetCurrentState() == GS_Level4)
	{
		int i, totalEnemies = 0;
		for(i = 0; i < PANELAMOUNT; ++i)
		{
			totalEnemies += EnemyPanelNumber[i];
		}

		sprintf(CharTemp, "Enemies Remaining: %d", totalEnemies);
		CurrentHUD->StatusText = CreateText(CharTemp, 0, 400, 100, TextTint, Center, Border);
		ChangeTextZIndex(CurrentHUD->StatusText, 400);
		TextAllVisible(CurrentHUD->StatusText);
	}
	else
	{
		sprintf(CharTemp, "Should never see me ^_^");
		CurrentHUD->StatusText = CreateText(CharTemp, 0, 400, 100, TextTint, Center, Border);
		ChangeTextZIndex(CurrentHUD->StatusText, 400);
		TextAllNotVisible(CurrentHUD->StatusText);
	}


	//Get the princess to show on the HUD
	switch(CurrentPlayer->Princess)
	{
	case Mayple:
		CurrentHUD->HUDBackground = (Sprite *)CreateSprite("TextureFiles/MaypleHUD.png", 448, 192, 400, 1, 1, GetCameraXPosition() - 450, 375);
		break;
	case Ginko:
		CurrentHUD->HUDBackground = (Sprite *)CreateSprite("TextureFiles/GinkoHUD.png", 448, 192, 400, 1, 1, GetCameraXPosition() - 450, 375); 
		break;
	case Kaya:
		CurrentHUD->HUDBackground = (Sprite *)CreateSprite("TextureFiles/KayaHUD.png", 448, 192, 400, 1, 1, GetCameraXPosition() - 450, 375);
		break;
	case Holly:
		CurrentHUD->HUDBackground = (Sprite *)CreateSprite("TextureFiles/HollyHUD.png", 448, 192, 400, 1, 1, GetCameraXPosition() - 450, 375);
		break;
	}

	//Text for coin amount
	Vec3Set(&TextTint, 1.0f, 1.0f, 0.0f);
	CurrentHUD->currentHUDCoinValue = CurrentPlayer->CurrentPlayerStats.Money;
	sprintf(CharTemp, "%i Coins", CurrentHUD->currentHUDCoinValue);
	CurrentHUD->CoinText = CreateText(CharTemp, 448, 192, 60, TextTint, Left, Border);
	ChangeTextZIndex(CurrentHUD->CoinText, 400);
	ChangeTextVisibility(CurrentHUD->CoinText);

	//Text for Rank Number
	Vec3Set(&TextTint, 0.62f, 0.11f, 0.67f);
	CurrentHUD->currentHUDRankValue = CurrentPlayer->CurrentPlayerStats.Rank;
	sprintf(CharTemp, "%i", CurrentHUD->currentHUDRankValue);
	CurrentHUD->RankText = CreateText(CharTemp, 448, 192, 56, TextTint, Left, Border);
	ChangeTextZIndex(CurrentHUD->RankText, 401);
	ChangeTextVisibility(CurrentHUD->RankText);

	//Text for FPS
	Vec3Set(&TextTint, 1.0f, 0.2f, 0.5f);
	sprintf(CharTemp, "%.2f FPS ", FRAMERATE * (1 / (FRAMERATE * GetDeltaTime())));
	CurrentHUD->FPSText = CreateText(CharTemp, 448, 192, 60, TextTint, Left, Border);
	ChangeTextZIndex(CurrentHUD->FPSText, 400);

	if (DebugHUD)
		ChangeTextVisibility(CurrentHUD->FPSText);

	//Create the HUD sprites for the buffs
	CurrentHUD->HUDItem[0] = (Sprite *)CreateSprite("TextureFiles/AgilityBuff.png", 70, 70, 401, 1, 1, GetCameraXPosition() - 548, 344);
	CurrentHUD->HUDItem[1] = (Sprite *)CreateSprite("TextureFiles/StrengthBuff.png", 70, 70, 401, 1, 1, GetCameraXPosition() - 548, 344);
	CurrentHUD->HUDItem[2] = (Sprite *)CreateSprite("TextureFiles/DefenseBuff.png", 70, 70, 401, 1, 1, GetCameraXPosition() - 548, 344);
	CurrentHUD->HUDItem[3] = (Sprite *)CreateSprite("TextureFiles/HealthBuff.png", 70, 70, 401, 1, 1, GetCameraXPosition() - 548, 344);

	// Create the HUD bars
	CurrentHUD->HealthBar = (Sprite *)CreateSprite("TextureFiles/HealthBarMid.png", 2, 22, 399, 1, 1, GetCameraXPosition() - 450, CurrentHUD->HUDBackground->Position.y + 28);
	CurrentHUD->HealthBarEnd = (Sprite *)CreateSprite("TextureFiles/HealthBarEnd.png", 5, 22, 399, 1, 1, GetCameraXPosition() - 450, CurrentHUD->HUDBackground->Position.y + 28);

	CurrentHUD->AbilityBar = (Sprite *)CreateSprite("TextureFiles/AbilityBarMid.png", 2, 22, 399, 1, 1, GetCameraXPosition() - 450, CurrentHUD->HUDBackground->Position.y - 1);
	CurrentHUD->AbilityBarEnd = (Sprite *)CreateSprite("TextureFiles/AbilityBarEnd.png", 5, 22, 399, 1, 1, GetCameraXPosition() - 450, CurrentHUD->HUDBackground->Position.y - 1);

	CurrentHUD->ExpBar = (Sprite *)CreateSprite("TextureFiles/ExperienceBarMid.png", 2, 19, 399, 1, 1, GetCameraXPosition() - 401, CurrentHUD->HUDBackground->Position.y - 28);
	CurrentHUD->ExpBarEnd = (Sprite *)CreateSprite("TextureFiles/ExperienceBarEnd.png", 4, 17, 399, 1, 1, GetCameraXPosition() - 401, CurrentHUD->HUDBackground->Position.y - 28);

	// Starting logic for the HUD buffs
	CurrentHUD->HUDItem[0]->Visible = FALSE;
	CurrentHUD->HUDItem[1]->Visible = FALSE;
	CurrentHUD->HUDItem[2]->Visible = FALSE;
	CurrentHUD->HUDItem[3]->Visible = FALSE;

	if(!CurrentPlayer->BuffSelected && CurrentPlayer->BuffHeld[0])
		CurrentHUD->HUDItem[0]->Visible = TRUE;
	else if(CurrentPlayer->BuffSelected == 1 && CurrentPlayer->BuffHeld[1])
		CurrentHUD->HUDItem[1]->Visible = TRUE;
	else if(CurrentPlayer->BuffSelected == 2 && CurrentPlayer->BuffHeld[2])
		CurrentHUD->HUDItem[2]->Visible = TRUE;
	else if(CurrentPlayer->BuffSelected == 3 && CurrentPlayer->BuffHeld[3])
		CurrentHUD->HUDItem[3]->Visible = TRUE;

	return CurrentHUD;
}

/*************************************************************************/
/*!
	\brief
	Updates the HUD position based on the camera

	\param CurrentHUD
	The pointer to the HUD to be updated
*/
/*************************************************************************/
void UpdateHUDPosition(HUD* CurrentHUD)
{
	Vec2 newPosition;
	char CharTemp[32];
	Vec2Set(&newPosition, (GetCameraXPosition() - 790), 300);

	//HUD items update position
	CurrentHUD->HUDBackground->Position.x = (GetCameraXPosition() - 735);

	CurrentHUD->HUDItem[0]->Position.x = (GetCameraXPosition() - 884);
	CurrentHUD->HUDItem[1]->Position.x = (GetCameraXPosition() - 884);
	CurrentHUD->HUDItem[2]->Position.x = (GetCameraXPosition() - 884);
	CurrentHUD->HUDItem[3]->Position.x = (GetCameraXPosition() - 884);

	// Maximum width of bar multiplied by the ratio of hp, CD, or exp
	CurrentHUD->HealthBar->ScaleX = 123.0f * (CurrentPlayer.CurrentPlayerStats.CurrentHealth / (float)CurrentPlayer.CurrentPlayerStats.MaxHealth);
	CurrentHUD->HealthBar->Position.x = (CurrentHUD->HUDBackground->Position.x - 54.0f + (CurrentHUD->HealthBar->ScaleX));
	CurrentHUD->HealthBarEnd->Position.x = (CurrentHUD->HUDBackground->Position.x - 52.0f + (CurrentHUD->HealthBar->ScaleX * 2));

	CurrentHUD->AbilityBar->ScaleX = 123.0f * (CurrentPlayer.WindAttackCooldown / (float)CurrentPlayer.WindAttackCooldownMax);
	CurrentHUD->AbilityBar->Position.x = (CurrentHUD->HUDBackground->Position.x - 54.0f + (CurrentHUD->AbilityBar->ScaleX));
	CurrentHUD->AbilityBarEnd->Position.x = (CurrentHUD->HUDBackground->Position.x - 52.0f + (CurrentHUD->AbilityBar->ScaleX * 2));

	CurrentHUD->ExpBar->ScaleX = 84.0f * (CurrentPlayer.CurrentPlayerStats.Experience / (float)GetMaxExperience(&CurrentPlayer));
	CurrentHUD->ExpBar->Position.x = (CurrentHUD->HUDBackground->Position.x - 5.0f + (CurrentHUD->ExpBar->ScaleX));
	CurrentHUD->ExpBarEnd->Position.x = (CurrentHUD->HUDBackground->Position.x - 3.0f + (CurrentHUD->ExpBar->ScaleX * 2));

	//Coin text update position
	ChangeTextPosition(CurrentHUD->CoinText, newPosition, Left);

	//See if coin amount has changed
	if (CurrentHUD->currentHUDCoinValue != CurrentPlayer.CurrentPlayerStats.Money)
	{
		//Make a new text
		CurrentHUD->currentHUDCoinValue = CurrentPlayer.CurrentPlayerStats.Money;
		sprintf(CharTemp, "%i Coins", CurrentHUD->currentHUDCoinValue);
		ChangeTextString(CurrentHUD->CoinText, CharTemp);
		ChangeTextZIndex(CurrentHUD->CoinText, 400);
	}

	// Status text update position
	Vec2Set(&newPosition, GetCameraXPosition(), CurrentHUD->StatusText->Glyph->Position.y);
	ChangeTextPosition(CurrentHUD->StatusText, newPosition, Center);

	//Rank text update position
	Vec2Set(&newPosition, GetCameraXPosition() - 758, 342);
	ChangeTextPosition(CurrentHUD->RankText, newPosition, Center);

	//Rank update
	if (CurrentHUD->currentHUDRankValue != CurrentPlayer.CurrentPlayerStats.Rank)
	{
		CurrentHUD->currentHUDRankValue = CurrentPlayer.CurrentPlayerStats.Rank;
		sprintf(CharTemp, "%i", CurrentHUD->currentHUDRankValue);
		ChangeTextString(CurrentHUD->RankText, CharTemp);
		ChangeTextZIndex(CurrentHUD->RankText, 401);
	}
	
	//Only show debug stuff on the HUD if debug is active
	if (!DebugHUD)
		return;

	//Framerate is probably always changing
	Vec2Set(&newPosition, (GetCameraXPosition() - 790), 300 - 48);
	ChangeTextPosition(CurrentHUD->FPSText, newPosition, Left);
	sprintf(CharTemp, "%.2f FPS ", FRAMERATE * (1 / (FRAMERATE * GetDeltaTime()))); //If Dt is ever 0 then... well...
	ChangeTextString(CurrentHUD->FPSText, CharTemp);
	ChangeTextZIndex(CurrentHUD->FPSText, 400);
}

/*************************************************************************/
/*!
	\brief
	Update the visible HUD item

	\param CurrentHUD
	HUD to be updated

	\param CurrentPlayer
	The player to check the HUD item with
*/
/*************************************************************************/
void UpdateHUDItems(HUD* CurrentHUD, Player* CurrentPlayer)
{
	CurrentHUD->HUDItem[0]->Visible = FALSE;
	CurrentHUD->HUDItem[1]->Visible = FALSE;
	CurrentHUD->HUDItem[2]->Visible = FALSE;
	CurrentHUD->HUDItem[3]->Visible = FALSE;

	if(!CurrentPlayer->BuffSelected && CurrentPlayer->BuffHeld[0])
		CurrentHUD->HUDItem[0]->Visible = TRUE;
	else if(CurrentPlayer->BuffSelected == 1 && CurrentPlayer->BuffHeld[1])
		CurrentHUD->HUDItem[1]->Visible = TRUE;
	else if(CurrentPlayer->BuffSelected == 2 && CurrentPlayer->BuffHeld[2])
		CurrentHUD->HUDItem[2]->Visible = TRUE;
	else if(CurrentPlayer->BuffSelected == 3 && CurrentPlayer->BuffHeld[3])
		CurrentHUD->HUDItem[3]->Visible = TRUE;
}

/*************************************************************************/
/*!
	\brief
	Memory angels hath cometh

	\param CurrentHUD
	The HUD to be freed
*/
/*************************************************************************/
void FreeHUD(HUD* CurrentHUD)
{
	FreeMyAlloc(CurrentHUD);
}
