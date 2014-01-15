#ifndef FOX_OBJ_MANAGER
#define FOX_OBJ_MANAGER

#include "../AEEngine.h"
#include "..\HeaderFiles\Sprite.h"

//Create object list
#define OBJECTAMOUNT 20
Sprite *objectList;

//Functionz
Sprite* addObject(void);
void resetObjectList(void);
void drawObjectList(void);
void freeObjectList(void);

#endif