/*****************************************************************************/
/*!
\file				PauseMenu.c
\author				Dan Muller (d.muller)
\date				Feb 2, 2014

\brief				Pause menu functions

\par				Functions:
\li					InitializePause
\li					UpdatePause
\li					DrawPause
\li					FreePause
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

#include "../HeaderFiles/UIButton.h"
#include "../HeaderFiles/PauseMenu.h"
#include "../HeaderFiles/FoxEngine.h"

Sprite* PauseText;

Sprite* SFXSliderGuide;
Sprite* BGMSliderGuide;

Sprite* SFXSliderBack;
Sprite* BGMSliderBack;

Sprite* EnableCheats;
Sprite* CheckMark;
Button* CheatsButton;

Sprite* PauseBackground;

Button* SFXSlider;
Button* BGMSlider;

Button* ResumeButton;
Button* MainMenuButton;

TextGlyphs* SFXText;
TextGlyphs* BGMText;

FoxSound BackgroundSnd;

void (*LevelToDraw)();

float SFXSliderPos, BGMSliderPos;
static char* volumestring;

static int pause;
static int newID;

void InitializePause(void (*DrawLevel)())
{
	float camX, camY;
	Vec3 TextColor;

	CreatePauseSound(&BackgroundSnd, "Sounds/awesome.mp3", LargeSnd);

	volumestring = (char *)MallocMyAlloc(5, sizeof(char));

	volumestring[0] = '1';
	volumestring[1] = '0';
	volumestring[2] = '0';
	volumestring[3] = (char)'%%';
	volumestring[4] = '\0';

	newID = 1;

	AEGfxGetCamPosition(&camX, &camY);
	pause = TRUE;
	PauseText = (Sprite *) CreateSprite("TextureFiles/Paused.png", 472, 178, 500, 1, 1, camX, 350);

	SFXSliderGuide = (Sprite *) CreateSprite("TextureFiles/VolumeSliderGuide.png", 492, 92, 501, 1, 1, camX, 100);
	BGMSliderGuide = (Sprite *) CreateSprite("TextureFiles/VolumeSliderGuide.png", 492, 92, 501, 1, 1, camX, -100);

	SFXSliderBack = (Sprite *) CreateSprite("TextureFiles/VolumeSliderBack.png", 552, 152, 500, 1, 1, camX, 100);
	BGMSliderBack = (Sprite *) CreateSprite("TextureFiles/VolumeSliderBack.png", 552, 152, 500, 1, 1, camX, -100);

	Vec3Set(&TextColor, 0, 0, 0);
	PauseBackground = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 499, 1, 1, camX, 0);
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

	ChangeTextVisibility(SFXText);
	ChangeTextVisibility(BGMText);

	EnableCheats = (Sprite *) CreateSprite("TextureFiles/EnableCheats.png", 592, 106.4f, 500, 1, 1, 180, -250);
	CheatsButton = CreateButton("TextureFiles/CheckBox.png", -250, -250, 100, 100, newID++);
	CheatsButton->ButtonSprite->ZIndex = 500;
	CheckMark = (Sprite *) CreateSprite("TextureFiles/CheckMark.png", 200, 200, 501, 1, 1, -250, -250);

	if(!Cheats)
		CheckMark->Visible = FALSE;

	ResumeButton = CreateButton("TextureFiles/ResumeButton.png", -250, -400, 300, 112.5f, newID++);
	MainMenuButton = CreateButton("TextureFiles/MainMenuButton.png", 250, -400, 300, 112.5f, newID++);
	ResumeButton->ButtonSprite->ZIndex = 502;
	MainMenuButton->ButtonSprite->ZIndex = 502;

	LevelToDraw = DrawLevel;
	CurrentPlayer.PlayerSpriteParts.Body->AnimationActive = 0;
	CurrentPlayer.PlayerSpriteParts.Body->CurrentFrame = 0;
	CurrentPlayer.PlayerSpriteParts.BlinkTimer = 0;

	FoxInput_Update();
}

void UpdatePause(void)
{
	while(pause)
	{
		AESysFrameStart();
		StartFoxFrame();
		PlayAudio(&BackgroundSnd);

		if(FoxInput_KeyTriggered(VK_ESCAPE))
		{
			pause = FALSE;
		}
		if(FoxInput_KeyTriggered(VK_SHIFT))
		{
			pause = FALSE;
			SetNextState(GS_MainMenu);
		}
		if(GetNextState() == GS_Quit)
		{
			pause = FALSE;
		}

		DrawPause();

		EventPause();
		
		FoxInput_Update();
		SaveSettings();
		SetChannelGroupVolume(EffectType, SFXVolume);
		SetChannelGroupVolume(MusicType, BGMVolume);
		UpdateSoundSystem();

		EndFoxFrame();
		AESysFrameEnd();
	}
	TogglePauseSound(&BackgroundSnd);
	SaveSettings();
	FreePause();
}

void DrawPause(void)
{
	LevelToDraw();
	DrawSprite(PauseText);
	DrawCollisionList();
}

void FreePause(void)
{
	
	freeObject(PauseText);
	freeObject(SFXSliderGuide);
	freeObject(BGMSliderGuide);
	freeObject(SFXSliderBack);
	freeObject(BGMSliderBack);
	freeObject(PauseBackground);
	freeObject(EnableCheats);
	freeObject(CheckMark);
	FreeButton(CheatsButton);
	FreeButton(SFXSlider);
	FreeButton(BGMSlider);
	FreeButton(ResumeButton);
	FreeButton(MainMenuButton);
	FreeText(SFXText);
	FreeText(BGMText);
	FreeMyAlloc(volumestring);
	//ReleaseSound(BackgroundSnd.Sound);
}

void EventPause(void)
{
	int worldX, worldY;
	Vec2 MouseClick;

	FoxInput_GetWorldPosition(&worldX, &worldY);
	Vec2Set(&MouseClick, (float)worldX, (float)worldY);

	if(FoxInput_MouseDown(MOUSE_BUTTON_LEFT))
	{
		if(PointRectCollision(&SFXSlider->ButtonCollider, &MouseClick))
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
		else if(PointRectCollision(&BGMSlider->ButtonCollider, &MouseClick))
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
		
		SFXVolume = (SFXSlider->Position.x + SFXSliderGuide->Width / 2) / SFXSliderGuide->Width;
		volumestring = VolumetoString(volumestring, SFXVolume * 100);
		volumestring = strcat(volumestring, "%");
		ChangeTextString(SFXText, volumestring);

		BGMVolume = (BGMSlider->Position.x + BGMSliderGuide->Width / 2) / BGMSliderGuide->Width;
		volumestring = VolumetoString(volumestring, BGMVolume * 100);
		volumestring = strcat(volumestring, "%");
		ChangeTextString(BGMText, volumestring);
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
	}

	if(PointRectCollision(&MainMenuButton->ButtonCollider, &MouseClick))
	{
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			pause = FALSE;
			SetNextState(GS_MainMenu);
		}
		MainMenuButton->ButtonSprite->ScaleX = 1.2f;
		MainMenuButton->ButtonSprite->ScaleY = 1.2f;
	}
	else
	{
		MainMenuButton->ButtonSprite->ScaleX = 1.0f;
		MainMenuButton->ButtonSprite->ScaleY = 1.0f;
	}

	if(PointRectCollision(&ResumeButton->ButtonCollider, &MouseClick))
	{
		ResumeButton->ButtonSprite->ScaleX = 1.2f;
		ResumeButton->ButtonSprite->ScaleY = 1.2f;

		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
			pause = FALSE;
	}
	else
	{
		ResumeButton->ButtonSprite->ScaleX = 1.0f;
		ResumeButton->ButtonSprite->ScaleY = 1.0f;
	}
}
