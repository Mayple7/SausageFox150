#ifndef FOX_OBJ_MANAGER
#define FOX_OBJ_MANAGER

#include "../AEEngine.h"
#include "Sprite.h"

//Create object list
#define OBJECTAMOUNT 20
Sprite *objectList;
Sprite **collideList;

//Functionz
Sprite* addObject(void);
void resetObjectList(void);
void drawObjectList(void);
void freeObjectList(void);
void freeObject(Sprite* objectNext);

#endif