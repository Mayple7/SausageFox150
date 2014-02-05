/*****************************************************************************/
/*!
\file				Weapon.c
\author				Dan Muller (d.muller)
\date				Feb 4, 2014

\brief				Functions for the weapon object

\par				Functions:
\li					CreateWeapon
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

#include "../HeaderFiles/Weapon.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxObjects.h"

Weapon* CreateDroppedWeapon(int weaponType, int weaponRarity, float width, float height, int objID, float xPos, float yPos)
{
	Weapon *CurrentWeapon = AddWeapon();
	Vec2 ColliderPos;
	Vec2Set(&ColliderPos, xPos, yPos);

	CurrentWeapon->WeaponFOF = PlayerWeapon; // Friend or Foe tag
	CurrentWeapon->objID = objID;
	CurrentWeapon->WeaponRarity = weaponRarity;
	CurrentWeapon->WeaponType = weaponType;

	CreateWeaponName(CurrentWeapon->WeaponName, CurrentWeapon->WeaponType, CurrentWeapon->WeaponRarity);

	CreateWeaponStats(CurrentWeapon->WeaponType, CurrentWeapon->WeaponRarity, &CurrentWeapon->BonusStrength, &CurrentWeapon->BonusAgility, &CurrentWeapon->BonusDefense);

	Vec2Set(&CurrentWeapon->Position, xPos * GetLoadRatio(), yPos * GetLoadRatio());

	CreateWeaponSprite(&CurrentWeapon->WeaponSprite, CurrentWeapon->WeaponType, CurrentWeapon->WeaponRarity, xPos, yPos);

	CreateCollisionBox(&CurrentWeapon->WeaponPickup, &CurrentWeapon->Position, WeaponDrop, width / 3, height, objID);
	//CollisionBox WeaponAttack;

	return CurrentWeapon;
}

void CreateWeaponName(char* Name, int Type, int Rarity)
{
	
}

void CreateWeaponStats(int WType, int WRarity, int* BonusStrength, int* BonusAgility, int* BonusDefense)
{
	if(WRarity == Common)
	{
		switch(WType)
		{
		case Sword:
			*BonusStrength = 0;
			*BonusAgility = 1;
			*BonusDefense = 1;
			break;
		case Axe:
			*BonusStrength = 1;
			*BonusAgility = 1;
			*BonusDefense = 0;
			break;
		case Hammer:
			*BonusStrength = 1;
			*BonusAgility = 0;
			*BonusDefense = 1;
			break;
		case Spear:
			*BonusStrength = 0;
			*BonusAgility = 2;
			*BonusDefense = 0;
			break;
		case FoxWeapon:
			*BonusStrength = 1;
			*BonusAgility = 1;
			*BonusDefense = 1;
			break;
		}

	}

}

void CreateWeaponSprite(Sprite *WeaponSprite, int WType, int WRarity, float xPos, float yPos)
{
	if(WRarity == Common)
	{
		switch(WType)
		{
		case Sword:
			CreateSprite("TextureFiles/Sword.png", 256, 256, 5, 1, 1, xPos, yPos);
			break;
		case Axe:
			CreateSprite("TextureFiles/Axe.png", 256, 256, 5, 1, 1, xPos, yPos);
			break;
		case Hammer:
			CreateSprite("TextureFiles/Hammer.png", 256, 256, 5, 1, 1, xPos, yPos);
			break;
		case Spear:
			CreateSprite("TextureFiles/Spear.png", 256, 256, 5, 1, 1, xPos, yPos);
			break;
		case FoxWeapon:
			CreateSprite("TextureFiles/GinkoSmall.png", 256, 256, 5, 1, 1, xPos, yPos);
			break;
		};
	}
}
