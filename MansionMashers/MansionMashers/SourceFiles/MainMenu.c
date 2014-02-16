/*****************************************************************************/
/*!
\file				MainMenu.c
\author				Dan Muller (d.muller)
\date				Jan 8, 2014

\brief				Functions for the main menu

\par				Functions:
\li					LoadMainMenu
\li					InitializeMainMenu
\li					UpdateMainMenu
\li					DrawMainMenu
\li					FreeMainMenu
\li					UnloadMainMenu
\li					InputHandling
\li					UpdateSelector
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/MainMenu.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals

Sprite* Level1;
Sprite* Level2;
Sprite* Level3;
Sprite* Level4;

Sprite* ArmGuy;
Sprite* HandGuy;
Sprite* YeahGuy;
Sprite* Kevin;

Sprite* Selector;
Sprite* Logo;

Sprite* HUD2;
Sprite* HUD;

HUDLayer HUDList;

AEGfxTexture *StartButtonTexture;
AEGfxTexture *ShowcaseTexture;
AEGfxTexture *ExitButtonTexture;

int numMenuButtons = 8;
int selectedButton = 0;								//0: start, 1: showcase 2: exit

// ---------------------------------------------------------------------------
// Static function protoypes

/*************************************************************************/
/*!
	\brief
	Loads the assets needed for the main menu
*/
/*************************************************************************/
void LoadMainMenu(void)
{
	//Allocate space for a large texture
	CreateTextureList();
}

/*************************************************************************/
/*!
	\brief
	Initializes all the objects needed for the main menu
*/
/*************************************************************************/
void InitializeMainMenu(void)
{
	// Reset the object list
	resetObjectList();

	// Create the start button
	Logo = CreateSprite("TextureFiles/MansionMashersLogo.png", 1920.0f, 1080.0f, 1, 1, 1, 0, 0);

	// Create the start button
	Level1 = CreateSprite("TextureFiles/Level1.png", 400.0f, 100.0f, 3, 1, 1, -300, 300);
	Level2 = CreateSprite("TextureFiles/Level2.png", 400.0f, 100.0f, 3, 1, 1, -300, 100);
	Level3 = CreateSprite("TextureFiles/Level3.png", 400.0f, 100.0f, 3, 1, 1, -300, -100);
	Level4 = CreateSprite("TextureFiles/Level4.png", 400.0f, 100.0f, 3, 1, 1, -300, -300);

	ArmGuy = CreateSprite("TextureFiles/ArmGuy.png", 400.0f, 100.0f, 3, 1, 1, 300, 300);
	HandGuy = CreateSprite("TextureFiles/HandGuy.png", 400.0f, 100.0f, 3, 1, 1, 300, 100);
	YeahGuy = CreateSprite("TextureFiles/YeahGuy.png", 400.0f, 100.0f, 3, 1, 1, 300, -100);
	Kevin = CreateSprite("TextureFiles/Kevin.png", 400.0f, 100.0f, 3, 1, 1, 300, -300);

	// Creates the selector button - set to default position of the start button
	Selector = CreateSprite("TextureFiles/Selector.png", 420.0f, 120.0f, 2, 1, 1, 100, 0);
	
	// Set camera to (0,0)
	ResetCamera();
	UpdateSelector(Selector);
}

/*************************************************************************/
/*!
	\brief
	The code to update the main menu
*/
/*************************************************************************/
void UpdateMainMenu(void)
{
	InputHandling();
}

/*************************************************************************/
/*!
	\brief
	Draws all the objects for the main menu
*/
/*************************************************************************/
void DrawMainMenu(void)
{
	DrawObjectList();
}

/*************************************************************************/
/*!
	\brief
	Frees all the allocated memory in the main menu
*/
/*************************************************************************/
void FreeMainMenu(void)
{
	// Freeing the objects and textures
	freeObjectList();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets used in the main menu
*/
/*************************************************************************/
void UnloadMainMenu(void)
{
	//Destroy the textures
	DestroyTextureList();
}

/*************************************************************************/
/*!
	\brief
	Handles the input for the main menu
*/
/*************************************************************************/
void InputHandling(void)
{
	// check if forcing the application to quit
	if (FoxInput_KeyTriggered(VK_UP) || FoxInput_KeyTriggered('W'))
	{
		if(selectedButton == 0)
			selectedButton = numMenuButtons - 1;
		else
			selectedButton--;

		UpdateSelector(Selector);
	}
	else if(FoxInput_KeyTriggered(VK_DOWN) || FoxInput_KeyTriggered('S'))
	{
		if(selectedButton == numMenuButtons - 1)
			selectedButton = 0;
		else
			selectedButton++;

		UpdateSelector(Selector);
	}
	else if(FoxInput_KeyTriggered(VK_LEFT) || FoxInput_KeyTriggered('A'))
	{
		if(selectedButton > 3)
		{
			selectedButton -= 4;
			UpdateSelector(Selector);
		}
	}
	else if(FoxInput_KeyTriggered(VK_RIGHT) || FoxInput_KeyTriggered('D'))
	{
		if(selectedButton < 4)
		{
			selectedButton += 4;
			UpdateSelector(Selector);
		}
	}
	else if(FoxInput_KeyTriggered(VK_RETURN) || FoxInput_KeyTriggered(VK_SPACE))
	{
		if(selectedButton == 0)
			SetNextState(GS_Level1);
		else if(selectedButton == 1)
			SetNextState(GS_Level2);
		else if(selectedButton == 2)
			SetNextState(GS_Level3);
		else if(selectedButton == 3)
			SetNextState(GS_Level4);
		else if(selectedButton == 4)
			SetNextState(GS_ArmGuy);
		else if(selectedButton == 5)
			SetNextState(GS_HandGuy);
		else if(selectedButton == 6)
			SetNextState(GS_YeahGuy);
		else if(selectedButton == 7)
			SetNextState(GS_Kevin);
	}

	// check if forcing the application to quit
	if (FoxInput_KeyTriggered(VK_ESCAPE))
		SetNextState(GS_Quit);
}

/*************************************************************************/
/*!
	\brief
	Updates the selector sprite to move to the correct position
	
	\param Selector
	Selector sprite to update
*/
/*************************************************************************/
void UpdateSelector(struct Sprite *Selector)
{
	switch(selectedButton)
	{
		case 0:
			Selector->Position = Level1->Position;
			UpdateMesh(Selector);
			break;
		case 1:
			Selector->Position = Level2->Position;
			UpdateMesh(Selector);
			break;
		case 2:
			Selector->Position = Level3->Position;
			UpdateMesh(Selector);
			break;
		case 3:
			Selector->Position = Level4->Position;
			UpdateMesh(Selector);
			break;
		case 4:
			Selector->Position = ArmGuy->Position;
			UpdateMesh(Selector);
			break;
		case 5:
			Selector->Position = HandGuy->Position;
			UpdateMesh(Selector);
			break;
		case 6:
			Selector->Position = YeahGuy->Position;
			UpdateMesh(Selector);
			break;
		case 7:
			Selector->Position = Kevin->Position;
			UpdateMesh(Selector);
			break;
	}
}
