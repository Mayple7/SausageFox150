#ifndef FOX_FOOD
#define FOX_FOOD

#include "Vector2.h"
#include "Sprite.h"
#include "CollisionBox.h"

typedef struct Food
{
	Sprite *FoodSprite;
	CollisionBox FoodCollider;
	Vec2 Position;
	int objID;
}Food;

Food* CreateFood(char* textureName, int collisionGroup, float width, float height, int objID, float xPos, float yPos);
void UpdateFoodPosition(Food *CurrentFood, float x, float y);

#endif