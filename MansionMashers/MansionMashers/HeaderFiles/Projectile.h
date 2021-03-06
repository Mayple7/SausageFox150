/*****************************************************************************/
/*!
\file				Projectile.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_SAUSAGE_MISSLE
#define FOX_SAUSAGE_MISSLE

#include "Vector2.h"
#include "Sprite.h"
#include "CollisionBox.h"

enum ProjectileType {Arrow, CannonBall, Wind, HandGuyProjectile};

typedef struct Projectile
{
	int ProjectileFOF; // Friend or Foe tag
	int objID;
	int ProjectileType;

	int Damage;
	float Speed;
	float Direction; //In Radians (BETWEEN -2PI and +2PI PLEASE)

	Vec2 Position;
	Sprite *ProjectileSprite;
	CollisionBox ProjectileAttack;
	struct ParticleSystem *ProjectileParticleSystem;
}Projectile;

Projectile *CreateProjectile(char *texture, float width, float height, float xPos, float yPos, enum ProjectileType type, enum collisionGroup collisionGroup, int damage, float speed, float direction);
void UpdateProjectile(Projectile *CurrentProjectile);
void UpdateAllProjectiles(void);
void PoofProjectile(Projectile *CurrentProjectile);

#endif
