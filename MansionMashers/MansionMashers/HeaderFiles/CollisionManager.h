#ifndef FOX_COL_MANAGER
#define FOX_COL_MANAGER

#include "CollisionBox.h"

enum SensorType{ RectangleCollider, CircleCollider };

void DetectCollision(void);
int CollisionRectangles(CollisionBox* objA, CollisionBox* objB);

#endif