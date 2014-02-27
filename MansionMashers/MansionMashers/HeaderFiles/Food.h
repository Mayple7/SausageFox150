#ifndef FOX_FOOD
#define FOX_FOOD

#include "Vector2.h"
#include "Sprite.h"
#include "CollisionBox.h"

enum StatsGained {Agility, Strength, Defense, Haste};

typedef struct Food
{
	Sprite *FoodSprite;
	CollisionBox FoodCollider;
	Vec2 Position;
	int objID;
	int FoodType;
}Food;

Food* CreateFood(int foodType, float width, float height, float xPos, float yPos, int objID);
void UpdateFoodPosition(Food *CurrentFood, float x, float y);

#endif