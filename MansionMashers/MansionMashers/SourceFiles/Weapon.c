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
#include "../HeaderFiles/WeaponNames.h"

Weapon* CreateWeapon(char* weaponName, char* weaponTexture, int weaponType, int weaponRarity, int collisionGroup, float width, float height, int objID)
{
	Weapon *CurrentWeapon = AddWeapon();
	
	CurrentWeapon->WeaponFOF = PlayerWeapon; // Friend or Foe tag
	CurrentWeapon->objID = objID;
	CurrentWeapon->WeaponRarity = weaponRarity;
	CurrentWeapon->WeaponType = weaponType;
	CurrentWeapon->WeaponName = (char *) calloc(MAX_NAME_LENGTH, MAX_NAME_LENGTH * sizeof(char));
	if(CurrentWeapon->WeaponName)
		strcpy(CurrentWeapon->WeaponName, weaponName);
	else
		CurrentWeapon->WeaponName = "Error: Memory Allocation Failed!";

	SetWeaponStats(CurrentWeapon, 0, 0, 0);

	CurrentWeapon->WeaponSprite = CreateSprite(weaponTexture, 256, 256, 22, 1, 1, 0, 0);
	CreateCollisionBox(&CurrentWeapon->WeaponPickup, &CurrentWeapon->Position, WeaponDrop, width / 3, height, objID);
	CreateCollisionBox(&CurrentWeapon->WeaponAttack, &CurrentWeapon->Position, collisionGroup, width / 3, height / 4, objID);
	CurrentWeapon->WeaponAttack.Offset.y += 5 * CurrentWeapon->WeaponAttack.height / 8;

	return CurrentWeapon;
}


Weapon* CreateDroppedWeapon(int weaponType, int weaponRarity, float width, float height, int objID, float xPos, float yPos)
{
	Weapon *CurrentWeapon = AddWeapon();
	Vec2 ColliderPos;
	Vec3 TextTint;
	Vec2Set(&ColliderPos, xPos, yPos);
	Vec3Set(&TextTint, 0, 0, 0);

	CurrentWeapon->WeaponFOF = DroppedWeapon; // Friend or Foe tag
	CurrentWeapon->objID = objID;
	CurrentWeapon->WeaponRarity = weaponRarity;
	CurrentWeapon->WeaponType = weaponType;
	CurrentWeapon->WeaponName = (char *) calloc(MAX_NAME_LENGTH, MAX_NAME_LENGTH * sizeof(char));
	if(CurrentWeapon->WeaponName)
		CreateWeaponName(&CurrentWeapon->WeaponName, CurrentWeapon->WeaponType, CurrentWeapon->WeaponRarity);
	else
		CurrentWeapon->WeaponName = "Error: Memory Allocation Failed!";

	CurrentWeapon->WeaponGlyphs = CreateText(CurrentWeapon->WeaponName, xPos + 300, yPos + 200, 50, TextTint);

	CreateWeaponStats(CurrentWeapon->WeaponType, CurrentWeapon->WeaponRarity, &CurrentWeapon->BonusStrength, &CurrentWeapon->BonusAgility, &CurrentWeapon->BonusDefense);

	Vec2Set(&CurrentWeapon->Position, xPos * GetLoadRatio(), yPos * GetLoadRatio());
	CurrentWeapon->WeaponSprite = CreateWeaponSprite(CurrentWeapon->WeaponType, CurrentWeapon->WeaponRarity, xPos, yPos);
	if (!strcmp(CurrentWeapon->WeaponName,"Sausage Sausage of sausage"))
		CurrentWeapon->WeaponSprite->SpriteTexture = LoadTexture("TextureFiles/BattleAxe.png");
	CreateCollisionBox(&CurrentWeapon->WeaponPickup, &CurrentWeapon->Position, WeaponDrop, width / 3, height, objID);
	CreateCollisionBox(&CurrentWeapon->WeaponAttack, &CurrentWeapon->Position, WeaponDrop, width / 3, height / 2, objID);

	return CurrentWeapon;
}

void CreateWeaponName(char** Name, int WType, int WRarity)
{
	if(WRarity == Common)
	{
		strcpy(*Name, GetCommonStart());
		switch(WType)
		{
		case Sword:
			strcat(*Name, GetSwordSynonym());
			break;
		case Axe:
			strcat(*Name, GetAxeSynonym());
			break;
		case Hammer:
			strcat(*Name, GetHammerSynonym());
			break;
		case Spear:
			strcat(*Name, GetSpearSynonym());
			break;
		}
		strcat(*Name, GetCommonEnd());
	}
}

void SetWeaponStats(Weapon* CurrentWeapon, int BonusStrength, int BonusAgility, int BonusDefense)
{
	CurrentWeapon->BonusAgility = BonusAgility;
	CurrentWeapon->BonusDefense = BonusDefense;
	CurrentWeapon->BonusStrength = BonusStrength;
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

Sprite* CreateWeaponSprite(int WType, int WRarity, float xPos, float yPos)
{
	Sprite *WeaponSprite;
	if(WRarity == Common)
	{
		switch(WType)
		{
		case Sword:
			WeaponSprite = CreateSprite("TextureFiles/Sword.png", 256, 256, 5, 1, 1, xPos, yPos);
			break;
		case Axe:
			WeaponSprite = CreateSprite("TextureFiles/Axe.png", 256, 256, 5, 1, 1, xPos, yPos);
			break;
		case Hammer:
			WeaponSprite = CreateSprite("TextureFiles/Hammer.png", 256, 256, 5, 1, 1, xPos, yPos);
			break;
		case Spear:
			WeaponSprite = CreateSprite("TextureFiles/Spear.png", 256, 256, 5, 1, 1, xPos, yPos);
			break;
		case FoxWeapon:
			WeaponSprite = CreateSprite("TextureFiles/GinkoSmall.png", 256, 256, 5, 1, 1, xPos, yPos);
			break;
		};
	}
	return WeaponSprite;
}
