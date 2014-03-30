/*
File:				Camera.c
Author:				Juli Gregg (j.gregg)
Creation Date:		Jan 11, 2014

Purpose:			Moves Camera based on given object position

Functions:			SetCamera (SausageFox Camera)
					ResetCamera
					GetCameraXPosition
					SetCameraXPosition
					SetCameraPan
					SetCameraLockState
					GetCameraLockState
					ResetGatedCamera
					SetCameraMovedState
					GetCameraMovedState
					SetUpCameraPanAndLock
					SetUpCameraPanAndLockNoSpawner

 
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

/*************************************************************************/
/*!
	\brief
	Sets camera to follow the player

	\param playerX
	Player's Position vector

	\param offset
	How far from center you want camera to start moving with player
*/
/*************************************************************************/
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
		if(playerX->x > (camX / GetLoadRatio() + offset))
			AEGfxSetCamPosition((playerX->x - offset) * GetLoadRatio(), camY);
	
		//Moves camera Left
		else if(playerX->x < (camX / GetLoadRatio() - offset))
			AEGfxSetCamPosition((playerX->x + offset) * GetLoadRatio(), camY);
	}
}

/*************************************************************************/
/*!
	\brief
	Set camera's position to (0,0)
*/
/*************************************************************************/
void ResetCamera(void)
{
	AEGfxSetCamPosition(0.0, 0.0);
}

/*************************************************************************/
/*!
	\brief
	Provides the cameras position

	\return
	Returns the camera's position;
*/
/*************************************************************************/
float GetCameraXPosition(void)
{
	float camX, camY;
	AEGfxGetCamPosition(&camX, &camY);
	return camX / GetLoadRatio();
}

/*************************************************************************/
/*!
	\brief
	Sets the camera's X position

	\param newX
	Position to set camera to
*/
/*************************************************************************/
void SetCameraXPosition(float newX)
{
	float camX, camY;
	AEGfxGetCamPosition(&camX, &camY);
	AEGfxSetCamPosition(newX * GetLoadRatio(), camY);
}

/*************************************************************************/
/*!
	\brief
	Has camera pan left and right based on panels

	\param newX
	Final position you want camera to move to

	\param PanelSize
	the size of a panel in the level
*/
/*************************************************************************/
void SetCameraPan(float newX, float PanelSize)
{
	float camX, camY;

	//Get camera position
	AEGfxGetCamPosition(&camX, &camY);

	CameraMoved = FALSE;

	if(camX >= newX * GetLoadRatio() - (PanelSize / 128) && camX <= newX * GetLoadRatio() + (PanelSize / 128))
		CameraMoved = TRUE;

	if(GateCamera == TRUE)
		return;
	else
	{
		//Pans camera Right
		if(camX < newX * GetLoadRatio() - (PanelSize / 128))
		{
			camX += ((PanelSize / 1.25f) * GetLoadRatio()) * GetDeltaTime();
			AEGfxSetCamPosition(camX, camY);
		}
		//Pans Camera Left
		else if (camX > newX * GetLoadRatio() + (PanelSize / 128))
		{
			camX -= ((PanelSize / 1.25f) * GetLoadRatio()) * GetDeltaTime();
			AEGfxSetCamPosition(camX, camY);
		}
		else
		{
			//Keep Camera Where it is
			AEGfxSetCamPosition(newX * GetLoadRatio(), camY);
		}
	}
}

/*************************************************************************/
/*!
	\brief
	SetCameraLockState

	\param GateSet
	Bool for setting if camera is gated
*/
/*************************************************************************/
void SetCameraLockState(int GateSet)
{
	//True or False
	GateCamera = GateSet;
}

/*************************************************************************/
/*!
	\brief
	Sets that camera is not gated
*/
/*************************************************************************/
void ResetGatedCamera(void)
{
	GateCamera = FALSE;
	CameraMoved = FALSE;
}

/*************************************************************************/
/*!
	\brief
	Gets bool of if camera is locked

	\return
	Returns cameras lock state
*/
/*************************************************************************/
int GetCameraLockState(void)
{
	return GateCamera;
}

/*************************************************************************/
/*!
	\brief
	Bool for if camera has moved (for panning)

	\param GateSet
	What you are setting the moved state to
*/
/*************************************************************************/
void SetCameraMovedState(int GateSet)
{
	//True or False
	CameraMoved = GateSet;
}

/*************************************************************************/
/*!
	\brief
	Gets bool of if camera has moved

	\return
	Returns cameras moved state
*/
/*************************************************************************/
int GetCameraMovedState(void)
{
	return CameraMoved;
}

/*************************************************************************/
/*!
	\brief
	Sets up panning and locking/gating of the camera for levels

	\param levelComplete
	So you can update if the level has been completed

	\param panelsize
	Knows how far to pan

	\param Spawners
	Need to check if enemies have spawned (this should be an array)

	\param numPanels
	So it knows how many panels to loop through
*/
/*************************************************************************/
void SetUpCameraPanAndLock(int* levelComplete, float panelsize, EnemySpawner** Spawners, int numPanels)
{
	int i;

	SetCameraLockState(FALSE);

	for(i = 0; i < numPanels; i++)
	{
		//Checking player position
		if(CurrentPlayer.Position.x > (-(panelsize / 2) + (panelsize * i)) && CurrentPlayer.Position.x < ((panelsize / 2) + (panelsize * i)))
		{
			//Checking this spawner is set up
			if(Spawners[i + 1] == NULL)
			{
				//if not don't check inside of it
				if(EnemyPanelNumber[i] > 0 && GetCameraMovedState())
					SetCameraLockState(TRUE);
			}
			//Spawners will be set left to right always checking right one
			//Lock camera if there are enemies and they are spawned and camera has finished panning
			else if(EnemyPanelNumber[i] > 0 && GetCameraMovedState() && !Spawners[i + 1]->objID)
				SetCameraLockState(TRUE);

			//Set the camera to the next panel
			SetCameraPan((panelsize * i), panelsize);
		}
	}

		//Checking for level completed
	if(CurrentPlayer.Position.x > (panelsize / 2) * (2 * numPanels - 1) + CurrentPlayer.PlayerCollider.width)
		*levelComplete = TRUE;


}

/*************************************************************************/
/*!
	\brief
	Sets up panning and locking/gating of the camera for levels

	\param levelComplete
	So you can update if the level has been completed

	\param panelsize
	Knows how far to pan

	\param numPanels
	So it knows how many panels to loop through
*/
/*************************************************************************/
void SetUpCameraPanAndLockNoSpawner(int* levelComplete, float panelsize, int numPanels)
{
	int i;
	SetCameraLockState(FALSE);

	for(i = 0; i < numPanels; i++)
	{
		//Checking player position
		if(CurrentPlayer.Position.x > (-(panelsize / 2) + (panelsize * i)) && CurrentPlayer.Position.x < ((panelsize / 2) + (panelsize * i)))
		{
			//No spawners but their are enemies (tutorial)
			//Lock camera if there are enemies and camera has finished panning
			if(EnemyPanelNumber[i] > 0 && GetCameraMovedState())
				SetCameraLockState(TRUE);

			//Set the camera to the next panel
			SetCameraPan((panelsize * i), panelsize);
		}
	}

		//Checking for level completed
	if(CurrentPlayer.Position.x > (panelsize / 2) * (2 * numPanels - 1) + CurrentPlayer.PlayerCollider.width)
		*levelComplete = TRUE;

}

