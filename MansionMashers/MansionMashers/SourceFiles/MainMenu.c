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

Sprite* StartButton;
Sprite* ExitButton;
Sprite* Selector;
Sprite* ShowcaseButton;
Sprite* Logo;

Sprite* HUD2;
Sprite* HUD;

HUDLayer HUDList;

AEGfxTexture *StartButtonTexture;
AEGfxTexture *ShowcaseTexture;
AEGfxTexture *ExitButtonTexture;

int numMenuButtons = 3;
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
	Logo = CreateSprite("TextureFiles/MansionMashersLogo.png", 1920.0f, 1080.0f, 1, 1, 1, 0, 300);

	// Create the start button
	StartButton = CreateSprite("TextureFiles/StartButton.png", 480.0f, 180.0f, 3, 1, 1, 0, 0);

	// Creates the showcase button
	ShowcaseButton = CreateSprite("TextureFiles/ShowcaseButton.png", 640.0f, 180.0f, 3, 1, 1, 0, -200);
	
	// Creates the exit button
	ExitButton = CreateSprite("TextureFiles/ExitButton.png", 480.0f, 180.0f, 3, 1, 1, 0, -400);

	// Creates the selector button - set to default position of the start button
	Selector = CreateSprite("TextureFiles/Selector.png", 500.0f, 200.0f, 2, 1, 1, 100, 0);
	
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
	else if(FoxInput_KeyTriggered(VK_RETURN) || FoxInput_KeyTriggered(VK_SPACE))
	{
		// Play le gaem
		if(selectedButton == 0)
			SetNextState(GS_TestLevel);
		// Showcase button
		else if(selectedButton == 1)
			SetNextState(GS_ShowcaseLevel);
		// Exit button
		else if(selectedButton == 2)
			SetNextState(GS_Quit);
	}
	if(FoxInput_KeyTriggered(VK_SHIFT))
	{
		SetNextState(GS_Tutorial);
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
			Selector->Position = StartButton->Position;
			Selector->Width = 500 * GetLoadRatio();
			UpdateMesh(Selector);
			break;
		case 1:
			Selector->Position = ShowcaseButton->Position;
			Selector->Width = 660 * GetLoadRatio();
			UpdateMesh(Selector);
			break;
		case 2:
			Selector->Position = ExitButton->Position;
			Selector->Width = 500 * GetLoadRatio();
			UpdateMesh(Selector);
			break;
		default:
			Selector->Position = StartButton->Position;
			Selector->Width = 500 * GetLoadRatio();
			UpdateMesh(Selector);
			break;
	}
}
