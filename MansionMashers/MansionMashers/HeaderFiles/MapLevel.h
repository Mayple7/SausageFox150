#ifndef FOX_MAPLEVEL
#define FOX_MAPLEVEL

#include "FoxEngine.h"
#include "FoxMath.h"

void LoadMapLevel(void);
void InitializeMapLevel(void);
void UpdateMapLevel(void);
void DrawMapLevel(void);
void FreeMapLevel(void);
void UnloadMapLevel(void);

static void EventLevel(void);
void GetNewIconPosition(Vec2 *NewPosition, int newLocation);
void UpdateProgression(Player *CurrentPlayer);
void SetClearFlags(Player *CurrentPlayer);
int pointRectCollisionSprite(Sprite *objA, Vec2 *objB);
#endif