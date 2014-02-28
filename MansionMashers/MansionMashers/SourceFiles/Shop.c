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
void getWeaponCoin(WeaponShop *newShop, int weaponRarity)
{
	//For now just manually pick the coin value.
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
	}
}

/*************************************************************************/
/*!
	\brief
	Gets the item title of a weapon
*/
/*************************************************************************/
void getItemTextName(WeaponShop *newShop, int collisionGroup, Vec3 TextTint, float shopPosX, float shopPosY)
{
	Vec3Set(&TextTint, 0, 0, 0);

	//For now just manually pick the coin value.
	switch(collisionGroup)
	{
	case Sword:
		newShop->ItemTextName = CreateText("SWORD", shopPosX + 20, shopPosY + 120, 80, TextTint, Center);
		break;
	case Axe:
		newShop->ItemTextName = CreateText("AXE", shopPosX + 20, shopPosY + 120, 80, TextTint, Center);
		break;
	case Hammer:
		newShop->ItemTextName = CreateText("HAMMER", shopPosX + 20, shopPosY + 120, 80, TextTint, Center);
		break;
	case Spear:
		newShop->ItemTextName = CreateText("SPEAR", shopPosX + 20, shopPosY + 120, 80, TextTint, Center);
		break;
	default:
		newShop->ItemTextName = CreateText("WUT DID U DO", shopPosX + 20, shopPosY + 120, 80, TextTint, Center);
		break;
	}

	ChangeTextVisibility(newShop->ItemTextName);
}

/*************************************************************************/
/*!
	\brief
	Creates a Weapon Shop from given variables
*/
/*************************************************************************/
void CreateWeaponShop(float shopPosX, float shopPosY, int newID, int collisionGroup, int weaponRarity)
{
	Vec3 TextTint;
	WeaponShop *newShop = AddWeaponShop();
	getWeaponCoin(newShop, weaponRarity);

	//Sign
	newShop->ShopSprite = (Sprite *) CreateSprite("TextureFiles/WoodSign.png", 512, 512, 5, 1, 1, shopPosX, shopPosY);

	//Text
	Vec3Set(&TextTint, 0.2f, 0.2f, 0.2f);
	newShop->ItemTextCoin = CreateText(strcat(newShop->CoinChar, " coin"), shopPosX + 10, shopPosY + 60, 60, TextTint, Center);
	ChangeTextVisibility(newShop->ItemTextCoin);
	getItemTextName(newShop, collisionGroup, TextTint, shopPosX, shopPosY);

	//Weapon
	newShop->Weapon = CreateDroppedWeapon(collisionGroup, weaponRarity, 250, 250, newID, shopPosX + 50, shopPosY);
	newShop->Weapon->WeaponSprite->Rotation = (float)FOX_PI / 2;
	newShop->Weapon->CurrentShop = newShop;

	//Gongratz
	newShop->Created = TRUE;
}
