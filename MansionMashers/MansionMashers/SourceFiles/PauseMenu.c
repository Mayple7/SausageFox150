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

#include "../HeaderFiles/PauseMenu.h"
#include "../HeaderFiles/FoxEngine.h"

Sprite* PauseText;

void (*LevelToDraw)();

int pause;

void InitializePause(void (*DrawLevel)())
{
	pause = TRUE;
	PauseText = CreateSprite("TextureFiles/Paused.png", 472, 178, 500, 1, 1);
	LevelToDraw = DrawLevel;
}

void UpdatePause(void)
{
	float camX, camY;
	LARGE_INTEGER CycleStart, CycleEnd, Frequency; 		//for framerate controller
	double DeltaTime = 0;
	int FrameRate = 60;									//Make a define in the future

	AEGfxGetCamPosition(&camX, &camY);
	PauseText->Position.x = camX;
	PauseText->Position.y = camY;

	while(pause)
	{
		AESysFrameStart();
		StartFoxFrame();		

		AEInputUpdate();

		if(AEInputCheckTriggered(VK_ESCAPE))
		{
			pause = FALSE;
		}
		if(AEInputCheckTriggered(VK_RSHIFT))
		{
			pause = FALSE;
			SetNextState(GS_MainMenu);
		}

		DrawPause();

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
}
