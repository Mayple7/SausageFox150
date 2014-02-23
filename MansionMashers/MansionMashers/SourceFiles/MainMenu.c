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

Sprite* GameLogo;

Sprite* EP1Button;
Sprite* EP2Button;
Sprite* TutorialButton;
Sprite* QuitButton;

Sprite* Selector;

//Delete save file objects
Sprite* BlackBackground;
Sprite* DeleteText;
Button* YesButton;
Button* NoButton;

enum Buttons { EP1But, EP2But, TutBut, QuitBut};
static int selectedButton;								//0: EP1, 1: EP2, 2: tutorial, 3: quit
static int deleteSave; // If true, dialogue box to delete save is up
static int newID;

/*************************************************************************/
/*!
	\brief
	Loads the assets needed for the EP menu
*/
/*************************************************************************/
void LoadMainMenu(void)
{
	//Allocate space for a large texture
	CreateTextureList();
}

void InitializeMainMenu(void)
{
	Vec3 Tint;
	newID = 10;
	// Reset the object list
	ResetObjectList();

	selectedButton = EP1But;

	GameLogo = (Sprite *) CreateSprite("TextureFiles/MansionMashersLogo.png", 1920.0f, 1080.0f, 1, 1, 1, 0, 0);

	// Create the start button
	EP1Button = (Sprite *) CreateSprite("TextureFiles/EP1_button.png", 300.0f, 100.0f, 3, 1, 1, -500, 300);

	// Create the start button
	EP2Button = (Sprite *) CreateSprite("TextureFiles/EP2_button.png", 300.0f, 100.0f, 3, 1, 1, 500, 300);

	// Creates the showcase button
	TutorialButton = (Sprite *) CreateSprite("TextureFiles/tutorial_button.png", 300.0f, 100.0f, 3, 1, 1, -500, -300);
	
	// Creates the exit button
	QuitButton = (Sprite *) CreateSprite("TextureFiles/quit_button.png", 300.0f, 100.0f, 3, 1, 1, 500, -300);

	// Creates the selector button - set to default position of the start button
	Selector = (Sprite *) CreateSprite("TextureFiles/Selector.png", 500.0f, 200.0f, 2, 1, 1, 100, 0);
	
	Vec3Set(&Tint, 0, 0, 0);
	BlackBackground = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 499, 1, 1, 0, 0);
	BlackBackground->Tint = Tint;
	BlackBackground->Alpha = 0.5f;
	DeleteText = (Sprite *) CreateSprite("TextureFiles/DeleteText.png", 651, 334, 500, 1, 1, 0, 100);
	YesButton = CreateButton("TextureFiles/DeleteButton.png", -300, -200, 400, 150, newID++);
	YesButton->ButtonSprite->ZIndex = 500;
	NoButton = CreateButton("TextureFiles/BackButton.png", 300, -200, 400, 150, newID++);
	NoButton->ButtonSprite->ZIndex = 500;

	BlackBackground->Visible = FALSE;
	DeleteText->Visible = FALSE;
	YesButton->ButtonSprite->Visible = FALSE;
	NoButton->ButtonSprite->Visible = FALSE;

	// Set camera to (0,0)
	ResetCamera();
	UpdateSelector(Selector);
}

void UpdateMainMenu(void)
{
	InputHandling();
}

void DrawMainMenu(void)
{
	DrawObjectList();
}

void FreeMainMenu(void)
{
	// Freeing the objects and textures
	FreeObjectList();
}

void UnloadMainMenu(void)
{
	//Destroy the textures
	DestroyTextureList();
}

void InputHandling(void)
{
	// check if forcing the application to quit
	switch(selectedButton)
	{
	// EP 1 button
	case EP1But:
		if(FoxInput_KeyTriggered(VK_DOWN) || FoxInput_KeyTriggered('S'))
		{
			selectedButton = TutBut;
			UpdateSelector(Selector);
		}
		else if(FoxInput_KeyTriggered(VK_RIGHT) || FoxInput_KeyTriggered('D'))
		{
			selectedButton = EP2But;
			UpdateSelector(Selector);
		}
		else if(FoxInput_KeyTriggered(VK_RETURN) || FoxInput_KeyTriggered(VK_SPACE))
		{
			SetNextState(GS_EP1Slides);
		}
		break;
	case EP2But:
		if(FoxInput_KeyTriggered(VK_DOWN) || FoxInput_KeyTriggered('S'))
		{
			selectedButton = QuitBut;
			UpdateSelector(Selector);
		}
		else if(FoxInput_KeyTriggered(VK_LEFT) || FoxInput_KeyTriggered('A'))
		{
			selectedButton = EP1But;
			UpdateSelector(Selector);
		}
		else if(FoxInput_KeyTriggered(VK_RETURN) || FoxInput_KeyTriggered(VK_SPACE))
		{
			SetNextState(GS_EP2Slides);
		}
		break;
	case TutBut:
		if(FoxInput_KeyTriggered(VK_UP) || FoxInput_KeyTriggered('W'))
		{
			selectedButton = EP1But;
			UpdateSelector(Selector);
		}
		else if(FoxInput_KeyTriggered(VK_RIGHT) || FoxInput_KeyTriggered('D'))
		{
			selectedButton = QuitBut;
			UpdateSelector(Selector);
		}
		else if(FoxInput_KeyTriggered(VK_RETURN) || FoxInput_KeyTriggered(VK_SPACE))
		{
			SetNextState(GS_Tutorial);
		}
		break;
	case QuitBut:
		if(FoxInput_KeyTriggered(VK_UP) || FoxInput_KeyTriggered('W'))
		{
			selectedButton = EP2But;
			UpdateSelector(Selector);
		}
		else if(FoxInput_KeyTriggered(VK_LEFT) || FoxInput_KeyTriggered('A'))
		{
			selectedButton = TutBut;
			UpdateSelector(Selector);
		}
		else if(FoxInput_KeyTriggered(VK_RETURN) || FoxInput_KeyTriggered(VK_SPACE))
		{
			SetNextState(GS_Quit);
		}
		break;
	}

	if(FoxInput_KeyTriggered('C'))
	{
		FILE *fp = fopen("../GameData.cfg", "r");
		if(!fp)
		{
			SetNextState(GS_CharacterSelect);
		}
		else
		{
			deleteSave = TRUE;
			BlackBackground->Visible = TRUE;
			DeleteText->Visible = TRUE;
			YesButton->ButtonSprite->Visible = TRUE;
			NoButton->ButtonSprite->Visible = TRUE;
			fclose(fp);
		}
	}

	if(deleteSave)
	{
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			int worldX, worldY;
			Vec2 MouseClick;

			FoxInput_GetWorldPosition(&worldX, &worldY);
			Vec2Set(&MouseClick, (float)worldX, (float)worldY);

			if(PointRectCollision(&YesButton->ButtonCollider, &MouseClick))
			{
				remove("../GameData.cfg");
				SetNextState(GS_CharacterSelect);
			}
			else if(PointRectCollision(&NoButton->ButtonCollider, &MouseClick))
			{
				deleteSave = FALSE;
				BlackBackground->Visible = FALSE;
				DeleteText->Visible = FALSE;
				YesButton->ButtonSprite->Visible = FALSE;
				NoButton->ButtonSprite->Visible = FALSE;
			}
		}

	}


	if(FoxInput_KeyTriggered(VK_SHIFT) || FoxInput_KeyTriggered(VK_RSHIFT))
	{
		SetNextState(GS_DevMenu);
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
		case EP1But:
			Selector->Position = EP1Button->Position;
			Selector->Width = EP1Button->Width * (float)1.1;
			Selector->Height = EP1Button->Height * (float)1.2;
			UpdateMesh(Selector);
			break;
		case EP2But:
			Selector->Position = EP2Button->Position;
			Selector->Width = EP2Button->Width * (float)1.1;
			Selector->Height = EP1Button->Height * (float)1.2;
			UpdateMesh(Selector);
			break;
		case TutBut:
			Selector->Position = TutorialButton->Position;
			Selector->Width = TutorialButton->Width * (float)1.1;
			Selector->Height = EP1Button->Height * (float)1.2;
			UpdateMesh(Selector);
			break;
		case QuitBut:
			Selector->Position = QuitButton->Position;
			Selector->Width = QuitButton->Width * (float)1.1;
			Selector->Height = EP1Button->Height * (float)1.2;
			UpdateMesh(Selector);
			break;
		default:
			Selector->Position = EP1Button->Position;
			Selector->Width = EP1Button->Width * (float)1.1;
			UpdateMesh(Selector);
			break;
	}
}