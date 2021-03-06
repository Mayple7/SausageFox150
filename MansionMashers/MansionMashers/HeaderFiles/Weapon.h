/*****************************************************************************/
/*!
\file				Weapon.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_WEAPON
#define FOX_WEAPON

#include "Sprite.h"
#include "CollisionBox.h"
#include "FoxMath.h"

#define MAX_NAME_LENGTH 50

enum WeaponAlly {PlayerWeapon, EnemyWeapon, DroppedWeapon};
enum WeaponType {Sword, Axe, Hammer, Spear, FoxWeapon};

typedef struct Weapon
{
	char *WeaponName;
	char *WeaponStatsString;

	int WeaponFOF; // Friend or Foe tag
	int objID;
	int WeaponRarity;
	int WeaponType;
	int WeaponFalling;

	int BonusStrength;
	int BonusAgility;
	int BonusDefense;

	float WeaponLength;
	Vec2 WeaponAttackPosition;

	Vec2 Position;

	Sprite* WeaponSprite;
	Sprite* WeaponHoverBackground;
	struct TextGlyphs* WeaponGlyphs;
	struct TextGlyphs* WeaponStatsGlyphs;

	struct ParticleSystem *WeaponParticle;

	CollisionBox WeaponPickup;
	CollisionBox WeaponAttack;

	//Will be 0 if not in a shop
	struct WeaponShop *CurrentShop;
}Weapon;

Weapon* CreateWeapon(char* weaponName, char* weaponTexture, int weaponType, int weaponRarity, int collisionGroup, float width, float height);
Weapon* CreateDroppedWeapon(int collisionGroup, int weaponRarity, float width, float height, float xPos, float yPos);
void WeaponOnTheRun(Weapon* CurrentWeapon);
void CreateWeaponName(char** Name, int Type, int Rarity);
void SetWeaponStats(Weapon* CurrentWeapon, int BonusStrength, int BonusAgility, int BonusDefense);
void CreateWeaponStats(int WeaponType, int WeaponRarity, int* BonusStrength, int* BonusAgility, int* BonusDefense);
Sprite* CreateWeaponSprite(int WeaponType, int WeaponRarity, float xPos, float yPos);
void SwapWeapons(Weapon* firstWeapon, Weapon* secondWeapon);
void CreateStatsString(char* StatsString, int str, int agi, int def);

#endif
