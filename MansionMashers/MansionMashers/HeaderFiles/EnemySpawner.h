#ifndef FOX_ENEMYSPAWNER
#define FOX_ENEMYSPAWNER

#include "Vector2.h"
#include "Sprite.h"
#include "CollisionBox.h"

typedef struct EnemySpawner
{
	CollisionBox SpawnerCollider;

	int objID;

	Vec2 spawnPosition;
	int numEnemies;
	int enemyType;

}EnemySpawner;

EnemySpawner* CreateEnemySpawner(int numEnemies, int enemyType, Vec2 spawnPosition, float width, float height, Vec2 spawnerPosition, int *objID);
int SpawnEnemies(EnemySpawner* CurrentSpawner);

#endif