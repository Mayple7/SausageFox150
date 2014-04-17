/*****************************************************************************/
/*!
\file				Platform.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
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

Platform* CreatePlatform(char* textureName, int collisionGroup, float width, float height, float xPos, float yPos);
void UpdatePlatformPosition(Platform* CurrentPlatform, float x, float y);

#endif