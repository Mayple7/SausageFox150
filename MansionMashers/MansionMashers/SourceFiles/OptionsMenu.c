/*****************************************************************************/
/*!
\file				OptionsMenu.c
\author				Dan Muller (d.muller)
\date				Feb 23, 2014

\brief				Options menu functions

\par				Functions:
\li					LoadOptions
\li					InitializeOptions
\li					UpdateOptions
\li					DrawOptions
\li					FreeOptions
\li					UnloadOptions
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

//#include "../HeaderFiles/Sound.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/OptionsMenu.h"
#include "../HeaderFiles/MainMenu.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/BoundingBox.h"


#define MAX_TEXTURES 14

//The backgrounds for animation
Sprite* FirstBackground;
Sprite* SecondBackground;

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

Sprite* Overlay;

Sprite* OptionsTitle;
Sprite* EnableCheats;
Sprite* CheckMark;

Sprite* SFXSliderGuide;
Sprite* BGMSliderGuide;

Sprite* SFXSliderBack;
Sprite* BGMSliderBack;

Button* SFXSlider;
Button* BGMSlider;

Button* BackButton;
Button* CheatsButton;

Sprite* EnableLookAt;
Sprite* LookAtCheckMark;
Button* LookAtButton;

TextGlyphs* SFXText;
TextGlyphs* BGMText;

TextGlyphs* SFXLabel;
TextGlyphs* BGMLabel;

float SFXSliderPos, BGMSliderPos;
static char* volumestring;

/*************************************************************************/
/*!
	\brief
	Loads the assets needed for the options menu
*/
/*************************************************************************/
void LoadOptions(void)
{
	//Allocate space for a large texture
	CreateTextureList();

	// Loads all the blur textures
	LoadTexture("TextureFiles/BlurFoxHall.png");
	LoadTexture("TextureFiles/BlurFoxMansion.png");
	LoadTexture("TextureFiles/BlurFoxCafe.png");
	LoadTexture("TextureFiles/BlurLevel21.png");
	LoadTexture("TextureFiles/BlurLevel22.png");
	LoadTexture("TextureFiles/BlurLevel31.png");
	LoadTexture("TextureFiles/BlurLevel32.png");
	LoadTexture("TextureFiles/BlurLevel41.png");
	LoadTexture("TextureFiles/BlurLevel42.png");
	LoadTexture("TextureFiles/BlurLevel60.png");
	LoadTexture("TextureFiles/BlurLevel61.png");
	LoadTexture("TextureFiles/BlurLevel62.png");
	LoadTexture("TextureFiles/BlurLevel71.png");
	LoadTexture("TextureFiles/BlurLevel72.png");
}

/*************************************************************************/
/*!
	\brief
	Initializes everything for the options screen
*/
/*************************************************************************/
void InitializeOptions(void)
{
	Vec3 TextColor;
	float xPos, yPos;

	ResetObjectList();

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

	//Grabs the first starting location
	SetOptionsStartLocation(&xPos, &yPos, firstStartLocation);

	FirstBackground = (Sprite *) CreateSprite("TextureFiles/BlurFoxHall.png", 2560, 1440, 1, 1, 1, xPos, yPos);
	SecondBackground = (Sprite *) CreateSprite("TextureFiles/BlurFoxMansion.png", 2560, 1440, 1, 1, 1, xPos, yPos);

	//Randomizes the textures for the animation
	firstTextureNum = RandomNewOptionsTexture(FirstBackground, secondTextureNum);
	secondTextureNum = RandomNewOptionsTexture(SecondBackground, firstTextureNum);

	//Second background should not be seen
	SecondBackground->Alpha = 0.0f;

	//Initializes the initial volume string
	volumestring = (char *)MallocMyAlloc(5, sizeof(char));
	volumestring[0] = '1';
	volumestring[1] = '0';
	volumestring[2] = '0';
	volumestring[3] = (char)'%%';
	volumestring[4] = '\0';

	//Fake text initializations
	OptionsTitle = (Sprite *) CreateSprite("TextureFiles/OptionsTitle.png", 423, 179, 10, 1, 1, 0, 380);

	//Volume sliders
	SFXSliderGuide = (Sprite *) CreateSprite("TextureFiles/VolumeSliderGuide.png", 492, 92, 4, 1, 1, 0, 200);
	BGMSliderGuide = (Sprite *) CreateSprite("TextureFiles/VolumeSliderGuide.png", 492, 92, 4, 1, 1, 0, 0);

	SFXSliderBack = (Sprite *) CreateSprite("TextureFiles/VolumeSliderBack.png", 552, 152, 3, 1, 1, 0, 200);
	BGMSliderBack = (Sprite *) CreateSprite("TextureFiles/VolumeSliderBack.png", 552, 152, 3, 1, 1, 0, 0);

	// Find default slider position based on sound volumes
	SFXSliderPos = -SFXSliderGuide->Width / 2 + SFXSliderGuide->Width * SFXVolume;
	BGMSliderPos = -BGMSliderGuide->Width / 2 + BGMSliderGuide->Width * BGMVolume;

	//Create the slider buttons
	SFXSlider = CreateButton("TextureFiles/fox_head.png", SFXSliderPos, 200, 80, 80);
	SFXSlider->ButtonSprite->ZIndex = 502;
	SFXSlider->ButtonCollider.width *= 3;
	SFXSlider->ButtonCollider.height = SFXSliderBack->Height;

	BGMSlider = CreateButton("TextureFiles/fox_head.png", BGMSliderPos, 0, 80, 80);
	BGMSlider->ButtonSprite->ZIndex = 502;
	BGMSlider->ButtonCollider.width *= 3;
	BGMSlider->ButtonCollider.height = BGMSliderBack->Height;

	//Create the volume text
	Vec3Set(&TextColor, 1, 1, 1);
	
	SFXText = CreateText(volumestring, (SFXSliderBack->Position.x + (SFXSliderBack->Width / 2)) + 50, 200, 100, TextColor, Left, Border);
	volumestring = VolumetoString(volumestring, SFXVolume * 100);
	volumestring = strcat(volumestring, "%");
	ChangeTextString(SFXText, volumestring);
	ChangeTextZIndex(SFXText, 510);

	BGMText = CreateText(volumestring, (BGMSliderBack->Position.x + (BGMSliderBack->Width / 2)) + 50, 0, 100, TextColor, Left, Border);
	volumestring = VolumetoString(volumestring, BGMVolume * 100);
	volumestring = strcat(volumestring, "%");
	ChangeTextString(BGMText, volumestring);
	ChangeTextZIndex(BGMText, 510);

	SFXLabel = CreateText("SFX", SFXSliderBack->Position.x - 50 - SFXSliderBack->Width / 2, 200, 100, TextColor, Right, Border);
	BGMLabel = CreateText("BGM", BGMSliderBack->Position.x - 50 - BGMSliderBack->Width / 2, 0, 100, TextColor, Right, Border);

	//Set all text to be visible
	TextAllVisible(SFXText);
	TextAllVisible(BGMText);
	
	TextAllVisible(SFXLabel);
	TextAllVisible(BGMLabel);

	// Create the back button and cheats objects
	BackButton = CreateButton("TextureFiles/BackButton.png", 0, -400, 300, 112.5f);

	//Cheats check mark
	EnableCheats = (Sprite *) CreateSprite("TextureFiles/EnableCheats.png", 592, 106.4f, 4000, 1, 1, 180, -150);
	CheatsButton = CreateButton("TextureFiles/CheckBox.png", -250, -150, 100, 100);
	CheatsButton->ButtonSprite->ZIndex = 4000;
	UpdateCollider(&CheatsButton->ButtonCollider, 800, CheatsButton->ButtonCollider.height);
	CheatsButton->ButtonCollider.Position.x = 100;
	CheckMark = (Sprite *) CreateSprite("TextureFiles/CheckMark.png", 200, 200, 4001, 1, 1, -250, -150);

	//Look at mouse check mark
	EnableLookAt = (Sprite *) CreateSprite("TextureFiles/FaceMouse.png", 592, 106.4f, 4000, 1, 1, 180, -280);
	LookAtButton = CreateButton("TextureFiles/CheckBox.png", -250, -280, 100, 100);
	LookAtButton->ButtonSprite->ZIndex = 4000;
	UpdateCollider(&LookAtButton->ButtonCollider, 800, LookAtButton->ButtonCollider.height);
	LookAtButton->ButtonCollider.Position.x = 100;
	LookAtCheckMark = (Sprite *) CreateSprite("TextureFiles/CheckMark.png", 200, 200, 4001, 1, 1, -250, -280);

	Overlay = (Sprite *) CreateSprite("TextureFiles/MenuOverlay.png", 1920, 1080, 2, 1, 1, 0, 0);

	if(!Cheats)
		CheckMark->Visible = FALSE;

	if(!LookAtMouse)
		LookAtCheckMark->Visible = FALSE;

	CreateBoundingBoxes();
}

/*************************************************************************/
/*!
	\brief
	Updates all objects in the options screen
*/
/*************************************************************************/
void UpdateOptions(void)
{
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		SetNextState(GS_MainMenu);
	}

	EventOptions();

	PlayAudio(&MenuBackSnd);

	BackgroundOptionsAnimation();
}

/*************************************************************************/
/*!
	\brief
	Draws the options screen
*/
/*************************************************************************/
void DrawOptions(void)
{
	DrawObjectList();
	DrawCollisionList();
}

/*************************************************************************/
/*!
	\brief
	Saves the settings then frees all the memory
*/
/*************************************************************************/
void FreeOptions(void)
{
	FreeMyAlloc(volumestring);
	SaveSettings();
	FreeAllLists();
}

/*************************************************************************/
/*!
	\brief
	Unloads the assets needed for the options menu
*/
/*************************************************************************/
void UnloadOptions(void)
{
	//Destroy the textures
	DestroyTextureList();
}

/*************************************************************************/
/*!
	\brief
	Handles events sent to the game
*/
/*************************************************************************/
void EventOptions(void)
{
	int worldX, worldY;
	Vec2 MouseClick;

	//Grab the mouse position and store in a vector
	FoxInput_GetWorldPosition(&worldX, &worldY);
	Vec2Set(&MouseClick, (float)worldX, (float)worldY);

	//On mouse down for the sliders
	if(FoxInput_MouseDown(MOUSE_BUTTON_LEFT))
	{
		if(PointRectCollision(&SFXSlider->ButtonCollider, &MouseClick))
		{
			//Sets bounds for the slider
			if(worldX > SFXSliderGuide->Width / 2 + SFXSliderGuide->Position.x)
				SFXSlider->Position.x = SFXSliderGuide->Width / 2 + SFXSliderGuide->Position.x;
			else if(worldX < -SFXSliderGuide->Width / 2 + SFXSliderGuide->Position.x)
				SFXSlider->Position.x = -SFXSliderGuide->Width / 2 + SFXSliderGuide->Position.x;
			else
				SFXSlider->Position.x = (float)worldX;
			//Sets positions after checking bounds
			SFXSlider->ButtonSprite->Position.x = SFXSlider->Position.x;
			SFXSlider->ButtonCollider.Position.x = SFXSlider->Position.x;

			//Adjust the sounds based on the slider position
			SFXVolume = (SFXSlider->Position.x + SFXSliderGuide->Width / 2) / SFXSliderGuide->Width;

			//Bounds checking
			if(SFXVolume < 0)
				SFXVolume = 0.0f;
			else if(SFXVolume > 1.0f)
				SFXVolume = 1.0f;

			volumestring = VolumetoString(volumestring, SFXVolume * 100);
			volumestring = strcat(volumestring, "%");
			ChangeTextString(SFXText, volumestring);
		}
		else if(PointRectCollision(&BGMSlider->ButtonCollider, &MouseClick))
		{
			//Sets bounds for the slider
			if(worldX > SFXSliderGuide->Width / 2 + BGMSliderGuide->Position.x)
				BGMSlider->Position.x = BGMSliderGuide->Width / 2 + BGMSliderGuide->Position.x;
			else if(worldX < -BGMSliderGuide->Width / 2 + BGMSliderGuide->Position.x)
				BGMSlider->Position.x = -BGMSliderGuide->Width / 2 + BGMSliderGuide->Position.x;
			else
				BGMSlider->Position.x = (float)worldX;
			//Sets positions after checking bounds
			BGMSlider->ButtonSprite->Position.x = BGMSlider->Position.x;
			BGMSlider->ButtonCollider.Position.x = BGMSlider->Position.x;

			//Adjust the sounds based on the slider position
			BGMVolume = (BGMSlider->Position.x + BGMSliderGuide->Width / 2) / BGMSliderGuide->Width;

			//Bounds checking
			if(BGMVolume < 0)
				BGMVolume = 0.0f;
			else if(BGMVolume > 1.0f)
				BGMVolume = 1.0f;

			volumestring = VolumetoString(volumestring, BGMVolume * 100);
			volumestring = strcat(volumestring, "%");
			ChangeTextString(BGMText, volumestring);
		}
	}

	//Hover effects for the back button
	if(PointRectCollision(&BackButton->ButtonCollider, &MouseClick))
	{
		BackButton->ButtonSprite->ScaleX = 1.2f;
		BackButton->ButtonSprite->ScaleY = 1.2f;
	}
	else
	{
		BackButton->ButtonSprite->ScaleX = 1.0f;
		BackButton->ButtonSprite->ScaleY = 1.0f;
	}

	// On a single mouse click
	if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		//Check the cheats check box and the back button
		if(PointRectCollision(&CheatsButton->ButtonCollider, &MouseClick))
		{
			Cheats = !Cheats;
			CheckMark->Visible = !(CheckMark->Visible);
		}
		else if(PointRectCollision(&LookAtButton->ButtonCollider, &MouseClick))
		{
			LookAtMouse = !LookAtMouse;
			LookAtCheckMark->Visible = !(LookAtCheckMark->Visible);
		}
		else if(PointRectCollision(&BackButton->ButtonCollider, &MouseClick))
		{
			SetNextState(GS_MainMenu);
		}
	}

	//Sets the group volumes on mouse up
	if(FoxInput_MouseUp(MOUSE_BUTTON_LEFT))
	{
		SetChannelGroupVolume(EffectType, SFXVolume);
		SetChannelGroupVolume(MusicType, BGMVolume);
	}

}

/*************************************************************************/
/*!
	\brief
	The logic/magic behind the amazing background animation
*/
/*************************************************************************/
void BackgroundOptionsAnimation(void)
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
			FirstBackground->Position.x -= 640 / (10 / GetDeltaTime());
			FirstBackground->Position.y -= 360 / (10 / GetDeltaTime());
			//Fade out is complete
			if(firstMoveTimer >= 10)
			{
				//Ensure the alphas are correct and get a new texture for the first background
				firstAnimated = FALSE;
				FirstBackground->Alpha = 0.0f;
				SecondBackground->Alpha = 1.0f;
				firstTextureNum = RandomNewOptionsTexture(FirstBackground, secondTextureNum);
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
					SetOptionsStartLocation(&SecondBackground->Position.x, &SecondBackground->Position.y, secondStartLocation);
					secondAnimated = TRUE;
				}
			}
			break;
		// Bottom right
		case 1:
			FirstBackground->Position.x -= 640 / (10 / GetDeltaTime());
			FirstBackground->Position.y -= -360 / (10 / GetDeltaTime());
			if(firstMoveTimer >= 10)
			{
				firstAnimated = FALSE;
				FirstBackground->Alpha = 0.0f;
				SecondBackground->Alpha = 1.0f;
				firstTextureNum = RandomNewOptionsTexture(FirstBackground, secondTextureNum);
			}
			else if(firstMoveTimer > 8)
			{
				FirstBackground->Alpha -= GetDeltaTime() / 2.0f;
				SecondBackground->Alpha += GetDeltaTime() / 2.0f;
				if(!secondAnimated)
				{
					secondMoveTimer = 0;
					secondStartLocation = rand() % 4;
					SetOptionsStartLocation(&SecondBackground->Position.x, &SecondBackground->Position.y, secondStartLocation);
					secondAnimated = TRUE;
				}
			}
			break;
		// Top left
		case 2:
			FirstBackground->Position.x -= -640 / (10 / GetDeltaTime());
			FirstBackground->Position.y -= 360 / (10 / GetDeltaTime());
			if(firstMoveTimer >= 10)
			{
				firstAnimated = FALSE;
				FirstBackground->Alpha = 0.0f;
				SecondBackground->Alpha = 1.0f;
				firstTextureNum = RandomNewOptionsTexture(FirstBackground, secondTextureNum);
			}
			else if(firstMoveTimer > 8)
			{
				FirstBackground->Alpha -= GetDeltaTime() / 2.0f;
				SecondBackground->Alpha += GetDeltaTime() / 2.0f;
				if(!secondAnimated)
				{
					secondMoveTimer = 0;
					secondStartLocation = rand() % 4;
					SetOptionsStartLocation(&SecondBackground->Position.x, &SecondBackground->Position.y, secondStartLocation);
					secondAnimated = TRUE;
				}
			}
			break;
		// Bottom left
		case 3:
			FirstBackground->Position.x -= -640 / (10 / GetDeltaTime());
			FirstBackground->Position.y -= -360 / (10 / GetDeltaTime());
			if(firstMoveTimer >= 10)
			{
				firstAnimated = FALSE;
				FirstBackground->Alpha = 0.0f;
				SecondBackground->Alpha = 1.0f;
				firstTextureNum = RandomNewOptionsTexture(FirstBackground, secondTextureNum);
			}
			else if(firstMoveTimer > 8)
			{
				FirstBackground->Alpha -= GetDeltaTime() / 2.0f;
				SecondBackground->Alpha += GetDeltaTime() / 2.0f;
				if(!secondAnimated)
				{
					secondMoveTimer = 0;
					secondStartLocation = rand() % 4;
					SetOptionsStartLocation(&SecondBackground->Position.x, &SecondBackground->Position.y, secondStartLocation);
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
			SecondBackground->Position.x -= 640 / (10 / GetDeltaTime());
			SecondBackground->Position.y -= 360 / (10 / GetDeltaTime());
			if(secondMoveTimer >= 10)
			{
				secondAnimated = FALSE;
				SecondBackground->Alpha = 0.0f;
				FirstBackground->Alpha = 1.0f;
				secondTextureNum = RandomNewOptionsTexture(SecondBackground, firstTextureNum);
			}
			else if(secondMoveTimer > 8)
			{
				SecondBackground->Alpha -= GetDeltaTime() / 2.0f;
				FirstBackground->Alpha += GetDeltaTime() / 2.0f;
				if(!firstAnimated)
				{
					firstMoveTimer = 0;
					firstStartLocation = rand() % 4;
					SetOptionsStartLocation(&FirstBackground->Position.x, &FirstBackground->Position.y, firstStartLocation);
					firstAnimated = TRUE;
				}
			}
			break;
		// Bottom right
		case 1:
			SecondBackground->Position.x -= 640 / (10 / GetDeltaTime());
			SecondBackground->Position.y -= -360 / (10 / GetDeltaTime());
			if(secondMoveTimer >= 10)
			{
				secondAnimated = FALSE;
				SecondBackground->Alpha = 0.0f;
				FirstBackground->Alpha = 1.0f;
				secondTextureNum = RandomNewOptionsTexture(SecondBackground, firstTextureNum);
			}
			else if(secondMoveTimer > 8)
			{
				SecondBackground->Alpha -= GetDeltaTime() / 2.0f;
				FirstBackground->Alpha += GetDeltaTime() / 2.0f;
				if(!firstAnimated)
				{
					firstMoveTimer = 0;
					firstStartLocation = rand() % 4;
					SetOptionsStartLocation(&FirstBackground->Position.x, &FirstBackground->Position.y, firstStartLocation);
					firstAnimated = TRUE;
				}
			}
			break;
		// Top left
		case 2:
			SecondBackground->Position.x -= -640 / (10 / GetDeltaTime());
			SecondBackground->Position.y -= 360 / (10 / GetDeltaTime());
			if(secondMoveTimer >= 10)
			{
				secondAnimated = FALSE;
				SecondBackground->Alpha = 0.0f;
				FirstBackground->Alpha = 1.0f;
				secondTextureNum = RandomNewOptionsTexture(SecondBackground, firstTextureNum);
			}
			else if(secondMoveTimer > 8)
			{
				SecondBackground->Alpha -= GetDeltaTime() / 2.0f;
				FirstBackground->Alpha += GetDeltaTime() / 2.0f;
				if(!firstAnimated)
				{
					firstMoveTimer = 0;
					firstStartLocation = rand() % 4;
					SetOptionsStartLocation(&FirstBackground->Position.x, &FirstBackground->Position.y, firstStartLocation);
					firstAnimated = TRUE;
				}
			}
			break;
		// Bottom left
		case 3:
			SecondBackground->Position.x -= -640 / (10 / GetDeltaTime());
			SecondBackground->Position.y -= -360 / (10 / GetDeltaTime());
			if(secondMoveTimer >= 10)
			{
				secondAnimated = FALSE;
				SecondBackground->Alpha = 0.0f;
				FirstBackground->Alpha = 1.0f;
				secondTextureNum = RandomNewOptionsTexture(SecondBackground, firstTextureNum);
			}
			else if(secondMoveTimer > 8)
			{
				SecondBackground->Alpha -= GetDeltaTime() / 2.0f;
				FirstBackground->Alpha += GetDeltaTime() / 2.0f;
				if(!firstAnimated)
				{
					firstMoveTimer = 0;
					firstStartLocation = rand() % 4;
					SetOptionsStartLocation(&FirstBackground->Position.x, &FirstBackground->Position.y, firstStartLocation);
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
void SetOptionsStartLocation(float *xPos, float *yPos, int startNum)
{
	switch(startNum)
	{
	// Top right
	case 0:
		*xPos = 320.0f;
		*yPos = 180.0f;
		break;
	// Bottom right
	case 1:
		*xPos = 320.0f;
		*yPos = -180.0f;
		break;
	// Top left
	case 2:
		*xPos = -320.0f;
		*yPos = 180.0f;
		break;
	// Bottom left
	case 3:
		*xPos = -320.0f;
		*yPos = -180.0f;
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
int RandomNewOptionsTexture(Sprite* CurrentSprite, int prevTexture)
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
		CurrentSprite->SpriteTexture = LoadTexture("TextureFiles/BlurLevel21.png");
		break;
	case 4:
		CurrentSprite->SpriteTexture = LoadTexture("TextureFiles/BlurLevel22.png");
		break;
	case 5:
		CurrentSprite->SpriteTexture = LoadTexture("TextureFiles/BlurLevel31.png");
		break;
	case 6:
		CurrentSprite->SpriteTexture = LoadTexture("TextureFiles/BlurLevel32.png");
		break;
	case 7:
		CurrentSprite->SpriteTexture = LoadTexture("TextureFiles/BlurLevel41.png");
		break;
	case 8:
		CurrentSprite->SpriteTexture = LoadTexture("TextureFiles/BlurLevel42.png");
		break;
	case 9:
		CurrentSprite->SpriteTexture = LoadTexture("TextureFiles/BlurLevel60.png");
		break;
	case 10:
		CurrentSprite->SpriteTexture = LoadTexture("TextureFiles/BlurLevel61.png");
		break;
	case 11:
		CurrentSprite->SpriteTexture = LoadTexture("TextureFiles/BlurLevel62.png");
		break;
	case 12:
		CurrentSprite->SpriteTexture = LoadTexture("TextureFiles/BlurLevel71.png");
		break;
	case 13:
		CurrentSprite->SpriteTexture = LoadTexture("TextureFiles/BlurLevel72.png");
		break;
	}
	//Returns the texture assigned
	return newTextureNum;
}
