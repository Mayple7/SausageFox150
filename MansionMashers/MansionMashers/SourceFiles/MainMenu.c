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

Sprite* Background;

Button* NewGameButton;
Button* LoadGameButton;
Button* OptionsButton;
Button* CreditsButton;
Button* QuitGameButton;

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
	FILE *fp;
	Vec3 Tint;
	newID = 10;
	// Reset the object list
	ResetObjectList();

	Background = (Sprite *) CreateSprite("TextureFiles/MainMenuBack.png", 1920, 1080, 1, 1, 1, 0, 0);

	NewGameButton = CreateButton("TextureFiles/NewGameButton.png", 0, -170, 394, 394, newID++);
	LoadGameButton = CreateButton("TextureFiles/LoadGameButton.png", -290, -48, 439, 170, newID++);
	OptionsButton = CreateButton("TextureFiles/OptionsButton.png", 290, -48, 439, 170, newID++);
	CreditsButton = CreateButton("TextureFiles/CreditsButton.png", -290, -320, 439, 170, newID++);
	QuitGameButton = CreateButton("TextureFiles/QuitGameButton.png", 290, -320, 439, 170, newID++);

	NewGameButton->ButtonSprite->ZIndex = 5;
	LoadGameButton->ButtonSprite->ZIndex = 3;
	OptionsButton->ButtonSprite->ZIndex = 3;
	CreditsButton->ButtonSprite->ZIndex = 3;
	QuitGameButton->ButtonSprite->ZIndex = 3;
	
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
