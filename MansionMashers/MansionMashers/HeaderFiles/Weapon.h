#ifndef FOX_WEAPON
#define FOX_WEAPON

#include "Sprite.h"
#include "CollisionBox.h"
#include "FoxMath.h"

#define MAX_NAME_LENGTH 50

enum WeaponAlly {PlayerWeapon, EnemyWeapon};
enum WeaponType {Sword, Axe, Hammer, Spear, FoxWeapon};

typedef struct Weapon
{
	char *WeaponName;

	int WeaponFOF; // Friend or Foe tag
	int objID;
	int WeaponRarity;
	int WeaponType;

	int BonusStrength;
	int BonusAgility;
	int BonusDefense;

	Vec2 Position;

	Sprite* WeaponSprite;
	struct TextGlyphs* WeaponGlyphs;

	CollisionBox WeaponPickup;
	//CollisionBox WeaponAttack;
}Weapon;

Weapon* CreateDroppedWeapon(int collisionGroup, int weaponRarity, float width, float height, int objID, float xPos, float yPos);
void CreateWeaponName(char** Name, int Type, int Rarity);
void CreateWeaponStats(int WeaponType, int WeaponRarity, int* BonusStrength, int* BonusAgility, int* BonusDefense);
Sprite* CreateWeaponSprite(int WeaponType, int WeaponRarity, float xPos, float yPos);

#endif