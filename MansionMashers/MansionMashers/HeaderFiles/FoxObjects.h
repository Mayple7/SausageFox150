/*****************************************************************************/
/*!
\file				FoxObjects.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_OBJECTS
#define FOX_OBJECTS

//Create object list
#define OBJECTAMOUNT 4000
//Create collide list
#define COLLIDEAMOUNT (OBJECTAMOUNT / 5)
//Create particle system
#define PARTICLESYSTEMAMOUNT 120
//Create particle list
#define PARTICLEAMOUNT 2400 //FUN FACT: If there are all 120 particle systems, with 20 paticles each, that is the max
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