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
#include "../HeaderFiles/Vector2.h"


// ---------------------------------------------------------------------------

void SetCamera(Vec2 *playerX, int offset, int move_dist)
{

	float camX, camY;	

	//Get camera position
	AEGfxGetCamPosition(&camX, &camY);

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