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
					WallType};

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
void UpdateCollider(CollisionBox *Collider, float width, float height);
void displayCollisionDebug(CollisionBox *Collider);

#endif