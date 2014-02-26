#ifndef FOX_SHOP
#define FOX_SHOP

#include "TextCreation.h"

#define SHOPAMOUNT 5

typedef struct WeaponShop
{
	int Created;

	int Coin;
	char CoinChar[4];

	struct TextGlyphs *ItemTextName;
	struct TextGlyphs *ItemTextCoin;

	Sprite *ShopSprite;
	Weapon *Weapon;
}WeaponShop;

void GetWeaponCoin(WeaponShop *newShop, int weaponRarity);
void CreateWeaponShop(float shopPosX, float shopPosY, int *newID, int collisionGroup, int weaponRarity);

#endif
