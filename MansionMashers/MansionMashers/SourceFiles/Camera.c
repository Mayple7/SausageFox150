/*
File:				Camera.c
Author:				Juli Gregg (j.gregg)
Creation Date:		Jan 11, 2014

Purpose:			Moves Camera based on given object position

Functions:			SF_SetCamera (SausageFox Camera)
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 

// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Camera.h"
#include "../HeaderFiles/Sprite.h"


// ---------------------------------------------------------------------------

void SetCamera(Vec2 *playerX, int offset, int move_dist, struct HUDLayer* hudmain)
{
	float camX, camY;	
	int i;
	//Get camera position
	AEGfxGetCamPosition(&camX, &camY);

	for (i = 0; hudmain->HudItem[i]; i++)
	{
		if (i < 4)
		{
			hudmain->HudItem[i]->Position.x = camX - 504 + (i*330);
			hudmain->HudItem[i]->Position.y = camY + 250;
			DrawSprite(hudmain->HudItem[i]);
		}
		else
		{
			hudmain->HudItem[i]->Position.x = camX - 504 - 112 + ((i-4)*330);
			hudmain->HudItem[i]->Position.y = camY + 250 - 37;
			DrawSprite(hudmain->HudItem[i]);
		}
	}

	//Moves camera Right
	if(playerX->x > (camX + offset))
		AEGfxSetCamPosition(camX + move_dist, camY);
	
	//Moves camera Left
	else if(playerX->x < (camX - offset))
		AEGfxSetCamPosition(camX - move_dist, camY);


}

void ResetCamera(void)
{
	AEGfxSetCamPosition(0.0, 0.0);
}