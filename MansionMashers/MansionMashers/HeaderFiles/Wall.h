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

Wall* CreateWall(char* textureName, float width, float height, int objID, float xPos, float yPos);
void UpdateWallPosition(Wall* CurrentWall, float x, float y);

#endif