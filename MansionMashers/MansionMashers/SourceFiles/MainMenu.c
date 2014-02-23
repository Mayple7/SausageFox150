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
static int canLoad;

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

	fp = fopen("../GameData.cfg", "r");
	if(fp)
	{
		canLoad = TRUE;
		fclose(fp);
	}
	else
	{
		canLoad = FALSE;
	}

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
	else if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		int worldX, worldY;
		Vec2 MouseClick;

		FoxInput_GetWorldPosition(&worldX, &worldY);
		Vec2Set(&MouseClick, (float)worldX, (float)worldY);
		if(PointRectCollision(&NewGameButton->ButtonCollider, &MouseClick))
		{
			if(!canLoad)
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
			}
		}
		else if(canLoad && PointRectCollision(&LoadGameButton->ButtonCollider, &MouseClick))
		{
			// Load Game Function hurrr
			SetNextState(GS_Tutorial);
		}
		else if(PointRectCollision(&OptionsButton->ButtonCollider, &MouseClick))
		{
			// Load Game Function hurrr
			//SetNextState(GS_Options);
		}
		else if(PointRectCollision(&CreditsButton->ButtonCollider, &MouseClick))
		{
			// Load Game Function hurrr
			//SetNextState(GS_Credits);
		}
		else if(PointRectCollision(&QuitGameButton->ButtonCollider, &MouseClick))
		{
			//Quit game :(
			SetNextState(GS_Quit);
		}
	}
	else 
	{
		int worldX, worldY;
		Vec2 MouseHover;

		FoxInput_GetWorldPosition(&worldX, &worldY);
		Vec2Set(&MouseHover, (float)worldX, (float)worldY);
		if(PointCircleCollision(&NewGameButton->ButtonCollider.Position, NewGameButton->ButtonSprite->Width / 2.0f, &MouseHover))
		{
			NewGameButton->ButtonSprite->ScaleX = 1.1f;
			NewGameButton->ButtonSprite->ScaleY = 1.1f;

			LoadGameButton->ButtonSprite->ScaleX = 1.0f;
			LoadGameButton->ButtonSprite->ScaleY = 1.0f;
			LoadGameButton->ButtonSprite->Position.x = -290 * GetLoadRatio();

			OptionsButton->ButtonSprite->ScaleX = 1.0f;
			OptionsButton->ButtonSprite->ScaleY = 1.0f;
			OptionsButton->ButtonSprite->Position.x = 290 * GetLoadRatio();

			CreditsButton->ButtonSprite->ScaleX = 1.0f;
			CreditsButton->ButtonSprite->ScaleY = 1.0f;
			CreditsButton->ButtonSprite->Position.x = -290 * GetLoadRatio();

			QuitGameButton->ButtonSprite->ScaleX = 1.0f;
			QuitGameButton->ButtonSprite->ScaleY = 1.0f;
			QuitGameButton->ButtonSprite->Position.x = 290 * GetLoadRatio();
		}
		else
		{
			NewGameButton->ButtonSprite->ScaleX = 1.0f;
			NewGameButton->ButtonSprite->ScaleY = 1.0f;

			if(canLoad && PointRectCollision(&LoadGameButton->ButtonCollider, &MouseHover))
			{
				LoadGameButton->ButtonSprite->ScaleX = 1.2f;
				LoadGameButton->ButtonSprite->ScaleY = 1.2f;
				LoadGameButton->ButtonSprite->Position.x = -319 * GetLoadRatio();
			}
			else
			{
				LoadGameButton->ButtonSprite->ScaleX = 1.0f;
				LoadGameButton->ButtonSprite->ScaleY = 1.0f;
				LoadGameButton->ButtonSprite->Position.x = -290 * GetLoadRatio();
			}

			if(PointRectCollision(&OptionsButton->ButtonCollider, &MouseHover))
			{
				OptionsButton->ButtonSprite->ScaleX = 1.2f;
				OptionsButton->ButtonSprite->ScaleY = 1.2f;
				OptionsButton->ButtonSprite->Position.x = 319 * GetLoadRatio();
			}
			else
			{
				OptionsButton->ButtonSprite->ScaleX = 1.0f;
				OptionsButton->ButtonSprite->ScaleY = 1.0f;
				OptionsButton->ButtonSprite->Position.x = 290 * GetLoadRatio();
			}

			if(PointRectCollision(&CreditsButton->ButtonCollider, &MouseHover))
			{
				CreditsButton->ButtonSprite->ScaleX = 1.2f;
				CreditsButton->ButtonSprite->ScaleY = 1.2f;
				CreditsButton->ButtonSprite->Position.x = -319 * GetLoadRatio();
			}
			else
			{
				CreditsButton->ButtonSprite->ScaleX = 1.0f;
				CreditsButton->ButtonSprite->ScaleY = 1.0f;
				CreditsButton->ButtonSprite->Position.x = -290 * GetLoadRatio();
			}

			if(PointRectCollision(&QuitGameButton->ButtonCollider, &MouseHover))
			{
				QuitGameButton->ButtonSprite->ScaleX = 1.2f;
				QuitGameButton->ButtonSprite->ScaleY = 1.2f;
				QuitGameButton->ButtonSprite->Position.x = 319 * GetLoadRatio();
			}
			else
			{
				QuitGameButton->ButtonSprite->ScaleX = 1.0f;
				QuitGameButton->ButtonSprite->ScaleY = 1.0f;
				QuitGameButton->ButtonSprite->Position.x = 290 * GetLoadRatio();
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
