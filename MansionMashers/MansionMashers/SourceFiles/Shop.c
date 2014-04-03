/*****************************************************************************/
/*!
\file				Shop.c
\author				Kaden Nugent (kaden.n)
\date				Feb 28, 2014

\brief				Manages Shop creation

\par				Functions:
\li					getWeaponCoin
\li					getItemTextName
\li					CreateWeaponShop
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

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
	int weaponWorth = newShop->Weapon->BonusStrength * 10 + newShop->Weapon->BonusDefense * 5 + newShop->Weapon->BonusAgility * 5;

	//Manually pick the coin value and then sprintf it into the coinchar value for use
	switch(weaponRarity)
	{
	case Common:
		newShop->Coin = 20 + weaponWorth;
		break;
	case Uncommon:
		newShop->Coin = 200 + weaponWorth;
		break;
	case Rare:
		newShop->Coin = 500 + weaponWorth;
		break;
	case Sausage:
		newShop->Coin = 900 + weaponWorth;
		break;
	default:
		newShop->Coin = 50 + weaponWorth;
		break;
	}

	//Sprint a lot to get coins
	sprintf(newShop->CoinChar, "%i", newShop->Coin);
}

/*************************************************************************/
/*!
	\brief
	Gets the item title of a weapon
*/
/*************************************************************************/
void getItemTextName(WeaponShop *newShop, int collisionGroup, Vec3 TextTint, float shopPosX, float shopPosY, int weaponRarity)
{
	char placardName[50];
	Vec3Set(&TextTint, 0, 0, 0);

	switch(weaponRarity)
	{
	case Common:
		strcpy(placardName, "Common ");
		break;
	case Uncommon:
		strcpy(placardName, "Uncommon ");
		break;
	case Rare:
		strcpy(placardName, "Rare ");
		break;
	case Sausage:
		strcpy(placardName, "Sausage ");
		break;
	}

	//For now just manually create the generic weapon text
	switch(collisionGroup)
	{
	case Sword:
		strcat(placardName, "sword");
		newShop->ItemTextName = CreateText(placardName, shopPosX + 20, shopPosY + 120, 65, TextTint, Center, Plain);
		break;
	case Axe:
		strcat(placardName, "axe");
		newShop->ItemTextName = CreateText(placardName, shopPosX + 20, shopPosY + 120, 65, TextTint, Center, Plain);
		break;
	case Hammer:
		strcat(placardName, "hammer");
		newShop->ItemTextName = CreateText(placardName, shopPosX + 20, shopPosY + 120, 65, TextTint, Center, Plain);
		break;
	case Spear:
		strcat(placardName, "spear");
		newShop->ItemTextName = CreateText(placardName, shopPosX + 20, shopPosY + 120, 65, TextTint, Center, Plain);
		break;
	default:
		strcat(placardName, "wut did u do");
		newShop->ItemTextName = CreateText(placardName, shopPosX + 20, shopPosY + 120, 65, TextTint, Center, Plain);
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
	
	//Sign
	newShop->ShopSprite     = (Sprite *) CreateSprite("TextureFiles/WoodSign.png", 512, 512, 5, 1, 1, shopPosX, shopPosY);
	newShop->ShopTextSprite = (Sprite *) CreateSprite("TextureFiles/WoodSignText.png", 512, 512, 5, 1, 1, shopPosX, shopPosY);

	//Weapon
	newShop->Weapon = CreateDroppedWeapon(collisionGroup, weaponRarity, 250, 250, newID, shopPosX + 50, shopPosY);
	newShop->Weapon->WeaponSprite->Rotation = FOX_PI / 2;
	newShop->Weapon->CurrentShop = newShop;
	FreeParticleSystem(newShop->Weapon->WeaponParticle);

	//Based on the weapon, get the price
	getWeaponCoin(newShop, weaponRarity);

	//Text
	Vec3Set(&TextTint, 0.2f, 0.2f, 0.2f);
	newShop->ItemTextCoin = CreateText(strcat(newShop->CoinChar, " coin"), shopPosX + 10, shopPosY + 60, 60, TextTint, Center, Plain);
	ChangeTextVisibility(newShop->ItemTextCoin);
	getItemTextName(newShop, collisionGroup, TextTint, shopPosX, shopPosY, weaponRarity);

	//Gongratz
	newShop->Created = TRUE;
}
