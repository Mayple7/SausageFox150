#ifndef FOX_OBJ_MANAGER
#define FOX_OBJ_MANAGER

#include "../AEEngine.h"
#include "Sprite.h"
#include "FoxObjects.h"
#include "TextCreation.h"

//Objects that are in the workspace
Sprite *drawList;

Platform *platformList;
Food *foodList;
Enemy *enemyList;
Weapon *weaponList;
struct TextGlyphs **floatTextList;

//Functionz
Sprite* AddObject(void);
Platform* AddPlatform(void);
Food* AddFood(void);
Weapon* AddWeapon(void);
Enemy* AddEnemy(void);
void AddFloatingText(struct TextGlyphs* FirstLetter);

void resetObjectList(void);
void DrawObjectList(void);
void freeObjectList(void);
void freeObject(Sprite* objectNext);
void DrawCollisionList(void);
void ResizeObjects(float ratio);

void FreeFood(Food *CurrentFood);
void FreePlatform(Platform *CurrentPlatform);
void FreeEnemy(Enemy *CurrentEnemy);
void FreeWeapon(Weapon *CurrentWeapon);
void FreeFloatingText(struct TextGlyphs *FirstLetter);

void SetDebugMode(void);
void RemoveDebugMode(void);

#endif