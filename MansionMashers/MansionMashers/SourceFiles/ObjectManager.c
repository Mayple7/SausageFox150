/*****************************************************************************/
/*!
\file				ObjectManager.c
\author				Kaden Nugent (kaden.n)
\author				Dan Muller (d.muller)
\date				Jan 8, 2014

\brief				Manages the objects and allocating and freeing the memory

\par				Functions:
\li					AddObject
\li					AddCollidable
\li					resetObjectList
\li					DrawObjectList
\li					freeObject
\li					freeObjectList
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxObjects.h"

// ---------------------------------------------------------------------------
// Main

/*************************************************************************/
/*!
	\brief
	Adds an object to the list
	
	\return
	Returns a sprite pointer of the object added
*/
/*************************************************************************/
Sprite* AddObject(void)
{
	int i;
	for (i = 0; i < OBJECTAMOUNT; i++)
	{
		//Find a sprite that is empty
		if (drawList[i].Created != 1)
		{
			return &drawList[i];
		}
	}
	return NULL;
}

/*************************************************************************/
/*!
	\brief
	Adds a platform to the platform list
	\return
	The platform added to the list
*/
/*************************************************************************/
Platform* AddPlatform(void)
{
	int i;
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		if(platformList[i].objID == 0 || platformList[i].objID == -1 )
		{
			return &platformList[i];
		}
	}
	return NULL;
}

/*************************************************************************/
/*!
	\brief
	Adds a food to the food list
	
	\return
	The food added to the list
*/
/*************************************************************************/
Food* AddFood(void)
{
	int i;
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		if(foodList[i].objID == 0 || foodList[i].objID == -1)
		{
			return &foodList[i];
		}
		
	}
	return NULL;
}

/*************************************************************************/
/*!
	\brief
	Adds an enemy to the enemy list
	
	\return
	The enemy added to the list
*/
/*************************************************************************/
Weapon* AddWeapon(void)
{
	int i;
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		if(weaponList[i].objID == 0 || weaponList[i].objID == -1)
		{
			return &weaponList[i];
		}
	}
	return NULL;
}

/*************************************************************************/
/*!
	\brief
	Adds an enemy to the enemy list
	
	\return
	The enemy added to the list
*/
/*************************************************************************/
Enemy* AddEnemy(void)
{
	int i;
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		if(enemyList[i].objID == 0 || enemyList[i].objID == -1)
		{
			printf("Enemy at %i Created\n", i);
			return &enemyList[i];
		}
	}
	return NULL;
}

/*************************************************************************/
/*!
	\brief
	Adds an enemy to the enemy list
	
	\return
	The enemy added to the list
*/
/*************************************************************************/
void AddFloatingText(TextGlyphs* FirstLetter)
{
	int i;
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		if(!floatTextList[i])
		{
			floatTextList[i] = FirstLetter;
			printf("Floating Text at %i Created\n", i);
			return;
		}
	}
}

/*************************************************************************/
/*!
	\brief
	Adds a particle system to the particle system list
	
	\return
	The particle system added to the list
*/
/*************************************************************************/
ParticleSystem* AddParticleSystem(void)
{
	int i;
	for (i = 0; i < PARTICLESYSTEMAMOUNT; i++)
	{
		if(particleSystemList[i].objID == 0 || particleSystemList[i].objID == -1)
		{
			printf("ParticleSystem at %i Created\n", i);
			return &particleSystemList[i];
		}
	}
	return NULL;
}

/*************************************************************************/
/*!
	\brief
	Adds a particle system to the particle system list
	
	\return
	The particle system added to the list
*/
/*************************************************************************/
Particle* AddParticle(void)
{
	int i;
	for (i = 0; i < PARTICLEAMOUNT; i++)
	{
		if(particleList[i].objID == 0 || particleList[i].objID == -1)
		{
			return &particleList[i];
		}
	}
	return NULL;
}

/*************************************************************************/
/*!
	\brief
	Resets all the objects in the list and callocs a new array of sprites
*/
/*************************************************************************/
void resetObjectList(void)
{
	int i;
	//Set up the memory to fit the desired amount of objects
	drawList  = (Sprite *) CallocMyAlloc(OBJECTAMOUNT, sizeof(Sprite));

	//Make sure the allocation is not NULL
	if (drawList)
	{
		printf("\nOBJECT LIST SET UP COMPLETE\n\n");

		//Set up object lists
		platformList		= (Platform *) CallocMyAlloc(COLLIDEAMOUNT, sizeof(Platform));
		foodList			= (Food *) CallocMyAlloc(COLLIDEAMOUNT, sizeof(Food));
		enemyList			= (Enemy *) CallocMyAlloc(COLLIDEAMOUNT, sizeof(Enemy));
		weaponList			= (Weapon *) CallocMyAlloc(COLLIDEAMOUNT, sizeof(Weapon));
		floatTextList		= (TextGlyphs **) CallocMyAlloc(COLLIDEAMOUNT, sizeof(TextGlyphs));
		particleList		= (Particle *) CallocMyAlloc(COLLIDEAMOUNT, sizeof(Particle));
		particleSystemList  = (ParticleSystem *) CallocMyAlloc(COLLIDEAMOUNT, sizeof(ParticleSystem));

		for(i = 0; i < COLLIDEAMOUNT; i++)
		{
			platformList[i].objID = -1;
			foodList[i].objID = -1;
			enemyList[i].objID = -1;
			weaponList[i].objID = -1;
		}
				
		for(i = 0; i < PARTICLEAMOUNT; i++)
		{
			particleList[i].objID = -1;
		}

		for(i = 0; i < PARTICLESYSTEMAMOUNT; i++)
		{
			particleSystemList[i].objID = -1;
		}
	}
	else
	{
		printf("\nOBJECT LIST SET UP FAILED\n\n");
	}
}

/*************************************************************************/
/*!
	\brief
	Draws all the objects in the object list
*/
/*************************************************************************/
void DrawObjectList(void)
{
	int currentZ = 0;
	int nextZ    = 0;
	//Draw the sprites so the highest Z is in front
	while (nextZ != -1)
	{
		int i;
		nextZ = -1;
		for (i = 0; i < OBJECTAMOUNT; i++)
		{
			Sprite* objectNext = (drawList + i);

			//Make sure the sprite exists
			if (objectNext && objectNext->Created && objectNext->ZIndex >= currentZ && !objectNext->isHUD)
			{
				if (objectNext->ZIndex == currentZ)
				{
					//Go to sprite.c itself and draw it
					DrawSprite(objectNext);
				}
				else
				{
					//Find the next Z index
					if (nextZ == -1 || nextZ > objectNext->ZIndex)
						nextZ = objectNext->ZIndex;
				}
			}
		}
		currentZ = nextZ;
	}
}

/*************************************************************************/
/*!
	\brief
	Draws all collide boxes when needed
*/
/*************************************************************************/
void DrawCollisionList(void)
{
	int i;
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (platformList[i].objID && platformList[i].PlatformCollider.collisionDebug)
		{
			//Free the mesh and texture data
			displayCollisionDebug(&platformList[i].PlatformCollider);
		}
		//Make sure the sprite exists
		if (foodList[i].objID && foodList[i].FoodCollider.collisionDebug)
		{
			//Free the mesh and texture data
			displayCollisionDebug(&foodList[i].FoodCollider);
		}
		//Make sure the sprite exists
		if (enemyList[i].objID && enemyList[i].EnemyCollider.collisionDebug)
		{
			//Free the mesh and texture data
			displayCollisionDebug(&enemyList[i].EnemyCollider);
		}
		if (weaponList[i].objID && weaponList[i].WeaponPickup.collisionDebug)
		{
			//Free the mesh and texture data
			displayCollisionDebug(&weaponList[i].WeaponPickup);
			displayCollisionDebug(&weaponList[i].WeaponAttack);
		}
	}

	//Players are special
	if(CurrentPlayer.PlayerCollider.collisionDebug)
	{
		displayCollisionDebug(&CurrentPlayer.PlayerCollider);
	}
}

/*************************************************************************/
/*!
	\brief
	Frees the sprite being passed
	
	\param objectNext
	The pointer to a sprite whose memory is to be deallocated
*/
/*************************************************************************/
void freeObject(Sprite* object)
{
	//Make sure the sprite exists
	if (object && object->Created)
	{
		//Free the mesh and texture data
		object->Created = 0;
		if (object->MeshOwner)
			AEGfxMeshFree(object->SpriteMesh);
	}
}

/*************************************************************************/
/*!
	\brief
	Frees the object being passed
	
	\param objectNext
	The pointer to a object whose memory is to be deallocated
*/
/*************************************************************************/
void FreeFood(Food *CurrentFood)
{
	//BECAUSE EVERYONE LIKES FREE FOOD
	if(CurrentFood->objID > -1)
	{
		CurrentFood->objID = -1;
		CurrentFood->FoodCollider.collisionDebug = FALSE;
		AEGfxMeshFree(CurrentFood->FoodCollider.DebugMesh);

		if (CurrentFood->FoodSprite->Created)
			freeObject(CurrentFood->FoodSprite);
	}
}

/*************************************************************************/
/*!
	\brief
	Frees the object being passed
	
	\param objectNext
	The pointer to a object whose memory is to be deallocated
*/
/*************************************************************************/
void FreePlatform(Platform *CurrentPlatform)
{
	if(CurrentPlatform->objID > -1)
	{
		//Free that platform
		CurrentPlatform->objID = -1;
		CurrentPlatform->PlatformCollider.collisionDebug = FALSE;
		AEGfxMeshFree(CurrentPlatform->PlatformCollider.DebugMesh);

		if (CurrentPlatform->PlatformSprite->Created)
			freeObject(CurrentPlatform->PlatformSprite);
	}
}

/*************************************************************************/
/*!
	\brief
	Frees the object being passed
	
	\param CurrentWeapon
	The pointer to the weapon whose memory is to be deallocated
*/
/*************************************************************************/
void FreeWeapon(Weapon *CurrentWeapon)
{
	//BECAUSE EVERYONE LIKES FREE FOOD
	if(CurrentWeapon->objID > -1)
	{
		CurrentWeapon->objID = -1;
		CurrentWeapon->WeaponPickup.collisionDebug = FALSE;
		AEGfxMeshFree(CurrentWeapon->WeaponPickup.DebugMesh);

		if(CurrentWeapon->WeaponSprite->Created)
			freeObject(CurrentWeapon->WeaponSprite);
	}
}

/*************************************************************************/
/*!
	\brief
	Frees the object being passed
	
	\param objectNext
	The pointer to a object whose memory is to be deallocated
*/
/*************************************************************************/
void FreeEnemy(Enemy *CurrentEnemy)
{
	if(CurrentEnemy->objID > -1)
	{
		//I'm sure someone will miss you enemy
		CurrentEnemy->objID = -1;
		CurrentEnemy->EnemyCollider.collisionDebug = FALSE;
		AEGfxMeshFree(CurrentEnemy->EnemyCollider.DebugMesh);

		if (CurrentEnemy->EnemySprite->Created)
			freeObject(CurrentEnemy->EnemySprite);
	}
}

/*************************************************************************/
/*!
	\brief
	Frees the object being passed
	
	\param objectNext
	The pointer to a object whose memory is to be deallocated
*/
/*************************************************************************/
void FreeFloatingText(TextGlyphs *FirstLetter)
{
	int i = 0;

	while(floatTextList[i] != FirstLetter)
	{
		i++;
	}
	if(floatTextList[i] == FirstLetter)
	{
		floatTextList[i] = NULL;

		FreeText(FirstLetter);
	}

}

/*************************************************************************/
/*!
	\brief
	Frees the object being passed
	
	\param CurrentSystem
	The pointer to a object whose memory is to be deallocated
*/
/*************************************************************************/
void FreeParticleSystem(ParticleSystem *CurrentSystem)
{
	if(CurrentSystem->objID > -1)
	{
		//I'm sure everyone will miss you particle system
		CurrentSystem->objID = -1;
	}
}

/*************************************************************************/
/*!
	\brief
	Frees the object being passed
	
	\param CurrentParticle
	The pointer to a object whose memory is to be deallocated
*/
/*************************************************************************/
void FreeParticle(Particle *CurrentParticle)
{
	if(CurrentParticle->objID > -1)
	{
		//I'm sure everyone will miss you particle
		CurrentParticle->objID = -1;

		//if (CurrentParticle->ParticleSprite->Created)
		//	freeObject(CurrentParticle->ParticleSprite);
	}
}

/*************************************************************************/
/*!
	\brief
	Cycles through the object list freeing all objects in the list
*/
/*************************************************************************/
void freeObjectList(void)
{
	////////--Freeing the drawn objects and textures
	int i;
	for (i = 0; i < OBJECTAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (drawList[i].Created)
		{
			//Free the mesh and texture data
			freeObject(&drawList[i]);
		}
	}

	////////--Freeing the collision objects and textures
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (platformList[i].objID)
		{
			//Free the mesh and texture data
			FreePlatform(&platformList[i]);
		}
	}
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (foodList[i].objID)
		{
			//Free the mesh and texture data
			FreeFood(&foodList[i]);
		}
	}
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (weaponList[i].objID)
		{
			//Free the mesh and texture data
			FreeWeapon(&weaponList[i]);
		}
	}
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (enemyList[i].objID)
		{
			//Free the mesh and texture data
			FreeEnemy(&enemyList[i]);
		}
	}
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		if (floatTextList[i])
		{
			FreeText(floatTextList[i]);
		}

	}

	for (i = 0; i < PARTICLEAMOUNT; i++)
	{
		//Free the mesh and texture data
		FreeParticle(&particleList[i]);
	}

	for (i = 0; i < PARTICLESYSTEMAMOUNT; i++)
	{
		//Free the mesh and texture data
		FreeParticleSystem(&particleSystemList[i]);
	}

	//Free the object list data allocation
	FreeMyAlloc(drawList);
	FreeMyAlloc(particleList);
	FreeMyAlloc(particleSystemList);

	//Free collision lists data allocation
	FreeMyAlloc(platformList);
	FreeMyAlloc(foodList);
	FreeMyAlloc(enemyList);
	FreeMyAlloc(weaponList);
	FreeMyAlloc(floatTextList);
}

void SetDebugMode(void)
{
	int i;

	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (platformList[i].objID)
		{
			//Free the mesh and texture data
			platformList[i].PlatformCollider.collisionDebug = TRUE;
		}
	}
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (weaponList[i].objID)
		{
			//Free the mesh and texture data
			weaponList[i].WeaponPickup.collisionDebug = TRUE;
			weaponList[i].WeaponAttack.collisionDebug = TRUE;
		}
	}
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (foodList[i].objID)
		{
			//Free the mesh and texture data
			foodList[i].FoodCollider.collisionDebug = TRUE;
		}
	}
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (enemyList[i].objID)
		{
			//Free the mesh and texture data
			enemyList[i].EnemyCollider.collisionDebug = TRUE;
		}
	}

	CurrentPlayer.PlayerCollider.collisionDebug = TRUE;
}

void RemoveDebugMode(void)
{
	int i;

	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (platformList[i].objID)
		{
			//Free the mesh and texture data
			platformList[i].PlatformCollider.collisionDebug = FALSE;
		}
	}
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (weaponList[i].objID)
		{
			//Free the mesh and texture data
			weaponList[i].WeaponPickup.collisionDebug = FALSE;
			weaponList[i].WeaponAttack.collisionDebug = FALSE;
		}
	}
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (foodList[i].objID)
		{
			//Free the mesh and texture data
			foodList[i].FoodCollider.collisionDebug = FALSE;
		}
	}
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (enemyList[i].objID)
		{
			//Free the mesh and texture data
			enemyList[i].EnemyCollider.collisionDebug = FALSE;
		}
	}
	CurrentPlayer.PlayerCollider.collisionDebug = FALSE;
}
