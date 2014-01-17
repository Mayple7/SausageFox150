#ifndef FOX_CAMERA
#define FOX_CAMERA

#include "FoxMath.h"
#include "FoxEngine.h"
#include "../HeaderFiles/Sprite.h"

void SetCamera(Vec2 *playerX, int offset, int move_dist, struct Sprite *hudmain); 
void ResetCamera(void);

#endif