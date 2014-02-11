/*****************************************************************************/
/*!
\file				EnemyCollision.c
\author				Dan Muller (d.muller)
\date				Feb 10, 2014

\brief				Collision resolution functions for the enemy

\par				Functions:
\li					EnemyCollidePlatform
\li					EnemyCollideWeapon
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/ObjectManager.h"
#include "../HeaderFiles/TextCreation.h"

/*************************************************************************/
/*!
	\brief
	Handles the collision between an enemy and platform
	
	\param CurrentEnemy
	Pointer to the enemy object

	\param CurrentPlatform
	Pointer to the platform object
*/
/*************************************************************************/
void EnemyCollidePlatform(Enemy *CurrentEnemy, Platform *CurrentPlatform)
{
	if(CurrentEnemy->EnemyRigidBody.Velocity.y <= 0)
	{
		if(CurrentEnemy->EnemyCollider.Position.y + CurrentEnemy->EnemyCollider.Offset.y - CurrentEnemy->EnemyCollider.height / 2.0f > CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y)
		{
			if(CurrentEnemy->EnemyRigidBody.Velocity.y != 0 && CurrentPlatform->PlatformCollider.collisionGroup == PlatformType)
			{
				CurrentEnemy->Position.y = CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y + CurrentPlatform->PlatformCollider.height / 2 - CurrentEnemy->EnemyCollider.Offset.y + CurrentEnemy->EnemyCollider.height / 2 - 0.01f;
				CurrentEnemy->EnemyRigidBody.onGround = TRUE;
			}
			else if(CurrentEnemy->EnemyRigidBody.Velocity.y != 0 && CurrentPlatform->PlatformCollider.collisionGroup == BounceType)
			{
				if(CurrentEnemy->EnemyRigidBody.Velocity.y > -(CurrentPlatform->PlatformRigidBody.Restitution + CurrentEnemy->EnemyRigidBody.Restitution))
				{
					CurrentEnemy->Position.y = CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y + CurrentPlatform->PlatformCollider.height / 2 - CurrentEnemy->EnemyCollider.Offset.y + CurrentEnemy->EnemyCollider.height / 2 - 0.01f;
					CurrentEnemy->EnemyRigidBody.onGround = TRUE;
				}
				else
				{
					CurrentEnemy->Position.y = CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y + CurrentPlatform->PlatformCollider.height / 2 - CurrentEnemy->EnemyCollider.Offset.y + CurrentEnemy->EnemyCollider.height / 2 + 0.5f;
					BounceObject(&CurrentEnemy->EnemyRigidBody, &CurrentPlatform->PlatformRigidBody);
					
				}
			}
			else
				CurrentEnemy->EnemyRigidBody.onGround = TRUE;
		}
	}
}

/*************************************************************************/
/*!
	\brief
	Handles the collision between an enemy and the attacking weapon
	
	\param CurrentEnemy
	Pointer to the enemy object
*/
/*************************************************************************/
void EnemyCollideWeapon(Enemy *CurrentEnemy)
{
	int damageDealt;
	char num[10];
	Vec3 textColor;
	TextGlyphs *FirstLetter;
	Vec3Set(&textColor, 1.0f, 0.0f, 0.0f);

	// Calculate damage including enemy's damage reduction
	damageDealt = CurrentPlayer.CurrentPlayerStats.Damage;
	
	CurrentEnemy->CurrentEnemyStats.CurrentHealth -= damageDealt;
	sprintf(num, "%d", damageDealt);
	// Create Floating Combat Text
	FirstLetter = CreateText(num, CurrentEnemy->Position.x, CurrentEnemy->Position.y, 100, textColor, Center);
	AddFloatingText(FirstLetter);
	ChangeTextVisibility(FirstLetter);
}
