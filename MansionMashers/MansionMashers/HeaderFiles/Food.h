/*****************************************************************************/
/*!
\file				Food.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_FOOD
#define FOX_FOOD

#include "Vector2.h"
#include "Sprite.h"
#include "CollisionBox.h"
#include "ParticleSystem.h"

enum StatsGained {Agility, Strength, Defense, Heal, Key};

typedef struct Food
{
	Sprite *FoodSprite;
	CollisionBox FoodCollider;
	Vec2 Position;
	int objID;
	int FoodType;
	struct ParticleSystem *FoodParticle;
	int KeyBool;
}Food;

Food *CreateFood(int foodType, float width, float height, float xPos, float yPos);
void UpdateFoodPosition(Food *CurrentFood, float x, float y);

#endif