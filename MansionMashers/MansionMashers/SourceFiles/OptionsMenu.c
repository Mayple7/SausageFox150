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

Sprite* PauseBackground;

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

void InitializeOptions(void)
{
	Vec3 TextColor;

	ResetObjectList();

	volumestring = (char *)MallocMyAlloc(5, sizeof(char));

	volumestring[0] = '1';
	volumestring[1] = '0';
	volumestring[2] = '0';
	volumestring[3] = (char)'%%';
	volumestring[4] = '\0';

	newID = 10;

	OptionsTitle = (Sprite *) CreateSprite("TextureFiles/OptionsTitle.png", 423, 179, 10, 1, 1, 0, 380);
	EnableCheats = (Sprite *) CreateSprite("TextureFiles/EnableCheats.png", 592, 106.4f, 10, 1, 1, -380, -200);

	SFXSliderGuide = (Sprite *) CreateSprite("TextureFiles/VolumeSliderGuide.png", 492, 92, 501, 1, 1, -480, 200);
	BGMSliderGuide = (Sprite *) CreateSprite("TextureFiles/VolumeSliderGuide.png", 492, 92, 501, 1, 1, -480, 0);

	SFXSliderBack = (Sprite *) CreateSprite("TextureFiles/VolumeSliderBack.png", 552, 152, 500, 1, 1, -480, 200);
	BGMSliderBack = (Sprite *) CreateSprite("TextureFiles/VolumeSliderBack.png", 552, 152, 500, 1, 1, -480, 0);

	Vec3Set(&TextColor, 0.3f, 0.3f, 0.3f);
	PauseBackground = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 1, 1, 1, 0, 0);
	PauseBackground->Alpha = 0.5;
	PauseBackground->Tint = TextColor;

	SFXSliderPos = SFXSliderGuide->Position.x - SFXSliderGuide->Width / 2 + SFXSliderGuide->Width * SFXVolume;
	BGMSliderPos = BGMSliderGuide->Position.x - BGMSliderGuide->Width / 2 + BGMSliderGuide->Width * BGMVolume;

	SFXSliderPos /= GetLoadRatio();
	BGMSliderPos /= GetLoadRatio();

	SFXSlider = CreateButton("TextureFiles/fox_head.png", SFXSliderPos, 200, 80, 80, newID++);
	SFXSlider->ButtonSprite->ZIndex = 502;
	SFXSlider->ButtonCollider.width *= 3;
	SFXSlider->ButtonCollider.height = SFXSliderBack->Height;

	BGMSlider = CreateButton("TextureFiles/fox_head.png", BGMSliderPos, 0, 80, 80, newID++);
	BGMSlider->ButtonSprite->ZIndex = 502;
	BGMSlider->ButtonCollider.width *= 3;
	BGMSlider->ButtonCollider.height = BGMSliderBack->Height;

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

	TextAllVisible(SFXText);
	TextAllVisible(BGMText);
	
	TextAllVisible(SFXLabel);
	TextAllVisible(BGMLabel);

	BackButton = CreateButton("TextureFiles/BackButton.png", 0, -400, 400, 150, newID++);
	CheatsButton = CreateButton("TextureFiles/CheckBox.png", -800, -200, 100, 100, newID++);
	CheckMark = (Sprite *) CreateSprite("TextureFiles/CheckMark.png", 200, 200, 11, 1, 1, -800, -200);

	if(!Cheats)
		CheckMark->Visible = FALSE;

	CreateBoundingBoxes();
}

void UpdateOptions(void)
{
	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		SetNextState(GS_MainMenu);
	}

	EventOptions();
}

void DrawOptions(void)
{
	DrawObjectList();
	DrawCollisionList();
}

void FreeOptions(void)
{
	SaveSettings();
	FreeAllLists();
}

void UnloadOptions(void)
{
	//Destroy the textures
	DestroyTextureList();
}

void EventOptions(void)
{
	int i, worldX, worldY;
	Vec2 MouseClick;

	FoxInput_GetWorldPosition(&worldX, &worldY);
	Vec2Set(&MouseClick, (float)worldX, (float)worldY);

	if(FoxInput_MouseDown(MOUSE_BUTTON_LEFT))
	{
		for(i = 0; i < BUTTONAMOUNT; i++)
		{
			if(!buttonList[i].objID)
				continue;
			else if(buttonList[i].objID == SFXSlider->objID && PointRectCollision(&buttonList[i].ButtonCollider, &MouseClick))
			{
				if(worldX > SFXSliderGuide->Width / 2 + SFXSliderGuide->Position.x)
					SFXSlider->Position.x = SFXSliderGuide->Width / 2 + SFXSliderGuide->Position.x;
				else if(worldX < -SFXSliderGuide->Width / 2 + SFXSliderGuide->Position.x)
					SFXSlider->Position.x = -SFXSliderGuide->Width / 2 + SFXSliderGuide->Position.x;
				else
					SFXSlider->Position.x = (float)worldX;
				SFXSlider->ButtonSprite->Position.x = SFXSlider->Position.x;
				SFXSlider->ButtonCollider.Position.x = SFXSlider->Position.x;
			}
			else if(buttonList[i].objID == BGMSlider->objID && PointRectCollision(&buttonList[i].ButtonCollider, &MouseClick))
			{
				if(worldX > SFXSliderGuide->Width / 2 + BGMSliderGuide->Position.x)
					BGMSlider->Position.x = BGMSliderGuide->Width / 2 + BGMSliderGuide->Position.x;
				else if(worldX < -BGMSliderGuide->Width / 2 + BGMSliderGuide->Position.x)
					BGMSlider->Position.x = -BGMSliderGuide->Width / 2 + BGMSliderGuide->Position.x;
				else
					BGMSlider->Position.x = (float)worldX;
				BGMSlider->ButtonSprite->Position.x = BGMSlider->Position.x;
				BGMSlider->ButtonCollider.Position.x = BGMSlider->Position.x;
			}
			else if(buttonList[i].objID == BackButton->objID && PointRectCollision(&buttonList[i].ButtonCollider, &MouseClick))
			{
				SetNextState(GS_MainMenu);
			}
		}
		SFXVolume = (SFXSlider->Position.x + 480 * GetLoadRatio() + SFXSliderGuide->Width / 2) / SFXSliderGuide->Width;
		volumestring = VolumetoString(volumestring, SFXVolume * 100);
		volumestring = strcat(volumestring, "%");
		ChangeTextString(SFXText, volumestring);

		BGMVolume = (BGMSlider->Position.x + 480 * GetLoadRatio() + BGMSliderGuide->Width / 2) / BGMSliderGuide->Width;
		volumestring = VolumetoString(volumestring, BGMVolume * 100);
		volumestring = strcat(volumestring, "%");
		ChangeTextString(BGMText, volumestring);
	}

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

	if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		if(PointRectCollision(&CheatsButton->ButtonCollider, &MouseClick))
		{
			Cheats = !Cheats;
			CheckMark->Visible = !(CheckMark->Visible);
		}
	}

	if(FoxInput_MouseUp(MOUSE_BUTTON_LEFT))
	{
		SetChannelGroupVolume(EffectType, SFXVolume);
		SetChannelGroupVolume(MusicType, BGMVolume);
	}

}
