/*****************************************************************************/
/*!
\file				CollisionManager.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_COL_MANAGER
#define FOX_COL_MANAGER

#include "CollisionBox.h"
#include "Vector2.h"

enum SensorType{ RectangleCollider, CircleCollider };

void DetectCollision(void);
int CollisionRectangles(CollisionBox* objA, CollisionBox* objB);
int PointRectCollision(CollisionBox* objA, Vec2* objB);
int PointCircleCollision(Vec2* APos, float radius, Vec2* objB);
int RectCircleCollision(Vec2* APos, float radius, CollisionBox* objB);

#endif