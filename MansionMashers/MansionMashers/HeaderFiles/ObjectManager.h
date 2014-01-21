#ifndef FOX_OBJ_MANAGER
#define FOX_OBJ_MANAGER

#include "../AEEngine.h"
#include "Sprite.h"

//Create object list
#define OBJECTAMOUNT 20
//Objects that are in the workspace
Sprite *objectList;

//Objects that have been collided with
Sprite *collideList;
//Objects that search for collision
Sprite *collidables;

//Functionz
Sprite* AddObject(void);
void AddCollidable(Sprite *newCollidable);
void resetObjectList(void);
void drawObjectList(void);
void freeObjectList(void);
void freeObject(Sprite* objectNext);

#endif