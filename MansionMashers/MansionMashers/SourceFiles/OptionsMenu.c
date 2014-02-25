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
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/BoundingBox.h"

Sprite* OptionsTitle;
Sprite* EnableCheats;
Sprite* CheckMark;

Sprite* SFXSliderGuide;
Sprite* BGMSliderGuide;

Sprite* SFXSliderBack;
Sprite* BGMSliderBack;

Sprite* OptionsBackground;

Button* SFXSlider;
Button* BGMSlider;

Button* BackButton;
Button* CheatsButton;

TextGlyphs* SFXText;
TextGlyphs* BGMText;

TextGlyphs* SFXLabel;
TextGlyphs* BGMLabel;

float SFXSliderPos, BGMSliderPos;
static char* volumestring;

static int newID;

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
	newID = 10;

	ResetObjectList();

	//Initializes the initial volume string
	volumestring = (char *)MallocMyAlloc(5, sizeof(char));
	volumestring[0] = '1';
	volumestring[1] = '0';
	volumestring[2] = '0';
	volumestring[3] = (char)'%%';
	volumestring[4] = '\0';

	//Fake text initializations
	OptionsTitle = (Sprite *) CreateSprite("TextureFiles/OptionsTitle.png", 423, 179, 10, 1, 1, 0, 380);
	EnableCheats = (Sprite *) CreateSprite("TextureFiles/EnableCheats.png", 592, 106.4f, 10, 1, 1, -380, -200);

	//Create the volume bars
	SFXSliderGuide = (Sprite *) CreateSprite("TextureFiles/VolumeSliderGuide.png", 492, 92, 501, 1, 1, -480, 200);
	BGMSliderGuide = (Sprite *) CreateSprite("TextureFiles/VolumeSliderGuide.png", 492, 92, 501, 1, 1, -480, 0);

	SFXSliderBack = (Sprite *) CreateSprite("TextureFiles/VolumeSliderBack.png", 552, 152, 500, 1, 1, -480, 200);
	BGMSliderBack = (Sprite *) CreateSprite("TextureFiles/VolumeSliderBack.png", 552, 152, 500, 1, 1, -480, 0);

	//Background for the options
	Vec3Set(&TextColor, 0.3f, 0.3f, 0.3f);
	OptionsBackground = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 1, 1, 1, 0, 0);
	OptionsBackground->Alpha = 0.5;
	OptionsBackground->Tint = TextColor;

	// Find default slider position based on sound volumes
	SFXSliderPos = SFXSliderGuide->Position.x - SFXSliderGuide->Width / 2 + SFXSliderGuide->Width * SFXVolume;
	BGMSliderPos = BGMSliderGuide->Position.x - BGMSliderGuide->Width / 2 + BGMSliderGuide->Width * BGMVolume;
	SFXSliderPos /= GetLoadRatio();
	BGMSliderPos /= GetLoadRatio();

	//Create the slider buttons
	SFXSlider = CreateButton("TextureFiles/fox_head.png", SFXSliderPos, 200, 80, 80, newID++);
	SFXSlider->ButtonSprite->ZIndex = 502;
	SFXSlider->ButtonCollider.width *= 3;
	SFXSlider->ButtonCollider.height = SFXSliderBack->Height;

	BGMSlider = CreateButton("TextureFiles/fox_head.png", BGMSliderPos, 0, 80, 80, newID++);
	BGMSlider->ButtonSprite->ZIndex = 502;
	BGMSlider->ButtonCollider.width *= 3;
	BGMSlider->ButtonCollider.height = BGMSliderBack->Height;

	//Create the volume text
	Vec3Set(&TextColor, 1, 1, 1);
	
	SFXText = CreateText(volumestring, SFXSliderBack->Position.x + (SFXSliderBack->Width / 2) * GetLoadRatio() + 50 * GetLoadRatio(), 200, 100, TextColor, Left);
	volumestring = VolumetoString(volumestring, SFXVolume * 100);
	volumestring = strcat(volumestring, "%");
	ChangeTextString(SFXText, volumestring);
	ChangeTextZIndex(SFXText, 510);

	BGMText = CreateText(volumestring, BGMSliderBack->Position.x + (BGMSliderBack->Width / 2) * GetLoadRatio() + 50 * GetLoadRatio(), 0, 100, TextColor, Left);
	volumestring = VolumetoString(volumestring, BGMVolume * 100);
	volumestring = strcat(volumestring, "%");
	ChangeTextString(BGMText, volumestring);

	SFXLabel = CreateText("SFX", SFXSliderBack->Position.x - SFXSliderBack->Width - 140 * GetLoadRatio(), 200, 100, TextColor, Right);
	BGMLabel = CreateText("BGM", BGMSliderBack->Position.x - BGMSliderBack->Width - 140 * GetLoadRatio(), 0, 100, TextColor, Right);

	//Set all text to be visible
	TextAllVisible(SFXText);
	TextAllVisible(BGMText);
	
	TextAllVisible(SFXLabel);
	TextAllVisible(BGMLabel);

	// Create the back button and cheats objects
	BackButton = CreateButton("TextureFiles/BackButton.png", 0, -400, 400, 150, newID++);
	CheatsButton = CreateButton("TextureFiles/CheckBox.png", -800, -200, 100, 100, newID++);
	CheckMark = (Sprite *) CreateSprite("TextureFiles/CheckMark.png", 200, 200, 11, 1, 1, -800, -200);

	if(!Cheats)
		CheckMark->Visible = FALSE;

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
		}
		
		//Adjust the sounds based on the slider position
		SFXVolume = (SFXSlider->Position.x + 480 * GetLoadRatio() + SFXSliderGuide->Width / 2) / SFXSliderGuide->Width;
		volumestring = VolumetoString(volumestring, SFXVolume * 100);
		volumestring = strcat(volumestring, "%");
		ChangeTextString(SFXText, volumestring);

		BGMVolume = (BGMSlider->Position.x + 480 * GetLoadRatio() + BGMSliderGuide->Width / 2) / BGMSliderGuide->Width;
		volumestring = VolumetoString(volumestring, BGMVolume * 100);
		volumestring = strcat(volumestring, "%");
		ChangeTextString(BGMText, volumestring);
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
