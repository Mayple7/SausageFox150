#ifndef FOX_SHOP
#define FOX_SHOP

#include "TextCreation.h"

#define SHOPAMOUNT 5

typedef struct WeaponShop
{
	int Created;

	struct TextGlyphs *ItemName;
	struct TextGlyphs *ItemCoin;
}WeaponShop;

void CreateWeaponShop(float shopPosX, float shopPosY, Weapon *Weapon, int *newID, int collisionGroup, int weaponRarity);

#endif