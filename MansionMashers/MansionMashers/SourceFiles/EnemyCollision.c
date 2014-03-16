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
	if(CurrentEnemy->dropdownTimer > 0.0f)
		return;
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
	Vec2 velocity;
	Vec3Set(&textColor, 1.0f, 0.0f, 0.0f);

	// Calculate damage including enemy's damage reduction
	if(Cheats)
		damageDealt = 9999;
	else
		damageDealt = CurrentPlayer.CurrentPlayerStats.Damage;
	
	CurrentEnemy->CurrentEnemyStats.CurrentHealth -= damageDealt;
	CurrentEnemy->EnemyParticleSystem->amountTotal += 5;
	if (CurrentEnemy->EnemyType != Dummy)
	{
		CurrentEnemy->KnockBack = TRUE;
		CurrentEnemy->KnockBackTime = (int)(0.25f / GetDeltaTime());
		CurrentEnemy->KnockBackDir = CurrentPlayer.PlayerDirection;
		Vec2Set(&velocity, 0.0f, 270.0f);
		if(CurrentEnemy->Position.y <= GROUNDLEVEL)
			Vec2Set(&CurrentEnemy->Position, CurrentEnemy->Position.x, GROUNDLEVEL + 0.1f);
		CurrentEnemy->EnemyRigidBody.onGround = FALSE;
		ApplyVelocity(&CurrentEnemy->EnemyRigidBody, &velocity);
	}
	PlayAudio(CurrentEnemy->CurrentEnemySounds.GetHit1);

	sprintf(num, "-%d", damageDealt);
	// Create Floating Combat Text
	FirstLetter = CreateText(num, (CurrentEnemy->Position.x + rand() % 81 - 40), (CurrentEnemy->Position.y + CurrentEnemy->EnemySprite->Height / 2), 80, textColor, Center, Border);
	AddFloatingText(FirstLetter);
	ChangeTextVisibility(FirstLetter);
	ChangeTextZIndex(FirstLetter, 201);
}

/*************************************************************************/
/*!
	\brief
	Handles the collision between a player and platform
	
	\param CurrentEnemy
	Pointer to the player object

	\param CurrentPlatform
	Pointer to the platform object
*/
/*************************************************************************/
void EnemyCollideWall(Enemy *CurrentEnemy, Wall *CurrentWall)
{
	if (CurrentWall->enemyNotCollidable)
		return;
	else
	{
		// Grab the edges of each collider box
		float EnemyLeft = CurrentEnemy->EnemyCollider.Position.x + CurrentEnemy->EnemyCollider.Offset.x - CurrentEnemy->EnemyCollider.width / 2;
		float EnemyRight = CurrentEnemy->EnemyCollider.Position.x + CurrentEnemy->EnemyCollider.Offset.x + CurrentEnemy->EnemyCollider.width / 2;
		float EnemyTop = CurrentEnemy->EnemyCollider.Position.y + CurrentEnemy->EnemyCollider.Offset.y + CurrentEnemy->EnemyCollider.height / 2;
		float EnemyBottom = CurrentEnemy->EnemyCollider.Position.y + CurrentEnemy->EnemyCollider.Offset.y - CurrentEnemy->EnemyCollider.height / 2;
	
		float WallLeft = CurrentWall->WallCollider.Position.x + CurrentWall->WallCollider.Offset.x - CurrentWall->WallCollider.width / 2;
		float WallRight = CurrentWall->WallCollider.Position.x + CurrentWall->WallCollider.Offset.x + CurrentWall->WallCollider.width / 2;
		float WallTop = CurrentWall->WallCollider.Position.y + CurrentWall->WallCollider.Offset.y + CurrentWall->WallCollider.height / 2;
		float WallBottom = CurrentWall->WallCollider.Position.y + CurrentWall->WallCollider.Offset.y - CurrentWall->WallCollider.height / 2;

		//Logic for being on top of the wall
		if(CurrentEnemy->EnemyRigidBody.Velocity.y <= 0 && EnemyBottom > WallTop - CurrentWall->WallCollider.height / 8)
		{
			CurrentEnemy->Position.y = WallTop - CurrentEnemy->EnemyCollider.Offset.y + CurrentEnemy->EnemyCollider.height / 2 - 0.01f;
			CurrentEnemy->EnemyRigidBody.Velocity.y = 0;
			CurrentEnemy->EnemyRigidBody.onGround = TRUE;
		}
		//Logic for being on the left side of the wall
		else if(CurrentEnemy->Speed > 0 && CurrentEnemy->EnemyDirection == RIGHT && EnemyRight < WallLeft + CurrentWall->WallCollider.width / 8)
		{
			CurrentEnemy->Speed = 0.0f;
			CurrentEnemy->Position.x = WallLeft - CurrentEnemy->EnemyCollider.Offset.x - CurrentEnemy->EnemyCollider.width / 2 + 0.01f;
		}
		//Logic for being on the right side of the wall
		else if(CurrentEnemy->Speed > 0 && CurrentEnemy->EnemyDirection == LEFT && EnemyLeft > WallRight - CurrentWall->WallCollider.width / 8)
		{
			CurrentEnemy->Speed = 0.0f;
			CurrentEnemy->Position.x = WallRight - CurrentEnemy->EnemyCollider.Offset.x + CurrentEnemy->EnemyCollider.width / 2 - 0.01f;
		}
		//Logic for being under a wall
		else if(CurrentEnemy->EnemyRigidBody.Velocity.y > 0 && EnemyTop < WallBottom + CurrentWall->WallCollider.height / 8)
		{
			CurrentEnemy->Position.y = WallBottom - CurrentEnemy->EnemyCollider.Offset.y - CurrentEnemy->EnemyCollider.height / 2 + 0.01f;
			CurrentEnemy->EnemyRigidBody.Velocity.y = 0;
			CurrentEnemy->EnemyRigidBody.onGround = TRUE;
		}
	}
}
