struct RigidBody
{
	Vec2 Velocity;
	Vec2 Acceleration;
	Vec2 ForceEffect;

	float Mass;
	float Drag;
	float Friction;
};

void UpdateVelocity(struct RigidBody *RigidBody);