// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/EPMenu.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"

Sprite* GameLogo;

Sprite* EP1Button;
Sprite* EP2Button;
Sprite* TutorialButton;
Sprite* QuitButton;

Sprite* Selector;

enum Buttons { EP1But, EP2But, TutBut, QuitBut};
int selectedEPButton;								//0: EP1, 1: EP2, 2: tutorial, 3: quit

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
	// Reset the object list
	resetObjectList();

	selectedEPButton = EP1But;

	GameLogo = CreateSprite("TextureFiles/MansionMashersLogo.png", 1920.0f, 1080.0f, 1, 1, 1, 0, 0);

	// Create the start button
	EP1Button = CreateSprite("TextureFiles/EP1_button.png", 300.0f, 100.0f, 3, 1, 1, -500, 300);

	// Create the start button
	EP2Button = CreateSprite("TextureFiles/EP2_button.png", 300.0f, 100.0f, 3, 1, 1, 500, 300);

	// Creates the showcase button
	TutorialButton = CreateSprite("TextureFiles/tutorial_button.png", 300.0f, 100.0f, 3, 1, 1, -500, -300);
	
	// Creates the exit button
	QuitButton = CreateSprite("TextureFiles/quit_button.png", 300.0f, 100.0f, 3, 1, 1, 500, -300);

	// Creates the selector button - set to default position of the start button
	Selector = CreateSprite("TextureFiles/Selector.png", 500.0f, 200.0f, 2, 1, 1, 100, 0);
	
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
	freeObjectList();
}

void UnloadEPMenu(void)
{
	//Destroy the textures
	DestroyTextureList();
}

void EPMenuInput(void)
{
	// check if forcing the application to quit
	switch(selectedEPButton)
	{
	// EP 1 button
	case EP1But:
		if(AEInputCheckTriggered(VK_DOWN) || AEInputCheckTriggered('S'))
		{
			selectedEPButton = TutBut;
			UpdateEPSelector(Selector);
		}
		else if(AEInputCheckTriggered(VK_RIGHT) || AEInputCheckTriggered('D'))
		{
			selectedEPButton = EP2But;
			UpdateEPSelector(Selector);
		}
		else if(AEInputCheckTriggered(VK_RETURN) || AEInputCheckTriggered(VK_SPACE))
		{
			SetNextState(GS_MainMenu);
		}
		break;
	case EP2But:
		if(AEInputCheckTriggered(VK_DOWN) || AEInputCheckTriggered('S'))
		{
			selectedEPButton = QuitBut;
			UpdateEPSelector(Selector);
		}
		else if(AEInputCheckTriggered(VK_LEFT) || AEInputCheckTriggered('A'))
		{
			selectedEPButton = EP1But;
			UpdateEPSelector(Selector);
		}
		else if(AEInputCheckTriggered(VK_RETURN) || AEInputCheckTriggered(VK_SPACE))
		{
			SetNextState(GS_MainMenu);
		}
		break;
	case TutBut:
		if(AEInputCheckTriggered(VK_UP) || AEInputCheckTriggered('W'))
		{
			selectedEPButton = EP1But;
			UpdateEPSelector(Selector);
		}
		else if(AEInputCheckTriggered(VK_RIGHT) || AEInputCheckTriggered('D'))
		{
			selectedEPButton = QuitBut;
			UpdateEPSelector(Selector);
		}
		else if(AEInputCheckTriggered(VK_RETURN) || AEInputCheckTriggered(VK_SPACE))
		{
			SetNextState(GS_SplashScreen);
		}
		break;
	case QuitBut:
		if(AEInputCheckTriggered(VK_UP) || AEInputCheckTriggered('W'))
		{
			selectedEPButton = EP2But;
			UpdateEPSelector(Selector);
		}
		else if(AEInputCheckTriggered(VK_LEFT) || AEInputCheckTriggered('A'))
		{
			selectedEPButton = TutBut;
			UpdateEPSelector(Selector);
		}
		else if(AEInputCheckTriggered(VK_RETURN) || AEInputCheckTriggered(VK_SPACE))
		{
			SetNextState(GS_Quit);
		}
		break;
	}

	if(AEInputCheckTriggered(VK_RSHIFT))
	{
		SetNextState(GS_MainMenu);
	}

	// check if forcing the application to quit
	if (AEInputCheckTriggered(VK_ESCAPE))
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
void UpdateEPSelector(struct Sprite *Selector)
{
	switch(selectedEPButton)
	{
		case EP1But:
			Selector->Position = EP1Button->Position;
			Selector->Width = EP1Button->Width * 1.1 * GetLoadRatio();
			Selector->Height = EP1Button->Height * 1.2 * GetLoadRatio();
			UpdateMesh(Selector);
			break;
		case EP2But:
			Selector->Position = EP2Button->Position;
			Selector->Width = EP2Button->Width * 1.1 * GetLoadRatio();
			Selector->Height = EP1Button->Height * 1.2 * GetLoadRatio();
			UpdateMesh(Selector);
			break;
		case TutBut:
			Selector->Position = TutorialButton->Position;
			Selector->Width = TutorialButton->Width * 1.1 * GetLoadRatio();
			Selector->Height = EP1Button->Height * 1.2 * GetLoadRatio();
			UpdateMesh(Selector);
			break;
		case QuitBut:
			Selector->Position = QuitButton->Position;
			Selector->Width = QuitButton->Width * 1.1 * GetLoadRatio();
			Selector->Height = EP1Button->Height * 1.2 * GetLoadRatio();
			UpdateMesh(Selector);
			break;
		default:
			Selector->Position = EP1Button->Position;
			Selector->Width = EP1Button->Width * 1.1 * GetLoadRatio();
			UpdateMesh(Selector);
			break;
	}
}