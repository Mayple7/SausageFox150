/*****************************************************************************/
/*!
\file				Camera.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_CAMERA
#define FOX_CAMERA

#include "FoxMath.h"
#include "FoxEngine.h"
#include "Sprite.h"
#include "EnemySpawner.h"

void SetCamera(Vec2 *playerX, int offset); 
void ResetCamera(void);
float GetCameraXPosition();
void SetCameraXPosition(float newX);
void SetCameraPan(float newX, float PanelSize);
void SetCameraLockState(int GateSet);
int GetCameraLockState(void);
void ResetGatedCamera(void);
void SetCameraMovedState(int MoveState);
int GetCameraMovedState(void);
void SetUpCameraPanAndLock(int* levelComplete, float panelsize, EnemySpawner** Spawners, int numPanels);
void SetUpCameraPanAndLockNoSpawner(int* levelComplete, float panelsize, int numPanels);

#endif