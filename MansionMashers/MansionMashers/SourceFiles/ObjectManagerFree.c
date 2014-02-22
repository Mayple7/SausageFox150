/*****************************************************************************/
/*!
\file				ObjectManagerFree.c
\author				Kaden Nugent (kaden.n)
\author				Dan Muller (d.muller)
\date				Jan 8, 2014

\brief				Manages Freeing Objects

\par				Functions:
\li					freeObject
\li					FreeObjectList
  
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
void freeObject(Sprite* object)
{
	//Make sure the sprite exists
	if (object && (object->Created & CREATED))
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
	//Weapon, pronounced: "We-pown"
	if(CurrentWeapon->objID > -1)
	{
		CurrentWeapon->objID = -1;
		FreeMyAlloc(CurrentWeapon->WeaponName);
		FreeMyAlloc(CurrentWeapon->WeaponStatsString);
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
		{
			freeObject(CurrentEnemy->EnemySprite);
			freeObject(CurrentEnemy->EnemySpriteParts.ArmLower);
			freeObject(CurrentEnemy->EnemySpriteParts.ArmLower2);
			freeObject(CurrentEnemy->EnemySpriteParts.ArmUpper);
			freeObject(CurrentEnemy->EnemySpriteParts.ArmUpper2);
			freeObject(CurrentEnemy->EnemySpriteParts.LegLower);
			freeObject(CurrentEnemy->EnemySpriteParts.LegLower2);
			freeObject(CurrentEnemy->EnemySpriteParts.LegUpper);
			freeObject(CurrentEnemy->EnemySpriteParts.LegUpper2);
			freeObject(CurrentEnemy->EnemySpriteParts.Body);
			freeObject(CurrentEnemy->EnemySpriteParts.Tail);
			freeObject(CurrentEnemy->EnemySpriteParts.Skirt);
			freeObject(CurrentEnemy->EnemySpriteParts.Weapon);

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
	Frees the button being passed
	
	\param objectNext
	The pointer to a button whose memory is to be deallocated
*/
/*************************************************************************/
void FreeButton(Button* CurrentButton)
{
	//Make sure the sprite exists
	if (CurrentButton->objID && CurrentButton->ButtonSprite->Created)
	{
		freeObject(CurrentButton->ButtonSprite);
		//Free the mesh and texture data
		CurrentButton->objID = 0;
	}
}

/*************************************************************************/
/*!
	\brief
	Cycles through the object list freeing all objects in the list
*/
/*************************************************************************/
void FreeObjectList(void)
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
		if (foodList[i].objID)
		{
			//Free the mesh and texture data
			FreeFood(&foodList[i]);
		}
		//Make sure the sprite exists
		if (weaponList[i].objID)
		{
			//Free the mesh and texture data
			FreeWeapon(&weaponList[i]);
		}
		//Make sure the sprite exists
		if (enemyList[i].objID)
		{
			//Free the mesh and texture data
			FreeEnemy(&enemyList[i]);
		}
		//For floating damage text only!
		if (floatTextList[i])
		{
			FreeText(floatTextList[i]);
		}
		//For static normal text only!
		if (staticTextList[i])
		{
			FreeText(staticTextList[i]);
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
	FreeMyAlloc(staticTextList);
	FreeMyAlloc(buttonList);
}
