/*****************************************************************************/
/*!
\file				Wall.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_WALL
#define FOX_WALL

#include "Vector2.h"
#include "RigidBody.h"
#include "Sprite.h"

typedef struct Wall
{
	Sprite *WallSprite;
	RigidBody WallRigidBody;
	CollisionBox WallCollider;
	Vec2 Position;
	int enemyNotCollidable;
	int objID;
}Wall;

Wall* CreateWall(char* textureName, float width, float height, float xPos, float yPos);
void UpdateWallPosition(Wall* CurrentWall, float x, float y);

#endif