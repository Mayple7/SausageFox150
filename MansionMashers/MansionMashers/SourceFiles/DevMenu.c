/*****************************************************************************/
/*!
\file				DevMenu.c
\author				Dan Muller (d.muller)
\date				Feb 22, 2014

\brief				Functions for the main menu

\par				Functions:
\li					LoadDevMenu
\li					InitializeDevMenu
\li					UpdateDevMenu
\li					DrawDevMenu
\li					FreeDevMenu
\li					UnloadDevMenu
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
#include "../HeaderFiles/DevMenu.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals

Button* Level1;
Button* Level2;
Button* Level3;
Button* Level4;

Button* ArmGuy;
Button* HandGuy;
Button* YeahGuy;
Button* Kevin;

Sprite* Selector;
Sprite* Logo;

AEGfxTexture *StartButtonTexture;
AEGfxTexture *ShowcaseTexture;
AEGfxTexture *ExitButtonTexture;

static int numMenuButtons = 8;
static int selectedButton = 0;								//0: start, 1: showcase 2: exit
static int newID;

// ---------------------------------------------------------------------------
// Static function protoypes

/*************************************************************************/
/*!
	\brief
	Loads the assets needed for the main menu
*/
/*************************************************************************/
void LoadDevMenu(void)
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
void InitializeDevMenu(void)
{
	newID = 10;
	// Reset the object list
	ResetObjectList();

	// Create the start button
	Logo = (Sprite *) CreateSprite("TextureFiles/MansionMashersLogo.png", 1920.0f, 1080.0f, 1, 1, 1, 0, 0);

	// Create the start button
	Level1 = CreateButton("TextureFiles/Level1.png", -300, 300, 400.0f, 100.0f, newID++);
	Level2 = CreateButton("TextureFiles/Level2.png", -300, 100, 400.0f, 100.0f, newID++);
	Level3 = CreateButton("TextureFiles/Level3.png", -300, -100, 400.0f, 100.0f, newID++);
	Level4 = CreateButton("TextureFiles/Level4.png", -300, -300, 400.0f, 100.0f, newID++);

	ArmGuy = CreateButton("TextureFiles/ArmGuy.png", 300, 300, 400.0f, 100.0f, newID++);
	HandGuy = CreateButton("TextureFiles/HandGuy.png", 300, 100, 400.0f, 100.0f, newID++);
	YeahGuy = CreateButton("TextureFiles/YeahGuy.png", 300, -100, 400.0f, 100.0f, newID++);
	Kevin = CreateButton("TextureFiles/Kevin.png", 300, -300, 400.0f, 100.0f, newID++);

	// Creates the selector button - set to default position of the start button
	Selector = (Sprite *) CreateSprite("TextureFiles/Selector.png", 420.0f, 120.0f, 2, 1, 1, 100, 0);
	
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
void UpdateDevMenu(void)
{
	InputHandling();
}

/*************************************************************************/
/*!
	\brief
	Draws all the objects for the main menu
*/
/*************************************************************************/
void DrawDevMenu(void)
{
	DrawObjectList();
	DrawCollisionList();
}

/*************************************************************************/
/*!
	\brief
	Frees all the allocated memory in the main menu
*/
/*************************************************************************/
void FreeDevMenu(void)
{
	// Freeing the objects and textures
	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads all the assets used in the main menu
*/
/*************************************************************************/
void UnloadDevMenu(void)
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
	if(FoxInput_KeyTriggered('U'))
	{
		SetDebugMode();
		//OverlayGrid->Visible = TRUE;
	}
	if(FoxInput_KeyTriggered('I'))
	{
		RemoveDebugMode();
		//OverlayGrid->Visible = FALSE;
	}
	// check if forcing the application to quit
	if (FoxInput_KeyTriggered(VK_ESCAPE))
		SetNextState(GS_Quit);

	if (FoxInput_KeyTriggered(VK_SHIFT) || FoxInput_KeyTriggered(VK_RSHIFT))
		SetNextState(GS_MainMenu);

	if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		int i, worldX, worldY;
		Vec2 MouseClick;

		FoxInput_GetWorldPosition(&worldX, &worldY);
		Vec2Set(&MouseClick, (float)worldX, (float)worldY);

		for(i = 0; i < BUTTONAMOUNT; i++)
		{
			if(!buttonList[i].ButtonCollider.collisionID)
				continue;
			else if(buttonList[i].ButtonCollider.collisionID == Level1->ButtonCollider.collisionID)
			{
				if(PointRectCollision(&buttonList[i].ButtonCollider, &MouseClick))
				{
					SetNextState(GS_Level1);
				}
			}
			else if(buttonList[i].ButtonCollider.collisionID == Level2->ButtonCollider.collisionID)
			{
				if(PointRectCollision(&buttonList[i].ButtonCollider, &MouseClick))
				{
					SetNextState(GS_Level2);
				}
			}
			else if(buttonList[i].ButtonCollider.collisionID == Level3->ButtonCollider.collisionID)
			{
				if(PointRectCollision(&buttonList[i].ButtonCollider, &MouseClick))
				{
					SetNextState(GS_Level3);
				}
			}
			else if(buttonList[i].ButtonCollider.collisionID == Level4->ButtonCollider.collisionID)
			{
				if(PointRectCollision(&buttonList[i].ButtonCollider, &MouseClick))
				{
					SetNextState(GS_Level4);
				}
			}
			else if(buttonList[i].ButtonCollider.collisionID == ArmGuy->ButtonCollider.collisionID)
			{
				if(PointRectCollision(&buttonList[i].ButtonCollider, &MouseClick))
				{
					SetNextState(GS_ArmGuy);
				}
			}
			else if(buttonList[i].ButtonCollider.collisionID == HandGuy->ButtonCollider.collisionID)
			{
				if(PointRectCollision(&buttonList[i].ButtonCollider, &MouseClick))
				{
					SetNextState(GS_HandGuy);
				}
			}
			else if(buttonList[i].ButtonCollider.collisionID == YeahGuy->ButtonCollider.collisionID)
			{
				if(PointRectCollision(&buttonList[i].ButtonCollider, &MouseClick))
				{
					SetNextState(GS_YeahGuy);
				}
			}
			else if(buttonList[i].ButtonCollider.collisionID == Kevin->ButtonCollider.collisionID)
			{
				if(PointRectCollision(&buttonList[i].ButtonCollider, &MouseClick))
				{
					SetNextState(GS_Kevin);
				}
			}
		}
	}
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
			Vec2Scale(&Selector->Position, &Level1->Position, GetLoadRatio());
			UpdateMesh(Selector);
			break;
		case 1:
			Vec2Scale(&Selector->Position, &Level2->Position, GetLoadRatio());
			UpdateMesh(Selector);
			break;
		case 2:
			Vec2Scale(&Selector->Position, &Level3->Position, GetLoadRatio());
			UpdateMesh(Selector);
			break;
		case 3:
			Vec2Scale(&Selector->Position, &Level4->Position, GetLoadRatio());
			UpdateMesh(Selector);
			break;
		case 4:
			Vec2Scale(&Selector->Position, &ArmGuy->Position, GetLoadRatio());
			UpdateMesh(Selector);
			break;
		case 5:
			Vec2Scale(&Selector->Position, &HandGuy->Position, GetLoadRatio());
			UpdateMesh(Selector);
			break;
		case 6:
			Vec2Scale(&Selector->Position, &YeahGuy->Position, GetLoadRatio());
			UpdateMesh(Selector);
			break;
		case 7:
			Vec2Scale(&Selector->Position, &Kevin->Position, GetLoadRatio());
			UpdateMesh(Selector);
			break;
	}
}
