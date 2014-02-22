#ifndef FOX_PLAYER_COLLISION
#define FOX_PLAYER_COLLISION

#include "FoxObjects.h"
#include "Food.h"
#include "Weapon.h"

void PlayerCollidePlatform(Player *CurrentPlayer, Platform *CurrentPlatform);
void PlayerCollideFood(Player *CurrentPlayer, Food *CurrentFood);
void PlayerCollideWeaponDrop(Player *CurrentPlayer, Weapon *wList);
void PlayerCollideEnemyWeapon(Player* CurrentPlayer, Enemy *CurrentEnemy);

#endif