#ifndef FOX_CAMERA
#define FOX_CAMERA

#include "FoxMath.h"
#include "FoxEngine.h"
#include "Sprite.h"

typedef struct HUDLayer
{
	Sprite* HudItem[20];
}HUDLayer;

void SetCamera(Vec2 *playerX, int offset, HUDLayer *hudmain); 
void ResetCamera(void);

#endif