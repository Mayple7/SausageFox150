#ifndef FOX_SAUSAGE_MISSLE
#define FOX_SAUSAGE_MISSLE

#include "Vector2.h"
#include "Sprite.h"
#include "CollisionBox.h"

enum ProjectileType {Arrow, CannonBall};

typedef struct Projectile
{
	int ProjectileFOF; // Friend or Foe tag
	int objID;
	int ProjectileType;

	int Damage;
	float Speed;

	Vec2 Position;
	Sprite *ProjectileSprite;
	CollisionBox ProjectileAttack;
}Projectile;

Projectile *CreateProjectile(char *texture, float width, float height, float xPos, float yPos, enum ProjectileType type, enum collisionGroup collisionGroup, int objID, int damage, float speed);
void UpdateProjectile(Projectile *CurrentProjectile);
void UpdateAllProjectiles(void);

#endif
