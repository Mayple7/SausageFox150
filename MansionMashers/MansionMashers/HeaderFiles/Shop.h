#ifndef FOX_SHOP
#define FOX_SHOP

#include "TextCreation.h"

#define SHOPAMOUNT 5

typedef struct WeaponShop
{
	int Created;

	int Coin;
	char CoinChar[32];

	struct TextGlyphs *ItemTextName;
	struct TextGlyphs *ItemTextCoin;

	Sprite *ShopSprite;
	Sprite *ShopTextSprite; //Confusing, but this is for text with symbols not in the text system, make them as a texture
	Weapon *Weapon;
}WeaponShop;

void getWeaponCoin(WeaponShop *newShop, int weaponRarity);
void getItemTextName(WeaponShop *newShop, int collisionGroup, Vec3 TextTint, float shopPosX, float shopPosY, int weaponRarity);
void CreateWeaponShop(float shopPosX, float shopPosY, int collisionGroup, int weaponRarity);

#endif
