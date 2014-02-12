#ifndef FOX_OBJECTS
#define FOX_OBJECTS

//Create object list
#define OBJECTAMOUNT 1500
//Create collide list
#define COLLIDEAMOUNT (OBJECTAMOUNT / 5)
//Create particle system
#define PARTICLESYSTEMAMOUNT 10
//Create particle list
#define PARTICLEAMOUNT 500

enum Rarity {Common, Uncommon, Rare, Epic, Sausage};

//-------------------------------------------------------------------------
// Fox object includes

#include "Player.h"
#include "Platform.h"
#include "Enemy.h"
#include "EnemyCollision.h"
#include "PlayerCollision.h"
#include "Food.h"
#include "Weapon.h"
#include "ParticleSystem.h"

#endif