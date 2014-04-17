/*****************************************************************************/
/*!
\file				EnemyCollision.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_ENEMY_COLLISION
#define FOX_ENEMY_COLLISION

#include "FoxObjects.h"

void EnemyCollidePlatform(Enemy *CurrentEnemy, Platform *CurrentPlatform);
void EnemyCollideWeapon(Enemy *CurrentEnemy);
void EnemyCollideWall(Enemy *CurrentEnemy, Wall *CurrentWall);
void EnemyCollidePlayerProjectile(Enemy *CurrentEnemy, struct Projectile *CurrentProjectile);

#endif