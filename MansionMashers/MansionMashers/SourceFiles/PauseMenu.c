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

Button* SFXSlider;
Button* BGMSlider;

void (*LevelToDraw)();

int pause;
int newID;

void InitializePause(void (*DrawLevel)())
{
	float camX, camY;
	newID = 1;

	AEGfxGetCamPosition(&camX, &camY);
	pause = TRUE;
	PauseText = CreateSprite("TextureFiles/Paused.png", 472, 178, 500, 1, 1, camX, 350);

	SFXSliderGuide = CreateSprite("TextureFiles/VolumeSliderGuide.png", 492, 92, 501, 1, 1, camX, 100);
	BGMSliderGuide = CreateSprite("TextureFiles/VolumeSliderGuide.png", 492, 92, 501, 1, 1, camX, -100);

	SFXSliderBack = CreateSprite("TextureFiles/VolumeSliderBack.png", 552, 152, 500, 1, 1, camX, 100);
	BGMSliderBack = CreateSprite("TextureFiles/VolumeSliderBack.png", 552, 152, 500, 1, 1, camX, -100);

	SFXSlider = CreateButton("TextureFiles/fox_head.png", camX, 100, 80, 80, newID++);
	BGMSlider = CreateButton("TextureFiles/fox_head.png", camX, -100, 80, 80, newID++);
	SFXSlider->ButtonSprite->ZIndex = 502;
	BGMSlider->ButtonSprite->ZIndex = 502;

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

		if(FoxInput_KeyTriggered(VK_ESCAPE))
		{
			pause = FALSE;
		}
		if(FoxInput_KeyTriggered(VK_SHIFT))
		{
			pause = FALSE;
			SetNextState(GS_MainMenu);
		}
		DrawPause();
		FoxInput_Update();
		EndFoxFrame();
		AESysFrameEnd();
	}
	FreePause();

}

void DrawPause(void)
{
	LevelToDraw();
	DrawSprite(PauseText);
}

void FreePause(void)
{
	freeObject(PauseText);
	freeObject(SFXSliderGuide);
	freeObject(BGMSliderGuide);
	freeObject(SFXSliderBack);
	freeObject(BGMSliderBack);
	FreeButton(SFXSlider);
	FreeButton(BGMSlider);
}
