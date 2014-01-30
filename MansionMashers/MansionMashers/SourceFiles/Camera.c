/*
File:				Camera.c
Author:				Juli Gregg (j.gregg)
Creation Date:		Jan 11, 2014

Purpose:			Moves Camera based on given object position

Functions:			SetCamera (SausageFox Camera)
 
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

void SetCamera(Vec2 *playerX, int offset, struct HUDLayer* hudmain)
{
	float camX, camY;	

	//Get camera position
	AEGfxGetCamPosition(&camX, &camY);

	//Update/Draw the HUD
	DrawHUD(hudmain, camX, camY);

	//Moves camera Right
	if(playerX->x > (camX + offset))
		AEGfxSetCamPosition(playerX->x - offset, camY);
	
	//Moves camera Left
	else if(playerX->x < (camX - offset))
		AEGfxSetCamPosition(playerX->x + offset, camY);


}

void ResetCamera(void)
{
	AEGfxSetCamPosition(0.0, 0.0);
}

void DrawHUD(struct HUDLayer *hud, float camx, float camy)
{
	int i;
	for (i = 0; hud->HudItem[i]; i++)
	{
		if (i % 2 == 0)
		{
			hud->HudItem[i]->Position.x = camx - 480 + ((i/2)*320);
			hud->HudItem[i]->Position.y = camy + 275;
			DrawSprite(hud->HudItem[i]);
		}
		else
		{
			hud->HudItem[i]->Position.x = camx - 480 - 70 + (((i-1)/2)*320);
			hud->HudItem[i]->Position.y = camy + 275 - 23;
			DrawSprite(hud->HudItem[i]);
		}
	}
}