/*
File:				Player.c
Author:				Dan Muller (d.muller)
Creation Date:		Jan 8, 2014

Purpose:			All the player components and code are here.

Functions:			InitializePlayer - Creates the player object
					DrawPlayer - Draws the player based on the sprite.
					UpdatePosition - Updates the position of the player based on velocity and acceleration


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

Player CurrentPlayer;

// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------
void InitializePlayer(struct Player *CurrentPlayer)
{
	CurrentPlayer->PlayerSprite = CreateSprite("Player", "TextureFiles/SausageFox.png", 250.0f, 150.0f, 10, 4, 2);

	CurrentPlayer->PlayerSprite->Position.x = 0.0f;
	CurrentPlayer->PlayerSprite->Position.y = 0.0f;

	CurrentPlayer->PlayerSprite->AnimationActive = 1;
	CurrentPlayer->PlayerSprite->AnimationSpeed = 4; // STOP CHANGING HIS LEG SPEED -The Supreme Sausage

	CurrentPlayer->Position.x = 0.0f;
	CurrentPlayer->Position.y = 0.0f;

	CurrentPlayer->PlayerRigidBody.Static = FALSE;
	CurrentPlayer->PlayerRigidBody.Mass = 10;
	CurrentPlayer->PlayerRigidBody.Drag = 0.5;
	CurrentPlayer->PlayerRigidBody.Area = CurrentPlayer->PlayerSprite->Width * CurrentPlayer->PlayerSprite->Height;
	CurrentPlayer->PlayerRigidBody.Density = CurrentPlayer->PlayerRigidBody.Mass / CurrentPlayer->PlayerRigidBody.Area;

	SetGravity(&CurrentPlayer->PlayerRigidBody, 0.0f, -10.0f);
	ZeroAcceleration(&CurrentPlayer->PlayerRigidBody);
	SetVelocity(&CurrentPlayer->PlayerRigidBody, 0.0f, 0.0f);
}

void DrawPlayer(struct Player *CurrentPlayer)
{
	UpdatePosition(CurrentPlayer);
	CurrentPlayer->PlayerSprite->Position.x = CurrentPlayer->Position.x;
	CurrentPlayer->PlayerSprite->Position.y = CurrentPlayer->Position.y;
	//DrawSprite(CurrentPlayer->PlayerSprite);
}


void InputPlayer(struct Player *CurrentPlayer)
{
	if(AEInputCheckCurr('A'))
	{
		if(CurrentPlayer->Position.y > -225)
		{
			if(CurrentPlayer->PlayerSprite->CurrentFrame == 2)
				CurrentPlayer->PlayerSprite->AnimationActive = 0;
			else
				CurrentPlayer->PlayerSprite->AnimationActive = 1;
		}
		else
			CurrentPlayer->PlayerSprite->AnimationActive = 1;
		MoveObject(&CurrentPlayer->Position, LEFT, 8.0f);
		CurrentPlayer->PlayerSprite->FlipX = 0;
	}
	else if(AEInputCheckCurr('D'))
	{
		if(CurrentPlayer->Position.y > -225)
		{
			if(CurrentPlayer->PlayerSprite->CurrentFrame == 2)
				CurrentPlayer->PlayerSprite->AnimationActive = 0;
			else
				CurrentPlayer->PlayerSprite->AnimationActive = 1;
		}
		else
			CurrentPlayer->PlayerSprite->AnimationActive = 1;
		MoveObject(&CurrentPlayer->Position, RIGHT, 8.0f);
		CurrentPlayer->PlayerSprite->FlipX = 1;
	}
	else
	{
		if(CurrentPlayer->Position.y > -225)
		{
			if(CurrentPlayer->PlayerSprite->CurrentFrame == 2)
				CurrentPlayer->PlayerSprite->AnimationActive = 0;
			else
				CurrentPlayer->PlayerSprite->AnimationActive = 1;
		}
		else
		{
			if(CurrentPlayer->PlayerSprite->CurrentFrame == 0)
				CurrentPlayer->PlayerSprite->AnimationActive = 0;
			else if(CurrentPlayer->PlayerSprite->CurrentFrame == 4)
				CurrentPlayer->PlayerSprite->CurrentFrame = 0;
			else
				CurrentPlayer->PlayerSprite->AnimationActive = 1;
		}
	}
	if(AEInputCheckTriggered(VK_SPACE))
	{
		Vec2 velocity;
		Vec2Set(&velocity, 0.0f, 12.0f);
		if(CurrentPlayer->Position.y < -225)
		{
			Vec2Set(&CurrentPlayer->Position, CurrentPlayer->Position.x, -224.9f);
			ApplyVelocity(&CurrentPlayer->PlayerRigidBody, &velocity);
		}
	}
	if(AEInputCheckCurr(VK_BACK))
	{
		Vec2 force;
		CurrentPlayer->PlayerRigidBody.Acceleration.x = 0;
		CurrentPlayer->PlayerRigidBody.Acceleration.y = 0;
		Vec2Set(&force, 0.0f, 15.0f);
		if(CurrentPlayer->Position.y < -225)
		{
			Vec2Set(&CurrentPlayer->Position, CurrentPlayer->Position.x, -224.9f);
		}
		ApplyForce(&CurrentPlayer->PlayerRigidBody, &force);
	}
	else
	{
		CurrentPlayer->PlayerRigidBody.Acceleration.x = 0;
		CurrentPlayer->PlayerRigidBody.Acceleration.y = 0;
	}
}

void HandleCollision(Sprite *objHit)
{
	if (objHit->SpriteType == FoodType)
	{
		printf("YUM YUM YUM YUM  DELICIOUSO\n");
		freeObject(objHit);
	}
	else if (objHit->SpriteType == EnemyType)
	{
		if((objHit->Position.y + (objHit->Height / 3.0f) < CurrentPlayer.Position.y - (CurrentPlayer.PlayerSprite->Height / 2.0f)) && CurrentPlayer.PlayerRigidBody.Velocity.y < 0)
		{
			printf("BOOP!");
			freeObject(objHit);
			SetVelocity(&CurrentPlayer.PlayerRigidBody, 0.0f, 10.0f);
		}
	}

}

void UpdatePosition(Player *CurrentPlayer)
{
	if(CurrentPlayer->Position.y <= -225)
	{
		Vec2Zero(&CurrentPlayer->PlayerRigidBody.Acceleration);
		Vec2Zero(&CurrentPlayer->PlayerRigidBody.Velocity);
		ZeroGravity(&CurrentPlayer->PlayerRigidBody);
	}
	else
	{
		SetGravity(&CurrentPlayer->PlayerRigidBody, 0.0f, -15.0f);
	}


	UpdateVelocity(&CurrentPlayer->PlayerRigidBody);
	Vec2Add(&CurrentPlayer->Position, &CurrentPlayer->Position, &CurrentPlayer->PlayerRigidBody.Velocity);
}

void updateMaxHealth(PlayerStats *CurrentPlayerStats)
{
	//Placeholder max health formula
	CurrentPlayerStats->MaxHealth = 5 + CurrentPlayerStats->Strength * 2;
}

void updateMoveSpeed(PlayerStats *CurrentPlayerStats)
{
	//Placeholder move speed formula
	CurrentPlayerStats->MoveSpeed = CurrentPlayerStats->Agility + 3.0f;
}

void updateAttackSpeed(PlayerStats *CurrentPlayerStats)
{
	//Placeholder attack speed formula
	CurrentPlayerStats->AttackSpeed = CurrentPlayerStats->Agility * 5.0f + 10.0f;
}

void updateDamageReduction(PlayerStats *CurrentPlayerStats)
{
	//Placeholder damage reduction formula
	CurrentPlayerStats->DamageReduction = CurrentPlayerStats->Defense * 2.0f / 100.0f;
}
