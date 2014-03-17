/*****************************************************************************/
/*!
\file				Projectile.c
\author				Kaden Nugent (kaden.n)
\date				Jan 29, 2014

\brief				Functions to do with projectiles are here

\par				Functions:
\li					CreateProjectile
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../AEEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxObjects.h"

#define PANELSIZE 1920.0f

/*************************************************************************/
/*!
	\brief
	Creates a projectile object
	Functions sort of like a weapon
*/
/*************************************************************************/
Projectile *CreateProjectile(char *texture, float width, float height, float xPos, float yPos, enum ProjectileType type, enum collisionGroup collisionGroup, int objID, int damage, float speed)
{
	Projectile *CurrentProjectile = AddProjectile();

	CurrentProjectile->ProjectileFOF = EnemyWeapon; // Friend or Foe tag
	CurrentProjectile->objID = objID;
	CurrentProjectile->ProjectileType = type;

	CurrentProjectile->Damage = damage;
	CurrentProjectile->Speed  =  speed;

	CurrentProjectile->Position.x = xPos;
	CurrentProjectile->Position.y = yPos;

	CurrentProjectile->ProjectileSprite = (Sprite *) CreateSprite(texture, width, height, 36, 1, 1, xPos, yPos);
	CreateCollisionBox(&CurrentProjectile->ProjectileAttack, &CurrentProjectile->Position, collisionGroup, width, height, objID);

	return CurrentProjectile;
}

/*************************************************************************/
/*!
	\brief
	No need to have this loop in every level
*/
/*************************************************************************/
void UpdateAllProjectiles(void)
{
	int i;

	for(i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Only update the created projectiles
		if (projectileList[i].objID == -1)
			break;
		if (projectileList[i].objID == 0)
			continue;

		//Run the actual updates here
		UpdateProjectile(&projectileList[i]);
	}
}

/*************************************************************************/
/*!
	\brief
	Updates an individual projectile
*/
/*************************************************************************/
void UpdateProjectile(Projectile *CurrentProjectile)
{
	//Negative speed will go left, positive will go right, the more you know!
	CurrentProjectile->Position.x += CurrentProjectile->Speed * GetDeltaTime();
	CurrentProjectile->ProjectileAttack.Position.x  = CurrentProjectile->Position.x;
	CurrentProjectile->ProjectileSprite->Position.x = CurrentProjectile->Position.x;

	if (CurrentProjectile->Speed < 0)
		CurrentProjectile->ProjectileSprite->FlipX = TRUE;
	else
		CurrentProjectile->ProjectileSprite->FlipX = FALSE;

	//Delete it if it's far outside the camera
	if (CurrentProjectile->Position.x > GetCameraXPosition() + PANELSIZE || CurrentProjectile->Position.x < GetCameraXPosition() - PANELSIZE)
		FreeProjectile(CurrentProjectile);
}
