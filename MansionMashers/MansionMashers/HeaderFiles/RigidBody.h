#ifndef FOX_RIGIDBODY
#define FOX_RIGIDBODY

#include "Vector2.h"

typedef struct RigidBody
{
	Vec2 Velocity;
	Vec2 Acceleration;

	float Mass;
	float Density;			// Mass / (Volume(3D) || Area(2D))
	float Area;
	float Drag;				// Between 0 and 1
	float Friction;
}RigidBody;

void ZeroAcceleration(RigidBody* Result);
void ZeroVelocity(RigidBody* Result);

void ApplyForce(RigidBody* Result, Vec2* Force);
void ApplyVelocity(RigidBody* Result, Vec2* VelocityChange);

void SetVelocity(RigidBody* Result, float x, float y);
void SetAcceleration(RigidBody* Result, Vec2* NewAcceleration);

void UpdateVelocity(RigidBody* CurrentRigidBody);
void ApplyDrag(RigidBody* CurrentRigidBody);

#endif