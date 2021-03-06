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
EnemySpawner *CreateEnemySpawner(int numEnemies, int enemyType, int spawnSide, float width, float height, Vec2 spawnerPosition, int panelId)
{
	//Adds spawner to object manager
	EnemySpawner *CurrentSpawner = AddSpawner();
	int i;

	//Initialize spawner variables
	CurrentSpawner->EnemyArray = (Enemy **) CallocMyAlloc(numEnemies, sizeof(Enemy *));
	CurrentSpawner->enemyType = enemyType;
	CurrentSpawner->numEnemies = numEnemies;
	CurrentSpawner->objID = GetObjectID();
	CurrentSpawner->spawnSide = spawnSide;

	//Create the spawner collision box
	CreateCollisionBox(&CurrentSpawner->SpawnerCollider, &spawnerPosition, SpawnerType, width, height, GetObjectID());

	//Create the enemies and set them offscreen and AINone
	for(i = 0; i < numEnemies; ++i)
	{
		CurrentSpawner->EnemyArray[i] = CreateEnemy(CurrentSpawner->enemyType, EnemyType, 
								                    CurrentSpawner->SpawnerCollider.Position.x + i * 200, -10000, panelId);
		CurrentSpawner->EnemyArray[i]->HomePos = CurrentSpawner->SpawnerCollider.Position;
		CurrentSpawner->EnemyArray[i]->EnemyState = AINone;
	}

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
int SpawnEnemies(EnemySpawner *CurrentSpawner)
{
	int i;

	//Spawn all the enemies at the indicated position
	for(i = 0; i < CurrentSpawner->numEnemies; ++i)
	{
		float spawnOffset = (1920.0f + CurrentSpawner->EnemyArray[i]->EnemyCollider.width) / 2 + 120 * i + 10 * (rand() % 6 + 2);
		if(CurrentSpawner->spawnSide)
			CurrentSpawner->EnemyArray[i]->Position.x = GetCameraXPosition() + spawnOffset;
		else
			CurrentSpawner->EnemyArray[i]->Position.x = GetCameraXPosition() - spawnOffset;
		CurrentSpawner->EnemyArray[i]->Position.y = GROUNDLEVEL;
		CurrentSpawner->EnemyArray[i]->HomePos = CurrentSpawner->SpawnerCollider.Position;
		CurrentSpawner->EnemyArray[i]->EnemyState = AIIdle;
	}

	//Free the spawner since it is not used anymore
	//FreeMyAlloc(CurrentSpawner->EnemyArray);
	FreeSpawner(CurrentSpawner);

	// Returns the number of enemies spawned
	return i;
}
