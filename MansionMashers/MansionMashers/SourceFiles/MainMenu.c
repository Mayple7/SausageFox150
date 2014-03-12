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
\li					BackgroundAnimation
\li					SetStartLocation
\li					RandomNewTexture
  
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

#define MAX_TEXTURES 4

//The backgrounds for animation
Sprite* FirstBackground;
Sprite* SecondBackground;

Sprite* Logo;
Sprite* Overlay;

Sprite* FoxScroll;
Sprite* DogScroll;

Sprite* DogScrollBottom;
Sprite* FoxScrollBottom;

//Dem buttons
Button* NewGameButton;
Button* LoadGameButton;
Button* OptionsButton;
Button* CreditsButton;
Button* QuitGameButton;

//Delete save file objects
Sprite* BlackBackground;
Sprite* DeleteText;
Sprite* ExitConfirm;
Button* YesButton;
Button* NoButton;
Button* ExitButton;

enum Buttons { EP1But, EP2But, TutBut, QuitBut};
static int selectedButton;								//0: EP1, 1: EP2, 2: tutorial, 3: quit
static int deleteSave; // If true, dialogue box to delete save is up
static int exitGame;
static int newID;
static int canLoad;

//All those static ints for the animation
static int firstTextureNum;
static int secondTextureNum;
static int firstStartLocation;
static int secondStartLocation;
static int firstAnimated;
static int secondAnimated;
static float firstMoveTimer;
static float secondMoveTimer;
static int fadeOut;

static int prevFrame;

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
	Initializes the main menu
*/
/*************************************************************************/
void InitializeMainMenu(void)
{
	FILE *fp;
	Vec3 Tint;
	float xPos, yPos;
	
	//Animation variables initialize
	firstAnimated = TRUE;
	secondAnimated = FALSE;
	fadeOut = FALSE;
	firstMoveTimer = 0;
	secondMoveTimer = 0;
	firstStartLocation = rand() % 4;
	secondStartLocation = rand() % 4;
	firstTextureNum = MAX_TEXTURES + 1;
	secondTextureNum = MAX_TEXTURES + 1;

	newID = 10;
	// Reset the object list
	ResetObjectList();
	deleteSave = FALSE;
	exitGame = FALSE;

	//Check if it is possible to load a game
	fp = fopen(GameData, "r");
	if(fp)
	{
		canLoad = TRUE;
		fclose(fp);
	}
	else
	{
		canLoad = FALSE;
	}

	//Grabs the first starting location
	SetStartLocation(&xPos, &yPos, firstStartLocation);

	FirstBackground = (Sprite *) CreateSprite("TextureFiles/BlurFoxHall.png", 2560, 1440, 1, 1, 1, xPos, yPos);
	SecondBackground = (Sprite *) CreateSprite("TextureFiles/BlurFoxMansion.png", 2560, 1440, 1, 1, 1, xPos, yPos);

	//Randomizes the textures for the animation
	firstTextureNum = RandomNewTexture(FirstBackground, secondTextureNum);
	secondTextureNum = RandomNewTexture(SecondBackground, firstTextureNum);

	//Second background should not be seen
	SecondBackground->Alpha = 0.0f;

	//Nice looking things for the menu
	Overlay = (Sprite *) CreateSprite("TextureFiles/MenuOverlay.png", 1920, 1080, 2, 1, 1, 0, 0);
	Logo = (Sprite *) CreateSprite("TextureFiles/MansionMashersMainMenu.png", 1200, 675, 3, 1, 1, 0, 200);

	//Button backgrounds
	FoxScroll = (Sprite *) CreateSprite("TextureFiles/MenuBackFoxAnimationFix.png", 447, 500, 4, 18, 1, -300, -130);
	FoxScroll->AnimationSpeed = 2.0f;
	DogScroll = (Sprite *) CreateSprite("TextureFiles/MenuBackDogAnimationFix.png", 447, 500, 4, 18, 1, 300, -130);
	DogScroll->AnimationSpeed = 2.0f;
	//Sets the frame that the scrolls are on.
	prevFrame = FoxScroll->CurrentFrame;
	FoxScrollBottom = (Sprite *) CreateSprite("TextureFiles/ScrollBottom.png", 447, 44, 9, 1, 1,  -300, 60);
	DogScrollBottom = (Sprite *) CreateSprite("TextureFiles/ScrollBottom.png", 447, 44, 9, 1, 1,  300, 60);

	//Menu buttons
	NewGameButton = CreateButton("TextureFiles/NewGameButton.png", 0, -130, 394, 394, newID++);
	LoadGameButton = CreateButton("TextureFiles/LoadGameButton.png", -290, -8, 439, 170, newID++);
	OptionsButton = CreateButton("TextureFiles/OptionsButton.png", 290, -8, 439, 170, newID++);
	CreditsButton = CreateButton("TextureFiles/CreditsButton.png", -290, -270, 439, 170, newID++);
	QuitGameButton = CreateButton("TextureFiles/QuitGameButton.png", 290, -270, 439, 170, newID++);

	//Update button Z index
	NewGameButton->ButtonSprite->ZIndex = 10;
	LoadGameButton->ButtonSprite->ZIndex = 8;
	OptionsButton->ButtonSprite->ZIndex = 8;
	CreditsButton->ButtonSprite->ZIndex = 8;
	QuitGameButton->ButtonSprite->ZIndex = 8;

	OptionsButton->ButtonSprite->Visible = FALSE;
	QuitGameButton->ButtonSprite->Visible = FALSE;
	CreditsButton->ButtonSprite->Visible = FALSE;
	LoadGameButton->ButtonSprite->Visible = FALSE;
	
	//Delete save game objects
	Vec3Set(&Tint, 0, 0, 0);
	BlackBackground = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 499, 1, 1, 0, 0);
	BlackBackground->Tint = Tint;
	BlackBackground->Alpha = 0.5f;
	DeleteText = (Sprite *) CreateSprite("TextureFiles/DeleteText.png", 651, 334, 500, 1, 1, 0, 100);
	YesButton = CreateButton("TextureFiles/DeleteButton.png", -300, -200, 400, 150, newID++);
	YesButton->ButtonSprite->ZIndex = 500;
	NoButton = CreateButton("TextureFiles/BackButton.png", 300, -200, 400, 150, newID++);
	NoButton->ButtonSprite->ZIndex = 500;

	//Exit Game Confirmation
	Vec3Set(&Tint, 0, 0, 0);
	ExitConfirm = (Sprite *) CreateSprite("TextureFiles/ExitConfirm.png", 639, 204, 500, 1, 1, 0, 100);
	ExitButton = CreateButton("TextureFiles/ExitButton.png", -300, -200, 400, 150, newID++);
	ExitButton->ButtonSprite->ZIndex = 500;

	//Should not be visible at the start
	BlackBackground->Visible = FALSE;
	DeleteText->Visible = FALSE;
	ExitConfirm->Visible = FALSE;
	YesButton->ButtonSprite->Visible = FALSE;
	NoButton->ButtonSprite->Visible = FALSE;
	ExitButton->ButtonSprite->Visible = FALSE;

	CreateBoundingBoxes();

	// Set camera to (0,0)
	ResetCamera();
}
/*************************************************************************/
/*!
	\brief
	Updates the main menu objects
*/
/*************************************************************************/
void UpdateMainMenu(void)
{
	//Handle input and run background animation
	if (DogScroll->CurrentFrame == 17)
	{
		DogScroll->AnimationActive = FALSE;
	}
	
	//If the position is greater than the final position
	if(DogScrollBottom->Position.y >= -345 * GetLoadRatio())
	{
		//Blaze the bottom down based on the total distance traveled divided by the number of frames and frames per change
		DogScrollBottom->Position.y -= (420 * GetLoadRatio()) / 18 / DogScroll->AnimationSpeed;
	}
	
	if (DogScrollBottom->Position.y < OptionsButton->ButtonSprite->Position.y)
	{
		OptionsButton->ButtonSprite->Visible = TRUE;
	}

	if (DogScrollBottom->Position.y < QuitGameButton->ButtonSprite->Position.y)
	{
		QuitGameButton->ButtonSprite->Visible = TRUE;
	}

	if (FoxScroll->CurrentFrame == 17)
		FoxScroll->AnimationActive = FALSE;
	
	if(FoxScrollBottom->Position.y >= -345 * GetLoadRatio())
	{
		//Blaze the bottom down based on the total distance traveled divided by the number of frames and frames per change
		FoxScrollBottom->Position.y -= (420 * GetLoadRatio()) / 18 / FoxScroll->AnimationSpeed;
	}

	if (FoxScrollBottom->Position.y < LoadGameButton->ButtonSprite->Position.y)
	{
		LoadGameButton->ButtonSprite->Visible = TRUE;
	}

	if (FoxScrollBottom->Position.y < CreditsButton->ButtonSprite->Position.y)
	{
		CreditsButton->ButtonSprite->Visible = TRUE;
	}

	InputHandling();
	BackgroundAnimation();
}

/*************************************************************************/
/*!
	\brief
	Draws the objects in the main menu
*/
/*************************************************************************/
void DrawMainMenu(void)
{
	DrawObjectList();
	DrawCollisionList();
}

/*************************************************************************/
/*!
	\brief
	Frees the menory allocated in the main menu
*/
/*************************************************************************/
void FreeMainMenu(void)
{
	// Freeing the objects and textures
	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads the textures used in the main menu
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
	Handles the input from the main menu
*/
/*************************************************************************/
void InputHandling(void)
{
	//If delete save option is up
	if(deleteSave)
	{
		// Mouse location data
		int worldX, worldY;
		Vec2 MouseHover;

		FoxInput_GetWorldPosition(&worldX, &worldY);
		Vec2Set(&MouseHover, (float)worldX, (float)worldY);

		//Adjust scales when mouse is on the delete save button
		if(PointRectCollision(&YesButton->ButtonCollider, &MouseHover))
		{
			YesButton->ButtonSprite->ScaleX = 1.1f;
			YesButton->ButtonSprite->ScaleY = 1.1f;

			NoButton->ButtonSprite->ScaleX = 1.0f;
			NoButton->ButtonSprite->ScaleY = 1.0f;
		}
		//On the back button
		else if(PointRectCollision(&NoButton->ButtonCollider, &MouseHover))
		{
			YesButton->ButtonSprite->ScaleX = 1.0f;
			YesButton->ButtonSprite->ScaleY = 1.0f;

			NoButton->ButtonSprite->ScaleX = 1.1f;
			NoButton->ButtonSprite->ScaleY = 1.1f;
		}
		//Back to normal
		else
		{
			YesButton->ButtonSprite->ScaleX = 1.0f;
			YesButton->ButtonSprite->ScaleY = 1.0f;

			NoButton->ButtonSprite->ScaleX = 1.0f;
			NoButton->ButtonSprite->ScaleY = 1.0f;
		}


		//On click
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			//Get mouse info
			int worldX, worldY;
			Vec2 MouseClick;

			FoxInput_GetWorldPosition(&worldX, &worldY);
			Vec2Set(&MouseClick, (float)worldX, (float)worldY);

			//We want a new game right meow!
			if(PointRectCollision(&YesButton->ButtonCollider, &MouseClick))
			{
				if(remove(GameData))
					printf("SOMETHING WENT WRONG\n");
				SetNextState(GS_CharacterSelect);
			}
			//I'm so close, lets just continue!
			else if(PointRectCollision(&NoButton->ButtonCollider, &MouseClick))
			{
				//Hides all the save deletion sprites
				deleteSave = FALSE;
				BlackBackground->Visible = FALSE;
				DeleteText->Visible = FALSE;
				YesButton->ButtonSprite->Visible = FALSE;
				NoButton->ButtonSprite->Visible = FALSE;
			}
		}
	}
	//If exit confirmation is up
	else if(exitGame)
	{
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			//Get mouse info
			int worldX, worldY;
			Vec2 MouseClick;

			FoxInput_GetWorldPosition(&worldX, &worldY);
			Vec2Set(&MouseClick, (float)worldX, (float)worldY);

			//We want a new game right meow!
			if(PointRectCollision(&ExitButton->ButtonCollider, &MouseClick))
			{
				SetNextState(GS_Quit);
			}
			//I'm so close, lets just continue!
			else if(PointRectCollision(&NoButton->ButtonCollider, &MouseClick))
			{
				//Hides all the save deletion sprites
				exitGame = FALSE;
				BlackBackground->Visible = FALSE;
				ExitConfirm->Visible = FALSE;
				ExitButton->ButtonSprite->Visible = FALSE;
				NoButton->ButtonSprite->Visible = FALSE;
			}
		}
	}
	//Mouse clicks for the regular buttons
	else if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		//Get mouse position
		int worldX, worldY;
		Vec2 MouseClick;

		FoxInput_GetWorldPosition(&worldX, &worldY);
		Vec2Set(&MouseClick, (float)worldX, (float)worldY);
		//New game button
		if(PointRectCollision(&NewGameButton->ButtonCollider, &MouseClick))
		{
			//If a save file exists bring up the save game deletion stuff
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
		//Load game button only activates if there is a save game file
		else if(canLoad && PointRectCollision(&LoadGameButton->ButtonCollider, &MouseClick))
		{
			//Loads the map level with the player at the correct spot
			SetNextState(GS_MapLevel);
		}
		//Options button
		else if(PointRectCollision(&OptionsButton->ButtonCollider, &MouseClick))
		{
			SetNextState(GS_Options);
		}
		//Roll those credits!
		else if(PointRectCollision(&CreditsButton->ButtonCollider, &MouseClick))
		{
			// Load Game Function hurrr
			SetNextState(GS_EPMenu);
		}
		//No one ever wants to quit...
		else if(PointRectCollision(&QuitGameButton->ButtonCollider, &MouseClick))
		{
			//Quit game :(
			exitGame = TRUE;
			BlackBackground->Visible = TRUE;
			ExitConfirm->Visible = TRUE;
			ExitButton->ButtonSprite->Visible = TRUE;
			NoButton->ButtonSprite->Visible = TRUE;
		}
	}
	//Mouse hover stuff
	else 
	{
		// Mouse location data
		int worldX, worldY;
		Vec2 MouseHover;

		FoxInput_GetWorldPosition(&worldX, &worldY);
		Vec2Set(&MouseHover, (float)worldX, (float)worldY);
		//Adjust scales when mouse is on the new game button
		if(PointCircleCollision(&NewGameButton->ButtonCollider.Position, NewGameButton->ButtonSprite->Width / 2.0f, &MouseHover))
		{
			NewGameButton->ButtonSprite->ScaleX = 1.1f;
			NewGameButton->ButtonSprite->ScaleY = 1.1f;

			LoadGameButton->ButtonSprite->ScaleX = 1.0f;
			LoadGameButton->ButtonSprite->ScaleY = 1.0f;

			OptionsButton->ButtonSprite->ScaleX = 1.0f;
			OptionsButton->ButtonSprite->ScaleY = 1.0f;

			CreditsButton->ButtonSprite->ScaleX = 1.0f;
			CreditsButton->ButtonSprite->ScaleY = 1.0f;

			QuitGameButton->ButtonSprite->ScaleX = 1.0f;
			QuitGameButton->ButtonSprite->ScaleY = 1.0f;
		}
		// Not the new game button
		else
		{
			//Scale back to normal size
			NewGameButton->ButtonSprite->ScaleX = 1.0f;
			NewGameButton->ButtonSprite->ScaleY = 1.0f;

			//Hover if mouse collision and you can load the game
			if(canLoad && PointRectCollision(&LoadGameButton->ButtonCollider, &MouseHover))
			{
				LoadGameButton->ButtonSprite->ScaleX = 1.2f;
				LoadGameButton->ButtonSprite->ScaleY = 1.2f;
			}
			// Back to normal
			else
			{
				LoadGameButton->ButtonSprite->ScaleX = 1.0f;
				LoadGameButton->ButtonSprite->ScaleY = 1.0f;
			}
			//Options hover effect
			if(PointRectCollision(&OptionsButton->ButtonCollider, &MouseHover))
			{
				OptionsButton->ButtonSprite->ScaleX = 1.2f;
				OptionsButton->ButtonSprite->ScaleY = 1.2f;
			}
			//Options back to normal
			else
			{
				OptionsButton->ButtonSprite->ScaleX = 1.0f;
				OptionsButton->ButtonSprite->ScaleY = 1.0f;
			}
			//Credits hover effect
			if(PointRectCollision(&CreditsButton->ButtonCollider, &MouseHover))
			{
				CreditsButton->ButtonSprite->ScaleX = 1.2f;
				CreditsButton->ButtonSprite->ScaleY = 1.2f;
			}
			//Credits back to normal
			else
			{
				CreditsButton->ButtonSprite->ScaleX = 1.0f;
				CreditsButton->ButtonSprite->ScaleY = 1.0f;
			}
			//I guess we should make the quit button have a hover effect
			if(PointRectCollision(&QuitGameButton->ButtonCollider, &MouseHover))
			{
				QuitGameButton->ButtonSprite->ScaleX = 1.2f;
				QuitGameButton->ButtonSprite->ScaleY = 1.2f;
			}
			//Yay! Back to normal!
			else
			{
				QuitGameButton->ButtonSprite->ScaleX = 1.0f;
				QuitGameButton->ButtonSprite->ScaleY = 1.0f;
			}
		}
	}

#if defined _DEBUG
	//Sneaky keys to get to the dev menu
	if(FoxInput_KeyTriggered(VK_SHIFT) || FoxInput_KeyTriggered(VK_RSHIFT))
	{
		SetNextState(GS_DevMenu);
	}
#endif

	// check if forcing the application to quit
	if (FoxInput_KeyTriggered(VK_ESCAPE))
	{
		if(!exitGame)
		{
			exitGame = TRUE;
			BlackBackground->Visible = TRUE;
			ExitConfirm->Visible = TRUE;
			ExitButton->ButtonSprite->Visible = TRUE;
			NoButton->ButtonSprite->Visible = TRUE;
		}
		else
		{
			exitGame = FALSE;
			BlackBackground->Visible = FALSE;
			ExitConfirm->Visible = FALSE;
			ExitButton->ButtonSprite->Visible = FALSE;
			NoButton->ButtonSprite->Visible = FALSE;
		}
	}
}

/*************************************************************************/
/*!
	\brief
	The logic/magic behind the amazing background animation
*/
/*************************************************************************/
void BackgroundAnimation(void)
{
	//If the first sprite is being animated
	if(firstAnimated)
	{
		//Update the move timer
		firstMoveTimer += GetDeltaTime();
		switch(firstStartLocation)
		{
		// Top right
		case 0:
			//Update the position
			FirstBackground->Position.x -= 640 * GetLoadRatio() / (10 / GetDeltaTime());
			FirstBackground->Position.y -= 360 * GetLoadRatio() / (10 / GetDeltaTime());
			//Fade out is complete
			if(firstMoveTimer >= 10)
			{
				//Ensure the alphas are correct and get a new texture for the first background
				firstAnimated = FALSE;
				FirstBackground->Alpha = 0.0f;
				SecondBackground->Alpha = 1.0f;
				firstTextureNum = RandomNewTexture(FirstBackground, secondTextureNum);
			}
			//Start fading out the first background and fading in the second background
			else if(firstMoveTimer > 8)
			{
				FirstBackground->Alpha -= GetDeltaTime() / 2.0f;
				SecondBackground->Alpha += GetDeltaTime() / 2.0f;
				//Start animating the second background and set its random location
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
				firstTextureNum = RandomNewTexture(FirstBackground, secondTextureNum);
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
				firstTextureNum = RandomNewTexture(FirstBackground, secondTextureNum);
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
				firstTextureNum = RandomNewTexture(FirstBackground, secondTextureNum);
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
	//Same code as above, but swapped for the second button
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
				secondTextureNum = RandomNewTexture(SecondBackground, firstTextureNum);
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
				secondTextureNum = RandomNewTexture(SecondBackground, firstTextureNum);
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
				secondTextureNum = RandomNewTexture(SecondBackground, firstTextureNum);
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
				secondTextureNum = RandomNewTexture(SecondBackground, firstTextureNum);
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

/*************************************************************************/
/*!
	\brief
	Sets the animation background starting location

	\param xPos
	Pointer to the new x position

	\param yPos
	Pointer to the new y position

	\param startNum
	The random number to position the background
*/
/*************************************************************************/
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

/*************************************************************************/
/*!
	\brief
	Chooses a random new texture that does not repeat

	\param CurrentSprite
	The current sprite to change texture

	\param prevTexture
	The other background's texture so we don't have repeats

	\return
	The texture chosen to save for the future so we don't have repeats
*/
/*************************************************************************/
int RandomNewTexture(Sprite* CurrentSprite, int prevTexture)
{
	//Get a new texture randomly!
	int newTextureNum = rand() % MAX_TEXTURES;

	//No repeats now...
	while((newTextureNum = rand() % MAX_TEXTURES) == prevTexture)
		continue;

	//Assign the texture
	switch(newTextureNum)
	{
	case 0:
		CurrentSprite->SpriteTexture = LoadTexture("TextureFiles/BlurFoxHall.png");
		break;
	case 1:
		CurrentSprite->SpriteTexture = LoadTexture("TextureFiles/BlurFoxMansion.png");
		break;
	case 2:
		CurrentSprite->SpriteTexture = LoadTexture("TextureFiles/BlurFoxCafe.png");
		break;
	case 3:
		CurrentSprite->SpriteTexture = LoadTexture("TextureFiles/BlurFoxHandGuy.png");
		break;
	}
	//Returns the texture assigned
	return newTextureNum;
}
