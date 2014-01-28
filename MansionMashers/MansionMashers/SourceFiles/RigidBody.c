/*****************************************************************************/
/*!
\file				RigidBody.c
\author				Dan Muller (d.muller)
\date				Jan 14, 2014

\brief				Physics based functions which act on an object

\par				Functions:
\li					InitializeRigidBody
\li					ZeroAcceleration
\li					ZeroVelocity
\li					ZeroGravity
\li					ApplyForce
\li					ApplyVelocity
\li					SetVelocity
\li					SetGravity
\li					ApplyGravity
\li					UpdateVelocity
\li					applyDrag

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 

// ---------------------------------------------------------------------------
// includes
#include "..\AEEngine.h"
#include "..\HeaderFiles\RigidBody.h"
#include "..\HeaderFiles\PhysicsConstants.h"

/*************************************************************************/
/*!
	\brief
	Sets the rigidbody to default global values
	
	\param Result
	The rigidbody to initialize
*/
/*************************************************************************/
void InitializeRigidBody(RigidBody* Result, int isStatic, float width, float height)
{
	Vec2Set(&Result->Velocity, 0.0f, 0.0f);
	Vec2Set(&Result->Acceleration, 0.0f, 0.0f);
	Vec2Set(&Result->Gravity, FOX_GRAVITY_X, FOX_GRAVITY_Y);

	Result->Static = isStatic;
	Result->Mass = FOX_DEFAULT_MASS;
	Result->Area = width * height;
	Result->Density = Result->Mass / Result->Area;
	Result->Drag = FOX_AIRDRAG;
	Result->Friction = FOX_DEFAULT_FRICTION;
	Result->Restitution = FOX_DEFAULT_RESTITUTION;
}

/*************************************************************************/
/*!
	\brief
	Zeros the acceleration of an object
	
	\param Result
	The object whose acceleration will be zero
*/
/*************************************************************************/
void ZeroAcceleration(RigidBody* Result)
{
	Vec2Zero(&Result->Acceleration);
}

/*************************************************************************/
/*!
	\brief
	Zeros the velocity of an object
	
	\param Result
	The object whose velocity will be zero
*/
/*************************************************************************/
void ZeroVelocity(RigidBody* Result)
{
	Vec2Zero(&Result->Velocity);
}

/*************************************************************************/
/*!
	\brief
	Applies a force to an object
	
	\param Result
	The object which the force will be applied to

	\param Force
	The force to be applied to the object
*/
/*************************************************************************/
void ApplyForce(RigidBody* Result, Vec2* Force)
{
	//Correctly applies force on the object based on the current acceleration in the X direction
	if((Result->Acceleration.x <= 0 && Force->x > 0) || (Result->Acceleration.x >= 0 && Force->x < 0))
	{
		Result->Acceleration.x += Force->x;
	}
	else if(Result->Acceleration.x * Result->Acceleration.x < Force->x * Force->x)
	{
		Result->Acceleration.x += Force->x;
	}

	//Correctly applies force on the object based on the current acceleration in the Y direction
	if((Result->Acceleration.y <= 0 && Force->y > 0) || (Result->Acceleration.y >= 0 && Force->y < 0))
	{
		Result->Acceleration.y += Force->y;
	}
	else if(Result->Acceleration.y * Result->Acceleration.y < Force->y * Force->y)
	{
		Result->Acceleration.y += Force->y;
	}
}

/*************************************************************************/
/*!
	\brief
	Applies the velocity change to an object
	
	\param Result
	The object whose velocity will be updated

	\param VelocityChange
	The change in velocity to be applied
*/
/*************************************************************************/
void ApplyVelocity(RigidBody* Result, Vec2* VelocityChange)
{
	Result->Velocity.x += VelocityChange->x;
	Result->Velocity.y += VelocityChange->y;
}

/*************************************************************************/
/*!
	\brief
	Sets the velocity of the object
	
	\param Result
	The object whose velocity will be set

	\param x
	The new velocity to be set in the x direction

	\param y
	The new velocity to be set in the y direction
*/
/*************************************************************************/
void SetVelocity(RigidBody* Result, float x, float y)
{
	Result->Velocity.x = x;
	Result->Velocity.y = y;
}

/*************************************************************************/
/*!
	\brief
	Updates the velocity of an object based on all forces acting upon it
	
	\param CurrentRigidBody
	The object's rigidbody whose velocity will be updated
*/
/*************************************************************************/
void UpdateVelocity(RigidBody* CurrentRigidBody) // V = Vi + at
{
	//Update all the physics effects to the object then update the velocity
	if(!CurrentRigidBody->Static)
	{
		Vec2 accelerationTime;
		applyDrag(CurrentRigidBody);
		applyGravity(CurrentRigidBody);
		Vec2Scale(&accelerationTime, &CurrentRigidBody->Acceleration, 1 / 60.0f);
		Vec2Add(&CurrentRigidBody->Velocity, &CurrentRigidBody->Velocity, &accelerationTime);
	}
}

/*************************************************************************/
/*!
	\brief
	Applies drag to the object
	
	\param CurrentRigidBody
	The object that the drag will be applied to
*/
/*************************************************************************/
void applyDrag(RigidBody* CurrentRigidBody)
{
	Vec2 dragForce;
	//CurrentVelocity * 0.5 * DragConstant * ObjectDensity * ObjectArea
	Vec2Scale(&dragForce, &CurrentRigidBody->Velocity, 0.5 * CurrentRigidBody->Drag * CurrentRigidBody->Density * CurrentRigidBody->Area);
	
	//Drag force is opposite the velocity direction
	Vec2Negate(&dragForce, &dragForce);

	//Drag force is inversely effected by the mass of the object
	Vec2Scale(&dragForce, &dragForce, 1 / CurrentRigidBody->Mass);

	//Update the acceleration if the dragforce is opposite the velocity
	if(CurrentRigidBody->Velocity.x > 0 && dragForce.x < 0)
	{
		CurrentRigidBody->Acceleration.x += dragForce.x;
	}
	else if(CurrentRigidBody->Velocity.x < 0 && dragForce.x > 0)
	{
		CurrentRigidBody->Acceleration.x += dragForce.x;
	}
	//Update the acceleration if the dragforce is opposite the velocity
	if(CurrentRigidBody->Velocity.y > 0 && dragForce.y < 0)
	{
		CurrentRigidBody->Acceleration.y += dragForce.y;
	}
	else if(CurrentRigidBody->Velocity.y < 0 && dragForce.y > 0)
	{
		CurrentRigidBody->Acceleration.y += dragForce.y;
	}
}

/*************************************************************************/
/*!
	\brief
	Zeros the gravity of an object
	
	\param CurrentRigidBody
	The object whose gravity will be zero
*/
/*************************************************************************/
void ZeroGravity(RigidBody* CurrentRigidBody)
{
	Vec2Set(&CurrentRigidBody->Gravity, 0.0f, 0.0f);
}

/*************************************************************************/
/*!
	\brief
	Sets the gravity of an object
	
	\param CurrentRigidBody
	The object whose gravity will be set

	\param GravityForceX
	The gravity effect in the X direction

	\param GravityForceY
	The gravity effect in the Y direction
*/
/*************************************************************************/
void SetGravity(RigidBody* CurrentRigidBody, float GravityForceX, float GravityForceY)
{
	Vec2Set(&CurrentRigidBody->Gravity, GravityForceX, GravityForceY);
}

/*************************************************************************/
/*!
	\brief
	Applies the gravity to an object
	
	\param CurrentRigidBody
	The object to apply gravity to
*/
/*************************************************************************/
void applyGravity(RigidBody* CurrentRigidBody)
{
	Vec2Add(&CurrentRigidBody->Acceleration, &CurrentRigidBody->Acceleration, &CurrentRigidBody->Gravity);
}

/*
void BounceObject(RigidBody* RB1, RigidBody* RB2)
{
	float totalRestitution;

	if(RB2->Static)
		Vec2Scale(&RB1->Velocity, &RB1->Velocity, totalRestitution);

}
*/