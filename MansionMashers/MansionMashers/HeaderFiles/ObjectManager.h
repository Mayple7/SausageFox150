#ifndef FOX_OBJ_MANAGER
#define FOX_OBJ_MANAGER

#include "../AEEngine.h"
#include "Sprite.h"
#include "FoxObjects.h"
#include "TextCreation.h"
#include "UIButton.h"
#include "EnemySpawner.h"

//Objects that are in the workspace
Sprite *drawList;

Platform *platformList;
Food *foodList;
Enemy *enemyList;
Weapon *weaponList;
Button *buttonList;
Wall *wallList;
EnemySpawner *spawnerList;
struct WeaponShop *weaponShopList;
struct TextGlyphs **floatTextList;
struct TextGlyphs **staticTextList;
struct ParticleSystem *particleSystemList;
struct Particle *particleList;

//Functionz
Sprite* AddObject(void);
Platform* AddPlatform(void);
Food* AddFood(void);
Weapon* AddWeapon(void);
Enemy* AddEnemy(void);
Button* AddButton(void);
Wall* AddWall(void);
EnemySpawner* AddSpawner(void);
void AddFloatingText(struct TextGlyphs* FirstLetter);
void AddStaticText(struct TextGlyphs* FirstLetter);
struct ParticleSystem *AddParticleSystem(void);
struct Particle *AddParticle(void);
struct WeaponShop* AddWeaponShop(void);


void ResetObjectList(void);
void DrawObjectList(void);
void freeSpriteList(void);
void freeDrawList(void);
void FreeAllLists(void);
void FreeSprite(Sprite* objectNext);
void DrawCollisionList(void);

void FreeFood(Food *CurrentFood);
void FreePlatform(Platform *CurrentPlatform);
void FreeEnemy(Enemy *CurrentEnemy);
void FreeButton(Button *CurrentButton);
void FreeWeapon(Weapon *CurrentWeapon);
void FreeWall(Wall *CurrentWall);
void FreeSpawner(EnemySpawner *CurrentSpawner);
void FreeFloatingText(struct TextGlyphs *FirstLetter);
void FreeParticleSystem(struct ParticleSystem *CurrentSystem);
void FreeParticle(struct Particle *CurrentParticle);

void SetDebugMode(void);
void RemoveDebugMode(void);

#endif