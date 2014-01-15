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

#include "..\AEEngine.h"
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
	if(Result->Acceleration.x * Result->Acceleration.x < Force->x * Force->x)
		Result->Acceleration.x += Force->x;
	if(Result->Acceleration.y * Result->Acceleration.y < Force->y * Force->y)
		Result->Acceleration.y += Force->y;
}

void ApplyVelocity(RigidBody* Result, Vec2* VelocityChange)
{
	Result->Velocity.x += VelocityChange->x;
	Result->Velocity.y += VelocityChange->y;
}

void SetVelocity(RigidBody* Result, float x, float y)
{
	Result->Velocity.x = x;
	Result->Velocity.y = y;
}

void UpdateVelocity(RigidBody* CurrentRigidBody) // V = Vi + at
{
	Vec2 accelerationTime;
	ApplyDrag(CurrentRigidBody);
	Vec2Scale(&accelerationTime, &CurrentRigidBody->Acceleration, 1 / 60.0f);
	Vec2Add(&CurrentRigidBody->Velocity, &CurrentRigidBody->Velocity, &accelerationTime);
}

void ApplyDrag(RigidBody* CurrentRigidBody)
{
	Vec2 dragForce;
	Vec2Scale(&dragForce, &CurrentRigidBody->Velocity, 0.5 * CurrentRigidBody->Drag * CurrentRigidBody->Density * CurrentRigidBody->Area);
	
	Vec2Negate(&dragForce, &dragForce);
	Vec2Scale(&dragForce, &dragForce, 1 / CurrentRigidBody->Mass);
	if(CurrentRigidBody->Velocity.x > 0 && dragForce.x < 0)
	{
		CurrentRigidBody->Acceleration.x += dragForce.x;
	}
	else if(CurrentRigidBody->Velocity.x < 0 && dragForce.x > 0)
	{
		CurrentRigidBody->Acceleration.x += dragForce.x;
	}

	/*
	if((CurrentRigidBody->Acceleration.x > 0 && (dragForce.x < 0 || dragForce.x < CurrentRigidBody->Acceleration.x)) || (CurrentRigidBody->Acceleration.x < 0 && (dragForce.x > 0 || dragForce.x > CurrentRigidBody->Acceleration.x)))
		//if(CurrentRigidBody->Velocity.x > 0 && dragForce.x + CurrentRigidBody->Velocity.x < 0)
			//CurrentRigidBody->Velocity.x = 0;
		//else if(CurrentRigidBody->Velocity.x < 0 && dragForce.x + CurrentRigidBody->Velocity.x > 0)
			//CurrentRigidBody->Velocity.x = 0;
		if(CurrentRigidBody->Acceleration.x + dragForce.x > 0 && CurrentRigidBody->Velocity.x < 0)
			CurrentRigidBody->Acceleration.x += dragForce.x;
		else if(CurrentRigidBody->Acceleration.x + dragForce.x < 0 && CurrentRigidBody->Velocity.x > 0)
			CurrentRigidBody->Acceleration.x += dragForce.x;
	if((CurrentRigidBody->Acceleration.y > 0 && dragForce.y < 0) || (CurrentRigidBody->Acceleration.y < 0 && dragForce.y > 0))
		CurrentRigidBody->Acceleration.y += dragForce.y;
	*/
}

