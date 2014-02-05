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

Weapon* CreateDroppedWeapon(char* textureName, int weaponType, int weaponRarity, float width, float height, int objID, float xPos, float yPos)
{
	Weapon *CurrentWeapon;

	CurrentWeapon->WeaponFOF = PlayerWeapon; // Friend or Foe tag
	CurrentWeapon->WeaponID = objID;
	CurrentWeapon->WeaponRarity = weaponRarity;
	CurrentWeapon->WeaponType = weaponType;

	CreateWeaponName(CurrentWeapon->WeaponName, CurrentWeapon->WeaponType, CurrentWeapon->WeaponRarity);

	CreateWeaponStats(CurrentWeapon->WeaponType, CurrentWeapon->WeaponRarity, &CurrentWeapon->BonusStrength, &CurrentWeapon->BonusAgility, &CurrentWeapon->BonusDefense);

	Vec2Set(&CurrentWeapon->Position, xPos, yPos);

	CreateWeaponSprite(&CurrentWeapon->WeaponSprite, CurrentWeapon->WeaponType, CurrentWeapon->WeaponRarity);

	CreateCollisionBox(&CurrentWeapon->WeaponPickup, &CurrentWeapon->Position, WeaponDrop, width, height, objID);
	//CollisionBox WeaponAttack;
}

void CreateWeaponName(char* Name, int Type, int Rarity)
{

}

void CreateWeaponStats(int WeaponType, int WeaponRarity, int* BonusStrength, int* BonusAgility, int* BonusDefense)
{

}

void CreateWeaponSprite(Sprite *WeaponSprite, int WeaponType, int WeaponRarity)
{

}
