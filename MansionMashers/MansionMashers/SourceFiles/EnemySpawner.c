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
EnemySpawner* CreateEnemySpawner(int numEnemies, int enemyType, int spawnSide, float width, float height, Vec2 spawnerPosition, int* objID)
{
	//Adds spawner to object manager
	EnemySpawner* CurrentSpawner = AddSpawner();
	int i;

	//Initialize spawner variables
	CurrentSpawner->EnemyArray = (Enemy **)CallocMyAlloc(numEnemies, sizeof(Enemy*));
	CurrentSpawner->enemyType = enemyType;
	CurrentSpawner->numEnemies = numEnemies;
	CurrentSpawner->objID = *objID;
	CurrentSpawner->spawnSide = spawnSide;

	//Create the spawner collision box
	CreateCollisionBox(&CurrentSpawner->SpawnerCollider, &spawnerPosition, SpawnerType, width, height, *objID);

	//Create the enemies and set them offscreen and AINone
	for(i = 0; i < numEnemies; ++i)
	{
		CurrentSpawner->EnemyArray[i] = CreateEnemy(CurrentSpawner->enemyType, EnemyType, CurrentSpawner->objID + i + 1, CurrentSpawner->SpawnerCollider.Position.x + i * 200 * GetLoadRatio(), -10000);
		CurrentSpawner->EnemyArray[i]->HomePos = CurrentSpawner->SpawnerCollider.Position;
		CurrentSpawner->EnemyArray[i]->EnemyState = AINone;
	}

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
		if(CurrentSpawner->spawnSide)
			CurrentSpawner->EnemyArray[i]->Position.x = GetCameraXPosition() + (1920.0f / 2) * GetLoadRatio() + 100 * i * GetLoadRatio() + 10 * (rand() % 10);
		else
			CurrentSpawner->EnemyArray[i]->Position.x = GetCameraXPosition() - (1920.0f / 2) * GetLoadRatio() - 100 * i * GetLoadRatio() - 10 * (rand() % 10);
		CurrentSpawner->EnemyArray[i]->Position.y = GROUNDLEVEL * GetLoadRatio();
		CurrentSpawner->EnemyArray[i]->HomePos = CurrentSpawner->SpawnerCollider.Position;
		CurrentSpawner->EnemyArray[i]->EnemyState = AIIdle;
	}

	//Free the spawner since it is not used anymore
	FreeMyAlloc(CurrentSpawner->EnemyArray);
	FreeSpawner(CurrentSpawner);

	// Returns the number of enemies spawned
	return i;
}
