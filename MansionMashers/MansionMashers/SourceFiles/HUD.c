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
	Loads assets for the showcase level
*/
/*************************************************************************/
HUD* CreateHUD(Player* CurrentPlayer)
{
	//Allocate memory for the HUD
	HUD *CurrentHUD = (HUD *)CallocMyAlloc(1, sizeof(HUD));

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

	//Create the HUD sprites for the items
	CurrentHUD->HUDItem[0] = (Sprite *)CreateSprite("TextureFiles/Taco.png", 50, 50, 401, 1, 1, GetCameraXPosition() - 548, 345);
	CurrentHUD->HUDItem[1] = (Sprite *)CreateSprite("TextureFiles/Ham.png", 50, 50, 401, 1, 1, GetCameraXPosition() - 548, 345);
	CurrentHUD->HUDItem[2] = (Sprite *)CreateSprite("TextureFiles/Pizza.png", 50, 50, 401, 1, 1, GetCameraXPosition() - 548, 345);
	CurrentHUD->HUDItem[3] = (Sprite *)CreateSprite("TextureFiles/Cake.png", 50, 50, 401, 1, 1, GetCameraXPosition() - 548, 345);

	if(!CurrentPlayer->BuffHeld[0])
		CurrentHUD->HUDItem[0]->Visible = FALSE;

	if(!CurrentPlayer->BuffHeld[1])
		CurrentHUD->HUDItem[1]->Visible = FALSE;

	if(!CurrentPlayer->BuffHeld[2])
		CurrentHUD->HUDItem[2]->Visible = FALSE;

	if(!CurrentPlayer->BuffHeld[3])
		CurrentHUD->HUDItem[3]->Visible = FALSE;

	return CurrentHUD;
}

void UpdateHUDPosition(HUD* CurrentHUD)
{
	CurrentHUD->HUDBackground->Position.x = (GetCameraXPosition() - 450);

	CurrentHUD->HUDItem[0]->Position.x = (GetCameraXPosition() - 548);
	CurrentHUD->HUDItem[1]->Position.x = (GetCameraXPosition() - 548);
	CurrentHUD->HUDItem[2]->Position.x = (GetCameraXPosition() - 548);
	CurrentHUD->HUDItem[3]->Position.x = (GetCameraXPosition() - 548);
}

void UpdateHUDItems(HUD* CurrentHUD, Player* CurrentPlayer)
{
	if(CurrentPlayer->BuffHeld[0])
		CurrentHUD->HUDItem[0]->Visible = TRUE;

	if(CurrentPlayer->BuffHeld[1])
		CurrentHUD->HUDItem[1]->Visible = TRUE;

	if(CurrentPlayer->BuffHeld[2])
		CurrentHUD->HUDItem[2]->Visible = TRUE;

	if(CurrentPlayer->BuffHeld[3])
		CurrentHUD->HUDItem[3]->Visible = TRUE;
}

void FreeHUD(HUD* CurrentHUD)
{
	FreeMyAlloc(CurrentHUD);
}
