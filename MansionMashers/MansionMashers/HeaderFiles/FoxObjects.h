#ifndef FOX_OBJECTS
#define FOX_OBJECTS

//Create object list
#define OBJECTAMOUNT 250
//Create collide list
#define COLLIDEAMOUNT (OBJECTAMOUNT / 5)

enum Rarity {Common, Uncommon, Rare, Epic, Sausage};

//-------------------------------------------------------------------------
// Fox object includes

#include "Player.h"
#include "Platform.h"
#include "Enemy.h"
#include "PlayerCollision.h"
#include "Food.h"
#include "Weapon.h"

#endif