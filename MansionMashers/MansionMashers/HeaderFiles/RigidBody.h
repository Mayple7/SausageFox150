#ifndef FOX_RIGIDBODY
#define FOX_RIGIDBODY

struct RigidBody
{
	Vec2 Velocity;
	Vec2 Acceleration;

	float Mass;
	float Drag;
	float Friction;
};

//void UpdateVelocity(struct RigidBody *CurrentRigidBody);

#endif