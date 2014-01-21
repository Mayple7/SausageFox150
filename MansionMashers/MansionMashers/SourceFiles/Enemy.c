/*
File:				Enemy.c
Author:				Dan Muller (d.muller)
Creation Date:		Jan 20, 2014

Purpose:			All the enemy components and code are here.

Functions:			InitializeEnemy - Creates the Enemy object
					DrawEnemy - Draws the Enemy based on the sprite.
					UpdatePosition - Updates the position of the Enemy based on velocity and acceleration


Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 



// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxEngine.h"


// ---------------------------------------------------------------------------

// Libraries

// ---------------------------------------------------------------------------
// globals

Enemy CurrentEnemy;
int LogicTimer;

// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------
void InitializeEnemy(struct Enemy *CurrentEnemy)
{
	CurrentEnemy->EnemySprite = CreateSprite("Enemy", "TextureFiles/EasyEnemy.png", 150.0f, 150.0f, 8, 8, 1);

	CurrentEnemy->EnemySprite->AnimationActive = 1;
	CurrentEnemy->EnemySprite->AnimationSpeed = 6;
	CurrentEnemy->EnemySprite->ItemType = EnemyType;
	CurrentEnemy->EnemySprite->CanCollide = 1;

	CurrentEnemy->Position.x = 300.0f;
	CurrentEnemy->Position.y = 0.0f;

	CurrentEnemy->EnemyRigidBody.Static = FALSE;
	CurrentEnemy->EnemyRigidBody.Mass = 10;
	CurrentEnemy->EnemyRigidBody.Drag = 0.5;
	CurrentEnemy->EnemyRigidBody.Area = CurrentEnemy->EnemySprite->Width * CurrentEnemy->EnemySprite->Height;
	CurrentEnemy->EnemyRigidBody.Density = CurrentEnemy->EnemyRigidBody.Mass / CurrentEnemy->EnemyRigidBody.Area;

	SetGravity(&CurrentEnemy->EnemyRigidBody, 0.0f, -15.0f);
	ZeroAcceleration(&CurrentEnemy->EnemyRigidBody);
	SetVelocity(&CurrentEnemy->EnemyRigidBody, 0.0f, 0.0f);
}

void DrawEnemy(struct Enemy *CurrentEnemy)
{
	UpdateEnemyPosition(CurrentEnemy);
	CurrentEnemy->EnemySprite->Position.x = CurrentEnemy->Position.x;
	CurrentEnemy->EnemySprite->Position.y = CurrentEnemy->Position.y;
	//DrawSprite(CurrentEnemy->EnemySprite);
}




void UpdateEnemyPosition(Enemy *CurrentEnemy)
{
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


	UpdateVelocity(&CurrentEnemy->EnemyRigidBody);
	Vec2Add(&CurrentEnemy->Position, &CurrentEnemy->Position, &CurrentEnemy->EnemyRigidBody.Velocity);
}

void EnemyLogic(Enemy *CurrentEnemy, Player *CurrentPlayer)
{
	LogicTimer++;

	if(LogicTimer > 180)
	{
		LogicTimer = 0;
	}
	else if(LogicTimer > 60)
	{
		if(CurrentPlayer->Position.x > CurrentEnemy->Position.x)
			CurrentEnemy->Position.x += 2;
		else
			CurrentEnemy->Position.x -= 2;
	}
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
