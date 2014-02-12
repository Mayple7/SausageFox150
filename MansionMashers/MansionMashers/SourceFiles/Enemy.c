/*****************************************************************************/
/*!
\file				Enemy.c
\author				Dan Muller (d.muller)
\date				Jan 20, 2014

\brief				Enemy components and logic code

\par				Functions:
\li					InitializeEnemy
\li					UpdateEnemy
\li					EnemyLogic
  
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

// ---------------------------------------------------------------------------
// globals
static int LogicTimer = 0;

/*************************************************************************/
/*!
	\brief
	Initializes and creates the enemy
	  
	\param CurrentEnemy
	A pointer to the enemy object to be initialized
*/
/*************************************************************************/
Enemy* CreateEnemy(int enemyType, int collisionGroup, int objID, float xPos, float yPos)
{
	float width, height;
	Vec2 position;
	Enemy *CurrentEnemy = AddEnemy();
	int i;

	for(i = 0; i < COLLIDEAMOUNT; i++)
	{
		CurrentEnemy->CollisionData[i] = -1;
	}

	switch(enemyType)
	{
	case Dummy:
		width = 261.0f;
		height = 373.0f;
		Vec2Set(&position, xPos, yPos);
		Vec2Scale(&CurrentEnemy->Position, &position, GetLoadRatio());
		CurrentEnemy->EnemyType = Dummy;
		CurrentEnemy->objID = objID;
		//Creates the enemy sprite
		CurrentEnemy->EnemySprite = CreateSprite("TextureFiles/StrawDummy.png", width, height, 8, 1, 1, xPos, yPos);
		
		InitializeRigidBody(&CurrentEnemy->EnemyRigidBody, TRUE, width, height);

		InitializeEnemyStats(CurrentEnemy, 50, 0, 0, 0, 0, 0, 10);

		CurrentEnemy->EnemyParticleSystem = CreateFoxParticleSystem("TextureFiles/StrawParticle.png", CurrentEnemy->Position.x, CurrentEnemy->Position.y, 1, 1, 5, 0.0f, 270, 90, 1.0f, 25, 24, 50, 2.0f);

		CreateCollisionBox(&CurrentEnemy->EnemyCollider, &position, EnemyType, width / 2, height / 2, objID);
		CurrentEnemy->EnemyCollider.Offset.y = -CurrentEnemy->EnemyCollider.height / 6;
		break;
	case BasicMelee:
		break;
	case BasicRanged:
		break;
	default:
		break;
	}
	return CurrentEnemy;
}

/*************************************************************************/
/*!
	\brief
	Updates the enemy's position and sets the enemy sprite's position
	  
	\param CurrentEnemy
	The pointer to the enemy struct
*/
/*************************************************************************/
void UpdateEnemy(Enemy *CurrentEnemy)
{
	switch(CurrentEnemy->EnemyType)
	{
	case Dummy:
		// Too dummy to do anything lololol
		//Still need to check collision :-/
		DetectEnemyCollision(CurrentEnemy);
		break;
	case BasicMelee:
		// Call enemy logic here
		break;
	case BasicRanged:
		// Call enemy logic here
		break;
	default:
		break;
	}
	if(CurrentEnemy->CurrentEnemyStats.CurrentHealth <= 0)
	{
		// Run on death stuff here
		printf("FREE ME!!!!\n");
		CurrentEnemy->EnemyParticleSystem->emitAngleRandom = 360;
		CurrentEnemy->EnemyParticleSystem->amountTotal = 20;
		CurrentEnemy->EnemyParticleSystem->emitAmount = 20;
		CurrentEnemy->EnemyParticleSystem->emitDisplacementX = 100;
		CurrentEnemy->EnemyParticleSystem->emitDisplacementY = 101;

		FreeEnemy(CurrentEnemy);
	}

	/*
	//Check if enemy is on the floor
	if(CurrentEnemy->Position.y <= -225)
	{
		ZeroAcceleration(&CurrentEnemy->EnemyRigidBody);
		ZeroVelocity(&CurrentEnemy->EnemyRigidBody);
		ZeroGravity(&CurrentEnemy->EnemyRigidBody);
	}
	else
	{
		SetGravity(&CurrentEnemy->EnemyRigidBody, 0.0f, -15.0f);
	}

	//Updates velocity and position
	UpdateVelocity(&CurrentEnemy->EnemyRigidBody);
	Vec2Add(&CurrentEnemy->Position, &CurrentEnemy->Position, &CurrentEnemy->EnemyRigidBody.Velocity);
	CurrentEnemy->EnemySprite->Position = CurrentEnemy->Position;
	CurrentEnemy->EnemyCollider.Position = CurrentEnemy->Position;
	*/
}

/*************************************************************************/
/*!
	\brief
	Enemy logic and AI function
	  
	\param CurrentEnemy
	The pointer to the enemy struct

	\param CurrentPlayer
	The pointer to the player struct
*/
/*************************************************************************/
/*void EnemyLogic(Enemy *CurrentEnemy, Player *CurrentPlayer)
{
	// ************************** //
	//	   THIS IS DEPRECATED	  //
	// ************************** //

	
	LogicTimer++;

	//Resets the logic timer
	if(LogicTimer > 180)
	{
		LogicTimer = 0;
	}
	//Move enemy toward player
	else if(LogicTimer > 60)
	{
		if(CurrentPlayer->Position.x > CurrentEnemy->Position.x)
			CurrentEnemy->Position.x += 2;
		else
			CurrentEnemy->Position.x -= 2;
	}
	//Randomly have the enemy jump
	if(LogicTimer == (int)(rand() / 180) && CurrentEnemy->Position.y <= -225.0f)
	{
		Vec2 velocity;
		Vec2Set(&velocity, 0.0f, 10.0f);
		if(CurrentEnemy->Position.y < -225)
			Vec2Set(&CurrentEnemy->Position, CurrentEnemy->Position.x, -224.9f);
		ApplyVelocity(&CurrentEnemy->EnemyRigidBody, &velocity);
	}
	else
	{
		ZeroAcceleration(&CurrentEnemy->EnemyRigidBody);
	}
	
}*/

void InitializeEnemyStats(Enemy *CurrentEnemy, int maxHP, float movSpeed, float atkSpeed, float dmgReduction, int dmg, int money, int exp)
{
	CurrentEnemy->CurrentEnemyStats.MaxHealth = maxHP;
	CurrentEnemy->CurrentEnemyStats.CurrentHealth = maxHP;

	CurrentEnemy->CurrentEnemyStats.MoveSpeed = movSpeed;
	CurrentEnemy->CurrentEnemyStats.AttackSpeed = atkSpeed;

	CurrentEnemy->CurrentEnemyStats.DamageReduction = dmgReduction;
	CurrentEnemy->CurrentEnemyStats.Damage = dmg;

	CurrentEnemy->CurrentEnemyStats.Money = money;
	CurrentEnemy->CurrentEnemyStats.Experience = exp;
}

void DetectEnemyCollision(Enemy *CurrentEnemy)
{
	Weapon* wList = weaponList;
	int hit = 0;
	int hitPrev = 0;
	
	while(wList->objID != -1)
	{
		if(wList->objID > 0 && wList->WeaponFOF == PlayerWeapon)
		{
			hit = CollisionRectangles(&CurrentEnemy->EnemyCollider, &wList->WeaponAttack);
			hitPrev = searchHitArray(CurrentEnemy->CollisionData, COLLIDEAMOUNT, wList->WeaponAttack.collisionID);
			if(hit && CurrentPlayer.isAttacking)
			{
				// New target, on start collision
				if(hitPrev < 0)
				{
					CurrentEnemy->CollisionData[-hitPrev] = wList->WeaponAttack.collisionID * 10 + 1;
					//printf("NOT FOUND: %i\n", -hitPrev);
					EnemyCollideWeapon(CurrentEnemy);
				}
				// Found target, hit previous frame, on persistant
				else if(CurrentEnemy->CollisionData[hitPrev] % 10 == 1)
				{
					//printf("FOUND PERSISTANT: %i\n", CurrentEnemy.CollisionData[hitPrev]);
				}
				// Found target, did not hit previous frame, on start collision
				else if(CurrentEnemy->CollisionData[hitPrev] % 10 == 0)
				{
					//printf("FOUND NEW COLLISION: %i\n", CurrentEnemy.CollisionData[hitPrev]);
					CurrentEnemy->CollisionData[hitPrev] = wList->WeaponPickup.collisionID * 10 + 1;
					EnemyCollideWeapon(CurrentEnemy);
				}
			}
			else if(hitPrev > 0 && !CurrentPlayer.isAttacking)
			{
				CurrentEnemy->CollisionData[hitPrev] = 0;
			}
			else
			{
				if(hitPrev < 0 || CurrentEnemy->CollisionData[hitPrev] % 10 == 0)
				{
					// NEVER COLLIDED OR DIDNT COLLIDE PREV FRAME
					AE_ASSERT_MESG("No collision and not colliding, should never be here.");
				}
				// Found target, collision ended
				else if(CurrentEnemy->CollisionData[hitPrev] % 10 == 1)
				{
					//printf("END COLLISION: %i\n", CurrentEnemy.CollisionData[hitPrev]);
					//CurrentEnemy->CollisionData[hitPrev] = 0;
				}
			}
		}
		wList++;
	}
}
