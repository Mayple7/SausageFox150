#ifndef FOX_ENEMYSPAWNER
#define FOX_ENEMYSPAWNER

#include "Vector2.h"
#include "Sprite.h"
#include "CollisionBox.h"

typedef struct EnemySpawner
{
	CollisionBox SpawnerCollider;

	int objID;
	struct Enemy** EnemyArray;
	int numEnemies;
	int enemyType;

	//Fake bool TRUE is for the right side
	int spawnSide;

}EnemySpawner;

EnemySpawner* CreateEnemySpawner(int numEnemies, int enemyType, int spawnSide, float width, float height, Vec2 spawnerPosition, int* objID);
int SpawnEnemies(EnemySpawner* CurrentSpawner);

#endif