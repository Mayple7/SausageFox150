/*****************************************************************************/
/*!
\file				CollisionBox.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_COLLISION_BOX
#define FOX_COLLISION_BOX

#include "FoxMath.h"

enum collisionGroup{DefaultType, 
					PlayerType,
					EnemyType, 
					BackgroundType, 
					FoodType, 
					HudType, 
					PlatformType, 
					BounceType, 
					ButtonType, 
					WeaponDrop, 
					WeaponFriendly, 
					WeaponEnemy,
					WallType,
					SpawnerType};

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

void CreateCollisionBox(CollisionBox *newBox, Vec2 *newPosition, int collisionGroup, float width, float height, int objectID);
void UpdateCollisionPosition(CollisionBox *Collider, Vec2 *newPosition);
void UpdateCollider(CollisionBox *Collider, float width, float height);
void displayCollisionDebug(CollisionBox *Collider);

#endif