#ifndef FOX_RIGIDBODY
#define FOX_RIGIDBODY

#include "Vector2.h"

typedef struct RigidBody
{
	Vec2 Velocity;
	Vec2 Acceleration;

	float Mass;
	float Drag;
	float Friction;
}RigidBody;

void ZeroAcceleration(RigidBody* Result);
void ZeroVelocity(RigidBody* Result);

void ApplyForce(RigidBody* Result, Vec2* Force);
void ApplyVelocity(RigidBody* Result, Vec2* VelocityChange);

void SetVelocity(RigidBody* Result, float x, float y);
void SetAcceleration(RigidBody* Result, Vec2* NewAcceleration);

void UpdateVelocity(RigidBody* CurrentRigidBody);

#endif