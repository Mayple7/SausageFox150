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

Sprite* PauseText;

Sprite* SFXSliderGuide;
Sprite* BGMSliderGuide;

Sprite* SFXSliderBack;
Sprite* BGMSliderBack;

Sprite* PauseBackground;

Button* SFXSlider;
Button* BGMSlider;

TextGlyphs* SFXText;
TextGlyphs* BGMText;

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

	PauseText = (Sprite *) CreateSprite("TextureFiles/Paused.png", 472, 178, 500, 1, 1, 0, 350);

	SFXSliderGuide = (Sprite *) CreateSprite("TextureFiles/VolumeSliderGuide.png", 492, 92, 501, 1, 1, -400, 100);
	BGMSliderGuide = (Sprite *) CreateSprite("TextureFiles/VolumeSliderGuide.png", 492, 92, 501, 1, 1, -400, -100);

	SFXSliderBack = (Sprite *) CreateSprite("TextureFiles/VolumeSliderBack.png", 552, 152, 500, 1, 1, -400, 100);
	BGMSliderBack = (Sprite *) CreateSprite("TextureFiles/VolumeSliderBack.png", 552, 152, 500, 1, 1, -400, -100);

	Vec3Set(&TextColor, 0.3f, 0.3f, 0.3f);
	PauseBackground = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 499, 1, 1, 0, 0);
	PauseBackground->Alpha = 0.5;
	PauseBackground->Tint = TextColor;

	SFXSliderPos = SFXSliderGuide->Position.x - SFXSliderGuide->Width / 2 + SFXSliderGuide->Width * SFXVolume;
	BGMSliderPos = BGMSliderGuide->Position.x - BGMSliderGuide->Width / 2 + BGMSliderGuide->Width * BGMVolume;

	SFXSliderPos /= GetLoadRatio();
	BGMSliderPos /= GetLoadRatio();

	SFXSlider = CreateButton("TextureFiles/fox_head.png", SFXSliderPos, 100, 80, 80, newID++);
	SFXSlider->ButtonSprite->ZIndex = 502;
	SFXSlider->ButtonCollider.width *= 3;
	SFXSlider->ButtonCollider.height = SFXSliderBack->Height;

	BGMSlider = CreateButton("TextureFiles/fox_head.png", BGMSliderPos, -100, 80, 80, newID++);
	BGMSlider->ButtonSprite->ZIndex = 502;
	BGMSlider->ButtonCollider.width *= 3;
	BGMSlider->ButtonCollider.height = BGMSliderBack->Height;

	Vec3Set(&TextColor, 1, 1, 1);
	
	SFXText = CreateText(volumestring, SFXSliderBack->Position.x + (SFXSliderBack->Width / 2) / GetLoadRatio() + 50 * GetLoadRatio(), 100, 100, TextColor, Left);
	volumestring = VolumetoString(volumestring, SFXVolume * 100);
	volumestring = strcat(volumestring, "%");
	ChangeTextString(SFXText, volumestring);
	ChangeTextZIndex(SFXText, 510);

	BGMText = CreateText(volumestring, BGMSliderBack->Position.x + (BGMSliderBack->Width / 2) / GetLoadRatio() + 50 * GetLoadRatio(), -100, 100, TextColor, Left);
	volumestring = VolumetoString(volumestring, BGMVolume * 100);
	volumestring = strcat(volumestring, "%");
	ChangeTextString(BGMText, volumestring);
	ChangeTextZIndex(BGMText, 510);

	TextAllVisible(SFXText);
	TextAllVisible(BGMText);
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
	SaveVolume();
	FreeAllLists();
}

void UnloadOptions(void)
{
	//Destroy the textures
	DestroyTextureList();
}

void EventOptions(void)
{
	if(FoxInput_MouseDown(MOUSE_BUTTON_LEFT))
	{
		int i, worldX, worldY;
		Vec2 MouseClick;

		FoxInput_GetWorldPosition(&worldX, &worldY);
		Vec2Set(&MouseClick, (float)worldX, (float)worldY);

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
		}
		SFXVolume = (SFXSlider->Position.x + 400 * GetLoadRatio() + SFXSliderGuide->Width / 2) / SFXSliderGuide->Width;
		volumestring = VolumetoString(volumestring, SFXVolume * 100);
		volumestring = strcat(volumestring, "%");
		ChangeTextString(SFXText, volumestring);

		BGMVolume = (BGMSlider->Position.x + 400 * GetLoadRatio() + BGMSliderGuide->Width / 2) / BGMSliderGuide->Width;
		volumestring = VolumetoString(volumestring, BGMVolume * 100);
		volumestring = strcat(volumestring, "%");
		ChangeTextString(BGMText, volumestring);
	}
	if(FoxInput_MouseUp(MOUSE_BUTTON_LEFT))
	{
		SetChannelGroupVolume(EffectType, SFXVolume);
		SetChannelGroupVolume(MusicType, BGMVolume);
	}

}
