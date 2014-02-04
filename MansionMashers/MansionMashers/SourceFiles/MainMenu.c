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
	StartButton = CreateSprite("TextureFiles/StartButton.png", 480.0f, 180.0f, 1, 1, 1);
	StartButton->Position.x = 0.0f;
	StartButton->Position.y = 200.0f;

	// Creates the showcase button
	ShowcaseButton = CreateSprite("TextureFiles/ShowcaseButton.png", 640.0f, 180.0f, 1, 1, 1);
	ShowcaseButton->Position.x = 0.0f;
	ShowcaseButton->Position.y = 0.0f;

	// Creates the exit button
	ExitButton = CreateSprite("TextureFiles/ExitButton.png", 480.0f, 180.0f, 1, 1, 1);
	ExitButton->Position.x = 0.0f;
	ExitButton->Position.y = -200.0f;

	// Creates the selector button - set to default position of the start button
	Selector = CreateSprite("TextureFiles/Selector.png", 500.0f, 200.0f, 0, 1, 1);
	Selector->Position.x = 100.0f;
	Selector->Position.y = 0.0f;
	
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
	if (AEInputCheckTriggered(VK_UP) || AEInputCheckTriggered('W'))
	{
		if(selectedButton == 0)
			selectedButton = numMenuButtons - 1;
		else
			selectedButton--;

		UpdateSelector(Selector);
	}
	else if(AEInputCheckTriggered(VK_DOWN) || AEInputCheckTriggered('S'))
	{
		if(selectedButton == numMenuButtons - 1)
			selectedButton = 0;
		else
			selectedButton++;

		UpdateSelector(Selector);
	}
	else if(AEInputCheckTriggered(VK_RETURN) || AEInputCheckTriggered(VK_SPACE))
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
	if(AEInputCheckTriggered(VK_RSHIFT))
	{
		SetNextState(GS_Tutorial);
	}

	// check if forcing the application to quit
	if (AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
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
			Selector->Width = 500;
			UpdateMesh(Selector);
			break;
		case 1:
			Selector->Position = ShowcaseButton->Position;
			Selector->Width = 660;
			UpdateMesh(Selector);
			break;
		case 2:
			Selector->Position = ExitButton->Position;
			Selector->Width = 500;
			UpdateMesh(Selector);
			break;
		default:
			Selector->Position = StartButton->Position;
			Selector->Width = 500;
			UpdateMesh(Selector);
			break;
	}
}
