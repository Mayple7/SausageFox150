/*****************************************************************************/
/*!
\file				ObjectManagerFree.c
\author				Kaden Nugent (kaden.n)
\author				Dan Muller (d.muller)
\date				Jan 8, 2014

\brief				Manages Freeing Objects

\par				Functions:
\li					FreeSprite
\li					freeSpriteList
  
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
	Frees the sprite being passed
	
	\param objectNext
	The pointer to a sprite whose memory is to be deallocated
*/
/*************************************************************************/
void FreeSprite(Sprite *object)
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
		CurrentFood->objID = 0;
		CurrentFood->FoodCollider.collisionDebug = FALSE;
		AEGfxMeshFree(CurrentFood->FoodCollider.DebugMesh);

		if (CurrentFood->FoodSprite->Created)
			FreeSprite(CurrentFood->FoodSprite);
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
		CurrentPlatform->objID = 0;
		CurrentPlatform->PlatformCollider.collisionDebug = FALSE;
		AEGfxMeshFree(CurrentPlatform->PlatformCollider.DebugMesh);

		if (CurrentPlatform->PlatformSprite->Created)
			FreeSprite(CurrentPlatform->PlatformSprite);
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
	//Weapon, pronounced: "We-pown"
	if(CurrentWeapon->objID > 0)
	{
		CurrentWeapon->objID = 0;
		FreeMyAlloc(CurrentWeapon->WeaponName);
		FreeMyAlloc(CurrentWeapon->WeaponStatsString);

		FreeText(CurrentWeapon->WeaponGlyphs);
		FreeText(CurrentWeapon->WeaponStatsGlyphs);

		CurrentWeapon->WeaponPickup.collisionDebug = FALSE;
		FreeSprite(CurrentWeapon->WeaponHoverBackground);
		AEGfxMeshFree(CurrentWeapon->WeaponPickup.DebugMesh);
		AEGfxMeshFree(CurrentWeapon->WeaponAttack.DebugMesh);

		if(CurrentWeapon->WeaponSprite->Created)
			FreeSprite(CurrentWeapon->WeaponSprite);
	}
}

/*************************************************************************/
/*!
	\brief
	Frees the object being passed
	
	\param CurrentWeapon
	The pointer to the projectile whose memory is to be deallocated
*/
/*************************************************************************/
void FreeProjectile(Projectile *CurrentProjectile)
{
	//Project me babe
	if(CurrentProjectile->objID > -1)
	{
		CurrentProjectile->objID = 0;
		CurrentProjectile->ProjectileAttack.collisionDebug = FALSE;
		AEGfxMeshFree(CurrentProjectile->ProjectileAttack.DebugMesh);

		if(CurrentProjectile->ProjectileSprite->Created)
			FreeSprite(CurrentProjectile->ProjectileSprite);
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
	if(CurrentEnemy->objID > 0)
	{
		//I'm sure someone will miss you enemy
		CurrentEnemy->objID = 0;
		CurrentEnemy->EnemyCollider.collisionDebug = FALSE;
		AEGfxMeshFree(CurrentEnemy->EnemyCollider.DebugMesh);
		if(CurrentEnemy->EnemyWeapon)
		{
			FreeWeapon(CurrentEnemy->EnemyWeapon);
			CurrentEnemy->EnemyWeapon->WeaponGlyphs = NULL;
			CurrentEnemy->EnemyWeapon->WeaponStatsGlyphs = NULL;
		}
		if (CurrentEnemy->EnemySprite->Created)
		{
			FreeSprite(CurrentEnemy->EnemySprite);
			FreeSprite(CurrentEnemy->EnemySpriteParts.ArmLower);
			FreeSprite(CurrentEnemy->EnemySpriteParts.ArmLower2);
			FreeSprite(CurrentEnemy->EnemySpriteParts.ArmUpper);
			FreeSprite(CurrentEnemy->EnemySpriteParts.ArmUpper2);
			FreeSprite(CurrentEnemy->EnemySpriteParts.LegLower);
			FreeSprite(CurrentEnemy->EnemySpriteParts.LegLower2);
			FreeSprite(CurrentEnemy->EnemySpriteParts.LegUpper);
			FreeSprite(CurrentEnemy->EnemySpriteParts.LegUpper2);
			FreeSprite(CurrentEnemy->EnemySpriteParts.Body);
			FreeSprite(CurrentEnemy->EnemySpriteParts.Tail);
			FreeSprite(CurrentEnemy->EnemySpriteParts.Skirt);
			FreeSprite(CurrentEnemy->EnemySpriteParts.Weapon);
		}
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
		CurrentSystem->objID = 0;
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
		CurrentParticle->objID = 0;

		//if (CurrentParticle->ParticleSprite->Created)
		//	FreeSprite(CurrentParticle->ParticleSprite);
	}
}

/*************************************************************************/
/*!
	\brief
	Frees the button being passed
	
	\param objectNext
	The pointer to a button whose memory is to be deallocated
*/
/*************************************************************************/
void FreeButton(Button *CurrentButton)
{
	//Make sure the sprite exists
	if (CurrentButton->objID && CurrentButton->ButtonSprite->Created)
	{
		FreeSprite(CurrentButton->ButtonSprite);
		//Free the mesh and texture data
		CurrentButton->objID = 0;
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
void FreeWall(Wall *CurrentWall)
{
	if(CurrentWall->objID > -1)
	{
		//Free that platform
		CurrentWall->objID = -1;
		CurrentWall->WallCollider.collisionDebug = FALSE;
		AEGfxMeshFree(CurrentWall->WallCollider.DebugMesh);

		if (CurrentWall->WallSprite->Created)
			FreeSprite(CurrentWall->WallSprite);
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
void FreeSpawner(EnemySpawner *CurrentSpawner)
{
	if(CurrentSpawner->objID > 0)
	{
		//Free that platform
		CurrentSpawner->objID = 0;
		CurrentSpawner->SpawnerCollider.collisionDebug = FALSE;
		AEGfxMeshFree(CurrentSpawner->SpawnerCollider.DebugMesh);

		//The enemies must be freed as well
		FreeMyAlloc(CurrentSpawner->EnemyArray);
	}
}

/*************************************************************************/
/*!
	\brief
	Cycles through the object list freeing all objects in the list
*/
/*************************************************************************/
void freeDrawList(void)
{
	int i;

	/*////////////////////////////////
	//      FREE DRAWN SPRITES      //
	////////////////////////////////*/
	for (i = 0; i < OBJECTAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (drawList[i].Created)
		{
			//Free the mesh and texture data
			FreeSprite(&drawList[i]);
		}
	}

	//Free the object list data allocation
	FreeMyAlloc(drawList);
}

/*************************************************************************/
/*!
	\brief
	Cycles through the object list freeing all objects in the list
*/
/*************************************************************************/
void freeSpriteList(void)
{
	int i;

	/*////////////////////////////////
	//    FREE COLLISION OBJECTS    //
	////////////////////////////////*/
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (platformList[i].objID)
		{
			//Free the mesh and texture data
			FreePlatform(&platformList[i]);
		}
		if (foodList[i].objID)
		{
			//Free the mesh and texture data
			FreeFood(&foodList[i]);
		}
		//Make sure the sprite exists
		if (weaponList[i].objID > 0)
		{
			//Free the mesh and texture data
			FreeWeapon(&weaponList[i]);
		}
		//Make sure the sprite exists
		if (projectileList[i].objID)
		{
			//Free the mesh and texture data
			FreeProjectile(&projectileList[i]);
		}
		//Make sure the sprite exists
		if (enemyList[i].objID)
		{
			//Free the mesh and texture data
			FreeEnemy(&enemyList[i]);
		}
		//For static normal text only!
		if (staticTextList[i])
		{
			FreeText(staticTextList[i]);
		}
		if (wallList[i].objID)
		{
			//Free the mesh and texture data
			FreeWall(&wallList[i]);
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

	for (i = 0; i < BUTTONAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (&buttonList[i])
		{
			//Free the mesh and texture data
			FreeButton(&buttonList[i]);
		}
	}

	for (i = 0; i < SHOPAMOUNT; i++)
	{
		//Free the mesh and texture data
		if(weaponShopList[i].Created)
		{
			//Free stuff here later if needed
		}
	}

	for(i = 0; i < SPAWNERAMOUNT; i++)
	{
		if(spawnerList[i].objID > 0)
		{
			FreeSpawner(&spawnerList[i]);
		}
	}

	//Free collision lists data allocation
	FreeMyAlloc(platformList);
	FreeMyAlloc(foodList);
	FreeMyAlloc(enemyList);
	FreeMyAlloc(weaponList);
	FreeMyAlloc(projectileList);
	FreeMyAlloc(floatTextList);
	FreeMyAlloc(staticTextList);
	FreeMyAlloc(buttonList);
	FreeMyAlloc(particleList);
	FreeMyAlloc(particleSystemList);
	FreeMyAlloc(wallList);
	FreeMyAlloc(weaponShopList);
	FreeMyAlloc(spawnerList);
}

/*************************************************************************/
/*!
	\brief
	Frees all allocated lists
*/
/*************************************************************************/
void FreeAllLists(void)
{
	//Free object list
	freeSpriteList();
	//Free draw list
	freeDrawList();
	//Free sound list
	FreeSoundList();
}
