#ifndef FOX_PLATFORM
#define FOX_PLATFORM

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"

typedef struct Platform
{
	Sprite *PlatformSprite;
	RigidBody PlatformRigidBody;
	CollisionBox PlatformCollider;
	Vec2 Position;
	int objID;
}Platform;

Platform* CreatePlatform(char* textureName, int collisionGroup, int objID, float width, float height);

#endif