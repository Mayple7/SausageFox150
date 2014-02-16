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
	Vec3 TextTint;
	int nameLen, statsLen;
	Vec3Set(&TextTint, 0, 0, 0);
	CurrentWeapon->WeaponFOF = PlayerWeapon; // Friend or Foe tag
	CurrentWeapon->objID = objID;
	CurrentWeapon->WeaponRarity = weaponRarity;
	CurrentWeapon->WeaponType = weaponType;
	CurrentWeapon->WeaponName = (char *) CallocMyAlloc(MAX_NAME_LENGTH, sizeof(char));
	CurrentWeapon->WeaponStatsString = (char *) CallocMyAlloc(MAX_NAME_LENGTH, sizeof(char));
	
	SetWeaponStats(CurrentWeapon, 0, 0, 0);
	if(CurrentWeapon->WeaponName)
		strcpy(CurrentWeapon->WeaponName, weaponName);
	else
		CurrentWeapon->WeaponName = "Error: Memory Allocation Failed!";

	CurrentWeapon->WeaponGlyphs = CreateText(CurrentWeapon->WeaponName, CurrentWeapon->WeaponPickup.Position.x / GetLoadRatio(), CurrentWeapon->WeaponPickup.Position.y / GetLoadRatio() + CurrentWeapon->WeaponPickup.height / GetLoadRatio() * 1.5f + 25, 50, TextTint, Center);
	CreateStatsString(CurrentWeapon->WeaponStatsString, CurrentWeapon->BonusStrength, CurrentWeapon->BonusAgility, CurrentWeapon->BonusDefense);
	CurrentWeapon->WeaponStatsGlyphs = CreateText(CurrentWeapon->WeaponStatsString, CurrentWeapon->WeaponPickup.Position.x / GetLoadRatio(), (CurrentWeapon->WeaponPickup.Position.y / GetLoadRatio() + CurrentWeapon->WeaponPickup.height / GetLoadRatio() * 1.5f - 25), 50, TextTint, Center);
	
	CurrentWeapon->WeaponSprite = CreateSprite(weaponTexture, 256, 256, 22, 1, 1, 0, 0);
	CreateCollisionBox(&CurrentWeapon->WeaponPickup, &CurrentWeapon->Position, WeaponDrop, width / 2, height, objID);
	CreateCollisionBox(&CurrentWeapon->WeaponAttack, &CurrentWeapon->Position, collisionGroup, height / 4, height / 4, objID);
	CurrentWeapon->WeaponLength = 80.0f * GetLoadRatio();

	nameLen = strlen(CurrentWeapon->WeaponName);
	statsLen = strlen(CurrentWeapon->WeaponStatsString);
	if(nameLen >= statsLen)
	{
		CurrentWeapon->WeaponHoverBackground = CreateSprite("TextureFiles/WeaponHoverBackground.png", nameLen * 25.0f, 120, 10, 1, 1, CurrentWeapon->WeaponPickup.Position.x / GetLoadRatio(), (CurrentWeapon->WeaponPickup.Position.y / GetLoadRatio() + CurrentWeapon->WeaponPickup.height / GetLoadRatio() * 1.5f));
	}
	else
	{
		CurrentWeapon->WeaponHoverBackground = CreateSprite("TextureFiles/WeaponHoverBackground.png", statsLen * 25.0f, 120, 10, 1, 1, CurrentWeapon->WeaponPickup.Position.x / GetLoadRatio(), (CurrentWeapon->WeaponPickup.Position.y / GetLoadRatio() + CurrentWeapon->WeaponPickup.height / GetLoadRatio() * 1.5f));
	}
	CurrentWeapon->WeaponHoverBackground->Visible = FALSE;

	return CurrentWeapon;
}


Weapon* CreateDroppedWeapon(int weaponType, int weaponRarity, float width, float height, int objID, float xPos, float yPos)
{
	Weapon *CurrentWeapon = AddWeapon();
	Vec2 ColliderPos;
	Vec3 TextTint;
	int nameLen, statsLen;
	Vec2Set(&ColliderPos, xPos, yPos);
	Vec3Set(&TextTint, 0, 0, 0);

	CurrentWeapon->WeaponFOF = DroppedWeapon; // Friend or Foe tag
	CurrentWeapon->objID = objID;
	CurrentWeapon->WeaponRarity = weaponRarity;
	CurrentWeapon->WeaponType = weaponType;
	CurrentWeapon->WeaponName = (char *) CallocMyAlloc(MAX_NAME_LENGTH, sizeof(char));
	CurrentWeapon->WeaponStatsString = (char *) CallocMyAlloc(MAX_NAME_LENGTH, sizeof(char));
	CurrentWeapon->WeaponLength = 80.0f * GetLoadRatio();

	if(CurrentWeapon->WeaponName)
		CreateWeaponName(&CurrentWeapon->WeaponName, CurrentWeapon->WeaponType, CurrentWeapon->WeaponRarity);
	else
		CurrentWeapon->WeaponName = "Error: Memory Allocation Failed!";

	CreateWeaponStats(CurrentWeapon->WeaponType, CurrentWeapon->WeaponRarity, &CurrentWeapon->BonusStrength, &CurrentWeapon->BonusAgility, &CurrentWeapon->BonusDefense);

	Vec2Set(&CurrentWeapon->Position, xPos, yPos);
	CurrentWeapon->WeaponSprite = CreateWeaponSprite(CurrentWeapon->WeaponType, CurrentWeapon->WeaponRarity, xPos, yPos);
	if (!strcmp(CurrentWeapon->WeaponName,"Sausage Sausage of sausage"))
		CurrentWeapon->WeaponSprite->SpriteTexture = LoadTexture("TextureFiles/BattleAxe.png");
	CreateCollisionBox(&CurrentWeapon->WeaponPickup, &CurrentWeapon->Position, WeaponDrop, width / 2, height, objID);
	CreateCollisionBox(&CurrentWeapon->WeaponAttack, &CurrentWeapon->Position, WeaponDrop, width / 3, height / 2, objID);

	CurrentWeapon->WeaponGlyphs = CreateText(CurrentWeapon->WeaponName, CurrentWeapon->WeaponPickup.Position.x / GetLoadRatio(), (CurrentWeapon->WeaponPickup.Position.y / GetLoadRatio() + CurrentWeapon->WeaponPickup.height / GetLoadRatio() * 1.5f + 25), 50, TextTint, Center);
	CreateStatsString(CurrentWeapon->WeaponStatsString, CurrentWeapon->BonusStrength, CurrentWeapon->BonusAgility, CurrentWeapon->BonusDefense);
	CurrentWeapon->WeaponStatsGlyphs = CreateText(CurrentWeapon->WeaponStatsString, CurrentWeapon->WeaponPickup.Position.x / GetLoadRatio(), (CurrentWeapon->WeaponPickup.Position.y / GetLoadRatio() + CurrentWeapon->WeaponPickup.height / GetLoadRatio() * 1.5f - 25), 50, TextTint, Center);

	nameLen = strlen(CurrentWeapon->WeaponName);
	statsLen = strlen(CurrentWeapon->WeaponStatsString);
	if(nameLen >= statsLen)
	{
		CurrentWeapon->WeaponHoverBackground = CreateSprite("TextureFiles/WeaponHoverBackground.png", nameLen * 25.0f, 120, 10, 1, 1, CurrentWeapon->WeaponPickup.Position.x / GetLoadRatio(), (CurrentWeapon->WeaponPickup.Position.y + CurrentWeapon->WeaponPickup.height * 1.5f) / GetLoadRatio());
	}
	else
	{
		CurrentWeapon->WeaponHoverBackground = CreateSprite("TextureFiles/WeaponHoverBackground.png", statsLen * 25.0f, 120, 10, 1, 1, CurrentWeapon->WeaponPickup.Position.x / GetLoadRatio(), (CurrentWeapon->WeaponPickup.Position.y + CurrentWeapon->WeaponPickup.height * 1.5f) / GetLoadRatio());
	}
	CurrentWeapon->WeaponHoverBackground->Visible = FALSE;
	
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
			*BonusStrength = rand() % 2;
			*BonusAgility = rand() % 2 + 1;
			*BonusDefense = rand() % 2 + 1;
			break;
		case Axe:
			*BonusStrength = rand() % 2 + 1;
			*BonusAgility = rand() % 2 + 1;
			*BonusDefense = rand() % 2;
			break;
		case Hammer:
			*BonusStrength = rand() % 2 + 1;
			*BonusAgility = rand() % 2;
			*BonusDefense = rand() % 2 + 1;
			break;
		case Spear:
			*BonusStrength = rand() % 2;
			*BonusAgility = rand() % 2 + 2;
			*BonusDefense = rand() % 2;
			break;
		case FoxWeapon:
			*BonusStrength = rand() % 2 + 1;
			*BonusAgility = rand() % 2 + 1;
			*BonusDefense = rand() % 2 + 1;
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

void SwapWeapons(Weapon* firstWeapon, Weapon* secondWeapon)
{
	Weapon temp;
	
	temp.WeaponPickup = firstWeapon->WeaponPickup;
	temp.WeaponPickup.Position = firstWeapon->WeaponPickup.Position;
	temp.WeaponRarity = firstWeapon->WeaponRarity;
	temp.WeaponGlyphs = firstWeapon->WeaponGlyphs;
	temp.WeaponStatsGlyphs = firstWeapon->WeaponStatsGlyphs;
	temp.WeaponAttack = firstWeapon->WeaponAttack;
	temp.Position = firstWeapon->Position;
	temp.objID = firstWeapon->objID;
	temp.WeaponSprite = firstWeapon->WeaponSprite;
	temp.WeaponName = firstWeapon->WeaponName;
	temp.WeaponStatsString = firstWeapon->WeaponStatsString;
	temp.WeaponType = firstWeapon->WeaponType;
	temp.WeaponHoverBackground = firstWeapon->WeaponHoverBackground;
	SetWeaponStats(&temp, firstWeapon->BonusStrength, firstWeapon->BonusAgility, firstWeapon->BonusDefense);

	firstWeapon->WeaponPickup = secondWeapon->WeaponPickup;
	firstWeapon->WeaponPickup.Position = secondWeapon->WeaponPickup.Position;
	firstWeapon->WeaponRarity = secondWeapon->WeaponRarity;
	firstWeapon->WeaponGlyphs = secondWeapon->WeaponGlyphs;
	firstWeapon->WeaponStatsGlyphs = secondWeapon->WeaponStatsGlyphs;
	firstWeapon->WeaponAttack = secondWeapon->WeaponAttack;
	firstWeapon->Position = secondWeapon->Position;
	firstWeapon->objID = secondWeapon->objID;
	firstWeapon->WeaponSprite = secondWeapon->WeaponSprite;
	firstWeapon->WeaponName = secondWeapon->WeaponName;
	firstWeapon->WeaponStatsString = secondWeapon->WeaponStatsString;
	firstWeapon->WeaponType = secondWeapon->WeaponType;
	firstWeapon->WeaponHoverBackground = secondWeapon->WeaponHoverBackground;
	SetWeaponStats(firstWeapon, secondWeapon->BonusStrength, secondWeapon->BonusAgility, secondWeapon->BonusDefense);

	secondWeapon->WeaponPickup = temp.WeaponPickup;
	secondWeapon->WeaponPickup.Position = firstWeapon->WeaponPickup.Position;
	secondWeapon->WeaponRarity =  temp.WeaponRarity;
	secondWeapon->WeaponGlyphs = temp.WeaponGlyphs;
	secondWeapon->WeaponStatsGlyphs = temp.WeaponStatsGlyphs;
	secondWeapon->WeaponAttack = temp.WeaponAttack;
	secondWeapon->Position = temp.Position;
	secondWeapon->objID = temp.objID;
	secondWeapon->WeaponSprite =  temp.WeaponSprite;
	secondWeapon->WeaponName = temp.WeaponName;
	secondWeapon->WeaponStatsString = temp.WeaponStatsString;
	secondWeapon->WeaponType =  temp.WeaponType;
	secondWeapon->WeaponHoverBackground = temp.WeaponHoverBackground;
	SetWeaponStats(secondWeapon, temp.BonusStrength, temp.BonusAgility, temp.BonusDefense);
}

void CreateStatsString(char* StatsString, int str, int agi, int def)
{
	char num[10];

	strcpy(StatsString, "+");
	sprintf(num, "%d", str);
	strcat(StatsString, num);
	strcat(StatsString, "str +");
	sprintf(num, "%d", agi);
	strcat(StatsString, num);
	strcat(StatsString, "agi +");
	sprintf(num, "%d", def);
	strcat(StatsString, num);
	strcat(StatsString, "def");
}
