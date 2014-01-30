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
int LogicTimer;

/*************************************************************************/
/*!
	\brief
	Initializes and creates the enemy
	  
	\param CurrentEnemy
	A pointer to the enemy object to be initialized
*/
/*************************************************************************/
Enemy* CreateEnemy(char* textureName, int collisionGroup, float width, float height, int objID)
{
	Enemy *CurrentEnemy = AddEnemy();

	//Creates the enemy sprite
	CurrentEnemy->EnemySprite = CreateSprite("TextureFiles/EasyEnemy.png", width, height, 8, 8, 1);

	//Physics variables initialized
	InitializeRigidBody(&CurrentEnemy->EnemyRigidBody, FALSE, width, height);

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
void EnemyLogic(Enemy *CurrentEnemy, Player *CurrentPlayer)
{
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


}
