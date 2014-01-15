/*
File:				RigidBody.c
Author:				Dan Muller (d.muller)
Creation Date:		Jan 14, 2014

Purpose:			The rigidbody and functions are here

Functions:			InitializePlayer - Creates the player object and sprites.
					DrawPlayer - Draws the player based on the sprite.
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 

#include "..\HeaderFiles\RigidBody.h"

void ZeroAcceleration(RigidBody* Result)
{
	Vec2Zero(&Result->Acceleration);
}

void ZeroVelocity(RigidBody* Result)
{
	Vec2Zero(&Result->Velocity);
}

void ApplyForce(RigidBody* Result, Vec2* Force)
{
	Result->Acceleration.x += Force->x;
	Result->Acceleration.y += Force->y;
}

void ApplyVelocity(RigidBody* Result, Vec2* VelocityChange)
{
	Result->Velocity.x += VelocityChange->x;
	Result->Velocity.y += VelocityChange->y;
}

void SetVelocity(RigidBody* Result, Vec2* NewVelocity)
{
	Result->Velocity.x = NewVelocity->x;
	Result->Velocity.y = NewVelocity->y;
}
void UpdateVelocity(RigidBody* CurrentRigidBody)
{
	Vec2Add(&CurrentRigidBody->Velocity, &CurrentRigidBody->Velocity, &CurrentRigidBody->Acceleration);
}
