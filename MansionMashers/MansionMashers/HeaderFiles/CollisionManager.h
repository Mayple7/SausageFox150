#ifndef FOX_COL_MANAGER
#define FOX_COL_MANAGER

#include "CollisionBox.h"
#include "Vector2.h"

enum SensorType{ RectangleCollider, CircleCollider };

void DetectCollision(void);
int CollisionRectangles(CollisionBox* objA, CollisionBox* objB);
int PointRectCollision(CollisionBox* objA, Vec2* objB);

#endif