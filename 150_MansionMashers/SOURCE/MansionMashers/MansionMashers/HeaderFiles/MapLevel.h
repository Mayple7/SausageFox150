/*****************************************************************************/
/*!
\file				MapMenu.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
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
void SetClearFlags(Player *CurrentPlayer);
int pointRectCollisionSprite(Sprite *objA, Vec2 *objB);
#endif