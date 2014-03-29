#ifndef FOX_OBJECTS
#define FOX_OBJECTS

//Create object list
#define OBJECTAMOUNT 2000
//Create collide list
#define COLLIDEAMOUNT (OBJECTAMOUNT / 5)
//Create particle system
#define PARTICLESYSTEMAMOUNT 60
//Create particle list
#define PARTICLEAMOUNT 1200 //FUN FACT: If there are all 60 particle systems, with 20 paticles each, that is the max
//Number of buttons
#define BUTTONAMOUNT 50
//Number of floating text
#define FLOATINGTEXTAMOUNT 60
//Number of spawners
#define SPAWNERAMOUNT 20
//Number of pannels
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
#include "Projectile.h"
#include "Weapon.h"
#include "ParticleSystem.h"
#include "Shop.h"
#include "PaperScroll.h"
#include "EnemySpawner.h"
#include "ArmGuyBoss.h"
#include "HandGuyBoss.h"
#include "YeahGuyBoss.h"
#include "Death.h"
#include "Level.h"
#include "BoundingBox.h"

#endif