/*****************************************************************************/
/*!
\file				EPMenu.c
\author				Dan Muller (d.muller
\date				Jan 8, 2014

\brief				Functions for fading in and out all the splash screen slides

\par				Functions:
\li					LoadEP2Screen
\li					InitializeEP2Screen
\li					UpdateEP2Screen
\li					DrawEP2Screen
\li					FreeEP2Screen
\li					UnloadEP2Screen
\li					fadeLogic
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/EPMenu.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"

Sprite* GameLogo;

Button* EP1Button;
Button* EP2Button;
Button* TutorialButton;
Button* QuitButton;

Sprite* Selector;

//Delete save file objects
Sprite* BlackBackground;
Sprite* DeleteText;
Button* YesButton;
Button* NoButton;

enum Buttons { EP1But, EP2But, TutBut, QuitBut};
int selectedEPButton;								//0: EP1, 1: EP2, 2: tutorial, 3: quit
static int deleteSave; // If true, dialogue box to delete save is up

/*************************************************************************/
/*!
	\brief
	Loads the assets needed for the EP menu
*/
/*************************************************************************/
void LoadEPMenu(void)
{
	//Allocate space for a large texture
	CreateTextureList();
}

void InitializeEPMenu(void)
{
	Vec3 Tint;
	// Reset the object list
	ResetObjectList();

	selectedEPButton = EP1But;

	GameLogo = (Sprite *) CreateSprite("TextureFiles/MansionMashersLogo.png", 1920.0f, 1080.0f, 1, 1, 1, 0, 0);

	// Create the start button
	EP1Button = CreateButton("TextureFiles/EP1_button.png", -500, 300, 300.0f, 100.0f);

	// Create the start button
	EP2Button = CreateButton("TextureFiles/EP2_button.png",  500, 300, 300.0f, 100.0f);

	// Creates the showcase button
	TutorialButton = CreateButton("TextureFiles/tutorial_button.png", -500, -300, 300.0f, 100.0f);
	
	// Creates the exit button
	QuitButton = CreateButton("TextureFiles/quit_button.png", 500, -300, 300.0f, 100.0f);

	// Creates the selector button - set to default position of the start button
	Selector = (Sprite *) CreateSprite("TextureFiles/Selector.png", EP1Button->ButtonSprite->Width * 1.1f, EP1Button->ButtonSprite->Height * 1.2f, 2, 1, 1, 100, 0);
	
	Vec3Set(&Tint, 0, 0, 0);
	BlackBackground = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 499, 1, 1, 0, 0);
	BlackBackground->Tint = Tint;
	BlackBackground->Alpha = 0.5f;
	DeleteText = (Sprite *) CreateSprite("TextureFiles/DeleteText.png", 651, 334, 500, 1, 1, 0, 100);
	YesButton = CreateButton("TextureFiles/DeleteButton.png", -300, -200, 400, 150);
	YesButton->ButtonSprite->ZIndex = 500;
	NoButton = CreateButton("TextureFiles/BackButton.png", 300, -200, 400, 150);
	NoButton->ButtonSprite->ZIndex = 500;

	BlackBackground->Visible = FALSE;
	DeleteText->Visible = FALSE;
	YesButton->ButtonSprite->Visible = FALSE;
	NoButton->ButtonSprite->Visible = FALSE;

	// Set camera to (0,0)
	ResetCamera();
	UpdateEPSelector(Selector);
}

void UpdateEPMenu(void)
{
	EPMenuInput();
}

void DrawEPMenu(void)
{
	DrawObjectList();
}

void FreeEPMenu(void)
{
	// Freeing the objects and textures
	FreeAllLists();
}

void UnloadEPMenu(void)
{
	//Destroy the textures
	DestroyTextureList();
}

void EPMenuInput(void)
{
	//Get mouse info
	int worldX, worldY;
	Vec2 MouseClick;

	FoxInput_GetWorldPosition(&worldX, &worldY);
	Vec2Set(&MouseClick, (float)worldX, (float)worldY);

	// check if forcing the application to quit
	switch(selectedEPButton)
	{
	// EP 1 button
	case EP1But:
		if(FoxInput_KeyTriggered(VK_DOWN) || FoxInput_KeyTriggered('S'))
		{
			selectedEPButton = TutBut;
			UpdateEPSelector(Selector);
		}
		else if(FoxInput_KeyTriggered(VK_RIGHT) || FoxInput_KeyTriggered('D'))
		{
			selectedEPButton = EP2But;
			UpdateEPSelector(Selector);
		}
		else if(FoxInput_KeyTriggered(VK_RETURN) || FoxInput_KeyTriggered(VK_SPACE))
		{
			SetNextState(GS_EP1Slides);
		}
		break;
	case EP2But:
		if(FoxInput_KeyTriggered(VK_DOWN) || FoxInput_KeyTriggered('S'))
		{
			selectedEPButton = QuitBut;
			UpdateEPSelector(Selector);
		}
		else if(FoxInput_KeyTriggered(VK_LEFT) || FoxInput_KeyTriggered('A'))
		{
			selectedEPButton = EP1But;
			UpdateEPSelector(Selector);
		}
		else if(FoxInput_KeyTriggered(VK_RETURN) || FoxInput_KeyTriggered(VK_SPACE))
		{
			SetNextState(GS_EP2Slides);
		}
		break;
	case TutBut:
		if(FoxInput_KeyTriggered(VK_UP) || FoxInput_KeyTriggered('W'))
		{
			selectedEPButton = EP1But;
			UpdateEPSelector(Selector);
		}
		else if(FoxInput_KeyTriggered(VK_RIGHT) || FoxInput_KeyTriggered('D'))
		{
			selectedEPButton = QuitBut;
			UpdateEPSelector(Selector);
		}
		else if(FoxInput_KeyTriggered(VK_RETURN) || FoxInput_KeyTriggered(VK_SPACE))
		{
			SetNextState(GS_MainMenu);
		}
		break;
	case QuitBut:
		if(FoxInput_KeyTriggered(VK_UP) || FoxInput_KeyTriggered('W'))
		{
			selectedEPButton = EP2But;
			UpdateEPSelector(Selector);
		}
		else if(FoxInput_KeyTriggered(VK_LEFT) || FoxInput_KeyTriggered('A'))
		{
			selectedEPButton = TutBut;
			UpdateEPSelector(Selector);
		}
		else if(FoxInput_KeyTriggered(VK_RETURN) || FoxInput_KeyTriggered(VK_SPACE))
		{
			SetNextState(GS_MainMenu);
		}
		break;
	}

	// Mouse functionality
	// EP1
	if(PointRectCollision(&EP1Button->ButtonCollider, &MouseClick))
	{
		selectedEPButton = EP1But;
		UpdateEPSelector(Selector);
		
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			SetNextState(GS_EP1Slides);
		}
	}
	// EP2
	else if(PointRectCollision(&EP2Button->ButtonCollider, &MouseClick))
	{
		selectedEPButton = EP2But;
		UpdateEPSelector(Selector);
		
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			SetNextState(GS_EP2Slides);
		}
	}
	// Tutorial
	else if(PointRectCollision(&TutorialButton->ButtonCollider, &MouseClick))
	{
		selectedEPButton = TutBut;
		UpdateEPSelector(Selector);
		
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			SetNextState(GS_MapLevel);
		}
	}
	// Quit
	else if(PointRectCollision(&QuitButton->ButtonCollider, &MouseClick))
	{
		selectedEPButton = QuitBut;
		UpdateEPSelector(Selector);
		
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			SetNextState(GS_MainMenu);
		}
	}

	// check if forcing the application to quit
	if (FoxInput_KeyTriggered(VK_ESCAPE))
		SetNextState(GS_MainMenu);
}


/*************************************************************************/
/*!
	\brief
	Updates the selector sprite to move to the correct position
	
	\param Selector
	Selector sprite to update
*/
/*************************************************************************/
void UpdateEPSelector(struct Sprite *Selector)
{
	switch(selectedEPButton)
	{
		case EP1But:
			Selector->Position = EP1Button->ButtonSprite->Position;
			UpdateMesh(Selector);
			break;
		case EP2But:
			Selector->Position = EP2Button->ButtonSprite->Position;
			UpdateMesh(Selector);
			break;
		case TutBut:
			Selector->Position = TutorialButton->ButtonSprite->Position;
			UpdateMesh(Selector);
			break;
		case QuitBut:
			Selector->Position = QuitButton->ButtonSprite->Position;
			UpdateMesh(Selector);
			break;
		default:
			Selector->Position = EP1Button->ButtonSprite->Position;
			UpdateMesh(Selector);
			break;
	}
}