#ifndef FOX_PLAYER_COLLISION
#define FOX_PLAYER_COLLISION

enum collisionGroup{ PlayerType, EnemyType, PartType, BackgroundType, FoodType, HudType, PlatformType, BounceType, ButtonType};

#include "FoxObjects.h"
#include "Food.h"

void PlayerCollidePlatform(Player *CurrentPlayer, Platform *CurrentPlatform);
void PlayerCollideFood(Player *CurrentPlayer, Food *CurrentFood);

#endif