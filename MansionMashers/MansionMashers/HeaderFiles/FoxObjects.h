#ifndef FOX_OBJECTS
#define FOX_OBJECTS

//Create object list
#define OBJECTAMOUNT 1000
//Create collide list
#define COLLIDEAMOUNT (OBJECTAMOUNT / 5)
//Create particle system
#define PARTICLESYSTEMAMOUNT 50
//Create particle list
#define PARTICLEAMOUNT 500
//Number of buttons
#define BUTTONAMOUNT 50
//Number of floating text
#define FLOATINGTEXTAMOUNT 30
//Number of spawners
#define SPAWNERAMOUNT 20

#define PANELAMOUNT 10

enum Rarity {Common, Uncommon, Rare, Epic, Sausage};

int EnemyPanelNumber[PANELAMOUNT];

//-------------------------------------------------------------------------
// Fox object includes

#include "Player.h"
#include "Platform.h"
#include "Wall.h"
#include "Enemy.h"
#include "EnemyCollision.h"
#include "PlayerCollision.h"
#include "Food.h"
#include "Weapon.h"
#include "ParticleSystem.h"
#include "Shop.h"
#include "PaperScroll.h"
#include "EnemySpawner.h"
#include "ArmGuyBoss.h"
#include "Death.h"

#endif