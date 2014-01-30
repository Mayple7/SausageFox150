#ifndef FOX_RIGIDBODY
#define FOX_RIGIDBODY

#include "Vector2.h"

typedef struct RigidBody
{
	Vec2 Velocity;
	Vec2 Acceleration;
	Vec2 Gravity;

	int onGround;
	int Static;
	float Mass;
	float Density;			// Mass / (Volume(3D) || Area(2D))
	float Area;
	float Drag;				
	float Friction;
	float Restitution;
}RigidBody;

void InitializeRigidBody(RigidBody* Result, int isStatic, float width, float height);

void ZeroAcceleration(RigidBody* Result);
void ZeroVelocity(RigidBody* Result);

void ApplyForce(RigidBody* Result, Vec2* Force);
void ApplyVelocity(RigidBody* Result, Vec2* VelocityChange);

void SetVelocity(RigidBody* Result, float x, float y);
void SetAcceleration(RigidBody* Result, Vec2* NewAcceleration);

void UpdateVelocity(RigidBody* CurrentRigidBody);
void applyDrag(RigidBody* CurrentRigidBody);

void ZeroGravity(RigidBody* CurrentRigidBody);
void SetGravity(RigidBody* CurrentRigidBody, float GravityForceX, float GravityForceY);
void applyGravity(RigidBody* CurrentRigidBody);

void BounceObject(RigidBody* RB1, RigidBody* RB2);

#endif