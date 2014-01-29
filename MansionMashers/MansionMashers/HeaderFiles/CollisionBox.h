#ifndef FOX_COLLISION_BOX
#define FOX_COLLISION_BOX

#include "FoxMath.h"

typedef struct CollisionBox
{
	int canCollide;

	Vec2 Position;
	Vec2 Offset;

	float width;
	float height;

}CollisionBox;

void CreateCollisionBox(CollisionBox *newBox, Vec2 *newPosition, float width, float height);
void UpdateCollisionPosition(CollisionBox *Collider, Vec2 *newPosition);

#endif