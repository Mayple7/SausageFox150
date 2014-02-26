/*****************************************************************************/
/*!
\file				EnemySpawner.c
\author				Dan Muller (d.muller)
\date				Feb 26, 2014

\brief				Functions for the enemy spawner events

\par				Functions:
\li					CreateEnemySpawner
\li					SpawnEnemies
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../AEEngine.h"
#include "../HeaderFiles/EnemySpawner.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/FoxMath.h"

// ---------------------------------------------------------------------------
// globals

/*************************************************************************/
/*!
	\brief
	Initializes the player
	
	\param CurrentPlayer
	A pointer to the player to be initialized
*/
/*************************************************************************/
EnemySpawner* CreateEnemySpawner(int numEnemies, int enemyType, Vec2 spawnPosition, float width, float height, Vec2 spawnerPosition, int* objID)
{
	//Adds spawner to object manager
	EnemySpawner* CurrentSpawner = AddSpawner();

	//Initialize spawner variables
	CurrentSpawner->enemyType = enemyType;
	CurrentSpawner->numEnemies = numEnemies;
	CurrentSpawner->objID = *objID;
	CurrentSpawner->spawnPosition = spawnPosition;

	//Create the spawner collision box
	CreateCollisionBox(&CurrentSpawner->SpawnerCollider, &spawnerPosition, SpawnerType, width, height, *objID);

	//Increment the objID by the number of enemies
	*objID += numEnemies;

	return CurrentSpawner;
}

/*************************************************************************/
/*!
	\brief
	Spawns the enemies stored in the spawner, then destroys the spawner
	
	\param CurrentSpawner
	A pointer to the spawner activated

	\return
	The number of enemies spawned
*/
/*************************************************************************/
int SpawnEnemies(EnemySpawner* CurrentSpawner)
{
	int i;

	//Spawn all the enemies at the indicated position
	for(i = 0; i < CurrentSpawner->numEnemies; ++i)
	{
		CreateEnemy(CurrentSpawner->enemyType, EnemyType, CurrentSpawner->objID + i + 1, CurrentSpawner->spawnPosition.x + i * 150 * GetLoadRatio(), CurrentSpawner->spawnPosition.y);
	}

	//Free the spawner since it is not used anymore
	FreeSpawner(CurrentSpawner);

	// Returns the number of enemies spawned
	return i;
}
