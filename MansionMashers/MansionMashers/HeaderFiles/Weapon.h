#ifndef FOX_WEAPON
#define FOX_WEAPON

#include "Sprite.h"
#include "CollisionBox.h"

struct Weapon
{
	char *WeaponName;

	int WeaponID;

	int BonusStrength;
	int BonusAgility;
	int BonusDefense;

	Sprite WeaponSprite;

	CollisionBox WeaponPickup;
};

#endif