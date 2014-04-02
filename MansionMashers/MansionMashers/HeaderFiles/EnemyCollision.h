#ifndef FOX_ENEMY_COLLISION
#define FOX_ENEMY_COLLISION

#include "FoxObjects.h"

void EnemyCollidePlatform(Enemy *CurrentEnemy, Platform *CurrentPlatform);
void EnemyCollideWeapon(Enemy *CurrentEnemy);
void EnemyCollideWall(Enemy *CurrentEnemy, Wall *CurrentWall);
void EnemyCollidePlayerProjectile(Enemy *CurrentEnemy, struct Projectile *CurrentProjectile);

#endif