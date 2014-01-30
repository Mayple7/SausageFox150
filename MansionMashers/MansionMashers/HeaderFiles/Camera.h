#ifndef FOX_CAMERA
#define FOX_CAMERA

#include "FoxMath.h"
#include "FoxEngine.h"
#include "../HeaderFiles/Sprite.h"

typedef struct HUDLayer
{
	struct Sprite* HudItem[20];
}HUDLayer;

void SetCamera(Vec2 *playerX, int offset, struct HUDLayer *hudmain); 
void ResetCamera(void);
void DrawHUD(struct HUDLayer *hud, float camx, float camy);
#endif