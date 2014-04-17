/*****************************************************************************/
/*!
\file				PlayerCollision.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_PLAYER_COLLISION
#define FOX_PLAYER_COLLISION

#include "FoxObjects.h"
#include "Food.h"
#include "Weapon.h"
#include "Wall.h"
#include "Projectile.h"

void PlayerCollidePlatform(Player *CurrentPlayer, Platform *CurrentPlatform);
void PlayerCollideFood(Player *CurrentPlayer, Food *CurrentFood);
void PlayerCollideWeaponDrop(Player *CurrentPlayer, Weapon *wList);
void PlayerCollideEnemyWeapon(Player* CurrentPlayer, Enemy *CurrentEnemy);
void PlayerCollideEnemyProjectile(Player* CurrentPlayer, Projectile *CurrentProjectile);
void PlayerCollideWall(Player* CurrentPlayer, Wall *CurrentWall);

#endif
