#ifndef FOX_CAMERA
#define FOX_CAMERA

#include "FoxMath.h"
#include "FoxEngine.h"
#include "Sprite.h"

void SetCamera(Vec2 *playerX, int offset); 
void ResetCamera(void);
float GetCameraXPosition();
void SetCameraXPosition(float newX);
void SetCameraPan(float newX, float PanelSize);

#endif