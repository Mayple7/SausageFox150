#ifndef FOX_SHOP
#define FOX_SHOP

#include "TextCreation.h"

#define SHOPAMOUNT 5

typedef struct WeaponShop
{
	int Created;

	int Coin;
	char CoinChar[4];

	struct TextGlyphs *ItemName;
	struct TextGlyphs *ItemCoin;
}WeaponShop;

void GetWeaponCoin(WeaponShop *newShop, int weaponRarity);
void CreateWeaponShop(float shopPosX, float shopPosY, Weapon *Weapon, int *newID, int collisionGroup, int weaponRarity);

#endif