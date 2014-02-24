#ifndef FOX_CAMERA
#define FOX_CAMERA

#include "FoxMath.h"
#include "FoxEngine.h"
#include "Sprite.h"

typedef struct HUDLayer
{
	Sprite* HudItem[20];
}HUDLayer;

void SetCamera(Vec2 *playerX, int offset); 
void ResetCamera(void);
float GetCameraXPosition();
void SetCameraXPosition(float newX);
void SetCameraPan(float newX, float PanelSize);
void DrawHUD(struct HUDLayer *hud);

#endif