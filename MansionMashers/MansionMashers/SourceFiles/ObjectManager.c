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
			printf("Slot %i is now filled\n", i);
			return &drawList[i];
		}
	}
	return NULL;
}

/*************************************************************************/
/*!
	\brief
	Adds a collidable sprite to the collidable list
	
	\param newCollidable
	The sprite of a collidable to add to the list
*/
/*************************************************************************/
void AddCollidable(Sprite *newCollidable)
{
	int i;
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Find a sprite that is empty
		if (collidables[i].Created != 1)
		{
			printf("Slot %i is now collidable\n", i);
			collidables = newCollidable;
			return;
		}
	}
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
			printf("Platform at %i Created\n", i);
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
			printf("Food at %i Created\n", i);
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
			printf("Weapon at %i Created\n", i);
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
	Resets all the objects in the list and callocs a new array of sprites
*/
/*************************************************************************/
void resetObjectList(void)
{
	int i;
	//Set up the memory to fit the desired amount of objects
	drawList  = (Sprite *) calloc(OBJECTAMOUNT, OBJECTAMOUNT * sizeof(Sprite));
	//Make sure the calloc is not NULL
	if (drawList)
	{
		printf("\nOBJECT LIST SET UP COMPLETE\n\n");

		//Set up object lists
		platformList = (Platform *) calloc(COLLIDEAMOUNT, COLLIDEAMOUNT * sizeof(Platform));
		foodList     = (Food *) calloc(COLLIDEAMOUNT, COLLIDEAMOUNT * sizeof(Food));
		enemyList    = (Enemy *) calloc(COLLIDEAMOUNT, COLLIDEAMOUNT * sizeof(Enemy));
		weaponList   = (Weapon *) calloc(COLLIDEAMOUNT, COLLIDEAMOUNT * sizeof(Weapon));
		collideList  = (Sprite *) calloc(COLLIDEAMOUNT, COLLIDEAMOUNT * sizeof(Sprite));
		collidables  = (Sprite *) calloc(COLLIDEAMOUNT, COLLIDEAMOUNT * sizeof(Sprite));

		for(i = 0; i < COLLIDEAMOUNT; i++)
		{
			platformList[i].objID = -1;
			foodList[i].objID = -1;
			enemyList[i].objID = -1;
			weaponList[i].objID = -1;
		}


		//Make sure the malloc is not NULL
		if (collideList && collidables && platformList && foodList && enemyList && weaponList)
		{
			printf("COLLIDE LIST SET UP COMPLETE\n\n");
		}
		else
		{
			printf("COLLIDE LIST SET UP FAILED\n\n");
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
					DrawSprite(objectNext);
				}
				else
				{
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
	if (object->Created)
	{
		//Free the mesh and texture data
		object->Created = 0;
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
			printf("Slot %i is now empty\n", i);
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
			printf("Platform %i is now freed\n", i);
		}
	}
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (foodList[i].objID)
		{
			//Free the mesh and texture data
			FreeFood(&foodList[i]);
			printf("Food %i is now freed\n", i);
		}
	}
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (weaponList[i].objID)
		{
			//Free the mesh and texture data
			FreeWeapon(&weaponList[i]);
			printf("Weapon %i is now freed\n", i);
		}
	}

	//Free the object list data allocation
	free(drawList);

	//Free collision lists data allocation
	free(platformList);
	free(foodList);
	free(weaponList);
}

void ResizeObjects(float ratio)
{
	int i;
	
	for (i = 0; i < OBJECTAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (drawList[i].Created == 1)
		{
			//Update sprite mesh
			drawList[i].Height *= ratio;
			drawList[i].Width *= ratio;
			Vec2Scale(&drawList[i].Position, &drawList[i].Position, ratio);
			UpdateMesh(&drawList[i]);
			//printf("Slot %i is now updated\n", i);
		}
	}
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (platformList[i].objID)
		{
			//Update platform collider
			platformList[i].PlatformCollider.width *= ratio;
			platformList[i].PlatformCollider.height *= ratio;
			Vec2Scale(&platformList[i].PlatformCollider.Offset, &platformList[i].PlatformCollider.Offset, ratio);
			Vec2Scale(&platformList[i].Position, &platformList[i].Position, ratio);
			UpdateCollisionPosition(&platformList[i].PlatformCollider, &platformList[i].Position);
			UpdateCollider(&platformList[i].PlatformCollider, platformList[i].PlatformCollider.width, platformList[i].PlatformCollider.height);
			//printf("Platform %i is now updated\n", i);
		}
	}
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (weaponList[i].objID)
		{
			//Update platform collider
			weaponList[i].WeaponPickup.width *= ratio;
			weaponList[i].WeaponPickup.height *= ratio;
			Vec2Scale(&weaponList[i].WeaponPickup.Offset, &weaponList[i].WeaponPickup.Offset, ratio);
			Vec2Scale(&weaponList[i].Position, &weaponList[i].Position, ratio);
			UpdateCollisionPosition(&weaponList[i].WeaponPickup, &weaponList[i].Position);
			UpdateCollider(&weaponList[i].WeaponPickup, weaponList[i].WeaponPickup.width, weaponList[i].WeaponPickup.height);
			//printf("Platform %i is now updated\n", i);
		}
	}
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (foodList[i].objID)
		{
			//Free the mesh and texture data
			Vec2Scale(&foodList[i].Position, &foodList[i].Position, ratio);
			foodList[i].FoodCollider.width *= ratio;
			foodList[i].FoodCollider.height *= ratio;
			foodList[i].FoodCollider.Offset.x *= ratio;
			foodList[i].FoodCollider.Offset.y *= ratio;
			//printf("Platform %i is now freed\n", i);
		}
	}
	Vec2Scale(&CurrentPlayer.PlayerCollider.Position, &CurrentPlayer.PlayerCollider.Position, ratio);
	Vec2Scale(&CurrentPlayer.Position, &CurrentPlayer.Position, ratio);
	CurrentPlayer.PlayerCollider.width *= ratio;
	CurrentPlayer.PlayerCollider.height *= ratio;
	UpdateCollider(&CurrentPlayer.PlayerCollider, CurrentPlayer.PlayerCollider.width, CurrentPlayer.PlayerCollider.height);

	Vec2Scale(&CurrentPlayer.PlayerCollider.Offset, &CurrentPlayer.PlayerCollider.Offset, ratio);

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
	CurrentPlayer.PlayerCollider.collisionDebug = FALSE;
}
