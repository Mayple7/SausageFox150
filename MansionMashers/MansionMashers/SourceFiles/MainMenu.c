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
#include "../HeaderFiles/BoundingBox.h"

Sprite* FirstBackground;
Sprite* SecondBackground;

Sprite* Logo;
Sprite* Overlay;

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

static int backgroundNum;
static int firstStartLocation;
static int secondStartLocation;
static int firstAnimated;
static int secondAnimated;
static float firstMoveTimer;
static float secondMoveTimer;
static int fadeOut;

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
	float xPos, yPos;
	
	firstAnimated = TRUE;
	secondAnimated = FALSE;
	fadeOut = FALSE;
	firstMoveTimer = 0;
	secondMoveTimer = 0;
	backgroundNum = rand() % 2;
	firstStartLocation = rand() % 4;
	secondStartLocation = rand() % 4;

	newID = 10;
	// Reset the object list
	ResetObjectList();
	deleteSave = FALSE;

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

	SetStartLocation(&xPos, &yPos, firstStartLocation);

	switch(backgroundNum)
	{
	case 0:
		FirstBackground = (Sprite *) CreateSprite("TextureFiles/BlurFoxHall.png", 2560, 1440, 1, 1, 1, xPos, yPos);
		SecondBackground = (Sprite *) CreateSprite("TextureFiles/BlurFoxMansion.png", 2560, 1440, 1, 1, 1, xPos, yPos);
		break;
	case 1:
		FirstBackground = (Sprite *) CreateSprite("TextureFiles/BlurFoxMansion.png", 2560, 1440, 1, 1, 1, xPos, yPos);
		SecondBackground = (Sprite *) CreateSprite("TextureFiles/BlurFoxHall.png", 2560, 1440, 1, 1, 1, xPos, yPos);
		break;
	}

	SecondBackground->Alpha = 0.0f;

	Overlay = (Sprite *) CreateSprite("TextureFiles/MenuOverlay.png", 1920, 1080, 2, 1, 1, 0, 0);

	Logo = (Sprite *) CreateSprite("TextureFiles/SausageFoxLogoNoBack.png", 960, 540, 3, 1, 1, 0, 270);

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

	CreateBoundingBoxes();

	// Set camera to (0,0)
	ResetCamera();
}

void UpdateMainMenu(void)
{
	InputHandling();

	BackgroundAnimation();
}

void DrawMainMenu(void)
{
	DrawObjectList();
	DrawCollisionList();
}

void FreeMainMenu(void)
{
	// Freeing the objects and textures
	FreeAllLists();
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
			SetNextState(GS_Options);
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

void BackgroundAnimation(void)
{
	if(firstAnimated)
	{
		firstMoveTimer += GetDeltaTime();
		switch(firstStartLocation)
		{
		// Top right
		case 0:
			FirstBackground->Position.x -= 640 * GetLoadRatio() / (10 / GetDeltaTime());
			FirstBackground->Position.y -= 360 * GetLoadRatio() / (10 / GetDeltaTime());
			if(firstMoveTimer >= 10)
			{
				firstAnimated = FALSE;
				FirstBackground->Alpha = 0.0f;
				SecondBackground->Alpha = 1.0f;
			}
			else if(firstMoveTimer > 8)
			{
				FirstBackground->Alpha -= GetDeltaTime() / 2.0f;
				SecondBackground->Alpha += GetDeltaTime() / 2.0f;
				if(!secondAnimated)
				{
					secondMoveTimer = 0;
					secondStartLocation = rand() % 4;
					SetStartLocation(&SecondBackground->Position.x, &SecondBackground->Position.y, secondStartLocation);
					secondAnimated = TRUE;
				}
			}
			break;
		// Bottom right
		case 1:
			FirstBackground->Position.x -= 640 * GetLoadRatio() / (10 / GetDeltaTime());
			FirstBackground->Position.y -= -360 * GetLoadRatio() / (10 / GetDeltaTime());
			if(firstMoveTimer >= 10)
			{
				firstAnimated = FALSE;
				FirstBackground->Alpha = 0.0f;
				SecondBackground->Alpha = 1.0f;
			}
			else if(firstMoveTimer > 8)
			{
				FirstBackground->Alpha -= GetDeltaTime() / 2.0f;
				SecondBackground->Alpha += GetDeltaTime() / 2.0f;
				if(!secondAnimated)
				{
					secondMoveTimer = 0;
					secondStartLocation = rand() % 4;
					SetStartLocation(&SecondBackground->Position.x, &SecondBackground->Position.y, secondStartLocation);
					secondAnimated = TRUE;
				}
			}
			break;
		// Top left
		case 2:
			FirstBackground->Position.x -= -640 * GetLoadRatio() / (10 / GetDeltaTime());
			FirstBackground->Position.y -= 360 * GetLoadRatio() / (10 / GetDeltaTime());
			if(firstMoveTimer >= 10)
			{
				firstAnimated = FALSE;
				FirstBackground->Alpha = 0.0f;
				SecondBackground->Alpha = 1.0f;
			}
			else if(firstMoveTimer > 8)
			{
				FirstBackground->Alpha -= GetDeltaTime() / 2.0f;
				SecondBackground->Alpha += GetDeltaTime() / 2.0f;
				if(!secondAnimated)
				{
					secondMoveTimer = 0;
					secondStartLocation = rand() % 4;
					SetStartLocation(&SecondBackground->Position.x, &SecondBackground->Position.y, secondStartLocation);
					secondAnimated = TRUE;
				}
			}
			break;
		// Bottom left
		case 3:
			FirstBackground->Position.x -= -640 * GetLoadRatio() / (10 / GetDeltaTime());
			FirstBackground->Position.y -= -360 * GetLoadRatio() / (10 / GetDeltaTime());
			if(firstMoveTimer >= 10)
			{
				firstAnimated = FALSE;
				FirstBackground->Alpha = 0.0f;
				SecondBackground->Alpha = 1.0f;
			}
			else if(firstMoveTimer > 8)
			{
				FirstBackground->Alpha -= GetDeltaTime() / 2.0f;
				SecondBackground->Alpha += GetDeltaTime() / 2.0f;
				if(!secondAnimated)
				{
					secondMoveTimer = 0;
					secondStartLocation = rand() % 4;
					SetStartLocation(&SecondBackground->Position.x, &SecondBackground->Position.y, secondStartLocation);
					secondAnimated = TRUE;
				}
			}
			break;
		}
	}

	if(secondAnimated)
	{
		secondMoveTimer += GetDeltaTime();
		switch(secondStartLocation)
		{
		// Top right
		case 0:
			SecondBackground->Position.x -= 640 * GetLoadRatio() / (10 / GetDeltaTime());
			SecondBackground->Position.y -= 360 * GetLoadRatio() / (10 / GetDeltaTime());
			if(secondMoveTimer >= 10)
			{
				secondAnimated = FALSE;
				SecondBackground->Alpha = 0.0f;
				FirstBackground->Alpha = 1.0f;
			}
			else if(secondMoveTimer > 8)
			{
				SecondBackground->Alpha -= GetDeltaTime() / 2.0f;
				FirstBackground->Alpha += GetDeltaTime() / 2.0f;
				if(!firstAnimated)
				{
					firstMoveTimer = 0;
					firstStartLocation = rand() % 4;
					SetStartLocation(&FirstBackground->Position.x, &FirstBackground->Position.y, firstStartLocation);
					firstAnimated = TRUE;
				}
			}
			break;
		// Bottom right
		case 1:
			SecondBackground->Position.x -= 640 * GetLoadRatio() / (10 / GetDeltaTime());
			SecondBackground->Position.y -= -360 * GetLoadRatio() / (10 / GetDeltaTime());
			if(secondMoveTimer >= 10)
			{
				secondAnimated = FALSE;
				SecondBackground->Alpha = 0.0f;
				FirstBackground->Alpha = 1.0f;
			}
			else if(secondMoveTimer > 8)
			{
				SecondBackground->Alpha -= GetDeltaTime() / 2.0f;
				FirstBackground->Alpha += GetDeltaTime() / 2.0f;
				if(!firstAnimated)
				{
					firstMoveTimer = 0;
					firstStartLocation = rand() % 4;
					SetStartLocation(&FirstBackground->Position.x, &FirstBackground->Position.y, firstStartLocation);
					firstAnimated = TRUE;
				}
			}
			break;
		// Top left
		case 2:
			SecondBackground->Position.x -= -640 * GetLoadRatio() / (10 / GetDeltaTime());
			SecondBackground->Position.y -= 360 * GetLoadRatio() / (10 / GetDeltaTime());
			if(secondMoveTimer >= 10)
			{
				secondAnimated = FALSE;
				SecondBackground->Alpha = 0.0f;
				FirstBackground->Alpha = 1.0f;
			}
			else if(secondMoveTimer > 8)
			{
				SecondBackground->Alpha -= GetDeltaTime() / 2.0f;
				FirstBackground->Alpha += GetDeltaTime() / 2.0f;
				if(!firstAnimated)
				{
					firstMoveTimer = 0;
					firstStartLocation = rand() % 4;
					SetStartLocation(&FirstBackground->Position.x, &FirstBackground->Position.y, firstStartLocation);
					firstAnimated = TRUE;
				}
			}
			break;
		// Bottom left
		case 3:
			SecondBackground->Position.x -= -640 * GetLoadRatio() / (10 / GetDeltaTime());
			SecondBackground->Position.y -= -360 * GetLoadRatio() / (10 / GetDeltaTime());
			if(secondMoveTimer >= 10)
			{
				secondAnimated = FALSE;
				SecondBackground->Alpha = 0.0f;
				FirstBackground->Alpha = 1.0f;
			}
			else if(secondMoveTimer > 8)
			{
				SecondBackground->Alpha -= GetDeltaTime() / 2.0f;
				FirstBackground->Alpha += GetDeltaTime() / 2.0f;
				if(!firstAnimated)
				{
					firstMoveTimer = 0;
					firstStartLocation = rand() % 4;
					SetStartLocation(&FirstBackground->Position.x, &FirstBackground->Position.y, firstStartLocation);
					firstAnimated = TRUE;
				}
			}
			break;
		}
	}


}

void SetStartLocation(float *xPos, float *yPos, int startNum)
{
	switch(startNum)
	{
	// Top right
	case 0:
		*xPos = 320.0f * GetLoadRatio();
		*yPos = 180.0f * GetLoadRatio();
		break;
	// Bottom right
	case 1:
		*xPos = 320.0f * GetLoadRatio();
		*yPos = -180.0f * GetLoadRatio();
		break;
	// Top left
	case 2:
		*xPos = -320.0f * GetLoadRatio();
		*yPos = 180.0f * GetLoadRatio();
		break;
	// Bottom left
	case 3:
		*xPos = -320.0f * GetLoadRatio();
		*yPos = -180.0f * GetLoadRatio();
		break;
	}
}
