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
#include "../HeaderFiles/MapLevel.h"

Sprite* PauseText;

Sprite* SFXSliderGuide;
Sprite* BGMSliderGuide;

Sprite* SFXSliderBack;
Sprite* BGMSliderBack;

Sprite* EnableCheats;
Sprite* CheckMark;
Button* CheatsButton;

Sprite* EnableLookAt;
Sprite* LookAtCheckMark;
Button* LookAtButton;

Sprite* PauseBackground;

Button* SFXSlider;
Button* BGMSlider;

Button* ResumeButton;
Button* RestartButton;
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
	float camX;
	Vec3 TextColor;
	
	CreatePauseSound(&BackgroundSnd, "Sounds/awesome.mp3", LargeSnd);

	volumestring = (char *)MallocMyAlloc(5, sizeof(char));

	volumestring[0] = '1';
	volumestring[1] = '0';
	volumestring[2] = '0';
	volumestring[3] = (char)'%%';
	volumestring[4] = '\0';

	newID = 10;

	camX = GetCameraXPosition();
	//printf("%f\n", camX);
	pause = TRUE;
	PauseText = (Sprite *) CreateSprite("TextureFiles/Paused.png", 450, 156, 4000, 1, 1, camX, 420);

	//Background
	Vec3Set(&TextColor, 0, 0, 0);
	PauseBackground = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 1499, 1, 1, camX, 0);
	PauseBackground->Alpha = 0.5;
	PauseBackground->Tint = TextColor;

	//Volume sliders
	SFXSliderGuide = (Sprite *) CreateSprite("TextureFiles/VolumeSliderGuide.png", 492, 92, 4001, 1, 1, camX, 200);
	BGMSliderGuide = (Sprite *) CreateSprite("TextureFiles/VolumeSliderGuide.png", 492, 92, 4001, 1, 1, camX, 0);

	SFXSliderBack = (Sprite *) CreateSprite("TextureFiles/VolumeSliderBack.png", 552, 152, 4000, 1, 1, camX, 200);
	BGMSliderBack = (Sprite *) CreateSprite("TextureFiles/VolumeSliderBack.png", 552, 152, 4000, 1, 1, camX, 0);

	SFXSliderPos = SFXSliderGuide->Position.x - SFXSliderGuide->Width / 2 + SFXSliderGuide->Width * SFXVolume;
	BGMSliderPos = BGMSliderGuide->Position.x - BGMSliderGuide->Width / 2 + BGMSliderGuide->Width * BGMVolume;

	SFXSlider = CreateButton("TextureFiles/fox_head.png", SFXSliderPos, 200, 80, 80, newID++);
	SFXSlider->ButtonSprite->ZIndex = 4002;
	SFXSlider->ButtonCollider.width *= 3;
	SFXSlider->ButtonCollider.height = SFXSliderBack->Height;

	BGMSlider = CreateButton("TextureFiles/fox_head.png", BGMSliderPos, 0, 80, 80, newID++);
	BGMSlider->ButtonSprite->ZIndex = 4002;
	BGMSlider->ButtonCollider.width *= 3;
	BGMSlider->ButtonCollider.height = BGMSliderBack->Height;

	Vec3Set(&TextColor, 1, 1, 1);
	
	SFXText = CreateText(volumestring, (SFXSliderBack->Position.x + (SFXSliderBack->Width / 2)) + 50, 200, 100, TextColor, Left, Border);
	volumestring = VolumetoString(volumestring, SFXVolume * 100);
	volumestring = strcat(volumestring, "%");
	ChangeTextString(SFXText, volumestring);
	ChangeTextZIndex(SFXText, 4010);

	BGMText = CreateText(volumestring, (BGMSliderBack->Position.x + (BGMSliderBack->Width / 2)) + 50, 0, 100, TextColor, Left, Border);
	volumestring = VolumetoString(volumestring, BGMVolume * 100);
	volumestring = strcat(volumestring, "%");
	ChangeTextString(BGMText, volumestring);
	ChangeTextZIndex(BGMText, 4010);

	ChangeTextVisibility(SFXText);
	ChangeTextVisibility(BGMText);

	//Cheats check mark
	EnableCheats = (Sprite *) CreateSprite("TextureFiles/EnableCheats.png", 592, 106.4f, 4000, 1, 1, 180 + camX, -150);
	CheatsButton = CreateButton("TextureFiles/CheckBox.png", -250 + camX, -150, 100, 100, newID++);
	CheatsButton->ButtonSprite->ZIndex = 4000;
	UpdateCollider(&CheatsButton->ButtonCollider, 800, CheatsButton->ButtonCollider.height);
	CheatsButton->ButtonCollider.Position.x = camX + 100;
	CheckMark = (Sprite *) CreateSprite("TextureFiles/CheckMark.png", 200, 200, 4001, 1, 1, -250 + camX, -150);

	if(!Cheats)
		CheckMark->Visible = FALSE;

	//Look at mouse check mark
	EnableLookAt = (Sprite *) CreateSprite("TextureFiles/EnableLookAt.png", 592, 106.4f, 4000, 1, 1, 180 + camX, -280);
	LookAtButton = CreateButton("TextureFiles/CheckBox.png", -250 + camX, -280, 100, 100, newID++);
	LookAtButton->ButtonSprite->ZIndex = 4000;
	UpdateCollider(&LookAtButton->ButtonCollider, 800, LookAtButton->ButtonCollider.height);
	LookAtButton->ButtonCollider.Position.x = camX + 100;
	LookAtCheckMark = (Sprite *) CreateSprite("TextureFiles/CheckMark.png", 200, 200, 4001, 1, 1, -250 + camX, -280);

	if(!LookAtMouse)
		LookAtCheckMark->Visible = FALSE;

	LevelToDraw = DrawLevel;

	// Check if we pause the map level or not
	if(GetCurrentState() == GS_MapLevel)
	{
		ResumeButton = CreateButton("TextureFiles/ResumeButton.png", -250 + camX, -400, 300, 112.5f, newID++);
		MainMenuButton = CreateButton("TextureFiles/MainMenuButton.png", 250 + camX, -400, 300, 112.5f, newID++);
	}
	else
	{
		ResumeButton = CreateButton("TextureFiles/ResumeButton.png", -400 + camX, -400, 300, 112.5f, newID++);
		RestartButton = CreateButton("TextureFiles/RestartButton.png", camX, -400, 300, 112.5f, newID++);	
		RestartButton->ButtonSprite->ZIndex = 4002;
		MainMenuButton = CreateButton("TextureFiles/GoToMapButton.png", 400 + camX, -400, 300, 112.5f, newID++);
	}

	ResumeButton->ButtonSprite->ZIndex = 4002;
	MainMenuButton->ButtonSprite->ZIndex = 4002;


	if(CurrentPlayer.PlayerActive)
	{
		CurrentPlayer.PlayerSpriteParts.Body->AnimationActive = 0;
		CurrentPlayer.PlayerSpriteParts.Body->CurrentFrame = 0;
		CurrentPlayer.PlayerSpriteParts.BlinkTimer = 0;
	}

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

		if(GetNextState() == GS_Quit || GetNextState() == GS_Restart)
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
	
	FreeSprite(PauseText);
	FreeSprite(SFXSliderGuide);
	FreeSprite(BGMSliderGuide);
	FreeSprite(SFXSliderBack);
	FreeSprite(BGMSliderBack);
	FreeSprite(PauseBackground);

	FreeSprite(EnableCheats);
	FreeSprite(CheckMark);
	FreeButton(CheatsButton);
	FreeSprite(EnableLookAt);
	FreeSprite(LookAtCheckMark);
	FreeButton(LookAtButton);

	FreeButton(SFXSlider);
	FreeButton(BGMSlider);

	FreeButton(ResumeButton);
	FreeButton(RestartButton);
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

			SFXVolume = (SFXSlider->Position.x + SFXSliderGuide->Width / 2 - GetCameraXPosition()) / SFXSliderGuide->Width;
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
			if(worldX > SFXSliderGuide->Width / 2 + BGMSliderGuide->Position.x)
				BGMSlider->Position.x = BGMSliderGuide->Width / 2 + BGMSliderGuide->Position.x;
			else if(worldX < -BGMSliderGuide->Width / 2 + BGMSliderGuide->Position.x)
				BGMSlider->Position.x = -BGMSliderGuide->Width / 2 + BGMSliderGuide->Position.x;
			else
				BGMSlider->Position.x = (float)worldX;
			BGMSlider->ButtonSprite->Position.x = BGMSlider->Position.x;
			BGMSlider->ButtonCollider.Position.x = BGMSlider->Position.x;

			BGMVolume = (BGMSlider->Position.x + BGMSliderGuide->Width / 2 - GetCameraXPosition()) / BGMSliderGuide->Width;
			if(BGMVolume < 0)
				BGMVolume = 0.0f;
			else if(BGMVolume > 1.0f)
				BGMVolume = 1.0f;
			volumestring = VolumetoString(volumestring, BGMVolume * 100);
			volumestring = strcat(volumestring, "%");
			ChangeTextString(BGMText, volumestring);
		}
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
		if(PointRectCollision(&LookAtButton->ButtonCollider, &MouseClick))
		{
			LookAtMouse = !LookAtMouse;
			LookAtCheckMark->Visible = !(LookAtCheckMark->Visible);
		}
	}

	if(PointRectCollision(&MainMenuButton->ButtonCollider, &MouseClick))
	{
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			if(GetCurrentState() == GS_MapLevel)
				SetNextState(GS_MainMenu);
			else
				SetNextState(GS_MapLevel);
			pause = FALSE;
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

	if(GetCurrentState() != GS_MapLevel && PointRectCollision(&RestartButton->ButtonCollider, &MouseClick))
	{
		RestartButton->ButtonSprite->ScaleX = 1.2f;
		RestartButton->ButtonSprite->ScaleY = 1.2f;

		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			SetNextState(GS_Restart);
			pause = FALSE;
		}
	}
	else if(GetCurrentState() != GS_MapLevel)
	{
		RestartButton->ButtonSprite->ScaleX = 1.0f;
		RestartButton->ButtonSprite->ScaleY = 1.0f;
	}
}
