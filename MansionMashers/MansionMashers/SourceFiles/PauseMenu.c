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
	PauseText = CreateSprite("TextureFiles/Paused.png", 472, 178, 500, 1, 1, 0, 0);
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
			
		//FrameRate Controller
		QueryPerformanceCounter(&CycleStart);			

		AEInputUpdate();

		//FrameRate Controller
		QueryPerformanceCounter(&CycleEnd);				
		QueryPerformanceFrequency(&Frequency);			
		DeltaTime = ((double)(CycleEnd.QuadPart - CycleStart.QuadPart) / (double)Frequency.QuadPart);
		
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

		//Framerate controller
		while(DeltaTime < (1.0/FrameRate))
		{
			QueryPerformanceCounter(&CycleEnd);
			DeltaTime = ((double)(CycleEnd.QuadPart - CycleStart.QuadPart) / (double)Frequency.QuadPart);
		}

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
