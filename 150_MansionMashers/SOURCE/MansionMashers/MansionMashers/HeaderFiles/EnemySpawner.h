/*****************************************************************************/
/*!
\file				EnemySpawner.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_ENEMYSPAWNER
#define FOX_ENEMYSPAWNER

#include "Vector2.h"
#include "Sprite.h"
#include "CollisionBox.h"

typedef struct EnemySpawner
{
	CollisionBox SpawnerCollider;

	int objID;
	struct Enemy **EnemyArray;
	int numEnemies;
	int enemyType;

	//Fake bool TRUE is for the right side
	int spawnSide;

}EnemySpawner;

EnemySpawner *CreateEnemySpawner(int numEnemies, int enemyType, int spawnSide, float width, float height, Vec2 spawnerPosition, int panelId);
int SpawnEnemies(EnemySpawner *CurrentSpawner);

#endif