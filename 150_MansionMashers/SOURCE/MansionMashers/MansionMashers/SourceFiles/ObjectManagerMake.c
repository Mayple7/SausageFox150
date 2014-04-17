/*****************************************************************************/
/*!
\file				ObjectManager.c
\author				Kaden Nugent (kaden.n)
\author				Dan Muller (d.muller)
\date				Jan 8, 2014

\brief				Manages Making Objects

\par				Functions:
\li					AddObject
\li					AddCollidable
\li					ResetObjectList
\li					DrawObjectList
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../HeaderFiles/FoxEngine.h"

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
Sprite *AddObject(void)
{
	int i;
	for (i = 0; i < OBJECTAMOUNT; i++)
	{
		//Find a sprite that is empty
		if (drawList[i].Created != 1)
		{
			//The location within the draw list
			drawList[i].SprID = i;
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
Platform *AddPlatform(void)
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
Food *AddFood(void)
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
Weapon *AddWeapon(void)
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
	Adds an projectile to the projectile list
	
	\return
	The projectile added to the list
*/
/*************************************************************************/
Projectile *AddProjectile(void)
{
	int i;
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		if(projectileList[i].objID == 0 || projectileList[i].objID == -1)
		{
			return &projectileList[i];
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
Enemy *AddEnemy(void)
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
void AddFloatingText(TextGlyphs *FirstLetter)
{
	int i;
	for (i = 0; i < FLOATINGTEXTAMOUNT; i++)
	{
		if(!floatTextList[i])
		{
			floatTextList[i] = FirstLetter;
			//printf("Floating Text at %i Created\n", i);
			return;
		}
	}
}

/*************************************************************************/
/*!
	\brief
	Adds an enemy to the enemy list
	
	\return
	The enemy added to the list
*/
/*************************************************************************/
void AddStaticText(TextGlyphs *FirstLetter)
{
	int i;
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		if(!staticTextList[i])
		{
			staticTextList[i] = FirstLetter;
			//printf("Static Text at %i Created\n", i);
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
ParticleSystem *AddParticleSystem(void)
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
Particle *AddParticle(void)
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
	Adds a particle system to the particle system list
	
	\return
	The particle system added to the list
*/
/*************************************************************************/
Button *AddButton(void)
{
	int i;
	for (i = 0; i < BUTTONAMOUNT; i++)
	{
		if(!buttonList[i].objID)
		{
			return &buttonList[i];
		}
	}
	return NULL;
}

/*************************************************************************/
/*!
	\brief
	Adds a wall to the wall list

	\return
	The wall added to the list
*/
/*************************************************************************/
Wall *AddWall(void)
{
	int i;
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		if(wallList[i].objID == 0 || wallList[i].objID == -1 )
		{
			return &wallList[i];
		}
	}
	return NULL;
}

/*************************************************************************/
/*!
	\brief
	Adds a WeaponShop to the WeaponShop list

	\return
	The WeaponShop added to the list
*/
/*************************************************************************/
WeaponShop *AddWeaponShop(void)
{
	int i;
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		if(weaponShopList[i].Created != 1)
		{
			return &weaponShopList[i];
		}
	}
	return NULL;
}

/*************************************************************************/
/*!
	\brief
	Adds a spawner to the spawner lists
	\return
	The spawner added
*/
/*************************************************************************/
EnemySpawner *AddSpawner(void)
{
	int i;
	for (i = 0; i < SPAWNERAMOUNT; i++)
	{
		if(spawnerList[i].objID == 0 || spawnerList[i].objID == -1 )
		{
			return &spawnerList[i];
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
void ResetObjectList(void)
{
	int i;
	//Set up the memory to fit the desired amount of objects
	drawList  = (Sprite *) CallocMyAlloc(OBJECTAMOUNT, sizeof(Sprite));

	//Reset the sound list here (Also frees in object list free)
	ResetSoundList();

	//Make sure the allocation is not NULL
	if (drawList)
	{
		printf("\nOBJECT LIST SET UP COMPLETE\n\n");

		//Set up object lists
		platformList		= (Platform *) CallocMyAlloc(COLLIDEAMOUNT, sizeof(Platform));
		foodList			= (Food *) CallocMyAlloc(COLLIDEAMOUNT, sizeof(Food));
		enemyList			= (Enemy *) CallocMyAlloc(COLLIDEAMOUNT, sizeof(Enemy));
		weaponList			= (Weapon *) CallocMyAlloc(COLLIDEAMOUNT, sizeof(Weapon));
		projectileList      = (Projectile *) CallocMyAlloc(COLLIDEAMOUNT, sizeof(Projectile));
		floatTextList		= (TextGlyphs **) CallocMyAlloc(FLOATINGTEXTAMOUNT, sizeof(TextGlyphs *));
		staticTextList		= (TextGlyphs **) CallocMyAlloc(COLLIDEAMOUNT, sizeof(TextGlyphs *));
		particleList		= (Particle *) CallocMyAlloc(PARTICLEAMOUNT, sizeof(Particle));
		particleSystemList  = (ParticleSystem *) CallocMyAlloc(PARTICLESYSTEMAMOUNT, sizeof(ParticleSystem));
		buttonList			= (Button *) CallocMyAlloc(BUTTONAMOUNT, sizeof(Button));
		wallList			= (Wall *) CallocMyAlloc(COLLIDEAMOUNT, sizeof(Wall));
		weaponShopList		= (WeaponShop *) CallocMyAlloc(SHOPAMOUNT, sizeof(WeaponShop));
		spawnerList			= (EnemySpawner *) CallocMyAlloc(SPAWNERAMOUNT, sizeof(EnemySpawner));

		for(i = 0; i < COLLIDEAMOUNT; i++)
		{
			platformList[i].objID = -1;
			foodList[i].objID = -1;
			enemyList[i].objID = -1;
			projectileList[i].objID = -1;
			weaponList[i].objID = -1;
			wallList[i].objID = -1;
		}
				
		for(i = 0; i < PARTICLEAMOUNT; i++)
		{
			particleList[i].objID = -1;
		}

		for(i = 0; i < PARTICLESYSTEMAMOUNT; i++)
		{
			particleSystemList[i].objID = -1;
		}

		for(i = 0; i < SPAWNERAMOUNT; i++)
		{
			spawnerList[i].objID = -1;
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
		if (platformList[i].objID > 0 && platformList[i].PlatformCollider.collisionDebug)
		{
			//Free the mesh and texture data
			displayCollisionDebug(&platformList[i].PlatformCollider);
		}
		//Make sure the sprite exists
		if (foodList[i].objID > 0 && foodList[i].FoodCollider.collisionDebug)
		{
			//Free the mesh and texture data
			displayCollisionDebug(&foodList[i].FoodCollider);
		}
		//Make sure the sprite exists
		if (enemyList[i].objID > 0 && enemyList[i].EnemyCollider.collisionDebug)
		{
			//Free the mesh and texture data
			displayCollisionDebug(&enemyList[i].EnemyCollider);
		}
		if (weaponList[i].objID > 0 && weaponList[i].WeaponPickup.collisionDebug)
		{
			//Free the mesh and texture data
			displayCollisionDebug(&weaponList[i].WeaponPickup);
			displayCollisionDebug(&weaponList[i].WeaponAttack);
		}
		//Make sure the sprite exists
		if (projectileList[i].objID > 0 && projectileList[i].ProjectileAttack.collisionDebug)
		{
			//Free the mesh and texture data
			displayCollisionDebug(&projectileList[i].ProjectileAttack);
		}
		if (wallList[i].objID > 0 && wallList[i].WallCollider.collisionDebug)
		{
			//Free the mesh and texture data
			displayCollisionDebug(&wallList[i].WallCollider);
		}
	}

	for(i = 0; i < BUTTONAMOUNT; i++)
	{
		if (buttonList[i].objID && buttonList[i].ButtonCollider.collisionDebug)
		{
			//Free the mesh and texture data
			displayCollisionDebug(&buttonList[i].ButtonCollider);
		}
	}

	for(i = 0; i < SPAWNERAMOUNT; i++)
	{
		if(spawnerList[i].objID > 0 && spawnerList[i].SpawnerCollider.collisionDebug)
		{
			displayCollisionDebug(&spawnerList[i].SpawnerCollider);
		}

	}

	//Players are special
	if(CurrentPlayer.PlayerCollider.collisionDebug)
	{
		displayCollisionDebug(&CurrentPlayer.PlayerCollider);
	}
}

void SetDebugMode(void)
{
#if defined _DEBUG
	int i;

	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (platformList[i].objID)
		{
			//Free the mesh and texture data
			platformList[i].PlatformCollider.collisionDebug = TRUE;
		}
		//Make sure the sprite exists
		if (weaponList[i].objID)
		{
			//Free the mesh and texture data
			weaponList[i].WeaponPickup.collisionDebug = TRUE;
			weaponList[i].WeaponAttack.collisionDebug = TRUE;
		}
		//Make sure the sprite exists
		if (projectileList[i].objID)
		{
			//Free the mesh and texture data
			projectileList[i].ProjectileAttack.collisionDebug = TRUE;
		}
		//Make sure the sprite exists
		if (foodList[i].objID)
		{
			//Free the mesh and texture data
			foodList[i].FoodCollider.collisionDebug = TRUE;
		}
		//Make sure the sprite exists
		if (enemyList[i].objID)
		{
			//Free the mesh and texture data
			enemyList[i].EnemyCollider.collisionDebug = TRUE;
		}
		//Make sure the sprite exists
		if (wallList[i].objID)
		{
			//Free the mesh and texture data
			wallList[i].WallCollider.collisionDebug = TRUE;
		}
	}

	for (i = 0; i < BUTTONAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (&buttonList[i])
		{
			//Free the mesh and texture data
			buttonList[i].ButtonCollider.collisionDebug = TRUE;
		}
	}

	//Spawner Debug stuff
	for(i = 0; i < SPAWNERAMOUNT; i++)
	{
		if(spawnerList[i].objID > 0)
		{
			spawnerList[i].SpawnerCollider.collisionDebug = TRUE;
		}
	}


	CurrentPlayer.PlayerCollider.collisionDebug = TRUE;

#endif
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
		//Make sure the sprite exists
		if (weaponList[i].objID)
		{
			//Free the mesh and texture data
			weaponList[i].WeaponPickup.collisionDebug = FALSE;
			weaponList[i].WeaponAttack.collisionDebug = FALSE;
		}
		//Make sure the sprite exists
		if (projectileList[i].objID)
		{
			//Free the mesh and texture data
			projectileList[i].ProjectileAttack.collisionDebug = FALSE;
		}
		//Make sure the sprite exists
		if (foodList[i].objID)
		{
			//Free the mesh and texture data
			foodList[i].FoodCollider.collisionDebug = FALSE;
		}
		//Make sure the sprite exists
		if (enemyList[i].objID)
		{
			//Free the mesh and texture data
			enemyList[i].EnemyCollider.collisionDebug = FALSE;
		}
		//Make sure the sprite exists
		if (wallList[i].objID)
		{
			//Free the mesh and texture data
			wallList[i].WallCollider.collisionDebug = FALSE;
		}
	}

	for (i = 0; i < BUTTONAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (&buttonList[i])
		{
			//Free the mesh and texture data
			buttonList[i].ButtonCollider.collisionDebug = FALSE;
		}
	}
	
	//Spawner Debug stuff
	for(i = 0; i < SPAWNERAMOUNT; i++)
	{
		if(spawnerList[i].objID > 0)
		{
			spawnerList[i].SpawnerCollider.collisionDebug = FALSE;
		}
	}

	CurrentPlayer.PlayerCollider.collisionDebug = FALSE;
}
