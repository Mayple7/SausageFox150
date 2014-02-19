#ifndef FOX_OBJ_MANAGER
#define FOX_OBJ_MANAGER

#include "../AEEngine.h"
#include "Sprite.h"
#include "FoxObjects.h"
#include "TextCreation.h"
#include "UIButton.h"

//Objects that are in the workspace
Sprite *drawList;

Platform *platformList;
Food *foodList;
Enemy *enemyList;
Weapon *weaponList;
Button *buttonList;
struct TextGlyphs **floatTextList;
struct TextGlyphs *floatTextComp;
struct ParticleSystem *particleSystemList;
struct Particle *particleList;

//Functionz
Sprite* AddObject(void);
Platform* AddPlatform(void);
Food* AddFood(void);
Weapon* AddWeapon(void);
Enemy* AddEnemy(void);
Button* AddButton(void);
void AddFloatingText(struct TextGlyphs* FirstLetter);
struct ParticleSystem *AddParticleSystem(void);
struct Particle *AddParticle(void);


void ResetObjectList(void);
void DrawObjectList(void);
void FreeObjectList(void);
void freeObject(Sprite* objectNext);
void DrawCollisionList(void);

void FreeFood(Food *CurrentFood);
void FreePlatform(Platform *CurrentPlatform);
void FreeEnemy(Enemy *CurrentEnemy);
void FreeButton(Button *CurrentButton);
void FreeWeapon(Weapon *CurrentWeapon);
void FreeFloatingText(struct TextGlyphs *FirstLetter);
void FreeParticleSystem(ParticleSystem *CurrentSystem);
void FreeParticle(Particle *CurrentParticle);

void SetDebugMode(void);
void RemoveDebugMode(void);

#endif