#ifndef FOX_WEAPON
#define FOX_WEAPON

#include "Sprite.h"

struct Weapon
{
	int BonusDamage;
	int SwingSpeed;

	struct Sprite WeaponSprite;

};

#endif