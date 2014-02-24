// ---------------------------------------------------------------------------
// includes
#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"

// ---------------------------------------------------------------------------
// Globals

/*************************************************************************/
/*!
	\brief
	Gets the price of a weapon
*/
/*************************************************************************/
void GetWeaponCoin(WeaponShop *newShop, int weaponRarity)
{
	switch(weaponRarity)
	{
	case Common:
		newShop->Coin = 5;
		newShop->CoinChar[0] = '5';
		break;
	case Uncommon:
		newShop->Coin = 20;
		newShop->CoinChar[0] = '2';
		newShop->CoinChar[1] = '0';
		break;
	case Rare:
		newShop->Coin = 80;
		newShop->CoinChar[0] = '8';
		newShop->CoinChar[1] = '0';
		break;
	case Epic:
		newShop->Coin = 200;
		newShop->CoinChar[0] = '2';
		newShop->CoinChar[1] = '0';
		newShop->CoinChar[2] = '0';
		break;
	case Sausage:
		newShop->Coin = 500;
		newShop->CoinChar[0] = '5';
		newShop->CoinChar[1] = '0';
		newShop->CoinChar[2] = '0';
		break;
	default:
		newShop->Coin = 50;
		newShop->CoinChar[0] = '5';
		newShop->CoinChar[1] = '0';
		break;
	};
}

/*************************************************************************/
/*!
	\brief
	Creates a Weapon Shop from given variables
*/
/*************************************************************************/
void CreateWeaponShop(float shopPosX, float shopPosY, Weapon *Weapon, int *newID, int collisionGroup, int weaponRarity)
{
	Vec3 TextTint;
	WeaponShop *newShop = AddWeaponShop();
	GetWeaponCoin(newShop, weaponRarity);

	//Sign
	CreateSprite("TextureFiles/WoodSign.png", 512, 512, 5, 1, 1, shopPosX, shopPosY);

	//Text
	Vec3Set(&TextTint, 0, 0, 0);
	newShop->ItemName = CreateText("SWORD", shopPosX + 20, shopPosY + 120, 80, TextTint, Center);
	ChangeTextVisibility((TextGlyphs *) newShop->ItemName);
	Vec3Set(&TextTint, 0.2f, 0.2f, 0.2f);
	newShop->ItemCoin = CreateText(strcat(newShop->CoinChar, " coin"), shopPosX + 10, shopPosY + 60, 60, TextTint, Center);
	ChangeTextVisibility((TextGlyphs *) newShop->ItemCoin);

	//Weapon
	Weapon = CreateDroppedWeapon(collisionGroup, weaponRarity, 250, 250, *newID++, -350, -140);
	Weapon->WeaponSprite->Rotation = (float)FOX_PI / 2;

	//Gongratz
	newShop->Created = TRUE;
}
