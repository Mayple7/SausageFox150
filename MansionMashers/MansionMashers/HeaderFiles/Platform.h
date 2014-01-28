#ifndef FOX_PLATFORM
#define FOX_PLATFORM

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"

typedef struct Platform
{
	Sprite *PlatformSprite;
	RigidBody PlatformRigidBody;
	Vec2 Position;
}Platform;

void InitializePlatform(Platform* CurrentPlatform, int SpriteType);

#endif