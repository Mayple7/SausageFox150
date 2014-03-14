/*****************************************************************************/
/*!
\file				HUD.c
\author				Dan Muller (d.muller)
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
	char CoinChar[32];
	Vec3 TextTint;

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
	sprintf(CoinChar, "%i", CurrentHUD->currentHUDCoinValue);
	CurrentHUD->CoinText = CreateText(strcat(CoinChar, " Coins"), 448, 192, 60, TextTint, Center);
	ChangeTextZIndex(CurrentHUD->CoinText, 400);
	ChangeTextVisibility(CurrentHUD->CoinText);

	//Create the HUD sprites for the items
	CurrentHUD->HUDItem[0] = (Sprite *)CreateSprite("TextureFiles/Taco.png", 50, 50, 398, 1, 1, GetCameraXPosition() - 548, 345);
	CurrentHUD->HUDItem[1] = (Sprite *)CreateSprite("TextureFiles/Ham.png", 50, 50, 398, 1, 1, GetCameraXPosition() - 548, 345);
	CurrentHUD->HUDItem[2] = (Sprite *)CreateSprite("TextureFiles/Pizza.png", 50, 50, 398, 1, 1, GetCameraXPosition() - 548, 345);
	CurrentHUD->HUDItem[3] = (Sprite *)CreateSprite("TextureFiles/Cake.png", 50, 50, 398, 1, 1, GetCameraXPosition() - 548, 345);

	CurrentHUD->HUDItem[4] = (Sprite *)CreateSprite("TextureFiles/HealthBarMid.png", 2, 13, 399, 1, 1, GetCameraXPosition() - 450, CurrentHUD->HUDBackground->Position.y / GetLoadRatio() + 36);
	CurrentHUD->HUDItem[5] = (Sprite *)CreateSprite("TextureFiles/HealthBarEnd.png", 5, 11, 399, 1, 1, GetCameraXPosition() - 450, CurrentHUD->HUDBackground->Position.y / GetLoadRatio() + 36);

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
	Vec2Set(&newPosition, (GetCameraXPosition() - 685 * GetLoadRatio()), 300 * GetLoadRatio());

	//HUD items update position
	CurrentHUD->HUDBackground->Position.x = (GetCameraXPosition() - 735 * GetLoadRatio());

	CurrentHUD->HUDItem[0]->Position.x = (GetCameraXPosition() - 800 * GetLoadRatio());
	CurrentHUD->HUDItem[1]->Position.x = (GetCameraXPosition() - 800 * GetLoadRatio());
	CurrentHUD->HUDItem[2]->Position.x = (GetCameraXPosition() - 800 * GetLoadRatio());
	CurrentHUD->HUDItem[3]->Position.x = (GetCameraXPosition() - 800 * GetLoadRatio());

	CurrentHUD->HUDItem[4]->ScaleX = 71.0f * (CurrentPlayer.CurrentPlayerStats.CurrentHealth / (float)CurrentPlayer.CurrentPlayerStats.MaxHealth);
	CurrentHUD->HUDItem[4]->Position.x = (CurrentHUD->HUDBackground->Position.x - (2 * GetLoadRatio()) + (CurrentHUD->HUDItem[4]->ScaleX * GetLoadRatio()));
	CurrentHUD->HUDItem[5]->Position.x = (CurrentHUD->HUDBackground->Position.x + (CurrentHUD->HUDItem[4]->ScaleX * GetLoadRatio() * 2));

	//Coin text update position
	ChangeTextPosition(CurrentHUD->CoinText, newPosition, Center);

	//See if coin amount has changed
	if (CurrentHUD->currentHUDCoinValue != CurrentPlayer.CurrentPlayerStats.Money)
	{
		char CoinChar[32];

		//Make a new text
		CurrentHUD->currentHUDCoinValue = CurrentPlayer.CurrentPlayerStats.Money;
		sprintf(CoinChar, "%i", CurrentHUD->currentHUDCoinValue);
		ChangeTextString(CurrentHUD->CoinText, strcat(CoinChar, " Coins"));
		ChangeTextZIndex(CurrentHUD->CoinText, 400);
	}
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
