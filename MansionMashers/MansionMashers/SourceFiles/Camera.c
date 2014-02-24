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

void SetCamera(Vec2 *playerX, int offset)
{
	float camX, camY;	

	//Get camera position
	AEGfxGetCamPosition(&camX, &camY);

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

float GetCameraXPosition()
{
	float camX, camY;
	AEGfxGetCamPosition(&camX, &camY);
	return camX;
}

void SetCameraXPosition(float newX)
{
	float camX, camY;
	AEGfxGetCamPosition(&camX, &camY);
	AEGfxSetCamPosition(newX, camY);
}


void SetCameraPan(float newX, float PanelSize)
{
	float camX, camY;	

	//Get camera position
	AEGfxGetCamPosition(&camX, &camY);

	//Pans camera Right
	if(camX < newX - ((PanelSize / 128) * GetLoadRatio()))
	{
		camX += ((PanelSize / 3) * GetLoadRatio()) * GetDeltaTime();
		AEGfxSetCamPosition(camX, camY);
	}
	//Pans Camera Left
	else if (camX > newX + ((PanelSize / 128) * GetLoadRatio()))
	{
		camX -= ((PanelSize / 3) * GetLoadRatio()) * GetDeltaTime();
		AEGfxSetCamPosition(camX, camY);
	}
	else
		//Keep Camera Where it is
		AEGfxSetCamPosition(newX, camY);
}

void DrawHUD(struct HUDLayer *hud)
{
	int i;
	float camX, camY;	

	//Get camera position
	AEGfxGetCamPosition(&camX, &camY);

	for (i = 0; hud->HudItem[i]; i++)
	{
		if (i % 2 == 0)
		{
			hud->HudItem[i]->Position.x = camX - (725 - ((i/2)*480)) * GetLoadRatio();
			hud->HudItem[i]->Position.y = camY + (350 * GetLoadRatio());
			DrawSprite(hud->HudItem[i]);
		}
		else
		{
			hud->HudItem[i]->Position.x = camX - (725 - (((i-1)/2)*480)) * GetLoadRatio() - (98 * GetLoadRatio());
			hud->HudItem[i]->Position.y = camY + (350 - 34) * GetLoadRatio();
			DrawSprite(hud->HudItem[i]);
		}
	}
}
