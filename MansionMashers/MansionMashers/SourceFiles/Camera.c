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

static int GateCamera;
static int CameraMoved;

// ---------------------------------------------------------------------------

void SetCamera(Vec2 *playerX, int offset)
{
	float camX, camY;	

	//Get camera position
	AEGfxGetCamPosition(&camX, &camY);

	if(GateCamera == TRUE)
		return;
	else
	{
		//Moves camera Right
		if(playerX->x > (camX + offset))
			AEGfxSetCamPosition(playerX->x - offset, camY);
	
		//Moves camera Left
		else if(playerX->x < (camX - offset))
			AEGfxSetCamPosition(playerX->x + offset, camY);
	}
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

	CameraMoved = FALSE;

	if(camX >= newX - ((PanelSize / 128) * GetLoadRatio()) && camX <= newX + ((PanelSize / 128) * GetLoadRatio()))
		CameraMoved = TRUE;

	if(GateCamera == TRUE)
		return;
	else
	{
		//Pans camera Right
		if(camX < newX - ((PanelSize / 128) * GetLoadRatio()))
		{
			camX += ((PanelSize / 1.25f) * GetLoadRatio()) * GetDeltaTime();
			AEGfxSetCamPosition(camX, camY);
		}
		//Pans Camera Left
		else if (camX > newX + ((PanelSize / 128) * GetLoadRatio()))
		{
			camX -= ((PanelSize / 1.25f) * GetLoadRatio()) * GetDeltaTime();
			AEGfxSetCamPosition(camX, camY);
		}
		else
		{
			//Keep Camera Where it is
			AEGfxSetCamPosition(newX, camY);
		}
	}
}

void SetCameraLockState(int GateSet)
{
	//True or False
	GateCamera = GateSet;
}

void ResetGatedCamera(void)
{
	GateCamera = FALSE;
	CameraMoved = FALSE;
}

int GetCameraLockState(void)
{
	return GateCamera;
}

void SetCameraMovedState(int GateSet)
{
	//True or False
	CameraMoved = GateSet;
}

int GetCameraMovedState(void)
{
	return CameraMoved;
}
