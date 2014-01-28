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

Enemy CurrentEnemy;
int LogicTimer;

/*************************************************************************/
/*!
	\brief
	Initializes and creates the sprite for the enemy
	  
	\param CurrentEnemy
	A pointer to the enemy object to be initialized
*/
/*************************************************************************/
void InitializeEnemy(struct Enemy *CurrentEnemy)
{
	//Creates the enemy sprite
	CurrentEnemy->EnemySprite = CreateSprite("Enemy", "TextureFiles/EasyEnemy.png", 150.0f, 150.0f, 8, 8, 1, EnemyType);

	//Animation properties
	CurrentEnemy->EnemySprite->AnimationActive = 1;
	CurrentEnemy->EnemySprite->AnimationSpeed = 6;

	//Collision properties
	CurrentEnemy->EnemySprite->CanCollide = 1;

	//Starting position
	CurrentEnemy->Position.x = 300.0f;
	CurrentEnemy->Position.y = 0.0f;

	//Physics variables initialized
	CurrentEnemy->EnemyRigidBody.Static = FALSE;
	CurrentEnemy->EnemyRigidBody.Mass = 10;
	CurrentEnemy->EnemyRigidBody.Drag = 0.5;
	CurrentEnemy->EnemyRigidBody.Area = CurrentEnemy->EnemySprite->Width * CurrentEnemy->EnemySprite->Height;
	CurrentEnemy->EnemyRigidBody.Density = CurrentEnemy->EnemyRigidBody.Mass / CurrentEnemy->EnemyRigidBody.Area;

	//Initialize physics systems
	SetGravity(&CurrentEnemy->EnemyRigidBody, 0.0f, -15.0f);
	ZeroAcceleration(&CurrentEnemy->EnemyRigidBody);
	SetVelocity(&CurrentEnemy->EnemyRigidBody, 0.0f, 0.0f);
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
