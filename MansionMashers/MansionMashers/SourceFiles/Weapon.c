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

	CurrentWeapon->WeaponGlyphs = CreateText(CurrentWeapon->WeaponName, CurrentWeapon->WeaponPickup.Position.x, CurrentWeapon->WeaponPickup.Position.y + CurrentWeapon->WeaponPickup.height * 1.5f + 25, 50, TextTint, Center, Plain);
	CreateStatsString(CurrentWeapon->WeaponStatsString, CurrentWeapon->BonusStrength, CurrentWeapon->BonusAgility, CurrentWeapon->BonusDefense);
	CurrentWeapon->WeaponStatsGlyphs = CreateText(CurrentWeapon->WeaponStatsString, CurrentWeapon->WeaponPickup.Position.x, (CurrentWeapon->WeaponPickup.Position.y + CurrentWeapon->WeaponPickup.height * 1.5f - 25), 50, TextTint, Center, Plain);
	
	ChangeTextZIndex(CurrentWeapon->WeaponGlyphs, 451);
	ChangeTextZIndex(CurrentWeapon->WeaponStatsGlyphs, 451);

	CurrentWeapon->WeaponSprite = (Sprite *) CreateSprite(weaponTexture, 256, 256, 22, 1, 1, 0, 0);
	CreateCollisionBox(&CurrentWeapon->WeaponPickup, &CurrentWeapon->Position, WeaponDrop, width / 2, height, objID);
	CreateCollisionBox(&CurrentWeapon->WeaponAttack, &CurrentWeapon->Position, collisionGroup, height / 4, height / 4, objID);
	CurrentWeapon->WeaponLength = 80.0f;

	nameLen = strlen(CurrentWeapon->WeaponName);
	statsLen = strlen(CurrentWeapon->WeaponStatsString);
	if(nameLen >= statsLen)
	{
		CurrentWeapon->WeaponHoverBackground = (Sprite *) CreateSprite("TextureFiles/WeaponHoverBackground.png", nameLen * 25.0f, 180, 450, 1, 1, CurrentWeapon->WeaponPickup.Position.x, (CurrentWeapon->WeaponPickup.Position.y + CurrentWeapon->WeaponPickup.height * 1.5f - 2 * CurrentWeapon->WeaponGlyphs->Glyph->Height));
	}
	else
	{
		CurrentWeapon->WeaponHoverBackground = (Sprite *) CreateSprite("TextureFiles/WeaponHoverBackground.png", statsLen * 25.0f, 180, 450, 1, 1, CurrentWeapon->WeaponPickup.Position.x, (CurrentWeapon->WeaponPickup.Position.y + CurrentWeapon->WeaponPickup.height * 1.5f - 2 * CurrentWeapon->WeaponGlyphs->Glyph->Height));
	}
	CurrentWeapon->WeaponHoverBackground->Visible = FALSE;

	//Start off shopless
	CurrentWeapon->CurrentShop = NULL;
	CurrentWeapon->WeaponFalling = FALSE;

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
	CurrentWeapon->WeaponLength = 80.0f;

	if(CurrentWeapon->WeaponName)
		CreateWeaponName(&CurrentWeapon->WeaponName, CurrentWeapon->WeaponType, CurrentWeapon->WeaponRarity);
	else
		CurrentWeapon->WeaponName = "Error: Memory Allocation Failed!";

	CreateWeaponStats(CurrentWeapon->WeaponType, CurrentWeapon->WeaponRarity, &CurrentWeapon->BonusStrength, &CurrentWeapon->BonusAgility, &CurrentWeapon->BonusDefense);

	Vec2Set(&CurrentWeapon->Position, xPos, yPos);
	CurrentWeapon->WeaponSprite = CreateWeaponSprite(CurrentWeapon->WeaponType, CurrentWeapon->WeaponRarity, xPos, yPos);

	// Special Weapon!!
	if (!strcmp(CurrentWeapon->WeaponName,"Sausage Sausage of sausage"))
	{
		CurrentWeapon->WeaponSprite->SpriteTexture = LoadTexture("TextureFiles/BattleAxe.png");
		CurrentWeapon->BonusAgility += 12;
		CurrentWeapon->BonusStrength += 12;
		CurrentWeapon->BonusDefense += 12;
	}

	CreateCollisionBox(&CurrentWeapon->WeaponPickup, &CurrentWeapon->Position, WeaponDrop, width / 2, height, objID);
	CreateCollisionBox(&CurrentWeapon->WeaponAttack, &CurrentWeapon->Position, WeaponDrop, width / 3, height / 2, objID);

	CurrentWeapon->WeaponGlyphs = CreateText(CurrentWeapon->WeaponName, CurrentWeapon->WeaponPickup.Position.x, CurrentWeapon->WeaponPickup.Position.y + CurrentWeapon->WeaponPickup.height * 1.5f + 25, 50, TextTint, Center, Plain);
	CreateStatsString(CurrentWeapon->WeaponStatsString, CurrentWeapon->BonusStrength, CurrentWeapon->BonusAgility, CurrentWeapon->BonusDefense);
	CurrentWeapon->WeaponStatsGlyphs = CreateText(CurrentWeapon->WeaponStatsString, CurrentWeapon->WeaponPickup.Position.x, (CurrentWeapon->WeaponPickup.Position.y + CurrentWeapon->WeaponPickup.height * 1.5f - 25), 50, TextTint, Center, Plain);
	
	ChangeTextZIndex(CurrentWeapon->WeaponGlyphs, 451);
	ChangeTextZIndex(CurrentWeapon->WeaponStatsGlyphs, 451);

	nameLen = strlen(CurrentWeapon->WeaponName);
	statsLen = strlen(CurrentWeapon->WeaponStatsString);
	if(nameLen >= statsLen)
	{
		CurrentWeapon->WeaponHoverBackground = (Sprite *) CreateSprite("TextureFiles/WeaponHoverBackground.png", nameLen * 25.0f, 180, 450, 1, 1, CurrentWeapon->WeaponPickup.Position.x, (CurrentWeapon->WeaponPickup.Position.y + CurrentWeapon->WeaponPickup.height * 1.5f));// + CurrentWeapon->WeaponGlyphs->Glyph->Height));
	}
	else
	{
		CurrentWeapon->WeaponHoverBackground = (Sprite *) CreateSprite("TextureFiles/WeaponHoverBackground.png", statsLen * 25.0f, 180, 450, 1, 1, CurrentWeapon->WeaponPickup.Position.x, (CurrentWeapon->WeaponPickup.Position.y + CurrentWeapon->WeaponPickup.height * 1.5f));// + CurrentWeapon->WeaponGlyphs->Glyph->Height));
	}
	CurrentWeapon->WeaponHoverBackground->Visible = FALSE;

	CurrentWeapon->WeaponParticle = CreateFoxParticleSystem("TextureFiles/ParticlePlatform.png", xPos, yPos, 9, -1, 3, .15f, 270, 0, 0.3f, 0, 110, 100, 50.0f, 1.0f, 0.7f);

	//Start off shopless
	CurrentWeapon->CurrentShop = NULL;
	CurrentWeapon->WeaponFalling = FALSE;
	
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
	else if(WRarity == Uncommon)
	{
		strcpy(*Name, GetUncommonStart());
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
		strcat(*Name, GetUncommonEnd());
	}
	else if(WRarity == Rare)
	{
		strcpy(*Name, GetRareStart());
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
		strcat(*Name, GetRareEnd());
	}
}

void WeaponOnTheRun(Weapon* CurrentWeapon)
{
	//Check if the weapon is falling
	if (CurrentWeapon->WeaponFalling)
	{
		//If collision, stop the falling
		Platform *pList = platformList;
		Wall *walls = wallList;
		Vec2 glyphPos;
		int hit = 0;

		if (CurrentWeapon->WeaponPickup.Position.y <= (GROUNDLEVEL - 80))
		{
			CurrentWeapon->WeaponFalling = FALSE;
			return;
		}

		while(pList->objID != -1)
		{
			if(pList->objID > 0)
			{
				hit = PointRectCollision(&pList->PlatformCollider, &CurrentWeapon->WeaponPickup.Position);
				if(hit)
				{
					CurrentWeapon->WeaponFalling = FALSE;
					return;
				}
			}
			pList++;
		}
		while(walls->objID != -1)
		{
			if(walls->objID > 0)
			{
				hit = PointRectCollision(&walls->WallCollider, &CurrentWeapon->WeaponPickup.Position);
				if(hit)
				{
					CurrentWeapon->WeaponFalling = FALSE;
					return;
				}
			}
			walls++;
		}

		//If not collision, MAKE IT FALL
		CurrentWeapon->WeaponSprite->Position.y -= 4;
		CurrentWeapon->WeaponPickup.Position = CurrentWeapon->WeaponSprite->Position;
		CurrentWeapon->WeaponHoverBackground->Position.x = CurrentWeapon->WeaponPickup.Position.x;
		CurrentWeapon->WeaponHoverBackground->Position.y = CurrentWeapon->WeaponPickup.Position.y + CurrentWeapon->WeaponPickup.height * 1.5f;
		Vec2Set(&glyphPos, CurrentWeapon->WeaponPickup.Position.x, (CurrentWeapon->WeaponPickup.Position.y + CurrentWeapon->WeaponPickup.height * 1.5f + CurrentWeapon->WeaponGlyphs->Glyph->Height / 2));
		ChangeTextPosition(CurrentWeapon->WeaponGlyphs, glyphPos, Center);
		Vec2Set(&glyphPos, CurrentWeapon->WeaponPickup.Position.x, (CurrentWeapon->WeaponPickup.Position.y + CurrentWeapon->WeaponPickup.height * 1.5f - CurrentWeapon->WeaponGlyphs->Glyph->Height / 2));
		ChangeTextPosition(CurrentWeapon->WeaponStatsGlyphs, glyphPos, Center);

		if (CurrentWeapon->WeaponParticle)
		{
			CurrentWeapon->WeaponParticle->Position.x = CurrentWeapon->WeaponSprite->Position.x;
			CurrentWeapon->WeaponParticle->Position.y = CurrentWeapon->WeaponSprite->Position.y;
		}
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
	else if(WRarity == Uncommon)
	{
		switch(WType)
		{
		case Sword:
			*BonusStrength = rand() % 3 + 3;
			*BonusAgility = rand() % 3 + 5;
			*BonusDefense = rand() % 3 + 5;
			break;
		case Axe:
			*BonusStrength = rand() % 3 + 5;
			*BonusAgility = rand() % 3 + 5;
			*BonusDefense = rand() % 3 + 3;
			break;
		case Hammer:
			*BonusStrength = rand() % 3 + 5;
			*BonusAgility = rand() % 3 + 3;
			*BonusDefense = rand() % 3 + 5;
			break;
		case Spear:
			*BonusStrength = rand() % 3 + 3;
			*BonusAgility = rand() % 3 + 7;
			*BonusDefense = rand() % 3 + 3;
			break;
		case FoxWeapon:
			*BonusStrength = rand() % 3 + 5;
			*BonusAgility = rand() % 3 + 5;
			*BonusDefense = rand() % 3 + 5;
			break;
		}
	}
	else if(WRarity == Rare)
	{
		switch(WType)
		{
		case Sword:
			*BonusStrength = rand() % 4 + 6;
			*BonusAgility = rand() % 4 + 8;
			*BonusDefense = rand() % 4 + 8;
			break;
		case Axe:
			*BonusStrength = rand() % 4 + 8;
			*BonusAgility = rand() % 4 + 8;
			*BonusDefense = rand() % 4 + 6;
			break;
		case Hammer:
			*BonusStrength = rand() % 4 + 8;
			*BonusAgility = rand() % 4 + 6;
			*BonusDefense = rand() % 4 + 8;
			break;
		case Spear:
			*BonusStrength = rand() % 4 + 6;
			*BonusAgility = rand() % 4 + 10;
			*BonusDefense = rand() % 4 + 6;
			break;
		case FoxWeapon:
			*BonusStrength = rand() % 4 + 8;
			*BonusAgility = rand() % 4 + 8;
			*BonusDefense = rand() % 4 + 8;
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
			WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Sword.png", 256, 256, 50, 1, 1, xPos, yPos);
			break;
		case Axe:
			WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Axe.png", 256, 256, 50, 1, 1, xPos, yPos);
			break;
		case Hammer:
			WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Hammer.png", 256, 256, 50, 1, 1, xPos, yPos);
			break;
		case Spear:
			WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Spear.png", 256, 256, 50, 1, 1, xPos, yPos);
			break;
		case FoxWeapon:
			WeaponSprite = (Sprite *) CreateSprite("TextureFiles/GinkoSmall.png", 256, 256, 50, 1, 1, xPos, yPos);
			break;
		}
	}
	else if(WRarity == Uncommon)
	{
		switch(WType)
		{
		case Sword:
			WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Sword.png", 256, 256, 50, 1, 1, xPos, yPos);
			break;
		case Axe:
			WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Axe.png", 256, 256, 50, 1, 1, xPos, yPos);
			break;
		case Hammer:
			WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Hammer.png", 256, 256, 50, 1, 1, xPos, yPos);
			break;
		case Spear:
			WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Spear.png", 256, 256, 50, 1, 1, xPos, yPos);
			break;
		case FoxWeapon:
			WeaponSprite = (Sprite *) CreateSprite("TextureFiles/GinkoSmall.png", 256, 256, 50, 1, 1, xPos, yPos);
			break;
		}
	}
	else if(WRarity == Rare)
	{
		switch(WType)
		{
		case Sword:
			WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Sword.png", 256, 256, 50, 1, 1, xPos, yPos);
			break;
		case Axe:
			WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Axe.png", 256, 256, 50, 1, 1, xPos, yPos);
			break;
		case Hammer:
			WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Hammer.png", 256, 256, 50, 1, 1, xPos, yPos);
			break;
		case Spear:
			WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Spear.png", 256, 256, 50, 1, 1, xPos, yPos);
			break;
		case FoxWeapon:
			WeaponSprite = (Sprite *) CreateSprite("TextureFiles/GinkoSmall.png", 256, 256, 50, 1, 1, xPos, yPos);
			break;
		}
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
	temp.CurrentShop = firstWeapon->CurrentShop;
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
	firstWeapon->CurrentShop = secondWeapon->CurrentShop;
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
