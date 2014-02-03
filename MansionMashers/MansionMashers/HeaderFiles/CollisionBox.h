#ifndef FOX_COLLISION_BOX
#define FOX_COLLISION_BOX

#include "FoxMath.h"

typedef struct CollisionBox
{
	int canCollide;
	int collisionGroup;
	int collisionID;
	int collisionDebug;

	Vec2 Position;
	Vec2 Offset;

	float width;
	float height;

	AEGfxVertexList *DebugMesh;
	AEGfxTexture *DebugTexture;

}CollisionBox;

void CreateCollisionBox(CollisionBox *newBox, Vec2 *newPosition, int collisionGroup, float width, float height, int newID);
void UpdateCollisionPosition(CollisionBox *Collider, Vec2 *newPosition);
void UpdatePlatformCollider(CollisionBox *Collider, float width, float height);
void displayCollisionDebug(CollisionBox *Collider);

#endif