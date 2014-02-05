#ifndef FOX_OBJ_MANAGER
#define FOX_OBJ_MANAGER

#include "../AEEngine.h"
#include "Sprite.h"
#include "FoxObjects.h"

//Objects that are in the workspace
Sprite *drawList;

//Objects that have been collided with
Sprite *collideList;
//Objects that search for collision
Sprite *collidables;
Platform *platformList;
Food *foodList;
Enemy *enemyList;

//Functionz
Sprite* AddObject(void);
Platform* AddPlatform(void);
Food* AddFood(void);
Enemy* AddEnemy(void);
void AddCollidable(Sprite *newCollidable);
void resetObjectList(void);
void DrawObjectList(void);
void freeObjectList(void);
void freeObject(Sprite* objectNext);
void DrawCollisionList(void);
void ResizeObjects(float ratio);

void FreeFood(Food *CurrentFood);
void FreePlatform(Platform *CurrentPlatform);
void FreeEnemy(Enemy *CurrentEnemy);

void SetDebugMode(void);
void RemoveDebugMode(void);

#endif