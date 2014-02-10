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

	int BonusStrength;
	int BonusAgility;
	int BonusDefense;

	Vec2 Position;

	Sprite* WeaponSprite;
	struct TextGlyphs* WeaponGlyphs;
	struct TextGlyphs* WeaponStatsGlyphs;

	CollisionBox WeaponPickup;
	CollisionBox WeaponAttack;
}Weapon;

Weapon* CreateWeapon(char* weaponName, char* weaponTexture, int weaponType, int weaponRarity, int collisionGroup, float width, float height, int objID);
Weapon* CreateDroppedWeapon(int collisionGroup, int weaponRarity, float width, float height, int objID, float xPos, float yPos);
void CreateWeaponName(char** Name, int Type, int Rarity);
void SetWeaponStats(Weapon* CurrentWeapon, int BonusStrength, int BonusAgility, int BonusDefense);
void CreateWeaponStats(int WeaponType, int WeaponRarity, int* BonusStrength, int* BonusAgility, int* BonusDefense);
Sprite* CreateWeaponSprite(int WeaponType, int WeaponRarity, float xPos, float yPos);
void SwapWeapons(Weapon* firstWeapon, Weapon* secondWeapon);
void CreateStatsString(char* StatsString, int str, int agi, int def);

#endif